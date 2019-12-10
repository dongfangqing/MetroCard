#ifndef _SPI_H_
#define _SPI_H_

#include "stm32f10x.h"

#define SPI_NSS_LOW()       GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define SPI_NSS_HIGH()      GPIO_SetBits(GPIOA, GPIO_Pin_4)



void SPI_InitConfig(void);

uint8_t RFID_SPI_WriterByte(uint8_t dat);


#endif

