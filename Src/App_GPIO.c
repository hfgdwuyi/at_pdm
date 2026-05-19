/**
  ******************************************************************************
  * @file    Src/App_GPIO.c
  * @author  WuYi
  * @brief   GPIO task.
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
GPIO_FilterTypeDef 	GPIO_FilterCount;
//FaultFlagTypeDef		FaultFlag;
ControlBoardStatusTypeDef	ControlBoardStatus;

//Digital tube displary array
uint8_t array[10] = {0x3f,	//0
					0x06,	//1
					0x5b,	//2 
					0x4f,	//3
					0x66,	//4
					0x6d,	//5
					0xfd,	//6
					0x07,	//7
					0x7f,	//8
					0x6f};	//9

/* Private define ------------------------------------------------------------*/
#define _DEBUG
#define	KEY_VALID_TIME	300
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t u8IsUpsMpde = 0;
uint8_t u8StatusD850 = 0;
uint8_t u8StatusLed;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void App_GPIO(void);
void Task_OCP(void);
void Task_RelayControl(void);
void Task_SystemStatus(void);
void Task_LedDisplay(void);		
void Task_KeyScan(void);											

/**
  * @brief GPIO application
  *        This function contain below functions:
  *           - OCP detect
  *           - Relay control
  *			10ms every time
  * @param None
  * @retval None
  */
void App_GPIO(void)
{
	static uint16_t u16BlinkTime = 0;
	static uint16_t u16LedFlashTime = 0;
	
	u16BlinkTime++;
	if(u16BlinkTime >= 50)
	{
		u16BlinkTime = 0;
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
	}
	
	u16LedFlashTime++;
	if(u16LedFlashTime >= 100)
	{
		u16LedFlashTime = 0;
		Task_LedDisplay();
	}

	Task_SystemStatus();
	Task_RelayControl();
}

/**
  * @brief OCP task
  *        This function contain below functions:
  *           - OCP detect
  * @param None
  * @retval None
  */
void Task_OCP(void)
{
	// if(FaultFlag.all != 0)
	// {
	// 	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0,GPIO_PIN_SET);
	// }
	// else
	// {
	// 	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0,GPIO_PIN_RESET);
	// }
}

void Task_SystemStatus(void)
{
	if(STATUS_ME_BOX_K1 == IO_LOW_LEVEL)
	{
		GPIO_FilterCount.u16MeBoxK1OnFilterCount++;
		if(GPIO_FilterCount.u16MeBoxK1OnFilterCount >= 10)
		{
			GPIO_FilterCount.u16MeBoxK1OnFilterCount = 0;
			ControlBoardStatus.u16MeBoxK1On = TRUE;
		}
	}
	else
	{
		GPIO_FilterCount.u16MeBoxK1OffFilterCount++;
		if(GPIO_FilterCount.u16MeBoxK1OffFilterCount >= 10)
		{
			GPIO_FilterCount.u16MeBoxK1OffFilterCount = 0;
			ControlBoardStatus.u16MeBoxK1On = FALSE;
		}
	}
	
	if(STATUS_ME_BOX_K2 == IO_HIGH_LEVEL)
	{
		GPIO_FilterCount.u16MeBoxK2OnFilterCount++;
		if(GPIO_FilterCount.u16MeBoxK2OnFilterCount >= 10)
		{
			GPIO_FilterCount.u16MeBoxK2OnFilterCount = 0;
			ControlBoardStatus.u16MeBoxK2On = TRUE;
		}
	}
	else
	{
		GPIO_FilterCount.u16MeBoxK2OffFilterCount++;
		if(GPIO_FilterCount.u16MeBoxK2OffFilterCount >= 10)
		{
			GPIO_FilterCount.u16MeBoxK2OffFilterCount = 0;
			ControlBoardStatus.u16MeBoxK2On = FALSE;
		}
	}
	
	if(STATUS_ME_BOX_K3 == IO_HIGH_LEVEL)
	{
		GPIO_FilterCount.u16MeBoxK3OffFilterCount = 0;
		GPIO_FilterCount.u16MeBoxK3OnFilterCount++;
		if(GPIO_FilterCount.u16MeBoxK3OnFilterCount >= 50)
		{
			GPIO_FilterCount.u16MeBoxK3OnFilterCount = 0;
			ControlBoardStatus.u16MeBoxK3On = TRUE;
		}
	}
	else
	{
		GPIO_FilterCount.u16MeBoxK3OnFilterCount = 0;
		GPIO_FilterCount.u16MeBoxK3OffFilterCount++;
		if(GPIO_FilterCount.u16MeBoxK3OffFilterCount >= 50)
		{
			GPIO_FilterCount.u16MeBoxK3OffFilterCount = 0;
			ControlBoardStatus.u16MeBoxK3On = FALSE;
		}
	}
	
	if(STATUS_ME_BOX_ERROR == IO_HIGH_LEVEL)
	{
		GPIO_FilterCount.u16MeBoxErrorOnFilterCount++;
		if(GPIO_FilterCount.u16MeBoxErrorOnFilterCount >= 10)
		{
			GPIO_FilterCount.u16MeBoxErrorOnFilterCount = 0;
			ControlBoardStatus.u16PfcOk = TRUE;
		}
	}
	else
	{
		GPIO_FilterCount.u16MeBoxErrorOffFilterCount++;
		if(GPIO_FilterCount.u16MeBoxErrorOffFilterCount >= 10)
		{
			GPIO_FilterCount.u16MeBoxErrorOffFilterCount = 0;
			ControlBoardStatus.u16PfcOk = FALSE;
		}
	}
}

