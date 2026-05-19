/**
  ******************************************************************************
  * @file    main.c
  * @author  WuYi
  * @brief   Main program entry. Initializes all peripherals, CANOpen and LWIP.
  ******************************************************************************
**/

#include "main.h"

static unsigned char MySlaveNodeId  = 0x04;
static unsigned char MyMasterNodeId = 0x01;

void Error_Handler(void)
{
  while (1) {}
}

#ifdef USE_FULL_ASSERT
void assert_failed(char *file, uint32_t line)
{
  while (1) {}
}
#endif

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  Interrupt_Init();

  GPIO_Init();
  Timer_Init();
  CAN_Init();
  UART_Init();
  WDG_Init();

  GM_initialize_memory();

  while (lwip_comm_init() != TRUE)
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

  while (1)
  {
    HAL_IWDG_Refresh(&IwdgHandle);
    AppScheduled();
    lwip_comm_init();
  }
}
