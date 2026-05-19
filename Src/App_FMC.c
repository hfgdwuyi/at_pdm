/**
  ******************************************************************************
  * @file    Src/App_FMC.c
  * @author  MikeWu
  * @brief   HAL MSP module.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BUFFER_SIZE         ((uint32_t)0x0100)
#define WRITE_READ_ADDR     ((uint32_t)0x0800)
#define SDRAM_BANK_ADDR     ((uint32_t)0xC0000000)


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Read/Write Buffers */
uint32_t TxBuffer[BUFFER_SIZE];
uint32_t RxBuffer1[BUFFER_SIZE];
uint8_t aTxBuffer[4];
/* Status variables */
__IO uint32_t uwWriteReadStatus = 0;

/* Counter index */
uint32_t uwIndex = 0;
/* Private function prototypes -----------------------------------------------*/
void App_FMC(void);
static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset);
/* Private functions ---------------------------------------------------------*/
void App_FMC(void)
{
  /*##-1- SDRAM memory read/write access #####################################*/  
  
  /* Fill the buffer to write */
  Fill_Buffer(TxBuffer, BUFFER_SIZE, 0xA244250F);   
  
  /* Write data to the SDRAM memory */
  for (uwIndex = 0; uwIndex < BUFFER_SIZE; uwIndex++)
  {
    *(__IO uint32_t*) (SDRAM_BANK_ADDR + WRITE_READ_ADDR + 4*uwIndex) = aTxBuffer[uwIndex];
  }    
  
  /* Read back data from the SDRAM memory */
  for (uwIndex = 0; uwIndex < BUFFER_SIZE; uwIndex++)
  {
    RxBuffer1[uwIndex] = *(__IO uint32_t*) (SDRAM_BANK_ADDR + WRITE_READ_ADDR + 4*uwIndex);
  } 
  
  /*##-2- Checking data integrity ############################################*/    
  
  for (uwIndex = 0; (uwIndex < BUFFER_SIZE) && (uwWriteReadStatus == 0); uwIndex++)
  {
    if (RxBuffer1[uwIndex] != aTxBuffer[uwIndex])
    {
      // u8StatusLed = 3;
    }
		else
		{
			// u8StatusLed = 6;
		}
  }	
}



/**
  * @brief  Fills buffer with user predefined data.
  * @param  pBuffer: pointer on the buffer to fill
  * @param  uwBufferLenght: size of the buffer to fill
  * @param  uwOffset: first value to fill on the buffer
  * @retval None
  */
static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset)
{
  uint32_t tmpIndex = 0;

  /* Put in global buffer different values */
  for (tmpIndex = 0; tmpIndex < uwBufferLenght; tmpIndex++ )
  {
    pBuffer[tmpIndex] = tmpIndex + uwOffset;
  }
}                  



/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/****END OF FILE****/
