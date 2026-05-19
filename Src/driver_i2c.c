/**
  ******************************************************************************
  * @file    Driver_i2c.c
  * @author  MCD Application Team
  * @brief   I2C peripheral initialization.
  ******************************************************************************
**/

#include "main.h"

I2C_HandleTypeDef I2cHandle;

void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
  (void)hi2c;

  GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_InitStruct.Pin       = GPIO_PIN_6;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin       = GPIO_PIN_7;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  __HAL_RCC_I2C1_CLK_ENABLE();
}

void I2C_Init(void)
{
  I2cHandle.Instance             = I2C1;
  I2cHandle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
  I2cHandle.Init.ClockSpeed      = 100000;
  I2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  I2cHandle.Init.DutyCycle       = I2C_DUTYCYCLE_16_9;
  I2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  I2cHandle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
  I2cHandle.Init.OwnAddress1     = 0x00;
  I2cHandle.Init.OwnAddress2     = 0xFE;

  if (HAL_I2C_Init(&I2cHandle) != HAL_OK)
    Error_Handler();
}
