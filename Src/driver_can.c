/**
  ******************************************************************************
  * @file    Driver_can.c
  * @author  WuYi
  * @brief   CAN peripheral initialization for STM32F429.
  ******************************************************************************
**/

#include "main.h"

CAN_HandleTypeDef   Can1Handle;
CAN_HandleTypeDef   Can2Handle;
CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;

void HAL_CAN_MspInit(CAN_HandleTypeDef *hcan)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  __HAL_RCC_CAN1_CLK_ENABLE();
  __HAL_RCC_CAN2_CLK_ENABLE();

  /* CAN1 TX: PH13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /* CAN1 RX: PI9 */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /* CAN2 TX: PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Alternate = GPIO_AF9_CAN2;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* CAN2 RX: PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Alternate = GPIO_AF9_CAN2;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 1, 0);
  HAL_NVIC_SetPriority(CAN1_SCE_IRQn, 1, 1);
  HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
  HAL_NVIC_EnableIRQ(CAN1_SCE_IRQn);

  HAL_NVIC_SetPriority(CAN2_RX0_IRQn, 1, 0);
  HAL_NVIC_SetPriority(CAN2_SCE_IRQn, 1, 1);
  HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);
  HAL_NVIC_EnableIRQ(CAN2_SCE_IRQn);
}

static void CAN_ConfigHandle(CAN_HandleTypeDef *hcan, CAN_TypeDef *instance)
{
  CAN_FilterTypeDef sFilterConfig;

  hcan->Instance = instance;
  hcan->Init.TimeTriggeredMode = DISABLE;
  hcan->Init.AutoBusOff = ENABLE;
  hcan->Init.AutoWakeUp = DISABLE;
  hcan->Init.AutoRetransmission = ENABLE;
  hcan->Init.ReceiveFifoLocked = DISABLE;
  hcan->Init.TransmitFifoPriority = DISABLE;
  hcan->Init.Mode = CAN_MODE_NORMAL;
  hcan->Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan->Init.TimeSeg1 = CAN_BS1_6TQ;
  hcan->Init.TimeSeg2 = CAN_BS2_2TQ;
  hcan->Init.Prescaler = 20;

  if (HAL_CAN_Init(hcan) != HAL_OK)
  {
    Error_Handler();
  }

  sFilterConfig.FilterBank = 0;
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  sFilterConfig.FilterIdHigh = 0x0000;
  sFilterConfig.FilterIdLow = 0x0000;
  sFilterConfig.FilterMaskIdHigh = 0x0000;
  sFilterConfig.FilterMaskIdLow = 0x0000;
  sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
  sFilterConfig.FilterActivation = ENABLE;
  sFilterConfig.SlaveStartFilterBank = 14;

  if (HAL_CAN_ConfigFilter(hcan, &sFilterConfig) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_CAN_Start(hcan) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
  {
    Error_Handler();
  }
}

void CAN_Init(void)
{
  CAN_ConfigHandle(&Can1Handle, CAN1);
  CAN_ConfigHandle(&Can2Handle, CAN2);
}
