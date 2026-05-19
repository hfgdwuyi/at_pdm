/**
  ******************************************************************************
  * @file    main.h
  * @author  WuYi
  * @brief   Header for main.c module
  ******************************************************************************
**/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stdio.h"
#include "msp_timer.h"
#include "APP_CAN.h"
#include "APP_GPIO.h"
// #include "APP_ADC.h"
// #include "APP_SPI.h"
#include "APP_UART.h"
// #include "APP_I2C.h"

#include "GM_Malloc.h"
#include "lwip_comm.h"
#include "lwip/snmp_msg.h"
#include "PDS_SocTICK.h"

#include "MySlave.h"
#include "applicfg.h"
#include "MyMaster.h"
#include "canfestival.h"

/* Exported types ------------------------------------------------------------*/
extern CAN_HandleTypeDef     Can1Handle;
extern CAN_HandleTypeDef     Can2Handle;
extern CAN_TxHeaderTypeDef   TxHeader;
extern CAN_RxHeaderTypeDef   RxHeader;
// extern ADC_HandleTypeDef    AdcHandle;
// extern SPI_HandleTypeDef SpiHandle;
extern UART_HandleTypeDef UartHandle;
extern IWDG_HandleTypeDef IwdgHandle;
// extern I2C_HandleTypeDef I2cHandle;


typedef struct
{
	uint16_t u16MeBoxK1On;  /*!< relay K1 enable flag. */
  uint16_t u16MeBoxK2On;  /*!< relay K2 enable flag. */
	uint16_t u16MeBoxK3On;  /*!< relay K3 enable flag. */
	uint16_t u16MeBoxK850On;/*!< relay K850 enable flag. */                        	  
  uint16_t u16PfcOk;      /*!< pfc status OK flag. */
  uint16_t u16UpsMode;    /*!< ups mode flag.*/
} ControlBoardStatusTypeDef;


/* Exported constants --------------------------------------------------------*/
extern uint8_t aRxBuffer[4];
extern uint8_t aTxBuffer[4];
extern uint8_t temp;

/* Exported macro ------------------------------------------------------------*/
#define TRUE	1
#define FALSE 0

#define IO_HIGH_LEVEL	1
#define IO_LOW_LEVEL	0
/* Exported functions ------------------------------------------------------- */
extern void GPIO_Init(void); 
extern void Timer_Init(void); 
extern void Error_Handler(void); 
extern void CAN_Init(void); 
extern void ADC_Init(void);
extern void SPI_Init(void);
extern void Interrupt_Init(void);
extern void UART_Init(void);
extern void I2C_Init(void);
extern void FMC_Init(void);
#endif /* __MAIN_H */

/*****END OF FILE****/
