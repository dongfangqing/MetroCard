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
#include "SysTick_Driver.h"


static uint32_t fu_ns = 0;   //用于存放1微妙的次数
static uint32_t fu_ms = 0;   //用于存放1毫妙的次数


void SysTick_Init(void)
{
	//1.选择时钟源
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);

	fu_ns = 9;
	fu_ms = 9 * 1000;

}

void delay_us(uint32_t nus)
{
	uint32_t temp = 0;
	
	// 2.配置重装载寄存器
	SysTick->LOAD = fu_ns *nus;

	//清空当前值寄存器值
	SysTick->VAL = 0X00;
	
	//3.打开SYSTick
	SysTick->CTRL |= 0x01;
	
	
	//4.等待时间到达
	do{
	
		temp = SysTick->CTRL;
		
	}while((temp && 0x01) && !(temp & (1 << 16)));   
	
	
	// 5.关闭SYSTick
	SysTick->CTRL |= 0x00;
	SysTick->VAL = 0X00;
	
}


void delay_ms(uint32_t nms)
{
	uint32_t temp = 0;
	
	// 2.配置重装载寄存器
	SysTick->LOAD = fu_ms *nms;

	//清空当前值寄存器值
	SysTick->VAL = 0X00;
	
	//3.打开SYSTick
	SysTick->CTRL |= 0x01;
	
	
	//4.等待时间到达
	do{
	
		temp = SysTick->CTRL;
		
	}while((temp && 0x01) && !(temp & (1 << 16)));   
	
	
	// 5.关闭SYSTick
	SysTick->CTRL |= 0x00;
	SysTick->VAL = 0X00;
	
}




