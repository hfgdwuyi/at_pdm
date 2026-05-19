/**
  ******************************************************************************
  * @file    Src/App_ADC.c
  * @author  WuYi
  * @brief   ADC task.
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
FilterADCValueTypeDef FilterADCValue;
/* Private define ------------------------------------------------------------*/
#define ADC_I_24V 		ADC_CHANNEL_8
#define ADC_POWER_24V ADC_CHANNEL_7


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
uint32_t ReadAdc(uint32_t u32Channel);
void App_ADC(void);
/* Private functions ---------------------------------------------------------*/
void App_ADC(void)
{
	uint8_t temp1,temp2;
	temp1 = ReadAdc(ADC_I_24V) & 0xFF;
	temp2 = ReadAdc(ADC_I_24V) >> 8;
	FilterADCValue.u16I24V = temp2 << 8 | temp1;
	
	temp1 = ReadAdc(ADC_POWER_24V) & 0xFF;
	temp2 = ReadAdc(ADC_POWER_24V) >> 8;
	FilterADCValue.u16Power24V = temp2 << 8 | temp1;
	
//	temp1 = ReadAdc(ADC_I_24V);
//	FilterADCValue.u16I24V = (uint16_t)(((uint32_t)FilterADCValue.u16I24V * 1000 + (uint32_t)temp1 * 1000) >> 10);
}

/**
  * @brief CAN MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - NVIC configuration for DMA interrupt request enable
  * @param hcan: CAN handle pointer
  * @retval None
  */
uint32_t ReadAdc(uint32_t u32Channel)
{
	ADC_ChannelConfTypeDef sConfig;
	/*##-2- Configure ADC regular channel ######################################*/  
  sConfig.Channel      = u32Channel;
  sConfig.Rank         = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  sConfig.Offset       = 0;
  
  if(HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
  {
    /* Channel Configuration Error */
    Error_Handler();
  }
  

  /*##-3- Start the conversion process #######################################*/  
  if(HAL_ADC_Start(&AdcHandle) != HAL_OK)
  {
    /* Start Conversation Error */
    Error_Handler();
  }
  
  /*##-4- Wait for the end of conversion #####################################*/  
  HAL_ADC_PollForConversion(&AdcHandle, 10);
  
  /* Check if the continuous conversion of regular channel is finished */
  if((HAL_ADC_GetState(&AdcHandle) & HAL_ADC_STATE_EOC_REG) == HAL_ADC_STATE_EOC_REG)
  {
    /*##-5- Get the converted value of regular channel  ######################*/
    return HAL_ADC_GetValue(&AdcHandle);
  }
	return 0;
}


/*****END OF FILE****/
