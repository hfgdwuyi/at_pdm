/**
  ******************************************************************************
  * @file    CAN/CAN_Networking/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MSP_TIMER_H
#define __MSP_TIMER_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
/* Exported types ------------------------------------------------------------*/
typedef struct
{
  uint16_t u16Timer1msFlag;    /*!< 1ms explore flag.
                          		  */

  uint16_t u16Timer10msFlag;   /*!< 10ms explore flag.
                          		  */
	
  uint16_t u16Timer100msFlag;  /*!< 100ms explore flag.
                          		  */

  uint16_t u16Timer1sFlag;     /*!< 1s explore flag.
                          		  */

} Timer_TimeBaseFlagTypeDef;

typedef struct
{
  uint16_t u16Timer1msCount;    /*!< 1ms explore count.
                          		  */

  uint16_t u16Timer10msCount;   /*!< 10ms explore count.
                          		  */

  uint16_t u16Timer100msCount;  /*!< 100ms explore count.
                          		  */

  uint16_t u16Timer1sCount;     /*!< 1s explore count.
                          		  */
	
	uint16_t u16TimerOcpCount;
	

} Timer_TimeBaseCountTypeDef;


/* Exported constants --------------------------------------------------------*/



/* Exported macro ------------------------------------------------------------*/
extern Timer_TimeBaseFlagTypeDef 	Timer_TimeBaseFlag;
extern Timer_TimeBaseCountTypeDef Timer_TimeBaseCount;
/* Exported functions ------------------------------------------------------- */


#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
