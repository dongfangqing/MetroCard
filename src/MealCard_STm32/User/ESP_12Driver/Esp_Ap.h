#ifndef __ESP_AP_H_
#define __ESP_AP_H_

#include "Uart_Driver.h"

void Esp_Init(void);
void Esp_Enable(void);
void Esp_Disable(void);
void Esp_SendDate(uint8_t *str);
uint8_t Esp_RetString_Compare(unsigned char* buf);

uint8_t Esp_Test(void);
uint8_t Esp_To_Configure(uint8_t* str);
uint8_t Esp_Tcp_AP(void);






#endif



