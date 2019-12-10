/*************************欢迎大家进入 《芯知识学堂》 ******************************************
**
**
**	
**	     ﹎    ●  ○ .﹎                        							★  作者：四叶草     
**	****┈ ┈ /█\/▓\ ﹎ ┈****                   						★单片机&工控技术QQ群： 82558344 ★        
**	 ☆°★    ∴°°∴ ☆°．·★°°Oooo          ★      		★物联网&嵌入式群QQ群: 544101253 ★
**	 ▅▆▇█████▇▆▅▃▂┈﹎▂▃▅▆    __▁▂▃▁__ ●     ★微信群向群主提交密语:  获取密钥★
										                    ^                                       |
																																								|
																														《好看的皮囊千篇一律，有料的大脑万一挑一
																																研发的梦想带你入门，无尽的学识助你成长》

本学堂走的是开源免费路线、里面所有的原创优质文章、书籍、视频就是免费获取！！！
共同学习进步！！！
********************************************************************************************/
#ifndef __UART_DRIVER_H_
#define __UART_DRIVER_H_


#include "stm32f10x.h"
#include <stdio.h>


#define USART_BUF 10

typedef struct arg_USART{

	u32 Reccount;
	u8* Recv_Start;
	u8* Recv_End;
}USARTx_TypeDef;
	
extern uint8_t usart2_buf[USART_BUF];
extern uint8_t usart1_buf[USART_BUF];    //接收串口1的数据

extern USARTx_TypeDef USARTx_BUF;
extern USARTx_TypeDef USART1_BUF;



void NVIC_USARTx_Init(void);
int fputc(int ch, FILE* f);
void USARTx_IintConfig(USART_TypeDef* USARTx , uint32_t BaudRate);
void USARTx_GPIO_InitConfig(void);
void Clean_Usart1_Buf(void);
void Clean_Usart2_Buf(void);

void USART2_PutStr(uint8_t* str);

#endif


