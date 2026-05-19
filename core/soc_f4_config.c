#include "data_type.h"
#include "soc_f4_config.h"
	 
/*设置NVIC分组 0~4 共5组*/		   
static void SOC_nvic_priority_group_config(U8_t group)	 
{
	U32_t temp1 = 0;
	U32_t temp2 = 0;

	temp2 = (~group)&0x07;		/*取后三位*/
	temp2 <<= 8;
	temp1 = SCB->AIRCR;			/*读取先前的设置*/
	temp1 &= 0x0000F8FF; 		/*清空先前分组*/
	temp1 |= 0x05FA0000; 		/*写入钥匙*/
	temp1 |= temp2;
	SCB->AIRCR = temp1;  		/*设置分组*/
}


/*NVIC初始化 抢占 相应 中断号 分组*/   
void SOC_nvic_init(U8_t preemption_priority, U8_t sub_priority, U8_t channel, U8_t group)
{ 
	U32_t temp;
	SOC_nvic_priority_group_config(group);			/*设置分组*/
	temp = preemption_priority<<(4-group);
	temp |= sub_priority&(0x0F>>group);
	temp &= 0xF;									/*取低四位*/
	NVIC->ISER[channel/32] |= 1<<channel%32;		/*使能中断位(要清除的话,设置ICER对应位为1即可)*/
	NVIC->IP[channel] |= temp<<4;					/*设置响应优先级和抢断优先级*/	    	  				   
}






