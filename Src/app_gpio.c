/**
  ******************************************************************************
  * @file    App_GPIO.c
  * @author  WuYi
  * @brief   GPIO application: system status, relay control, LED display.
  ******************************************************************************
**/

#include "main.h"

GPIO_FilterTypeDef          GPIO_FilterCount;
ControlBoardStatusTypeDef   ControlBoardStatus;

uint8_t u8StatusLed = 0;

/* 7-segment display pattern: 0-9 */
static const uint8_t seg_pattern[10] = {
  0x3f, 0x06, 0x5b, 0x4f, 0x66,
  0x6d, 0xfd, 0x07, 0x7f, 0x6f
};

/* LED segment pin mapping: {port, pin, enable_level} */
typedef struct {
  GPIO_TypeDef *port;
  uint16_t pin;
} LedSegPin;

static const LedSegPin seg_pins[7] = {
  {GPIOK, GPIO_PIN_6},  /* A */
  {GPIOK, GPIO_PIN_5},  /* B */
  {GPIOK, GPIO_PIN_4},  /* C */
  {GPIOK, GPIO_PIN_3},  /* D */
  {GPIOJ, GPIO_PIN_15}, /* E */
  {GPIOJ, GPIO_PIN_14}, /* F */
  {GPIOJ, GPIO_PIN_13}, /* G */
};

static void Task_RelayControl(void);
static void Task_SystemStatus(void);
static void Task_LedDisplay(void);

void App_GPIO(void)
{
  static uint16_t u16BlinkTime = 0;
  static uint16_t u16LedFlashTime = 0;

  u16BlinkTime++;
  if (u16BlinkTime >= 50)
  {
    u16BlinkTime = 0;
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
  }

  u16LedFlashTime++;
  if (u16LedFlashTime >= 100)
  {
    u16LedFlashTime = 0;
    Task_LedDisplay();
  }

  Task_SystemStatus();
  Task_RelayControl();
}

static void Task_SystemStatus(void)
{
  if (STATUS_ME_BOX_K1 == IO_LOW_LEVEL)
  {
    GPIO_FilterCount.u16MeBoxK1OnFilterCount++;
    if (GPIO_FilterCount.u16MeBoxK1OnFilterCount >= 10)
    {
      GPIO_FilterCount.u16MeBoxK1OnFilterCount = 0;
      ControlBoardStatus.u16MeBoxK1On = TRUE;
    }
  }
  else
  {
    GPIO_FilterCount.u16MeBoxK1OffFilterCount++;
    if (GPIO_FilterCount.u16MeBoxK1OffFilterCount >= 10)
    {
      GPIO_FilterCount.u16MeBoxK1OffFilterCount = 0;
      ControlBoardStatus.u16MeBoxK1On = FALSE;
    }
  }

  if (STATUS_ME_BOX_K2 == IO_HIGH_LEVEL)
  {
    GPIO_FilterCount.u16MeBoxK2OnFilterCount++;
    if (GPIO_FilterCount.u16MeBoxK2OnFilterCount >= 10)
    {
      GPIO_FilterCount.u16MeBoxK2OnFilterCount = 0;
      ControlBoardStatus.u16MeBoxK2On = TRUE;
    }
  }
  else
  {
    GPIO_FilterCount.u16MeBoxK2OffFilterCount++;
    if (GPIO_FilterCount.u16MeBoxK2OffFilterCount >= 10)
    {
      GPIO_FilterCount.u16MeBoxK2OffFilterCount = 0;
      ControlBoardStatus.u16MeBoxK2On = FALSE;
    }
  }

  if (STATUS_ME_BOX_K3 == IO_HIGH_LEVEL)
  {
    GPIO_FilterCount.u16MeBoxK3OffFilterCount = 0;
    GPIO_FilterCount.u16MeBoxK3OnFilterCount++;
    if (GPIO_FilterCount.u16MeBoxK3OnFilterCount >= 50)
    {
      GPIO_FilterCount.u16MeBoxK3OnFilterCount = 0;
      ControlBoardStatus.u16MeBoxK3On = TRUE;
    }
  }
  else
  {
    GPIO_FilterCount.u16MeBoxK3OnFilterCount = 0;
    GPIO_FilterCount.u16MeBoxK3OffFilterCount++;
    if (GPIO_FilterCount.u16MeBoxK3OffFilterCount >= 50)
    {
      GPIO_FilterCount.u16MeBoxK3OffFilterCount = 0;
      ControlBoardStatus.u16MeBoxK3On = FALSE;
    }
  }

  if (STATUS_ME_BOX_ERROR == IO_HIGH_LEVEL)
  {
    GPIO_FilterCount.u16MeBoxErrorOnFilterCount++;
    if (GPIO_FilterCount.u16MeBoxErrorOnFilterCount >= 10)
    {
      GPIO_FilterCount.u16MeBoxErrorOnFilterCount = 0;
      ControlBoardStatus.u16PfcOk = TRUE;
    }
  }
  else
  {
    GPIO_FilterCount.u16MeBoxErrorOffFilterCount++;
    if (GPIO_FilterCount.u16MeBoxErrorOffFilterCount >= 10)
    {
      GPIO_FilterCount.u16MeBoxErrorOffFilterCount = 0;
      ControlBoardStatus.u16PfcOk = FALSE;
    }
  }
}

static void Task_RelayControl(void)
{
#ifdef _DEBUG
  CLOSE_K2_RELAY;
  CLOSE_K32_RELAY;
  CLOSE_K850_RELAY;
  CLOSE_K31_RELAY;
  CLOSE_K33_RELAY;
#else
  if (ControlBoardStatus.u16PfcOk == FALSE)
  {
    CLOSE_K31_RELAY;
    CLOSE_K32_RELAY;
  }
  else
  {
    if (ControlBoardStatus.u16MeBoxK2On == TRUE)
      CLOSE_K2_RELAY;
    else
      OPEN_K2_RELAY;

    if (ControlBoardStatus.u16MeBoxK3On == TRUE)
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

static void Task_LedDisplay(void)
{
  uint8_t pattern = seg_pattern[u8StatusLed];
  uint8_t i;

  for (i = 0; i < 7; i++)
  {
    if (pattern & (1 << i))
      HAL_GPIO_WritePin(seg_pins[i].port, seg_pins[i].pin, GPIO_PIN_RESET);
    else
      HAL_GPIO_WritePin(seg_pins[i].port, seg_pins[i].pin, GPIO_PIN_SET);
  }

  u8StatusLed++;
  if (u8StatusLed > 9)
    u8StatusLed = 0;
}
