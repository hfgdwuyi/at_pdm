/**
  ******************************************************************************
  * @file    App_UART.c
  * @author  WuYi
  * @brief   UART application task.
  ******************************************************************************
**/

#include "main.h"
#include "app_uart.h"

static uint8_t TxMessage[8];
static uint8_t *p = TxMessage;
       uint8_t RxMessage[8];
       uint8_t u8ReveiveValid;
static uint8_t u8RxCount;
static uint16_t u16PollTime;

static void Task_PowerMessage(void);

void App_UART(void)
{
  u16PollTime++;
  if (u16PollTime >= 20)
    u16PollTime = 0;

  if ((u16PollTime == 10 || u16PollTime == 0) && u8ReveiveValid)
  {
    u8ReveiveValid = FALSE;
    Task_PowerMessage();
  }
}

static void Task_PowerMessage(void)
{
  uint8_t i;
  for (i = 0; i < 8; i++)
    TxMessage[i] = RxMessage[i];
  HAL_UART_Transmit(&UartHandle, p, 8, 1000);
}

int fputc(int ch, FILE *f)
{
  (void)f;
  while ((USART1->SR & 0x40) == 0) {}
  USART1->DR = (uint8_t)ch;
  return ch;
}
