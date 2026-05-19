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
#define MAX146_ADC_CHANNEL_IAC11	0x80//0b1000
#define MAX146_ADC_CHANNEL_IAC12	0xc0//0b1100
#define MAX146_ADC_CHANNEL_IAC13	0x90//0b1001
#define MAX146_ADC_CHANNEL_IAC21	0xd0//0b1101
#define MAX146_ADC_CHANNEL_IAC22	0xa0//0b1010
#define MAX146_ADC_CHANNEL_IAC23	0xe0//0b1110

#define NUM 10
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t u16ReceiveAdcValue[8];
uint16_t u16RawAdcValue;
/* Private function prototypes -----------------------------------------------*/
void App_SPI(void);
void Task_ReceiveAdcValue(void);
uint16_t Task_FilterAdcValue(uint16_t u16AdcValue);
/* Private functions ---------------------------------------------------------*/
void App_SPI(void)
{
	Task_ReceiveAdcValue();

	// FilterADCValue.u16IAc11 = Task_FilterAdcValue(u16ReceiveAdcValue[0]);
	// FilterADCValue.u16IAc12 = Task_FilterAdcValue(u16ReceiveAdcValue[1]);
	// FilterADCValue.u16IAc13 = Task_FilterAdcValue(u16ReceiveAdcValue[2]);
	// FilterADCValue.u16IAc21 = Task_FilterAdcValue(u16ReceiveAdcValue[3]);
	// FilterADCValue.u16IAc22 = Task_FilterAdcValue(u16ReceiveAdcValue[4]);
}

void Task_ReceiveAdcValue()
{
	static uint8_t i = MAX146_ADC_CHANNEL_IAC11,j = 0;
	uint8_t temp1,temp2;
	uint8_t SpiCommand;
	switch(i)
	{
		case MAX146_ADC_CHANNEL_IAC11:
			SpiCommand = MAX146_ADC_CHANNEL_IAC11 | 0x0f;
			i = MAX146_ADC_CHANNEL_IAC12; 
			j = 0;
		break;
		
		case MAX146_ADC_CHANNEL_IAC12:
			SpiCommand = MAX146_ADC_CHANNEL_IAC12 | 0x0f;
			i = MAX146_ADC_CHANNEL_IAC13; 
			j = 1;
		break;
		
		case MAX146_ADC_CHANNEL_IAC13:
			SpiCommand = MAX146_ADC_CHANNEL_IAC13 | 0x0f;
			i = MAX146_ADC_CHANNEL_IAC21; 
			j = 2;
		break;
		
		case MAX146_ADC_CHANNEL_IAC21:
			SpiCommand = MAX146_ADC_CHANNEL_IAC21 | 0x0f;
			i = MAX146_ADC_CHANNEL_IAC22; 
			j = 3;
		break;
		
		case MAX146_ADC_CHANNEL_IAC22:
			SpiCommand = MAX146_ADC_CHANNEL_IAC22 | 0x0f;
			i = MAX146_ADC_CHANNEL_IAC23; 
			j = 4;
		break;
		
		case MAX146_ADC_CHANNEL_IAC23:
			SpiCommand = MAX146_ADC_CHANNEL_IAC23 | 0x0f;
			i = MAX146_ADC_CHANNEL_IAC11; 
			j = 5;
		break;
		
		default:
			break;
		
	}
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4,GPIO_PIN_RESET);
	aTxBuffer[0] = SpiCommand;
	if(HAL_SPI_TransmitReceive(&SpiHandle, (uint8_t*)aTxBuffer, (uint8_t*)aRxBuffer, 1,
                                          5000) != HAL_ERROR)
	{		
	}
	
	aTxBuffer[0] = 0x00;
	if(HAL_SPI_TransmitReceive(&SpiHandle, (uint8_t*)aTxBuffer, (uint8_t*)aRxBuffer, 1,
                                          5000) != HAL_ERROR)
	{		
		temp1 = aRxBuffer[0];
	}

	aTxBuffer[0] = 0x00;
	if(HAL_SPI_TransmitReceive(&SpiHandle, (uint8_t*)aTxBuffer, (uint8_t*)aRxBuffer, 1,
                                          5000) != HAL_ERROR)
	{		
		temp2 = aRxBuffer[0];
	}
	u16ReceiveAdcValue[j] = (temp1 << 8 | temp2) >> 4;
	
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4,GPIO_PIN_SET);
}

uint16_t Task_FilterAdcValue(uint16_t u16AdcValue)
{
	static uint16_t u16Buf[NUM];
	static uint16_t u16Sum;
	static uint16_t index;
	u16Sum += u16AdcValue - u16Buf[index];
	u16Buf[index] = u16AdcValue;
	index++;
	
	if(index >= NUM)	index = 0;
	if(index == 0) return (u16Sum/NUM);
	else 
		return (u16Sum/index);
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
