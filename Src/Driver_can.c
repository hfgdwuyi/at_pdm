/**
  ******************************************************************************
  * @file    Src/stm32f4xx_hal_msp.c
  * @author  WuYi
  * @brief   HAL MSP module.
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
CAN_HandleTypeDef     Can1Handle;
CAN_HandleTypeDef     Can2Handle;
CAN_TxHeaderTypeDef   TxHeader;
CAN_RxHeaderTypeDef   RxHeader;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void CAN_Init(void);

/**
  * @brief CAN MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - NVIC configuration for DMA interrupt request enable
  * @param hcan: CAN handle pointer
  * @retval None
  */
void HAL_CAN_MspInit(CAN_HandleTypeDef *hcan)
{
  GPIO_InitTypeDef   GPIO_InitStruct;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* CAN1 Periph clock enable */
  __HAL_RCC_CAN1_CLK_ENABLE();
  __HAL_RCC_CAN2_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* CAN1 TX GPIO pin configuration */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Alternate =  GPIO_AF9_CAN1;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /* CAN1 RX GPIO pin configuration */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Alternate =  GPIO_AF9_CAN1;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /* CAN2 TX GPIO pin configuration */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Alternate =  GPIO_AF9_CAN2;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* CAN2 RX GPIO pin configuration */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Alternate =  GPIO_AF9_CAN2;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*##-3- Configure the NVIC #################################################*/
  /* NVIC configuration for CAN1 Reception complete interrupt */
  HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 1, 0);
	HAL_NVIC_SetPriority(CAN1_SCE_IRQn, 1, 1);
  HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
	HAL_NVIC_EnableIRQ(CAN1_SCE_IRQn);

  HAL_NVIC_SetPriority(CAN2_RX0_IRQn, 1, 0);
	HAL_NVIC_SetPriority(CAN2_SCE_IRQn, 1, 1);
  HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);
	HAL_NVIC_EnableIRQ(CAN2_SCE_IRQn);
}


/**
  * @brief  Configures the CAN.
  * @param  None
  * @retval None
  */
void CAN_Init(void)
{
  CAN_FilterTypeDef  sFilterConfig;

  /*##-1- Configure the CAN peripheral #######################################*/
  Can1Handle.Instance = CAN1;

  Can1Handle.Init.TimeTriggeredMode = DISABLE;
  Can1Handle.Init.AutoBusOff = ENABLE;
  Can1Handle.Init.AutoWakeUp = DISABLE;
  Can1Handle.Init.AutoRetransmission = ENABLE;
  Can1Handle.Init.ReceiveFifoLocked = DISABLE;
  Can1Handle.Init.TransmitFifoPriority = DISABLE;
  Can1Handle.Init.Mode = CAN_MODE_NORMAL;
  Can1Handle.Init.SyncJumpWidth = CAN_SJW_1TQ;
  Can1Handle.Init.TimeSeg1 = CAN_BS1_6TQ;
  Can1Handle.Init.TimeSeg2 = CAN_BS2_2TQ;
  Can1Handle.Init.Prescaler = 20;

  if (HAL_CAN_Init(&Can1Handle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-2- Configure the CAN Filter ###########################################*/
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

  if (HAL_CAN_ConfigFilter(&Can1Handle, &sFilterConfig) != HAL_OK)
  {
    /* Filter configuration Error */
    Error_Handler();
  }

  /*##-3- Start the CAN peripheral ###########################################*/
  if (HAL_CAN_Start(&Can1Handle) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }

  /*##-4- Activate CAN RX notification #######################################*/
  if (HAL_CAN_ActivateNotification(&Can1Handle, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
  {
    /* Notification Error */
    Error_Handler();
  }


  Can2Handle.Instance = CAN2;

  Can2Handle.Init.TimeTriggeredMode = DISABLE;
  Can2Handle.Init.AutoBusOff = ENABLE;
  Can2Handle.Init.AutoWakeUp = DISABLE;
  Can2Handle.Init.AutoRetransmission = ENABLE;
  Can2Handle.Init.ReceiveFifoLocked = DISABLE;
  Can2Handle.Init.TransmitFifoPriority = DISABLE;
  Can2Handle.Init.Mode = CAN_MODE_NORMAL;
  Can2Handle.Init.SyncJumpWidth = CAN_SJW_1TQ;
  Can2Handle.Init.TimeSeg1 = CAN_BS1_6TQ;
  Can2Handle.Init.TimeSeg2 = CAN_BS2_2TQ;
  Can2Handle.Init.Prescaler = 20;

  if (HAL_CAN_Init(&Can2Handle) != HAL_OK)
  {
    /* Initialization Error */
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

  if (HAL_CAN_ConfigFilter(&Can2Handle, &sFilterConfig) != HAL_OK)
  {
    /* Filter configuration Error */
    Error_Handler();
  }

  /*##-3- Start the CAN peripheral ###########################################*/
  if (HAL_CAN_Start(&Can2Handle) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }

  /*##-4- Activate CAN RX notification #######################################*/
  if (HAL_CAN_ActivateNotification(&Can2Handle, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
  {
    /* Notification Error */
    Error_Handler();
  }

}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
