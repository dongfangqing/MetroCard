#include "Key_Driver.h"
#include "SysTick_Driver.h"

 void Key_Init(void)
 {
	 
	 GPIO_InitTypeDef GPIO_InitStruct;
 
		// 1) 打开时钟 (调用别人的函数)
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
 
	 GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;           //配置引脚
	 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;    //配置浮空输入
	 GPIO_Init(GPIOA , &GPIO_InitStruct);
	
 }
 


/****************************************************
 ** 函数名 : scanKey
 **  功 能 : 控制按键
 **   参数 : 无
 **  返回值: 按键的状态
 **
******************************************************/ 
 uint8_t scanKey(void)
 {				    
	
	if( GPIO_ReadInputDataBit( GPIOA,GPIO_Pin_0) == 0)  //判断按键是否按下
	{
			delay_ms(30);   //消抖
		if( GPIO_ReadInputDataBit( GPIOA,GPIO_Pin_0) == 0)//再次判断
		{
	//			while( GPIO_ReadInputDataBit( GPIOA,GPIO_Pin_0) == 0); //等待按键抬起
			  return 1;   //表示成功
		}
	}
		return 0;
 }
 
 
 


