#ifndef _SOC_F4_CONFIG_H_
#define _SOC_F4_CONFIG_H_

#include "stm32f4xx.h"
#include "data_type.h"

/********* 管脚号 *********/
#define PIN0    0
#define PIN1    1
#define PIN2    2
#define PIN3    3
#define PIN4    4
#define PIN5    5
#define PIN6    6
#define PIN7    7
#define PIN8    8
#define PIN9    9
#define PIN10   10
#define PIN11   11
#define PIN12   12
#define PIN13   13
#define PIN14   14
#define PIN15   15

/********* GPIO配置选项 *********/
#define CFG_MODE_IN    	0		/*  普通输入模式	*/
#define CFG_MODE_OUT	1		/*  普通输出模式	*/
#define CFG_MODE_AF		2		/*  复用功能模式	*/
#define CFG_MODE_AIN	3		/*  模拟输入模式	*/
#define CFG_SPEED_2M	0		/*  最大速度2Mhz(低速)	*/
#define CFG_SPEED_25M	1		/*  最大速度25Mhz(中速)	*/
#define CFG_SPEED_50M	2		/*  最大速度50Mhz(快速)	*/
#define CFG_SPEED_100M	3		/*  最大速度100Mhz(高速)	*/
#define CFG_PUPD_NONE	0		/*  无上下拉	*/
#define CFG_PUPD_PU		1		/*  上拉 */
#define CFG_PUPD_PD		2		/*  下拉 */
#define CFG_PUPD_RES	3		/*  保留 */
#define CFG_OTYPE_PP	0		/*  推挽输出	*/
#define CFG_OTYPE_OD	1		/*  开漏输出	*/

/********* 复用功能选项 *********/
#define AF0     0       		/*  MCO/SWD/SWCLK/RTC      */
#define AF1     1       		/*  TIM1~TIM2  */
#define AF2     2       		/*  TIM3~TIM5  */
#define AF3     3       		/*  TIM8~TIM11  */
#define AF4     4       		/*  I2C1~I2C4  */
#define AF5     5       		/*  SPI1~SPI6  */
#define AF6     6       		/*  SPI3/SAI1  */
#define AF7     7       		/*  SPI2/SPI23/USART1~3/USART5/SPDIFRX     */
#define AF8     8       		/*  USART4~USART8/SPDIFRX/SAI2    */
#define AF9     9       		/*  CAN1~CAN2/TIM12~TIM14/LCD/QSPI      */
#define AF10    10      		/*  USB_OTG/USB_HS/SAI2/QSPI     */
#define AF11    11      		/*  ETH  */
#define AF12    12      		/*  FMC/SDMMC/OTG/HS     */
#define AF13    13      		/*  DCIM  */
#define AF14    14      		/*  LCD  */
#define AF15    15      		/*  EVENTOUT   */

/*********GPIOx模式配置*********/
#define Px_MODER_SET(x,pin,mode)		( x->MODER   = (x->MODER   & ~((U32_t)0x3<<pin*2) | ((U32_t)mode <<pin*2)) )

/*********GPIOx输出速度配置*********/
#define Px_OSPEEDR_SET(x,pin,speed)	    ( x->OSPEEDR = (x->OSPEEDR & ~((U32_t)0x3<<pin*2) | ((U32_t)speed <<pin*2)) )

/*********GPIOx上拉下拉配置*********/
#define Px_PUPDR_SET(x,pin,pupd)		( x->PUPDR   = (x->PUPDR   & ~((U32_t)0x3<<pin*2) | ((U32_t)pupd <<pin*2)) )

/*********GPIOx输出类型配置*********/
#define Px_OTYPER_SET(x,pin,type)		( x->OTYPER  = (x->OTYPER  & ~((U32_t)0x1<<pin*1) | ((U32_t)type <<pin*1)) )

/*********GPIOx复用功能配置*********/
#define Px_AF_SET(x,pin,af)             ( x->AFR[pin>>3] = x->AFR[pin>>3] & ~((U32_t)0xF<<((pin&0x7)*4)) | (U32_t)af<<((pin&0x7)*4) ) 

/*********GPIOx置位操作*********/
#define Px_SET(x,pin)	( x->BSRR |= ((U32_t)1<<(pin)) )

/*********GPIOx复位操作*********/
#define Px_CLR(x,pin)	( x->BSRR |= ((U32_t)1<<((pin)+16)) )

/*********GPIOx位读取操作*********/
#define Px_GET(x,pin)	( (x->IDR & ((U32_t)1<<(pin))) >> (pin) )

/*********寄存器置位操作*********/
#define REG_BIT_SET(reg,bit)	( reg |= ((U32_t)1<<(bit)) )

/*********寄存器复位操作*********/
#define REG_BIT_CLR(reg,bit)	( reg &= (~((U32_t)1<<(bit))) )

/*********寄存器改变位操作*********/
#define REG_BIT_CHG(reg,bit)	( reg = (reg&(~((U32_t)1<<(bit)))) | ((~(reg&((U32_t)1<<(bit))))&((U32_t)1<<(bit))) )

/*********寄存器整体置位操作*********/
#define REG_ALL_SET(reg,wid)	( reg |= 0xFFFFFFFF<<(31-(wid))>>(31-(wid)) )

/*********寄存器整体复位操作*********/
#define REG_ALL_CLR(reg,wid)	( reg &= ~(0xFFFFFFFF<<(31-(wid))>>(31-(wid))) )

/*********寄存器整体赋值操作*********/
#define REG_ALL_CHG(reg,val)	( reg = (val) )

/*********寄存器段赋值操作*********/
#define REG_SEG_CHG(reg,val,bit_l,bit_r)	\
( reg = (reg&(~(0xFFFFFFFF<<(31-(bit_l))>>(31-(bit_l)+(bit_r))<<(bit_r))))|(((val)&(0xFFFFFFFF<<(31-(bit_l))>>(31-(bit_l)+(bit_r))))<<(bit_r)) ) 


void SOC_nvic_init(U8_t preemption_priority, U8_t sub_priority, U8_t channel, U8_t group);

#endif
