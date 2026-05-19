/**
  ******************************************************************************
  * @file    Src/stm32f4xx_it.c
  * @author  WuYi
  * @brief   Main Interrupt Service Routines.
  *          This file provides rounts for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
#include "data.h"
#include "canfestival.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern UART_HandleTypeDef UartHandle;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void EXTILine0_Init(void);
static void EXTILine2_Init(void);
static void EXTILine3_Init(void);
static void EXTILine4_Init(void);
static void EXTILine9_5_Init(void);
static void EXTILine15_10_Init(void);
/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

//EasonWu:initialize all interrupt
void Interrupt_Init(void)
{
	EXTILine0_Init();
	EXTILine2_Init();
	EXTILine3_Init();
	EXTILine4_Init();
	EXTILine9_5_Init();
	EXTILine15_10_Init();
}
/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles TIM interrupt request.
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void)
{
  	HAL_TIM_IRQHandler(&TimHandle);
}



/**
* @brief  This function handles CAN1 RX0 interrupt request.
* @param  None
* @retval None
*/
void CAN1_RX0_IRQHandler(void)
{
  HAL_CAN_IRQHandler(&Can1Handle);
}

/**
  * @brief  Rx Fifo 0 message pending callback
  * @param  hcan: pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval None
  */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *canHandle)
{
	Message m;
	uint8_t *ide;
	// CO_Data* d;
	if (HAL_CAN_GetRxMessage(canHandle, CAN_RX_FIFO0, &packet.RxHeader, packet.RxMessage.all) == HAL_OK) 
	{	
		u8CanReceiveFlag = TRUE;
		HAL_CAN_ActivateNotification(canHandle, CAN_IT_RX_FIFO0_MSG_PENDING); 
		
		CAN_Rx_Msg(0,
							&m.cob_id,
							ide,
							&m.rtr,
							&m.len,
							m.data,
							packet);
		canDispatch(&MySlave_Data, &m);
	}
}


void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *canHandle)
{
//	u8CanErrorFlag = 1;
//	CAN_IT_ERROR
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @param  htim: TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	//EasonWu:set 10ms timer flag
  Timer_TimeBaseFlag.u16Timer1msFlag = TRUE;
	Timer_TimeBaseCount.u16Timer1msCount++;
	if(Timer_TimeBaseCount.u16Timer1msCount >= 10)
	{
		Timer_TimeBaseFlag.u16Timer10msFlag = TRUE;
		Timer_TimeBaseCount.u16Timer1msCount = 0;
		Timer_TimeBaseCount.u16Timer10msCount++;
		Timer_CanTransmitFlag.u8Timer10msFlag = TRUE;
		
		//EasonWu:clear flag if time out is 40ms
		Timer_TimeBaseCount.u16TimerOcpCount++;
		if(Timer_TimeBaseCount.u16TimerOcpCount >= 4)
		{
			Timer_TimeBaseCount.u16TimerOcpCount = 0;
		}
	}
	
	//EasonWu:set 100ms timer flag
	if(Timer_TimeBaseCount.u16Timer10msCount >= 10)
	{
		Timer_TimeBaseFlag.u16Timer100msFlag = TRUE;
		Timer_TimeBaseCount.u16Timer10msCount = 0;
		Timer_TimeBaseCount.u16Timer100msCount++;
		Timer_CanTransmitFlag.u8Timer100msFlag = TRUE;
	}
	
	//EasonWu:set 1000ms timer flag
	if(Timer_TimeBaseCount.u16Timer100msCount >= 10)
	{
		Timer_TimeBaseFlag.u16Timer1sFlag = TRUE;
		Timer_TimeBaseCount.u16Timer100msCount = 0;
		Timer_CanTransmitFlag.u8Timer1sFlag = TRUE;
	}
}

/**
  * @brief  This function handles External line 0 interrupt request.
  * @param  none
  * @retval None
  */