/**
  * @brief Relay control task
  *        This function contain below functions:
  *           - relay control
  * @param None
  * @retval None
  */
void Task_RelayControl()
{	
	// static uint8_t u8UpsToNormal = FALSE;
	// if(ControlBoardStatus.u16PfcOk == FALSE)
	// {
	// 	u8IsUpsMpde = TRUE;
	// 	if(u8UpsToNormal == FALSE)
	// 	{
	// 		u8UpsToNormal = TRUE;
	// 	}
	// }
	// else
	// {
	// 	u8IsUpsMpde = FALSE;
	// }
	
	// if(u8IsUpsMpde == FALSE)
	// {
	// 	if(ControlBoardStatus.u16MeBoxK2On == FALSE)
	// 	{
	// 		DISABLE_K2_RELAY;
	// 	}
	// 	else
	// 	{

	// 	}
		
	// 	if(ControlBoardStatus.u16MeBoxK3On == FALSE)
	// 	{
	// 		OPEN_K5_RELAY;
	// 		OPEN_K7_RELAY;
	// 		if(u8UpsToNormal == FALSE)
	// 		{
	// 			OPEN_K9_RELAY;
	// 			OPEN_K10_RELAY;
	// 			ME_BOX_K3_CTR_OFF;
	// 		}		
	// 	}
	// 	else
	// 	{
	// 		ME_BOX_K3_CTR_ON;
	// 	}		
	// }
	// else
	// {	
	// 	ME_BOX_K3_CTR_ON;
	// 	CLOSE_K9_RELAY;
	// 	CLOSE_K10_RELAY;
		
	// 	if(ControlBoardStatus.u16MeBoxK2On == FALSE)
	// 	{
	// 		DISABLE_K2_RELAY;
	// 	}
		
	// 	if(ControlBoardStatus.u16MeBoxK3On == FALSE)
	// 	{
	// 		OPEN_K5_RELAY;
	// 		OPEN_K7_RELAY;
	// 	}
	// }
#ifdef _DEBUG
		CLOSE_K2_RELAY;
		CLOSE_K32_RELAY;
		CLOSE_K850_RELAY;
		CLOSE_K31_RELAY;
		CLOSE_K33_RELAY;
#else
	if(ControlBoardStatus.u16PfcOk == FALSE)
	{
		CLOSE_K31_RELAY;
		CLOSE_K32_RELAY;
	}
	else
	{
		if(ControlBoardStatus.u16MeBoxK2On == TRUE)
		{
			CLOSE_K2_RELAY;
		}
		else
		{
			OPEN_K2_RELAY;
		}
		
		if(ControlBoardStatus.u16MeBoxK3On == TRUE)
		{
			CLOSE_K850_RELAY;
			CLOSE_K31_RELAY;
			CLOSE_K32_RELAY;
			CLOSE_K33_RELAY;
		}
		else
		{
			OPEN_K850_RELAY;
			OPEN_K31_RELAY;
			OPEN_K32_RELAY;
			OPEN_K33_RELAY;
		}
	}
	#endif
}

/**
  * @brief Led display task
  *        This function contain below functions:
  *           - led control
  * @param None
  * @retval None
  */
