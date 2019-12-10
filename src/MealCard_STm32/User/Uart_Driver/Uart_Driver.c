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
#include "Uart_Driver.h"
#include <stdio.h>


//����2��ʹ��
uint8_t usart2_buf[USART_BUF] = {0};
USARTx_TypeDef USARTx_BUF = {
	
	0,
	&usart2_buf[0],
	&usart2_buf[0],
};

//����1��ʹ��
uint8_t usart1_buf[USART_BUF] = {0};
USARTx_TypeDef USART1_BUF = {
	
	0,
	&usart1_buf[0],
	&usart1_buf[0],
};



//��ʼ�����ڵ�����
void USARTx_GPIO_InitConfig(void)
{
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//1����GPIO��ʱ�Ӻ͸���ʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;     //����PA10
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //��������
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);   //��ʼ��PA10

	//PA9   uart_Tx
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;   //��ʼ��PA9
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;   //ʱ��Ƶ��
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP ;    //�����������
	GPIO_Init(GPIOA, &GPIO_InitStruct);     //��ʼ��PA9
	
	//PA3   uart2_Rx
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;     //����PA3
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //��������
	GPIO_Init(GPIOA, &GPIO_InitStruct);       //��ʼ��PA3

	//PA2   uart2_Tx
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;    //����PA2����
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;  //����ʱ��Ƶ��
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP ;   //���óɸ����������
	GPIO_Init(GPIOA, &GPIO_InitStruct);       //��ʼ��PA2
	

}

void USARTx_IintConfig(USART_TypeDef* USARTx , uint32_t BaudRate)
{
	
	USART_InitTypeDef USART_InitStruct;
	
	// �򿪴��ڵ�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 , ENABLE);
	
	//����8N1
	USART_InitStruct.USART_BaudRate  = BaudRate;      //���ò�����
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;  //8λ����λ
	USART_InitStruct.USART_StopBits = USART_StopBits_1;        //1��ֹͣλ
	USART_InitStruct.USART_Parity = USART_Parity_No;           //û��У��λ
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx; //�շ�����
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //û������
	
	//	��ʼ�����ڣ�8N1��
	USART_Init(USARTx, &USART_InitStruct);
	USART_ClearFlag(USARTx , USART_FLAG_TC);  //�����־λ
	
	//��������ж�
	USART_ITConfig( USARTx, USART_IT_RXNE, ENABLE);
	//ʹ�ܴ���
	USART_Cmd(USARTx, ENABLE);
}

//�ض���printf����
int fputc(int ch, FILE* f)
{
	
	USART_SendData(USART1, ch);
	
	while(USART_GetFlagStatus( USART1, USART_FLAG_TC) == RESET);
	
	return ch;
}


void USART2_PutStr(uint8_t* str)
{
	while(*str)
	{
		USART_SendData(USART2, *str++);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TC) != SET);
	}
}




//�жϵ�����
void NVIC_USARTx_Init(void)
{
	
	NVIC_InitTypeDef NVIC_InitStruct;

	//ѡ��ڶ����ж�
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;   //���ô���1�ж�
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;  //��ռʽ���ȼ�Ϊ1
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;   //��Ӧʽ���ȼ�Ϊ0
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;      //�ж�ʹ��
	//��ʼ���ж�
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;	  //���ô���2�ж�
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;  //������ռʽ���ȼ�Ϊ0
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;  //������Ӧʽ���ȼ�Ϊ0
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;    //ʹ���ж�
	NVIC_Init(&NVIC_InitStruct);                       //��ʼ���ж�


}




//��մ���1����������
void Clean_Usart1_Buf(void)
{
	int i ;
	
	USART1_BUF.Recv_End = 	&usart1_buf[0];
	USART1_BUF.Recv_Start = 	&usart1_buf[0];
  USART1_BUF.Reccount = 0;

	for(i = 0;i < USART_BUF; i++)
	{
		usart1_buf[i] = 0;
	}
}

//��մ���2����������
void Clean_Usart2_Buf(void)
{
	int i ;
	
	USARTx_BUF.Recv_End = 	&usart2_buf[0];
	USARTx_BUF.Recv_Start = 	&usart2_buf[0];
  USARTx_BUF.Reccount = 0;

	for(i = 0;i < USART_BUF; i++)
	{
		usart2_buf[i] = 0;
	}
}








