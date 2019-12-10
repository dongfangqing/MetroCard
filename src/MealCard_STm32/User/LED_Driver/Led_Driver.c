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
#include "Led_Driver.h"


 void Led_Init(void)
 {
	 
	 GPIO_InitTypeDef GPIO_InitStruct;
 
		// 1) 打开时钟 (调用别人的函数)
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
 
	 GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;           //配置引脚
	 GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;   //配置输出的速率
	 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;    //配置成推完输出
	 GPIO_Init(GPIOC , &GPIO_InitStruct);
	
	 //关闭灯，给引脚高电平

	 GPIO_SetBits(GPIOC, GPIO_Pin_7|GPIO_Pin_6|GPIO_Pin_8);
 
 }
 


/****************************************************
 ** 函数名 : Led_Ctl
 **  功 能 : 控制RGB灯
 **   参数 : 参数一 灯的类型   参数二: 灯的状态
 **  返回值: 无
 **
******************************************************/ 
 void Led_Ctl(uint8_t LedType, uint8_t LedState)
 {				    
	
		switch(LedType)
		{
			//红灯控制
			case RED_LED:	
					if(LedState == LED_ON)
					{
						GPIO_ResetBits( GPIOC, GPIO_Pin_8);
					}else{
					
						GPIO_SetBits(GPIOC, GPIO_Pin_8);
					}
				break;
					
			//绿灯控制
			case GREEN_LED:	
					if(LedState == LED_ON)
					{
						GPIO_ResetBits( GPIOC, GPIO_Pin_7);
					}else{
					
						GPIO_SetBits(GPIOC, GPIO_Pin_7);
					}
				break;
					
				//蓝灯控制
				case BLUE_LED:	
					if(LedState == LED_ON)
					{
						GPIO_ResetBits( GPIOC, GPIO_Pin_6);
					}else{
					
						GPIO_SetBits(GPIOC, GPIO_Pin_6);
					}
				break;
					
					
					

}
 
 }
 
 
 





