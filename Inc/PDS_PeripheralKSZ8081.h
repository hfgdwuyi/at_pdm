#ifndef _PDS_PERIPHERAL_KSZ8081_H_
#define _PDS_PERIPHERAL_KSZ8081_H_

#include "soc_f4_config.h"

#define KSZ8081_PHY_ADDRESS  	0x00				//KSZ8081 PHY address

BOOL_t PDS_peripheral_ksz8081_init(void);
U8_t PDS_peripheral_ksz8081_get_speed(void);
BOOL_t PDS_peripheral_ksz8081_set_led_mode(U8_t mode);

#endif 

