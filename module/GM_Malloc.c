#include "GM_Malloc.h"	   

static void mem_mgmt_init(U8_t memx);
static U16_t mem_mgmt_perused(U8_t memx);

/* Memory Pool alignment=32 byte */
__align(32) U8_t inte_gen_mem_base[GEN_MEM_MAX_SIZE];                                   /* Interal general memory pool */
__align(32) U8_t inte_ccm_mem_base[CCM_MEM_MAX_SIZE] __attribute__((at(0x10000000)));	/* Interal ccm memory pool */

/* Memory management table */
U32_t inte_gen_mem_map_base[GEN_MEM_TABLE_SIZE];                                                    /* Interal general memory pool table */
U32_t inte_ccm_mem_map_base[CCM_MEM_TABLE_SIZE] __attribute__((at(0x10000000+CCM_MEM_MAX_SIZE)));	/* Interal ccm memory pool table */

/* Memory management parameter */
const U32_t mem_table_size[MEMORY_BANK_NUM] = {GEN_MEM_TABLE_SIZE,CCM_MEM_TABLE_SIZE};      /* Memory table size */
const U32_t mem_block_size[MEMORY_BANK_NUM] = {GEN_MEM_BLOCK_SIZE,CCM_MEM_BLOCK_SIZE};      /* Memory block size */
const U32_t mem_total_size[MEMORY_BANK_NUM] = {GEN_MEM_MAX_SIZE,CCM_MEM_MAX_SIZE};          /* Memory total size */

//内存管理控制器
static memory_mgmt_t mem_mgmt = {
    mem_mgmt_init,
    mem_mgmt_perused,
    inte_gen_mem_base,inte_ccm_mem_base,
    inte_gen_mem_map_base,inte_ccm_mem_map_base,
    0,0,
};



/* Copy memory */
static void mem_mgmt_memcpy(void *des, void *src, U32_t n)  
{
    U8_t *p_des=des;
	U8_t *p_src=src;

    while(n--)
    {
        *p_des++ = *p_src++;
    }
}

/* Set memory value to zero */
static void mem_mgmt_memset(void *src, U8_t data, U32_t count)  
{
    U8_t *p = src;

    while(count--)
    {
        *p++ = data;
    } 
}


/* Initialize the memory */
static void mem_mgmt_init(U8_t memx)
{  
    mem_mgmt_memset(mem_mgmt.memmap[memx],0,mem_table_size[memx]*4);	//内存状态表数据清零  
 	mem_mgmt.memrdy[memx] = 1;								//内存管理初始化OK  
}

//获取内存使用率
//memx:所属内存块
//返回值:使用率(扩大了10倍,0~1000,代表0.0%~100.0%)
/* Get the memory  */
static U16_t mem_mgmt_perused(U8_t memx)
{
    U32_t used=0;
    U32_t i;
    for(i=0;i<mem_table_size[memx];i++)  
    {
        if(mem_mgmt.memmap[memx][i])
        {
            used++;
        }
    }
    return (used*1000)/(mem_table_size[memx]);  
}

//内存分配(内部调用)
//memx:所属内存块
//size:要分配的内存大小(字节)
//返回值:0XFFFFFFFF,代表错误;其他,内存偏移地址 
static U32_t mem_mgmt_malloc(U8_t memx, U32_t size)
{  
    signed long offset=0;  
    U32_t nmemb;	//需要的内存块数
	U32_t cmemb=0;//连续空内存块数
    U32_t i;  
    if(!mem_mgmt.memrdy[memx])
    {
        mem_mgmt.init(memx);//未初始化,先执行初始化
    }

    if(size==0)
    {
        return 0XFFFFFFFF;//不需要分配
    }

    nmemb=size/mem_block_size[memx];  	//获取需要分配的连续内存块数
    
    if(size%mem_block_size[memx])
    {
        nmemb++;
    }  
    
    for(offset=mem_table_size[memx]-1;offset>=0;offset--)//搜索整个内存控制区  
    {     
		if(!mem_mgmt.memmap[memx][offset])
        {
            cmemb++;//连续空内存块数增加
        }
		else
        {
            cmemb=0;								//连续内存块清零
        }

		if(cmemb==nmemb)							//找到了连续nmemb个空内存块
		{
            for(i=0;i<nmemb;i++)  					//标注内存块非空 
            {  
                mem_mgmt.memmap[memx][offset+i]=nmemb;  
            }  
            return (offset*mem_block_size[memx]);//返回偏移地址  
		}
    }

    return 0xFFFFFFFF;//未找到符合分配条件的内存块  
}


//释放内存(内部调用) 
//memx:所属内存块
//offset:内存地址偏移
//返回值:0,释放成功;1,释放失败;  
static U8_t mem_mgmt_free(U8_t memx,U32_t offset)  
{
    int i;

    if(!mem_mgmt.memrdy[memx])//未初始化,先执行初始化
	{
		mem_mgmt.init(memx);    
        return 1;//未初始化  
    }

    if(offset<mem_total_size[memx])//偏移在内存池内
    {
        int index=offset/mem_block_size[memx];			//偏移所在内存块号码  
        int nmemb=mem_mgmt.memmap[memx][index];	//内存块数量

        for(i=0;i<nmemb;i++)  						//内存块清零
        {  
            mem_mgmt.memmap[memx][index+i]=0;  
        }  
        return 0;  
    }
    else
    {
        return 2;//偏移超区了.  
    }

}

BOOL_t GM_initialize_memory(void)
{
	mem_mgmt_init(INTERAL_GEN_MEM);
	mem_mgmt_init(INTERAL_CCM_MEM);
	
	return BOOL_TRUE;
}

//释放内存(外部调用) 
//memx:所属内存块
//ptr:内存首地址 
void GM_free(U8_t memx,void *ptr)
{  
	U32_t offset; 
	if(ptr==NULL)
    {
        return;//地址为0.
    }

 	offset=(U32_t)ptr-(U32_t)mem_mgmt.membase[memx];     
    
    mem_mgmt_free(memx,offset);	//释放内存      
}

//分配内存(外部调用)
//memx:所属内存块
//size:内存大小(字节)
//返回值:分配到的内存首地址.
void *GM_malloc(U8_t memx,U32_t size)
{  
    U32_t offset = 0;

	offset=mem_mgmt_malloc(memx,size);  	   	 	   
    
    if(offset==0xFFFFFFFF)
    {
        return NULL;
    }  
    else
    {
        return (void*)((U32_t)mem_mgmt.membase[memx]+offset);
    }
}


//重新分配内存(外部调用)
//memx:所属内存块
//*ptr:旧内存首地址
//size:要分配的内存大小(字节)
//返回值:新分配到的内存首地址.
void *GM_realloc(U8_t memx,void *ptr,U32_t size)  
{  
    U32_t offset = 0;

    offset = mem_mgmt_malloc(memx,size);
    if(offset==0xFFFFFFFF)
    {
        return NULL;
    }
    else  
    {  									   
	    mem_mgmt_memcpy((void*)((U32_t)mem_mgmt.membase[memx]+offset),ptr,size);	//拷贝旧内存内容到新内存   
        GM_free(memx,ptr);  											  		//释放旧内存
        return (void*)((U32_t)mem_mgmt.membase[memx]+offset);  				    //返回新内存首地址
    }
}
