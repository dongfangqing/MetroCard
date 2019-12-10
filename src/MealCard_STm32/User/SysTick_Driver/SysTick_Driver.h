#ifndef __SYSTICK_DRIVER_H_
#define __SYSTICK_DRIVER_H_

#include "stm32f10x.h"



void SysTick_Init(void);
void delay_us(uint32_t nus);
void delay_ms(uint32_t nms);



#endif


