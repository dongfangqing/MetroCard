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
#include "Beep_Driver.h"


void Beep_Init(void)
 {
	 
	 GPIO_InitTypeDef GPIO_InitStruct;
 
		// 1) ��ʱ�� (���ñ��˵ĺ���)
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);
 
	 GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 ;           //��������
	 GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;   //�������������
	 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;    //���ó��������
	 
	 GPIO_Init(GPIOC , &GPIO_InitStruct);
	 
	 
	 //�رգ������Ÿߵ�ƽ
	 GPIO_SetBits(GPIOC, GPIO_Pin_9);
 
 }
 


/****************************************************
 ** ������ : Beep_Ctl
 **  �� �� : ����BEEP
 **   ���� : ����һ ���ȵ�״̬
 **  ����ֵ: ��
 **
******************************************************/ 
 void Beep_Ctl(uint8_t BeepState)
 {				    
	
 
	if(BeepState == BEEP_ON)
	{
		GPIO_ResetBits( GPIOC, GPIO_Pin_9);
	}else{
	
		GPIO_SetBits(GPIOC, GPIO_Pin_9);
	}


}


