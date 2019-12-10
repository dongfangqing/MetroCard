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

/****************************************************
**********************引脚连接***********************
**                                                 **
**   ESP8266 WiFi                  STM32F103RCT6   **
**			     GND     <-------->			  GND		       **
**			     VCC     <-------->				5V		       **
**			     Rxd   <-------->				  PA2		       **
**					 Txd    <-------->				PA3          **
*****************************************************/

#include "Esp_Ap.h"

#include "Systick_Driver.h"
#include "ST_string.h"



/****************************************************
**函数名: Esp_Init
**功  能: 初始化wifi模块
**参  数: 无
**返回值: 无
******************************************************/
void Esp_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB,  ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
}


/****************************************************
**函数名: Esp_Enable
**功  能: 使能wifi模块
**参  数: 无
**返回值: 无
******************************************************/
void Esp_Enable(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_9);
}

/****************************************************
**函数名: Esp_Disable
**功  能: 失能wifi模块
**参  数: 无
**返回值: 无
******************************************************/
void Esp_Disable(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
}


/****************************************************
**函数名: Esp_SendDate
**功  能: 向wifi模块发送数据
**参  数: 无
**返回值: 无
******************************************************/
void Esp_SendDate(uint8_t *str)
{
	USART2_PutStr(str);
}


/****************************************************
**函数名: Esp_RetString_Compare
**功  能: wifi模块返回值比较函数，如果返回OK，表示成功，否则失败
**参  数: wifi返回的字符串
**返回值: SUCCESS or ERROR
******************************************************/
uint8_t Esp_RetString_Compare(unsigned char* buf)
{
	uint16_t i;
	
	for(i = 0;i < 512;i++)
	{
		if(buf[i] == 'O' && buf[i+1] == 'K')
		{
				return SUCCESS;
		}
	}
	return ERROR;
}

/****************************************************
**函数名: Esp_Test
**功  能: 检测wifi模块是否正常
**参  数: 无
**返回值: SUCCESS or ERROR
******************************************************/
uint8_t Esp_Test(void)
{
	
	Esp_SendDate("AT\r\n");
	if(Esp_RetString_Compare(usart2_buf) == SUCCESS)
	{
		return SUCCESS;
	}
	return ERROR;
}

/****************************************************
**函数名: Esp_To_Configure
**功  能: 向wifi发送AT指令进行对wifi模块的配置和操作
**参  数: 要发送的AT指令
**返回值: SUCCESS or ERROR
******************************************************/
uint8_t Esp_To_Configure(uint8_t* str)
{
	
	Esp_SendDate(str);
	if(Esp_RetString_Compare(usart2_buf) == SUCCESS)
	{
		return SUCCESS;
	}
	return ERROR;
}

/****************************************************
**函数名: Esp_Tcp_AP
**功  能: 配置成AP模式，成功返回SUCCESS，失败返回ERROR
**参  数: 无
**返回值: SUCCESS or ERROR
******************************************************/
uint8_t Esp_Tcp_AP(void)
{

	while(Esp_Test() == ERROR);
	
	delay_ms(100);
	
	if(Esp_To_Configure("AT+CWMODE_DEF=2\r\n") == SUCCESS)
	{
		delay_ms(1000);
		if(Esp_To_Configure("AT+CWSAP_DEF=\"XZSXT\",\"123456789\",5,4\r\n") == SUCCESS)
		{

			delay_ms(1000);
			delay_ms(1000);
			delay_ms(1000);
			
			//用于连接后，查看AP的IP地址
			Esp_To_Configure("AT+CIFSR\r\n") ;
			
			delay_ms(1000);
		 //开启多连接
			if(Esp_To_Configure("AT+CIPMUX=1\r\n") == SUCCESS)
			{			 	
  			delay_ms(1000);
				//开启服务器
				if(Esp_To_Configure("AT+CIPSERVER=1\r\n") == SUCCESS)
				{
					printf("AP_modle is OK!!!!!!!!!!!!\r\n");
					return SUCCESS;
				}			
			}		
		}
	}
	printf("wifi_configure failed!!!!!!!!\r\n");
	return ERROR;
}











