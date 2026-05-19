/**
  ******************************************************************************
  * @file    CAN/CAN_Networking/Src/stm32f4xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @defgroup CAN_Networking
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define I2C_ADDRESS_WRITE       0xA0
#define I2C_ADDRESS_READ				0xA1


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t TxBuffer1[4] = {0x00,0x00,0x55,0xaa};
uint8_t TxBuffer2[4] = {0x00,0x00,0xaa,0x55};
uint8_t TxBuffer3[4] = {0x00,0x00,0x66,0x99};
uint8_t RxBuffer[2];
uint8_t Reg[2] = {0x00,0x00};
/* Private function prototypes -----------------------------------------------*/

void App_I2C(void);
void Task_Calibration(uint8_t *TxBuffer);
/* Private functions ---------------------------------------------------------*/
void App_I2C(void)
{
	Task_Calibration(TxBuffer1);
	Task_Calibration(TxBuffer2);
	Task_Calibration(TxBuffer3);
}


void Task_Calibration(uint8_t *TxBuffer)
{
	/*##-2- Start the transmission process #####################################*/  
  /* While the I2C in reception process, user can transmit data through 
     "aTxBuffer" buffer */
  /* Timeout is set to 10S */
  while(HAL_I2C_Master_Transmit(&I2cHandle, (uint16_t)I2C_ADDRESS_WRITE, (uint8_t*)TxBuffer, 4, 10000)!= HAL_OK)
  {
    /* Error_Handler() function is called when Timeout error occurs.
       When Acknowledge failure occurs (Slave don't acknowledge it's address)
       Master restarts communication */
    if (HAL_I2C_GetError(&I2cHandle) != HAL_I2C_ERROR_AF)
    {
      Error_Handler();
    }
  }
  
  /*##-3- Put I2C peripheral in reception process ############################*/ 
  /* Timeout is set to 10S */ 
	
	while(HAL_I2C_Master_Transmit(&I2cHandle, (uint16_t)I2C_ADDRESS_WRITE, (uint8_t*)Reg, 2, 10000)!= HAL_OK)
  {
    /* Error_Handler() function is called when Timeout error occurs.
       When Acknowledge failure occurs (Slave don't acknowledge it's address)
       Master restarts communication */
    if (HAL_I2C_GetError(&I2cHandle) != HAL_I2C_ERROR_AF)
    {
      Error_Handler();
    }
  }
	
  while(HAL_I2C_Master_Receive(&I2cHandle, (uint16_t)I2C_ADDRESS_READ, (uint8_t *)RxBuffer, 2, 10000) != HAL_OK)
  {
    /* Error_Handler() function is called when Timeout error occurs.
       When Acknowledge failure occurs (Slave don't acknowledge it's address)
       Master restarts communication */
    if (HAL_I2C_GetError(&I2cHandle) != HAL_I2C_ERROR_AF)
    {
      Error_Handler();
    }   
  }
	
	if(RxBuffer[0] == TxBuffer[2]
		&&RxBuffer[1] == TxBuffer[3])
	{
		u8StatusLed = 6;
	}
	else
	{
		u8StatusLed = 0;
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
