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
#include "SysTick_Driver.h"


static uint32_t fu_ns = 0;   //���ڴ��1΢��Ĵ���
static uint32_t fu_ms = 0;   //���ڴ��1����Ĵ���


void SysTick_Init(void)
{
	//1.ѡ��ʱ��Դ
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);

	fu_ns = 9;
	fu_ms = 9 * 1000;

}

void delay_us(uint32_t nus)
{
	uint32_t temp = 0;
	
	// 2.������װ�ؼĴ���
	SysTick->LOAD = fu_ns *nus;

	//��յ�ǰֵ�Ĵ���ֵ
	SysTick->VAL = 0X00;
	
	//3.��SYSTick
	SysTick->CTRL |= 0x01;
	
	
	//4.�ȴ�ʱ�䵽��
	do{
	
		temp = SysTick->CTRL;
		
	}while((temp && 0x01) && !(temp & (1 << 16)));   
	
	
	// 5.�ر�SYSTick
	SysTick->CTRL |= 0x00;
	SysTick->VAL = 0X00;
	
}


void delay_ms(uint32_t nms)
{
	uint32_t temp = 0;
	
	// 2.������װ�ؼĴ���
	SysTick->LOAD = fu_ms *nms;

	//��յ�ǰֵ�Ĵ���ֵ
	SysTick->VAL = 0X00;
	
	//3.��SYSTick
	SysTick->CTRL |= 0x01;
	
	
	//4.�ȴ�ʱ�䵽��
	do{
	
		temp = SysTick->CTRL;
		
	}while((temp && 0x01) && !(temp & (1 << 16)));   
	
	
	// 5.�ر�SYSTick
	SysTick->CTRL |= 0x00;
	SysTick->VAL = 0X00;
	
}




