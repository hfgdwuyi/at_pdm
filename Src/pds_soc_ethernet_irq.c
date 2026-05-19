
/**
  ******************************************************************************
  * @file    main.c
  * @author  WuYi
  * @brief   This file sets up the ethnernet interrupt.
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "stm32f4x7_eth.h"
/* Public typedef -----------------------------------------------------------*/
extern ETH_DMADESCTypeDef  *DMARxDescToGet;
/* Public function prototypes -----------------------------------------------*/
extern void lwip_packet_handler(void);		//在lwip_comm.c里面定义

/**
  * @brief  Ethneter interrupt program.
  * @param  None
  * @retval None
  */
//以太网中断服务函数
void ETH_IRQHandler(void)
{ 
	while(ETH_GetRxPktSize(DMARxDescToGet)!=0) 	//检测是否收到数据包
	{ 		
		lwip_packet_handler();
	}

	ETH_DMAClearITPendingBit(ETH_DMA_IT_R);
	ETH_DMAClearITPendingBit(ETH_DMA_IT_NIS); 
}

