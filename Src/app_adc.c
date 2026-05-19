/**
  ******************************************************************************
  * @file    App_ADC.c
  * @author  WuYi
  * @brief   ADC application task.
  ******************************************************************************
**/

#include "main.h"
#include "app_adc.h"

FilterADCValueTypeDef FilterADCValue;

#define ADC_I_24V      ADC_CHANNEL_8
#define ADC_POWER_24V  ADC_CHANNEL_7

static uint32_t ReadAdc(uint32_t u32Channel);

void App_ADC(void)
{
  uint8_t temp1, temp2;
  temp1 = ReadAdc(ADC_I_24V) & 0xFF;
  temp2 = ReadAdc(ADC_I_24V) >> 8;
  FilterADCValue.u16I24V = (uint16_t)(temp2 << 8) | temp1;

  temp1 = ReadAdc(ADC_POWER_24V) & 0xFF;
  temp2 = ReadAdc(ADC_POWER_24V) >> 8;
  FilterADCValue.u16Power24V = (uint16_t)(temp2 << 8) | temp1;
}

static uint32_t ReadAdc(uint32_t u32Channel)
{
  ADC_ChannelConfTypeDef sConfig;

  sConfig.Channel      = u32Channel;
  sConfig.Rank         = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  sConfig.Offset       = 0;

  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
    Error_Handler();

  if (HAL_ADC_Start(&AdcHandle) != HAL_OK)
    Error_Handler();

  HAL_ADC_PollForConversion(&AdcHandle, 10);

  if ((HAL_ADC_GetState(&AdcHandle) & HAL_ADC_STATE_EOC_REG) == HAL_ADC_STATE_EOC_REG)
    return HAL_ADC_GetValue(&AdcHandle);

  return 0;
}
