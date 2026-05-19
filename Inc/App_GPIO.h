/**
  ******************************************************************************
  * @file    CAN/CAN_Networking/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_GPIO_H
#define __APP_GPIO_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>


/* Define -------------------------------------*/
#define CLOSE_K2_RELAY			HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_11,GPIO_PIN_SET)	
#define OPEN_K2_RELAY			  HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_11,GPIO_PIN_RESET)

#define CLOSE_K31_RELAY			HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_5,GPIO_PIN_SET)	
#define OPEN_K31_RELAY			HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_5,GPIO_PIN_RESET)

#define CLOSE_K32_RELAY			HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_6,GPIO_PIN_SET)	
#define OPEN_K32_RELAY			HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_6,GPIO_PIN_RESET)

#define CLOSE_K33_RELAY			HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_2,GPIO_PIN_SET)	
#define OPEN_K33_RELAY			HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_2,GPIO_PIN_RESET)

#define CLOSE_K850_RELAY		HAL_GPIO_WritePin(GPIOK, GPIO_PIN_1,GPIO_PIN_SET)	
#define OPEN_K850_RELAY			HAL_GPIO_WritePin(GPIOK, GPIO_PIN_1,GPIO_PIN_RESET)

#define ENABLE_LED_A_PIN		HAL_GPIO_WritePin(GPIOK, GPIO_PIN_6,GPIO_PIN_SET)	
#define DISABLE_LED_A_PIN		HAL_GPIO_WritePin(GPIOK, GPIO_PIN_6,GPIO_PIN_RESET)
#define ENABLE_LED_B_PIN		HAL_GPIO_WritePin(GPIOK, GPIO_PIN_5,GPIO_PIN_SET)	
#define DISABLE_LED_B_PIN		HAL_GPIO_WritePin(GPIOK, GPIO_PIN_5,GPIO_PIN_RESET)
#define ENABLE_LED_C_PIN		HAL_GPIO_WritePin(GPIOK, GPIO_PIN_4,GPIO_PIN_SET)	
#define DISABLE_LED_C_PIN		HAL_GPIO_WritePin(GPIOK, GPIO_PIN_4,GPIO_PIN_RESET)
#define ENABLE_LED_D_PIN		HAL_GPIO_WritePin(GPIOK, GPIO_PIN_3,GPIO_PIN_SET)	
#define DISABLE_LED_D_PIN		HAL_GPIO_WritePin(GPIOK, GPIO_PIN_3,GPIO_PIN_RESET)
#define ENABLE_LED_E_PIN		HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_15,GPIO_PIN_SET)	
#define DISABLE_LED_E_PIN		HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_15,GPIO_PIN_RESET)
#define ENABLE_LED_F_PIN		HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_14,GPIO_PIN_SET)	
#define DISABLE_LED_F_PIN		HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_14,GPIO_PIN_RESET)
#define ENABLE_LED_G_PIN		HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_13,GPIO_PIN_SET)	
#define DISABLE_LED_G_PIN		HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_13,GPIO_PIN_RESET)
														
																
					
#define STATUS_ME_BOX_K1		HAL_GPIO_ReadPin(GPIOI, GPIO_PIN_4)													
#define STATUS_ME_BOX_K2		HAL_GPIO_ReadPin(GPIOI, GPIO_PIN_3)
#define STATUS_ME_BOX_K3		HAL_GPIO_ReadPin(GPIOI, GPIO_PIN_0)
#define STATUS_ME_BOX_K4		HAL_GPIO_ReadPin(GPIOI, GPIO_PIN_2)
#define STATUS_ME_BOX_ERROR		HAL_GPIO_ReadPin(GPIOI, GPIO_PIN_15)

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  uint16_t u16MeBoxK1OnFilterCount;    /*!< 1ms explore flag.*/

  uint16_t u16MeBoxK1OffFilterCount;   /*!< 10ms explore flag.*/
	
  uint16_t u16MeBoxK2OnFilterCount;  /*!< 100ms explore flag.*/

  uint16_t u16MeBoxK2OffFilterCount;     /*!< 1s explore flag.*/

  uint16_t u16MeBoxK3OnFilterCount;    /*!< 1ms explore flag.*/

  uint16_t u16MeBoxK3OffFilterCount;   /*!< 10ms explore flag.*/
	
  uint16_t u16MeBoxErrorOnFilterCount;  /*!< 100ms explore flag.*/

  uint16_t u16MeBoxErrorOffFilterCount;     /*!< 1s explore flag.*/

} GPIO_FilterTypeDef;
/* Exported constants --------------------------------------------------------*/
extern uint8_t u8StatusLed;
/* Exported functions --------------------------------------------------------*/
extern void App_GPIO(void);



#endif /* __APP_GPIO_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
