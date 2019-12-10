/*************************��ӭ��ҽ��� ��о֪ʶѧ�á� ******************************************
**
**
**	
**	     �m    ��  �� .�m                        							��  ���ߣ���Ҷ��     
**	****�� �� /��\/��\ �m ��****                   						�ﵥƬ��&���ؼ���QQȺ�� 82558344 ��        
**	 ����    ����� ��㣮������Oooo          ��      		��������&Ƕ��ʽȺQQȺ: 544101253 ��
**	 �|�}�~�����������~�}�|�z�y���m�y�z�|�}    __�x�y�z�x__ ��     ��΢��Ⱥ��Ⱥ���ύ����:  ��ȡ��Կ��
										                    ^                                       |
																																								|
																														���ÿ���Ƥ��ǧƪһ�ɣ����ϵĴ�����һ��һ
																																�з�������������ţ��޾���ѧʶ����ɳ���

��ѧ���ߵ��ǿ�Դ���·�ߡ��������е�ԭ���������¡��鼮����Ƶ������ѻ�ȡ������
��ͬѧϰ����������
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
extern uint8_t usart1_buf[USART_BUF];    //���մ���1������

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


