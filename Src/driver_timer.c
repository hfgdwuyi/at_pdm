/**
  ******************************************************************************
  * @file    Driver_timer.c
  * @author  WuYi
  * @brief   Timer peripheral initialization.
  ******************************************************************************
**/

#include "main.h"

TIM_HandleTypeDef          TimHandle;
Timer_TimeBaseFlagTypeDef  Timer_TimeBaseFlag;
Timer_TimeBaseCountTypeDef Timer_TimeBaseCount;

void Timer_Init(void)
{
  uint32_t uwPrescalerValue = (uint32_t)((SystemCoreClock / 2) / 10000) - 1;

  TimHandle.Instance = TIM3;
  TimHandle.Init.Period = 10 - 1;
  TimHandle.Init.Prescaler = uwPrescalerValue;
  TimHandle.Init.ClockDivision = 0;
  TimHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
  TimHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

  if (HAL_TIM_Base_Init(&TimHandle) != HAL_OK)
    Error_Handler();

  if (HAL_TIM_Base_Start_IT(&TimHandle) != HAL_OK)
    Error_Handler();
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
  (void)htim;

  __HAL_RCC_TIM3_CLK_ENABLE();

  HAL_NVIC_SetPriority(TIM3_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(TIM3_IRQn);
}
