/**
  ******************************************************************************
  * @file    Driver_gpio.c
  * @author  WuYi
  * @brief   GPIO peripheral initialization.
  ******************************************************************************
**/

#include "main.h"

void GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Outputs */
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_13;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_5 | GPIO_PIN_6 |
                        GPIO_PIN_11 | GPIO_PIN_15 | GPIO_PIN_14 | GPIO_PIN_13;
  HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_9;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_6 | GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3;
  HAL_GPIO_Init(GPIOK, &GPIO_InitStruct);

  /* Inputs */
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  (void)GPIO_Pin;
}
