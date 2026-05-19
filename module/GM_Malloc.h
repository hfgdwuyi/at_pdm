#ifndef _GM_MALLOC_H_
#define _GM_MALLOC_H_
#include "data_type.h"
//#include "soc_f4_config.h" 

#ifndef NULL
#define NULL 0
#endif

/* Memory pool */
#define INTERAL_GEN_MEM		0	//Interal general memory pool
#define INTERAL_CCM_MEM		1	//Interal ccm memory pool
#define MEMORY_BANK_NUM		2	//定义支持的SRAM块数.

//mem1内存参数设定.mem1完全处于内部SRAM里面.
#define GEN_MEM_BLOCK_SIZE		64  	  							//内存块大小为64字节
#define GEN_MEM_MAX_SIZE		160*1024  							//最大管理内存 160K
#define GEN_MEM_TABLE_SIZE		GEN_MEM_MAX_SIZE/GEN_MEM_BLOCK_SIZE 	//内存表大小
		 
//mem3内存参数设定.mem3处于CCM,用于管理CCM(特别注意,这部分SRAM,仅CPU可以访问!!)
#define CCM_MEM_BLOCK_SIZE		64  	  						//内存块大小为64字节
#define CCM_MEM_MAX_SIZE		60 *1024  						//最大管理内存60K
#define CCM_MEM_TABLE_SIZE		CCM_MEM_MAX_SIZE/CCM_MEM_BLOCK_SIZE 	//内存表大小

//内存管理控制器
typedef struct _memory_mgmt_t
{
	void (*init)(U8_t);					//初始化
	U16_t (*perused)(U8_t);				//内存使用率
	U8_t *membase[MEMORY_BANK_NUM];		//内存池 管理SRAMBANK个区域的内存
	U32_t *memmap[MEMORY_BANK_NUM]; 	//内存管理状态表
	U8_t  memrdy[MEMORY_BANK_NUM]; 		//内存管理是否就绪
}memory_mgmt_t;

extern struct _m_mallco_dev mallco_dev;	 //在mallco.c里面定义

//用户调用函数
BOOL_t GM_initialize_memory(void);
void GM_free(U8_t memx,void *ptr);
void *GM_malloc(U8_t memx,U32_t size);
void *GM_realloc(U8_t memx,void *ptr,U32_t size);

#endif
