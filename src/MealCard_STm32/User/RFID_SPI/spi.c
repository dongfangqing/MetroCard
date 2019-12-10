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
#include "spi.h"
#include "SysTick_Driver.h"
#include "Beep_Driver.h"
#include "Uart_Driver.h"



/**********************************************************
**************************引脚定义*************************
**RFID RC522 device
**SPI_NSS:			PA12		SDA
**SPI_CLK:			PA11		SCK
**SPI_MISO:			PC2			
**SPI_MOSI:			PB8
***********************************************************/

#define RF_CLK_Clr()		GPIO_ResetBits(GPIOA, GPIO_Pin_11)
#define RF_CLK_Set()		GPIO_SetBits(GPIOA, GPIO_Pin_11)

#define RF_MOSI_Clr()		GPIO_ResetBits(GPIOB, GPIO_Pin_8)	
#define RF_MOSI_Set()		GPIO_SetBits(GPIOB, GPIO_Pin_8)

#define RF_NSS_Clr()		GPIO_ResetBits(GPIOA, GPIO_Pin_12)	
#define RF_NSS_Set()		GPIO_SetBits(GPIOA, GPIO_Pin_12)

#define RF_MISO_READ()		GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2)

void RFID_SPI_InitConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
//	RF_CLK_Clr();
	RF_CLK_Set();
	RF_NSS_Set();
}

#if 0
//CPOL: Low
//CPHA: 1Edge
uint8_t RFID_SPI_WriterByte(uint8_t dat)
{	
	uint8_t i;
	uint8_t rx_data = 0;
	
	for(i = 0;i < 8;i++)
	{
		RF_CLK_Set();
		rx_data <<= 1;
		if(RF_MISO_READ())
		{
			rx_data |= 1;
		}
		
		RF_CLK_Clr();
		if(dat&0x80)
		{
			RF_MOSI_Set();
		}
		else	RF_MOSI_Clr();
		dat <<= 1;
	}
	
	return rx_data;
}
#else
//CPOL: Hig
//CPHA: 2Edge
uint8_t RFID_SPI_WriterByte(uint8_t dat)
{	
	uint8_t i;
	uint8_t rx_data = 0;
//	RF_CLK_Set();
	for(i = 0;i < 8;i++)
	{
		RF_CLK_Clr();
		if(dat&0x80)
		{
			RF_MOSI_Set();
		}
		else	RF_MOSI_Clr();
		dat <<= 1;
		
		RF_CLK_Set();
		rx_data <<= 1;
		if(RF_MISO_READ())
		{
			rx_data |= 1;
		}
	}
	
	return rx_data;
}

#endif


/******************************************************
**函数功能：SPI初始化
*******************************************************/
void SPI_InitConfig(void)
{

	RFID_SPI_InitConfig();

}



