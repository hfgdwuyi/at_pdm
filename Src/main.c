/**
  ******************************************************************************
  * @file    main.c
  * @author  WuYi
  * @brief   This file initialize all the peripheral of Stm32f429. Especially, 
  * it contains the CANOpen and LWip protocols. Only we have to is allocation 
  * the tasks
  ******************************************************************************
**/

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
unsigned char MySlaveNodeId = 0x04;
unsigned char MyMasterNodeId = 0x01;
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
void Error_Handler(void);
void Interrupt_Init(void);
void AppScheduled(void);


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  
  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */
  HAL_Init();

  /* Configure the system clock to 180 MHz */
  SystemClock_Config();
	
	/* Initialize the system interrupt */
	Interrupt_Init();
	
	/* Init the GPIO peripheral */
  GPIO_Init();
	
	/* Init the Timer peripheral */
	Timer_Init();

  /* Init the CAN peripheral */
  CAN_Init();
	
	/* Init the CAN peripheral */
//  ADC_Init();
	
	/* Init the CAN peripheral */
//  SPI_Init();
	
	/* Init the UART peripheral */
	UART_Init();
	
	
	/* Init the I2C peripheral */
//	I2C_Init();

//  eth_init();
  GM_initialize_memory();

  while(lwip_comm_init() != TRUE)
	{
		PDS_soc_tick_delay_ms(1200);
	}

  snmp_init();
	
  setNodeId(&MyMaster_Data, MyMasterNodeId);
	setState(&MyMaster_Data, Initialisation);
	setState(&MyMaster_Data, Operational);

  setNodeId(&MySlave_Data, MySlaveNodeId);
	setState(&MySlave_Data, Initialisation);
	setState(&MySlave_Data, Operational);
	
	printf("MCU finish initial!\r\n");

  /* Infinite loop */
  while (1)
  {
		
		HAL_IWDG_Refresh(&IwdgHandle);
		AppScheduled();
//    lwip_periodic_handle();
    lwip_comm_init();
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 50000000
  *            PLL_M                          = 50
  *            PLL_N                          = 360
  *            PLL_P                          = 2
  *            PLL_Q                          = 4
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 50;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  
  /* Activate the Over-Drive mode */
  // HAL_PWREx_EnableOverDrive();
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
	
	/* Enable all ports clock*/
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOI_CLK_ENABLE();
	__HAL_RCC_GPIOJ_CLK_ENABLE();
	__HAL_RCC_GPIOK_CLK_ENABLE();
	

  /* Enable the LSI oscillator */
  __HAL_RCC_LSI_ENABLE();
  
  /* Wait till LSI is ready */
  while (__HAL_RCC_GET_FLAG(RCC_FLAG_LSIRDY) == RESET)
  {
  }
	
	
	  /*##-1- Check if the system has resumed from IWDG reset ####################*/
  if(__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST) != RESET)
  { 
    /* Clear reset flags */
    __HAL_RCC_CLEAR_RESET_FLAGS();
  }
  else
  {
  }
  
  /*##-2- Configure the IWDG peripheral ######################################*/
  IwdgHandle.Instance = IWDG;
  IwdgHandle.Init.Prescaler = IWDG_PRESCALER_256;
  IwdgHandle.Init.Reload   = 5000;
  
  if(HAL_IWDG_Init(&IwdgHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  while (1)
  {
  }
}






#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif



/**
  * @}
  */

void AppScheduled(void)
{
  if(Timer_TimeBaseFlag.u16Timer1msFlag == TRUE)
  {
    Timer_TimeBaseFlag.u16Timer1msFlag = FALSE;
	  timerForCan();
  }

	
	if(Timer_TimeBaseFlag.u16Timer10msFlag == TRUE)
	{
		Timer_TimeBaseFlag.u16Timer10msFlag = FALSE;
		// App_CAN();
		App_GPIO();
	}
	
	if(Timer_TimeBaseFlag.u16Timer100msFlag == TRUE)
	{
		Timer_TimeBaseFlag.u16Timer100msFlag = FALSE;
		 App_UART();
	}
	
	if(Timer_TimeBaseFlag.u16Timer1sFlag == TRUE)
	{
		Timer_TimeBaseFlag.u16Timer1sFlag = FALSE;		
	}
}

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
