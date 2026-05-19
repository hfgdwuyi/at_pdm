/**
  ******************************************************************************
  * @file    Src/App_CAN.c
  * @author  WuYi
  * @brief   CAN task.
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "can.h"

/* Private typedef -----------------------------------------------------------*/
Timer_CanTransmitTypeDef Timer_CanTransmitFlag;
CAN_TxEnableTypeDef TxCommand;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t              Tx1Mailbox;
uint32_t              Tx2Mailbox;
uint8_t 							u8CanReceiveFlag;
uint8_t 							u8CanErrorFlag;
CAN_RxPacketTypeDef 	packet;
CAN_TxPacketTypeDef 	Tx_packet;
CAN_UPSDataTypeDef	UPSData;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void App_CAN(void);

/**
  * @brief CAN_Rx_Msg
  *        This function 
  * @param hcan: CAN handle pointer
  * @retval None
  */
void CAN_Rx_Msg(uint8_t u8fifox,
				uint16_t*id,
				uint8_t *ide,
				uint8_t *rtr,
				uint8_t *len,
				uint8_t dat[8],
				CAN_RxPacketTypeDef packet)
{
	uint8_t i;
//	*ide = packet.RxHeader.IDE;
//	if(*ide == 0)
//	{
//		*id = packet.RxHeader.ExtId >> 21;
//	}
//	else
//	{
//		*id = packet.RxHeader.StdId >> 3;
//	}
	*id = packet.RxHeader.StdId;
	*rtr = packet.RxHeader.RTR;
	*len = packet.RxHeader.DLC;
	
	for(i=0;i<8;i++)
	{
		dat[i] = packet.RxMessage.all[i];
	}
}

unsigned char CAN1_Tx_Msg(uint16_t*id,
						uint8_t *rtr,
						uint8_t *len,
						uint8_t dat[8],
						CAN_TxPacketTypeDef Tx_packet)
{
	uint16_t u8DelayTime = 0;
	uint8_t i;
	Tx_packet.TxHeader.DLC = *len;
	Tx_packet.TxHeader.RTR = *rtr;
	Tx_packet.TxHeader.StdId = *id;
	
	for(i=0;i<8;i++)
	{
		Tx_packet.TxMessage.all[i] = dat[i];
	}
	while((Tx1Mailbox = HAL_CAN_GetTxMailboxesFreeLevel(&Can1Handle)) == 0)
	{
		 u8DelayTime++;
		 if(u8DelayTime >= 10000)
		 {
		 	u8DelayTime = 0;
		 	return 0;
		 }
	}
	HAL_CAN_AddTxMessage(&Can1Handle, &Tx_packet.TxHeader, Tx_packet.TxMessage.all, &Tx1Mailbox);
	
	u8DelayTime = 0;
	

	return 1;
}

unsigned char CAN2_Tx_Msg(uint16_t*id,
						uint8_t *rtr,
						uint8_t *len,
						uint8_t dat[8],
						CAN_TxPacketTypeDef Tx_packet)
{
	uint16_t u8DelayTime = 0;
	uint8_t i;

	Tx_packet.TxHeader.DLC = *len;
	Tx_packet.TxHeader.RTR = *rtr;
	Tx_packet.TxHeader.StdId = *id;
	for(i=0;i<8;i++)
	{
		Tx_packet.TxMessage.all[i] = 0;
	}

	while((Tx2Mailbox = HAL_CAN_GetTxMailboxesFreeLevel(&Can2Handle)) == 0)
	{
		 u8DelayTime++;
		 if(u8DelayTime >= 10000)
		 {
		 	u8DelayTime = 0;
		 	return 0;
		 }
	}
	HAL_CAN_AddTxMessage(&Can2Handle, &Tx_packet.TxHeader, Tx_packet.TxMessage.all, &Tx2Mailbox);
	
	return 1;
}
		
	
void App_CAN(void)
{	
	if(Timer_CanTransmitFlag.u8Timer10msFlag == TRUE)
	{
		Timer_CanTransmitFlag.u8Timer10msFlag = FALSE;
	}
	
	if(Timer_CanTransmitFlag.u8Timer100msFlag == TRUE)
	{
		Timer_CanTransmitFlag.u8Timer100msFlag = FALSE;
	}
	
	if(Timer_CanTransmitFlag.u8Timer1sFlag == TRUE)
	{
		Timer_CanTransmitFlag.u8Timer1sFlag = FALSE;
	}
	
}


/*****END OF FILE****/
