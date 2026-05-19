/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @author  WuYi
  * @brief   Interrupt Service Routines and EXTI initialization.
  ******************************************************************************
**/

#include "main.h"
#include "stm32f4xx_it.h"
#include "data.h"
#include "canfestival.h"

extern UART_HandleTypeDef UartHandle;

/* Interrupt_Init declared externally via main.h */

/* EXTI configuration table: {port, pin, irq, preempt_prio, sub_prio} */
typedef struct {
  GPIO_TypeDef *port;
  uint16_t     pin;
  IRQn_Type    irq;
  uint8_t      preempt_prio;
  uint8_t      sub_prio;
} EXTIConfig;

static const EXTIConfig exti_configs[] = {
  {GPIOJ, GPIO_PIN_0,  EXTI0_IRQn,     2, 0},
  {GPIOK, GPIO_PIN_2,  EXTI2_IRQn,     2, 1},
  {GPIOA, GPIO_PIN_3,  EXTI3_IRQn,     2, 2},
  {GPIOD, GPIO_PIN_4,  EXTI4_IRQn,     2, 3},
  {GPIOD, GPIO_PIN_5,  EXTI9_5_IRQn,   2, 4},  /* shared IRQ */
  {GPIOG, GPIO_PIN_7,  EXTI9_5_IRQn,   2, 4},  /* shared IRQ */
  {GPIOI, GPIO_PIN_12, EXTI15_10_IRQn, 2, 5},
};

void Interrupt_Init(void)
{
  uint8_t i;
  for (i = 0; i < sizeof(exti_configs) / sizeof(exti_configs[0]); i++)
  {
    GPIO_InitTypeDef gpio = {0};
    gpio.Mode = GPIO_MODE_IT_FALLING;
    gpio.Pull = GPIO_NOPULL;
    gpio.Pin  = exti_configs[i].pin;
    HAL_GPIO_Init(exti_configs[i].port, &gpio);

    HAL_NVIC_SetPriority(exti_configs[i].irq,
                         exti_configs[i].preempt_prio,
                         exti_configs[i].sub_prio);
    HAL_NVIC_EnableIRQ(exti_configs[i].irq);
  }
}

/******************************************************************************/
/*                 Cortex-M4 Processor Exception Handlers                      */
/******************************************************************************/

void NMI_Handler(void) {}
void HardFault_Handler(void) { while (1) {} }
void MemManage_Handler(void) { while (1) {} }
void BusFault_Handler(void)  { while (1) {} }
void UsageFault_Handler(void) { while (1) {} }
void SVC_Handler(void) {}
void DebugMon_Handler(void) {}
void PendSV_Handler(void) {}

void SysTick_Handler(void)
{
  HAL_IncTick();
}

/******************************************************************************/
/*               STM32F4xx Peripheral Interrupt Handlers                       */
/******************************************************************************/

void TIM3_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&TimHandle);
}

void CAN1_RX0_IRQHandler(void)
{
  HAL_CAN_IRQHandler(&Can1Handle);
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *canHandle)
{
  Message m;
  if (HAL_CAN_GetRxMessage(canHandle, CAN_RX_FIFO0,
                           &packet.RxHeader, packet.RxMessage.all) == HAL_OK)
  {
    u8CanReceiveFlag = TRUE;
    HAL_CAN_ActivateNotification(canHandle, CAN_IT_RX_FIFO0_MSG_PENDING);

    CAN_Rx_Msg(0, &m.cob_id, NULL, &m.rtr, &m.len, m.data, packet);
    canDispatch(&MySlave_Data, &m);
  }
}

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *canHandle)
{
  (void)canHandle;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  (void)htim;

  Timer_TimeBaseFlag.u16Timer1msFlag = TRUE;
  Timer_TimeBaseCount.u16Timer1msCount++;

  if (Timer_TimeBaseCount.u16Timer1msCount >= 10)
  {
    Timer_TimeBaseFlag.u16Timer10msFlag = TRUE;
    Timer_TimeBaseCount.u16Timer1msCount = 0;
    Timer_TimeBaseCount.u16Timer10msCount++;
    Timer_CanTransmitFlag.u8Timer10msFlag = TRUE;

    Timer_TimeBaseCount.u16TimerOcpCount++;
    if (Timer_TimeBaseCount.u16TimerOcpCount >= 4)
      Timer_TimeBaseCount.u16TimerOcpCount = 0;
  }

  if (Timer_TimeBaseCount.u16Timer10msCount >= 10)
  {
    Timer_TimeBaseFlag.u16Timer100msFlag = TRUE;
    Timer_TimeBaseCount.u16Timer10msCount = 0;
    Timer_TimeBaseCount.u16Timer100msCount++;
    Timer_CanTransmitFlag.u8Timer100msFlag = TRUE;
  }

  if (Timer_TimeBaseCount.u16Timer100msCount >= 10)
  {
    Timer_TimeBaseFlag.u16Timer1sFlag = TRUE;
    Timer_TimeBaseCount.u16Timer100msCount = 0;
    Timer_CanTransmitFlag.u8Timer1sFlag = TRUE;
  }
}

/* EXTI IRQ Handlers */

void EXTI0_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

void EXTI2_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
}

void EXTI3_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
}

void EXTI4_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
}

void EXTI9_5_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
}

void EXTI15_10_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_12);
}

/* UART */

void USART1_IRQHandler(void)
{
  HAL_UART_IRQHandler(&UartHandle);
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  (void)UartHandle;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  while (HAL_UART_Receive_IT(UartHandle, (uint8_t *)RxMessage, 8) != HAL_OK) {}
  u8ReveiveValid = TRUE;
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{
  (void)UartHandle;
}