void Task_LedDisplay(void)
{
	uint8_t u8PinAValue;
	uint8_t u8PinBValue;
	uint8_t u8PinCValue;
	uint8_t u8PinDValue;
	uint8_t u8PinEValue;
	uint8_t u8PinFValue;
	uint8_t u8PinGValue;

	u8PinAValue = (array[u8StatusLed] & 0x01);
	u8PinBValue = (array[u8StatusLed] >> 1 & 0x01);
	u8PinCValue = (array[u8StatusLed] >> 2 & 0x01);
	u8PinDValue = (array[u8StatusLed] >> 3 & 0x01);
	u8PinEValue = (array[u8StatusLed] >> 4 & 0x01);
	u8PinFValue = (array[u8StatusLed] >> 5 & 0x01);
	u8PinGValue = (array[u8StatusLed] >> 6 & 0x01);
	
	if(u8PinAValue)
	{
		// HAL_GPIO_WritePin(GPIOK, GPIO_PIN_6,GPIO_PIN_RESET);
		DISABLE_LED_A_PIN;
	}
	else
	{
		// HAL_GPIO_WritePin(GPIOK, GPIO_PIN_6,GPIO_PIN_SET);
		ENABLE_LED_A_PIN;
	}
	
	if(u8PinBValue)
	{
		// HAL_GPIO_WritePin(GPIOK, GPIO_PIN_5,GPIO_PIN_RESET);
		DISABLE_LED_B_PIN;
	}
	else
	{
		// HAL_GPIO_WritePin(GPIOK, GPIO_PIN_5,GPIO_PIN_SET);
		ENABLE_LED_B_PIN;
	}
	
	if(u8PinCValue)
	{
		// HAL_GPIO_WritePin(GPIOK, GPIO_PIN_4,GPIO_PIN_RESET);
		DISABLE_LED_C_PIN;
	}
	else
	{
		// HAL_GPIO_WritePin(GPIOK, GPIO_PIN_4,GPIO_PIN_SET);
		ENABLE_LED_C_PIN;
	}
	
	if(u8PinDValue)
	{
		// HAL_GPIO_WritePin(GPIOK, GPIO_PIN_3,GPIO_PIN_RESET);
		DISABLE_LED_D_PIN;
	}
	else
	{
		// HAL_GPIO_WritePin(GPIOK, GPIO_PIN_3,GPIO_PIN_SET);
		ENABLE_LED_D_PIN;
	}
	
	if(u8PinEValue)
	{
		// HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_15,GPIO_PIN_RESET);
		DISABLE_LED_E_PIN;
	}
	else
	{
		// HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_15,GPIO_PIN_SET);
		ENABLE_LED_E_PIN;
	}
	
	if(u8PinFValue)
	{
		// HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_14,GPIO_PIN_RESET);
		DISABLE_LED_F_PIN;
	}
	else
	{
		// HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_14,GPIO_PIN_SET);
		ENABLE_LED_F_PIN;
	}
	
	if(u8PinGValue)
	{
		// HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_13,GPIO_PIN_RESET);
		DISABLE_LED_G_PIN;
	}
	else
	{
		// HAL_GPIO_WritePin(GPIOJ, GPIO_PIN_13,GPIO_PIN_SET);
		ENABLE_LED_G_PIN;
	}
	
	u8StatusLed++;
	if(u8StatusLed > 9)
	{
		u8StatusLed = 0;
	}
}

/**
  * @brief Key scan task
  *        This function contain below functions:
  *           - key scan
	*		  - key control
  * @param None
  * @retval None
  */
void Task_KeyScan(void)
{
//	static uint16_t u16KeyDownTime = 0;
//	static uint16_t u16KeyUpTime = 0;
//	static uint8_t u8KeyStatus = 0;
	static uint8_t u8KeyCount = 0;
	
	#if 0
	if(STATUS_S13_KEY == IO_LOW_LEVEL)
	{
		u16KeyUpTime = 0;
		u16KeyDownTime++;
		if(u16KeyDownTime >= 10)
		{
			u8KeyStatus = 1;
		}
	}
	else
	{
		u16KeyDownTime = 0;
		if(u8KeyStatus == 1)
		{
			u8KeyStatus = 0;
			u8KeyCount++;
		}
	}
	
	if(u8KeyCount >= 1)
	{
		if(STATUS_S13_KEY == IO_HIGH_LEVEL)
		{
			u16KeyUpTime++;
			if(u16KeyUpTime >= 30)
			{
				u8KeyCount = 0;
				u16KeyUpTime = 0;
			}
		}
	}
	
	switch(u8KeyCount)
	{
		case 0:
			break;
		
		case 1:
			break;
		
		case 2:
			FaultFlag.all = 0;
			break;
		
		default:
			break;
	}
	#endif
	
	if(u8KeyCount == 1)
	{
		u8KeyCount = 0;
	}
}

/*****END OF FILE****/
