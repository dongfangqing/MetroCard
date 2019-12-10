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
#include "Uart_Driver.h"
#include <stdio.h>


//串口2的使用
uint8_t usart2_buf[USART_BUF] = {0};
USARTx_TypeDef USARTx_BUF = {
	
	0,
	&usart2_buf[0],
	&usart2_buf[0],
};

//串口1的使用
uint8_t usart1_buf[USART_BUF] = {0};
USARTx_TypeDef USART1_BUF = {
	
	0,
	&usart1_buf[0],
	&usart1_buf[0],
};



//初始化串口的引脚
void USARTx_GPIO_InitConfig(void)
{
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	//1）打开GPIO口时钟和复用时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;     //引脚PA10
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //浮空输入
	
	GPIO_Init(GPIOA, &GPIO_InitStruct);   //初始化PA10

	//PA9   uart_Tx
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;   //初始化PA9
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;   //时钟频率
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP ;    //复用推完输出
	GPIO_Init(GPIOA, &GPIO_InitStruct);     //初始化PA9
	
	//PA3   uart2_Rx
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;     //引脚PA3
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStruct);       //初始化PA3

	//PA2   uart2_Tx
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;    //配置PA2引脚
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;  //配置时钟频率
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP ;   //配置成复用推完输出
	GPIO_Init(GPIOA, &GPIO_InitStruct);       //初始化PA2
	

}

void USARTx_IintConfig(USART_TypeDef* USARTx , uint32_t BaudRate)
{
	
	USART_InitTypeDef USART_InitStruct;
	
	// 打开串口的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 , ENABLE);
	
	//配置8N1
	USART_InitStruct.USART_BaudRate  = BaudRate;      //配置波特率
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;  //8位数据位
	USART_InitStruct.USART_StopBits = USART_StopBits_1;        //1个停止位
	USART_InitStruct.USART_Parity = USART_Parity_No;           //没有校验位
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx; //收发数据
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //没有流控
	
	//	初始化串口（8N1）
	USART_Init(USARTx, &USART_InitStruct);
	USART_ClearFlag(USARTx , USART_FLAG_TC);  //清除标志位
	
	//打开外设的中断
	USART_ITConfig( USARTx, USART_IT_RXNE, ENABLE);
	//使能串口
	USART_Cmd(USARTx, ENABLE);
}

//重定义printf功能
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




//中断的配置
void NVIC_USARTx_Init(void)
{
	
	NVIC_InitTypeDef NVIC_InitStruct;

	//选择第二组中断
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;   //配置串口1中断
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;  //抢占式优先级为1
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;   //响应式优先级为0
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;      //中断使能
	//初始化中断
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;	  //配置串口2中断
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;  //配置抢占式优先级为0
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;  //配置响应式优先级为0
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;    //使能中断
	NVIC_Init(&NVIC_InitStruct);                       //初始化中断


}




//清空串口1的数组数据
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

//清空串口2的数组数据
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








