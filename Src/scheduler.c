/**
  ******************************************************************************
  * @file    scheduler.c
  * @author  WuYi
  * @brief   Application task scheduler.
  ******************************************************************************
**/

#include "main.h"

void AppScheduled(void)
{
  if (Timer_TimeBaseFlag.u16Timer1msFlag == TRUE)
  {
    Timer_TimeBaseFlag.u16Timer1msFlag = FALSE;
    timerForCan();
  }

  if (Timer_TimeBaseFlag.u16Timer10msFlag == TRUE)
  {
    Timer_TimeBaseFlag.u16Timer10msFlag = FALSE;
    App_GPIO();
  }

  if (Timer_TimeBaseFlag.u16Timer100msFlag == TRUE)
  {
    Timer_TimeBaseFlag.u16Timer100msFlag = FALSE;
    App_UART();
  }

  if (Timer_TimeBaseFlag.u16Timer1sFlag == TRUE)
  {
    Timer_TimeBaseFlag.u16Timer1sFlag = FALSE;
  }
}
