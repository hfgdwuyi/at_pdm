/**
  ******************************************************************************
  * @file    Src/stm32f4xx_hal_msp.c
  * @author  WuYi
  * @brief   HAL MSP module.
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void GPIO_Init(void);


/**
  * @brief  Configures the GPIO.
  * @param  None
  * @retval None
  */
void GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
/* Configure GPIO for output*/  
	GPIO_InitStruct.Pin = (GPIO_PIN_0 | GPIO_PIN_1);
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct); 

  GPIO_InitStruct.Pin = (GPIO_PIN_13);
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct); 

	GPIO_InitStruct.Pin = (GPIO_PIN_2 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_11 | GPIO_PIN_15 | GPIO_PIN_14 | GPIO_PIN_13);
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct); 

  GPIO_InitStruct.Pin = (GPIO_PIN_9);
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct); 
	
	GPIO_InitStruct.Pin = (GPIO_PIN_1 | GPIO_PIN_6 | GPIO_PIN_5 | GPIO_PIN_4 | GPIO_PIN_3);
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOK, &GPIO_InitStruct); 
	

/* Configure GPIO for input*/  
	GPIO_InitStruct.Pin = (GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_15);
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOI, &GPIO_InitStruct); 
}

/**
  * @brief  Interrupt routine for the GPIO peripheral
  * @param  GPIO_Pin: specifies the GPIO peripheral
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
// 	Timer_TimeBaseCount.u16TimerOcpCount = 0;
//   if(STATUS_OCP_24V == RESET)
//   {
//     GPIO_FilterCount.u16Ocp24vFilterCount++;
// 		if(GPIO_FilterCount.u16Ocp24vFilterCount >= 5)
// 		{
// 			GPIO_FilterCount.u16Ocp24vFilterCount = 0;
// 			FaultFlag.b.u16FaultOcp24vFlag = TRUE;
// 			OPEN_K9_RELAY;
// //			FaultFlag.u16PowerState = 2;
// 		}
//   }
  
//   if(STATUS_OCP_AC11 == RESET)
//   {
// 		GPIO_FilterCount.u16OcpAc11FilterCount++;
// 		if(GPIO_FilterCount.u16OcpAc11FilterCount >= 5)
// 		{
// 			GPIO_FilterCount.u16OcpAc11FilterCount = 0;
// 			FaultFlag.b.u16FaultOcpAc11Flag = TRUE;
// 			OPEN_K3_RELAY;
// //			FaultFlag.u16PowerState = 3;
// 		}
//   }
	
// 	if(STATUS_OCP_AC12 == RESET)
//   {
// 		GPIO_FilterCount.u16OcpAc12FilterCount++;
// 		if(GPIO_FilterCount.u16OcpAc12FilterCount >= 5)
// 		{
// 			GPIO_FilterCount.u16OcpAc12FilterCount = 0;
// 			FaultFlag.b.u16FaultOcpAc12Flag = TRUE;
// //			FaultFlag.u16PowerState = 4;
// 		}
//   }
	
// 	if(STATUS_OCP_AC13 == RESET)
//   {
// 		GPIO_FilterCount.u16OcpAc13FilterCount++;
// 		if(GPIO_FilterCount.u16OcpAc13FilterCount >= 5)
// 		{
// 			GPIO_FilterCount.u16OcpAc13FilterCount = 0;
// 			FaultFlag.b.u16FaultOcpAc13Flag = TRUE;
// 			OPEN_K6_RELAY;
// //			FaultFlag.u16PowerState = 5;
// 		}
//   }
	
// 	if(STATUS_OCP_AC21 == RESET)
//   {
// 		GPIO_FilterCount.u16OcpAc21FilterCount++;
// 		if(GPIO_FilterCount.u16OcpAc21FilterCount >= 5)
// 		{
// 			GPIO_FilterCount.u16OcpAc21FilterCount = 0;
// 			FaultFlag.b.u16FaultOcpAc21Flag = TRUE;
// 			OPEN_K7_RELAY;
// //			FaultFlag.u16PowerState = 6;
// 		}
//   }
	
// 	if(STATUS_OCP_AC22 == RESET)
//   {
// 		GPIO_FilterCount.u16OcpAc22FilterCount++;
// 		if(GPIO_FilterCount.u16OcpAc22FilterCount >= 5)
// 		{
// 			GPIO_FilterCount.u16OcpAc22FilterCount = 0;
// 			FaultFlag.b.u16FaultOcpAc22Flag = TRUE;
// 			OPEN_K5_RELAY;
// //			FaultFlag.u16PowerState = 7;
// 		}
//   }
	
// 	if(STATUS_OCP_AC23 == RESET)
//   {
// 		GPIO_FilterCount.u16OcpAc23FilterCount++;
// 		if(GPIO_FilterCount.u16OcpAc23FilterCount >= 5)
// 		{
// 			GPIO_FilterCount.u16OcpAc23FilterCount = 0;
// 			FaultFlag.b.u16FaultOcpAc23Flag = TRUE;
// 			OPEN_K10_RELAY;
// //			FaultFlag.u16PowerState = 8;
// 		}
//   }
}

/*****END OF FILE****/
