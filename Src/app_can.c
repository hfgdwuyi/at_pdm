/**
  ******************************************************************************
  * @file    App_CAN.c
  * @author  WuYi
  * @brief   CAN application task.
  ******************************************************************************
**/

#include "main.h"
#include "can.h"

Timer_CanTransmitTypeDef Timer_CanTransmitFlag;
CAN_TxEnableTypeDef      TxCommand;
CAN_RxPacketTypeDef      packet;
CAN_TxPacketTypeDef      Tx_packet;
CAN_UPSDataTypeDef       UPSData;

uint8_t  u8CanReceiveFlag;
uint8_t  u8CanErrorFlag;
uint32_t Tx1Mailbox;
uint32_t Tx2Mailbox;

void CAN_Rx_Msg(uint8_t u8fifox,
                uint16_t *id,
                uint8_t *ide,
                uint8_t *rtr,
                uint8_t *len,
                uint8_t dat[8],
                CAN_RxPacketTypeDef packet)
{
  uint8_t i;
  (void)u8fifox;
  (void)ide;

  *id  = packet.RxHeader.StdId;
  *rtr = packet.RxHeader.RTR;
  *len = packet.RxHeader.DLC;

  for (i = 0; i < 8; i++)
    dat[i] = packet.RxMessage.all[i];
}

static unsigned char CAN_Tx_Msg(CAN_HandleTypeDef *hcan,
                                uint16_t *id,
                                uint8_t *rtr,
                                uint8_t *len,
                                uint8_t dat[8],
                                CAN_TxPacketTypeDef *tx_pkt,
                                uint32_t *pTxMailbox)
{
  uint16_t delay = 0;
  uint8_t i;

  tx_pkt->TxHeader.DLC   = *len;
  tx_pkt->TxHeader.RTR   = *rtr;
  tx_pkt->TxHeader.StdId = *id;

  for (i = 0; i < 8; i++)
    tx_pkt->TxMessage.all[i] = dat[i];

  while (HAL_CAN_GetTxMailboxesFreeLevel(hcan) == 0)
  {
    delay++;
    if (delay >= 10000)
      return 0;
  }

  HAL_CAN_AddTxMessage(hcan, &tx_pkt->TxHeader, tx_pkt->TxMessage.all, pTxMailbox);
  return 1;
}

unsigned char CAN1_Tx_Msg(uint16_t *id, uint8_t *rtr, uint8_t *len,
                          uint8_t dat[8], CAN_TxPacketTypeDef Tx_packet)
{
  return CAN_Tx_Msg(&Can1Handle, id, rtr, len, dat, &Tx_packet, &Tx1Mailbox);
}

unsigned char CAN2_Tx_Msg(uint16_t *id, uint8_t *rtr, uint8_t *len,
                          uint8_t dat[8], CAN_TxPacketTypeDef Tx_packet)
{
  return CAN_Tx_Msg(&Can2Handle, id, rtr, len, dat, &Tx_packet, &Tx2Mailbox);
}

void App_CAN(void)
{
  if (Timer_CanTransmitFlag.u8Timer10msFlag == TRUE)
    Timer_CanTransmitFlag.u8Timer10msFlag = FALSE;

  if (Timer_CanTransmitFlag.u8Timer100msFlag == TRUE)
    Timer_CanTransmitFlag.u8Timer100msFlag = FALSE;

  if (Timer_CanTransmitFlag.u8Timer1sFlag == TRUE)
    Timer_CanTransmitFlag.u8Timer1sFlag = FALSE;
}
