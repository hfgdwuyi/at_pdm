#include "data_type.h"
#include "PDS_SocTICK.h"


void PDS_soc_tick_delay_ms(U16_t n_ms)
{
	U32_t tick_target = 0;
	U32_t tick_old = 0;
    U32_t tick_now = 0;
    U32_t tick_cnt = 0;
	U32_t tick_reload = 0;
	U32_t i = 0;
	
	tick_reload = SysTick->LOAD;
	tick_target = n_ms*180000;
	tick_old = SysTick->VAL;

	for (i=0; i<n_ms; i++)
	{
		while(1)
		{
			tick_now = SysTick->VAL;

			/*Get total tick count*/
			if (tick_now <= tick_old)
			{
				tick_cnt += tick_old-tick_now;
			}
			else
			{
				tick_cnt += tick_reload - tick_now + tick_old;
			}

			tick_old = tick_now;

			if (tick_cnt >= tick_target)
			{
				break;
			}
		}
	}
}
