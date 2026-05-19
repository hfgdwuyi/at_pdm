/**
  ******************************************************************************
  * @file    Driver_wdog.c
  * @author  WuYi
  * @brief   IWDG (Independent Watchdog) initialization.
  ******************************************************************************
**/

#include "main.h"

IWDG_HandleTypeDef IwdgHandle;

void WDG_Init(void)
{
  IwdgHandle.Instance = IWDG;
  IwdgHandle.Init.Prescaler = IWDG_PRESCALER_256;
  IwdgHandle.Init.Reload = 5000;

  if (HAL_IWDG_Init(&IwdgHandle) != HAL_OK)
  {
    Error_Handler();
  }
}
