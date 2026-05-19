/**
  ******************************************************************************
  * @file    App_I2C.c
  * @author  MCD Application Team
  * @brief   I2C application task (calibration).
  ******************************************************************************
**/

#include "main.h"
#include "app_i2c.h"

#define I2C_ADDRESS_WRITE  0xA0
#define I2C_ADDRESS_READ   0xA1

static uint8_t TxBuffer1[4] = {0x00, 0x00, 0x55, 0xaa};
static uint8_t TxBuffer2[4] = {0x00, 0x00, 0xaa, 0x55};
static uint8_t TxBuffer3[4] = {0x00, 0x00, 0x66, 0x99};
static uint8_t RxBuffer[2];
static uint8_t Reg[2] = {0x00, 0x00};

static void Task_Calibration(uint8_t *TxBuffer);

void App_I2C(void)
{
  Task_Calibration(TxBuffer1);
  Task_Calibration(TxBuffer2);
  Task_Calibration(TxBuffer3);
}

static void Task_Calibration(uint8_t *TxBuffer)
{
  while (HAL_I2C_Master_Transmit(&I2cHandle, (uint16_t)I2C_ADDRESS_WRITE,
                                  (uint8_t *)TxBuffer, 4, 10000) != HAL_OK)
  {
    if (HAL_I2C_GetError(&I2cHandle) != HAL_I2C_ERROR_AF)
      Error_Handler();
  }

  while (HAL_I2C_Master_Transmit(&I2cHandle, (uint16_t)I2C_ADDRESS_WRITE,
                                  (uint8_t *)Reg, 2, 10000) != HAL_OK)
  {
    if (HAL_I2C_GetError(&I2cHandle) != HAL_I2C_ERROR_AF)
      Error_Handler();
  }

  while (HAL_I2C_Master_Receive(&I2cHandle, (uint16_t)I2C_ADDRESS_READ,
                                 (uint8_t *)RxBuffer, 2, 10000) != HAL_OK)
  {
    if (HAL_I2C_GetError(&I2cHandle) != HAL_I2C_ERROR_AF)
      Error_Handler();
  }

  if (RxBuffer[0] == TxBuffer[2] && RxBuffer[1] == TxBuffer[3])
    u8StatusLed = 6;
  else
    u8StatusLed = 0;
}
