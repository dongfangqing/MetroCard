#ifndef __LED_DRIVER_H_
#define __LED_DRIVER_H_


#include "stm32f10x.h"


#define LED_ON 0
#define LED_OFF 1

typedef enum
{
	RED_LED = 1,
	GREEN_LED,
	BLUE_LED,

}LED_Type;
	

 void Led_Init(void);
 void Led_Ctl(uint8_t LedType, uint8_t LedState);


#endif






