/**
  ******************************************************************************
  * @file    Driver_adc.c
  * @author  WuYi
  * @brief   ADC peripheral initialization.
  ******************************************************************************
**/

#include "main.h"

ADC_HandleTypeDef AdcHandle;

void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
  (void)hadc;

  GPIO_InitTypeDef GPIO_InitStruct;

  __HAL_RCC_ADC3_CLK_ENABLE();

  GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
}

void ADC_Init(void)
{
  AdcHandle.Instance                   = ADC3;
  AdcHandle.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV2;
  AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;
  AdcHandle.Init.ScanConvMode          = DISABLE;
  AdcHandle.Init.ContinuousConvMode    = DISABLE;
  AdcHandle.Init.DiscontinuousConvMode = DISABLE;
  AdcHandle.Init.NbrOfDiscConversion   = 0;
  AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
  AdcHandle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;
  AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  AdcHandle.Init.NbrOfConversion       = 1;
  AdcHandle.Init.DMAContinuousRequests = DISABLE;
  AdcHandle.Init.EOCSelection          = DISABLE;

  if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
    Error_Handler();
}
