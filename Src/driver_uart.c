/**
  ******************************************************************************
  * @file    Driver_uart.c
  * @author  WuYi
  * @brief   UART peripheral initialization.
  ******************************************************************************
**/

#include "main.h"

UART_HandleTypeDef UartHandle;

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
  (void)huart;

  GPIO_InitTypeDef GPIO_InitStruct;

  __HAL_RCC_USART1_CLK_ENABLE();

  GPIO_InitStruct.Pin       = GPIO_PIN_9;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin       = GPIO_PIN_10;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void UART_Init(void)
{
  UartHandle.Instance          = USART1;
  UartHandle.Init.BaudRate     = 9600;
  UartHandle.Init.WordLength   = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits     = UART_STOPBITS_1;
  UartHandle.Init.Parity       = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode         = UART_MODE_TX_RX;
  UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;

  if (HAL_UART_Init(&UartHandle) != HAL_OK)
    Error_Handler();

  HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART1_IRQn);

  while (HAL_UART_Receive_IT(&UartHandle, (uint8_t *)RxMessage, 1) != HAL_OK) {}
}
