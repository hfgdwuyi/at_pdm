/**
  ******************************************************************************
  * @file    main.h
  * @author  WuYi
  * @brief   Header for main.c module
  ******************************************************************************
**/

#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stdio.h"
#include "msp_timer.h"
#include "app_can.h"
#include "app_gpio.h"
#include "app_uart.h"

#include "GM_Malloc.h"
#include "lwip_comm.h"
#include "lwip/snmp_msg.h"
#include "pds_soc_tick.h"

#include "MySlave.h"
#include "applicfg.h"
#include "MyMaster.h"
#include "canfestival.h"

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  uint16_t u16MeBoxK1On;
  uint16_t u16MeBoxK2On;
  uint16_t u16MeBoxK3On;
  uint16_t u16MeBoxK850On;
  uint16_t u16PfcOk;
  uint16_t u16UpsMode;
} ControlBoardStatusTypeDef;

/* Exported constants --------------------------------------------------------*/
extern CAN_HandleTypeDef   Can1Handle;
extern CAN_HandleTypeDef   Can2Handle;
extern CAN_TxHeaderTypeDef TxHeader;
extern CAN_RxHeaderTypeDef RxHeader;
extern UART_HandleTypeDef  UartHandle;
extern IWDG_HandleTypeDef  IwdgHandle;

extern uint8_t aRxBuffer[4];
extern uint8_t aTxBuffer[4];

/* Exported macro ------------------------------------------------------------*/
#define TRUE  1
#define FALSE 0

#define IO_HIGH_LEVEL 1
#define IO_LOW_LEVEL  0

/* Exported functions ------------------------------------------------------- */
extern void GPIO_Init(void);
extern void Timer_Init(void);
extern void CAN_Init(void);
extern void ADC_Init(void);
extern void SPI_Init(void);
extern void UART_Init(void);
extern void I2C_Init(void);
extern void FMC_Init(void);
extern void WDG_Init(void);
extern void Interrupt_Init(void);
extern void AppScheduled(void);
extern void SystemClock_Config(void);
extern void Error_Handler(void);

#endif /* __MAIN_H */
