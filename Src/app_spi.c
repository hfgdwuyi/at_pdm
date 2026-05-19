/**
  ******************************************************************************
  * @file    App_SPI.c
  * @author  MCD Application Team
  * @brief   SPI application: MAX146 ADC data acquisition.
  ******************************************************************************
**/

#include "main.h"

#define MAX146_ADC_CHANNEL_IAC11  0x80
#define MAX146_ADC_CHANNEL_IAC12  0xc0
#define MAX146_ADC_CHANNEL_IAC13  0x90
#define MAX146_ADC_CHANNEL_IAC21  0xd0
#define MAX146_ADC_CHANNEL_IAC22  0xa0
#define MAX146_ADC_CHANNEL_IAC23  0xe0

#define FILTER_NUM  10

static uint16_t u16ReceiveAdcValue[6];

static void Task_ReceiveAdcValue(void);
static uint16_t Task_FilterAdcValue(uint16_t u16AdcValue);

void App_SPI(void)
{
  Task_ReceiveAdcValue();
}

static void Task_ReceiveAdcValue(void)
{
  static uint8_t i = MAX146_ADC_CHANNEL_IAC11;
  static uint8_t j = 0;
  uint8_t SpiCommand;
  uint8_t temp1, temp2;

  static const uint8_t channel_seq[] = {
    MAX146_ADC_CHANNEL_IAC11, MAX146_ADC_CHANNEL_IAC12, MAX146_ADC_CHANNEL_IAC13,
    MAX146_ADC_CHANNEL_IAC21, MAX146_ADC_CHANNEL_IAC22, MAX146_ADC_CHANNEL_IAC23
  };

  j = (j + 1) % 6;
  SpiCommand = i | 0x0f;
  i = channel_seq[j];

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

  aTxBuffer[0] = SpiCommand;
  HAL_SPI_TransmitReceive(&SpiHandle, (uint8_t *)aTxBuffer, (uint8_t *)aRxBuffer, 1, 5000);

  aTxBuffer[0] = 0x00;
  HAL_SPI_TransmitReceive(&SpiHandle, (uint8_t *)aTxBuffer, (uint8_t *)aRxBuffer, 1, 5000);
  temp1 = aRxBuffer[0];

  aTxBuffer[0] = 0x00;
  HAL_SPI_TransmitReceive(&SpiHandle, (uint8_t *)aTxBuffer, (uint8_t *)aRxBuffer, 1, 5000);
  temp2 = aRxBuffer[0];

  u16ReceiveAdcValue[j] = (uint16_t)((temp1 << 8) | temp2) >> 4;

  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}

static uint16_t Task_FilterAdcValue(uint16_t u16AdcValue)
{
  static uint16_t buf[FILTER_NUM];
  static uint16_t sum;
  static uint16_t index;

  sum += u16AdcValue - buf[index];
  buf[index] = u16AdcValue;
  index++;

  if (index >= FILTER_NUM) index = 0;
  return (index == 0) ? (sum / FILTER_NUM) : (sum / index);
}
