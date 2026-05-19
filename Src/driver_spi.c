/**
  ******************************************************************************
  * @file    Driver_spi.c
  * @author  WuYi
  * @brief   SPI peripheral initialization.
  ******************************************************************************
**/

#include "main.h"

SPI_HandleTypeDef SpiHandle;
uint8_t aTxBuffer[4] = {0x11, 0x22, 0x33, 0x44};
uint8_t aRxBuffer[4];

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
  (void)hspi;

  GPIO_InitTypeDef GPIO_InitStruct;

  __HAL_RCC_SPI1_CLK_ENABLE();

  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;

  /* SCK: PA5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* MISO: PA6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* MOSI: PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void SPI_Init(void)
{
  SpiHandle.Instance               = SPI1;
  SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
  SpiHandle.Init.CLKPhase          = SPI_PHASE_1EDGE;
  SpiHandle.Init.CLKPolarity       = SPI_POLARITY_HIGH;
  SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
  SpiHandle.Init.CRCPolynomial     = 7;
  SpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
  SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
  SpiHandle.Init.NSS               = SPI_NSS_SOFT;
  SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLE;
  SpiHandle.Init.Mode              = SPI_MODE_MASTER;

  if (HAL_SPI_Init(&SpiHandle) != HAL_OK)
    Error_Handler();

  __HAL_SPI_ENABLE(&SpiHandle);
}