static void EXTILine0_Init(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;
  
  /* Configure PJ0(OCP_AC_23) pin as input floating */
  GPIO_InitStructure.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Pin = GPIO_PIN_0;
  HAL_GPIO_Init(GPIOJ, &GPIO_InitStructure);

  /* Enable and set EXTI Line0 Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

/**
  * @brief  This function handles External line 2 interrupt request.
  * @param  none
  * @retval None
  */
static void EXTILine2_Init(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;
  
  /* Configure PK2(OCP_AC_13) pin as input floating */
  GPIO_InitStructure.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Pin = GPIO_PIN_2;
  HAL_GPIO_Init(GPIOK, &GPIO_InitStructure);

  /* Enable and set EXTI Line2 Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(EXTI2_IRQn, 2, 1);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);
}

/**
  * @brief  This function handles External line 3 interrupt request.
  * @param  none
  * @retval None
  */
static void EXTILine3_Init(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;
  
  /* Configure PA3(OCP_AC_21) pin as input floating */
  GPIO_InitStructure.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Pin = GPIO_PIN_3;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Enable and set EXTI Line3 Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(EXTI3_IRQn, 2, 2);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);
}

/**
  * @brief  This function handles External line 4 interrupt request.
  * @param  none
  * @retval None
  */
static void EXTILine4_Init(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;
  
  /* Configure PD4(OCP_AC_11) pin as input floating */
  GPIO_InitStructure.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Pin = GPIO_PIN_4;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);

  /* Enable and set EXTI Line4 Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(EXTI4_IRQn, 2, 3);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);
}

/**
  * @brief  This function handles External line 9_5 interrupt request.
  * @param  none
  * @retval None
  */
static void EXTILine9_5_Init(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;
  
  /* Configure PD5(OCP_AC_22) pin as input floating */
  GPIO_InitStructure.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Pin = GPIO_PIN_5;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	/* Configure PG7(OCP_AC_12) pin as input floating */
  GPIO_InitStructure.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Pin = GPIO_PIN_7;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);

  /* Enable and set EXTI Line5-9 Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 2, 4);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

/**
  * @brief  This function handles External line 15_10 interrupt request.
  * @param  none
  * @retval None
  */
static void EXTILine15_10_Init(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;
  
	/* Configure PI12(OCP_24) pin as input floating */
	GPIO_InitStructure.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  GPIO_InitStructure.Pin = GPIO_PIN_12;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStructure);

  /* Enable and set EXTI Line10-15 Interrupt to the lowest priority */
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 5);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

/**
  * @brief  This function handles External line 0 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}
/**
  * @brief  This function handles External line 2 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI2_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
}
/**
  * @brief  This function handles External line 3 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI3_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
}
/**
  * @brief  This function handles External line 4 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI4_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
}
/**
  * @brief  This function handles External line 15_10 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
}
/**
  * @brief  This function handles External line 9_5 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI9_5_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
}

/**
  * @brief  This function handles UART interrupt request.  
  * @param  None
  * @retval None
  * @Note   This function is redefined in "main.h" and related to DMA stream 
  *         used for USART data transmission     
  */
void USART1_IRQHandler(void)
{
  HAL_UART_IRQHandler(&UartHandle);
	
}

/**
  * @brief  Tx Transfer completed callback
  * @param  UartHandle: UART handle. 
  * @note   This example shows a simple way to report end of IT Tx transfer, and 
  *         you can add your own implementation. 
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
}

/**
  * @brief  Rx Transfer completed callback
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report end of IT Rx transfer, and 
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	while(HAL_UART_Receive_IT(UartHandle, (uint8_t *)RxMessage, 8) != HAL_OK);
	u8ReveiveValid = TRUE;
	
}

/**
  * @brief  UART error callbacks
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report transfer error, and you can
  *         add your own implementation.
  * @retval None
  */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{

}


/*****END OF FILE****/
