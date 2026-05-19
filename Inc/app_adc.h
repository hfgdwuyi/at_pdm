#ifndef __APP_ADC_H
#define __APP_ADC_H

#include <stdint.h>
#include "stm32f4xx_hal.h"

typedef struct {
  uint16_t u16I24V;
  uint16_t u16Power24V;
} FilterADCValueTypeDef;

extern FilterADCValueTypeDef FilterADCValue;
extern ADC_HandleTypeDef AdcHandle;

void App_ADC(void);

#endif /* __APP_ADC_H */
