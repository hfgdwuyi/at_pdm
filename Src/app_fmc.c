/**
  ******************************************************************************
  * @file    App_FMC.c
  * @author  MikeWu
  * @brief   FMC SDRAM test application.
  ******************************************************************************
**/

#include "main.h"

#define BUFFER_SIZE      ((uint32_t)0x0100)
#define WRITE_READ_ADDR  ((uint32_t)0x0800)
#define SDRAM_BANK_ADDR  ((uint32_t)0xC0000000)

static uint32_t TxBuffer[BUFFER_SIZE];
static uint32_t RxBuffer1[BUFFER_SIZE];
static uint8_t  fmc_tx_buf[4];
static __IO uint32_t uwWriteReadStatus = 0;
static uint32_t uwIndex = 0;

static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset)
{
  uint32_t tmpIndex;
  for (tmpIndex = 0; tmpIndex < uwBufferLenght; tmpIndex++)
    pBuffer[tmpIndex] = tmpIndex + uwOffset;
}

void App_FMC(void)
{
  Fill_Buffer(TxBuffer, BUFFER_SIZE, 0xA244250F);

  for (uwIndex = 0; uwIndex < BUFFER_SIZE; uwIndex++)
    *(__IO uint32_t *)(SDRAM_BANK_ADDR + WRITE_READ_ADDR + 4 * uwIndex) = fmc_tx_buf[uwIndex];

  for (uwIndex = 0; uwIndex < BUFFER_SIZE; uwIndex++)
    RxBuffer1[uwIndex] = *(__IO uint32_t *)(SDRAM_BANK_ADDR + WRITE_READ_ADDR + 4 * uwIndex);

  for (uwIndex = 0; (uwIndex < BUFFER_SIZE) && (uwWriteReadStatus == 0); uwIndex++)
  {
    if (RxBuffer1[uwIndex] != fmc_tx_buf[uwIndex])
    {
      /* Data mismatch detected */
    }
  }
}
