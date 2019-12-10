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
#include "Led_Driver.h"


 void Led_Init(void)
 {
	 
	 GPIO_InitTypeDef GPIO_InitStruct;
 
		// 1) ��ʱ�� (���ñ��˵ĺ���)
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
 
	 GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;           //��������
	 GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;   //�������������
	 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;    //���ó��������
	 GPIO_Init(GPIOC , &GPIO_InitStruct);
	
	 //�رյƣ������Ÿߵ�ƽ

	 GPIO_SetBits(GPIOC, GPIO_Pin_7|GPIO_Pin_6|GPIO_Pin_8);
 
 }
 


/****************************************************
 ** ������ : Led_Ctl
 **  �� �� : ����RGB��
 **   ���� : ����һ �Ƶ�����   ������: �Ƶ�״̬
 **  ����ֵ: ��
 **
******************************************************/ 
 void Led_Ctl(uint8_t LedType, uint8_t LedState)
 {				    
	
		switch(LedType)
		{
			//��ƿ���
			case RED_LED:	
					if(LedState == LED_ON)
					{
						GPIO_ResetBits( GPIOC, GPIO_Pin_8);
					}else{
					
						GPIO_SetBits(GPIOC, GPIO_Pin_8);
					}
				break;
					
			//�̵ƿ���
			case GREEN_LED:	
					if(LedState == LED_ON)
					{
						GPIO_ResetBits( GPIOC, GPIO_Pin_7);
					}else{
					
						GPIO_SetBits(GPIOC, GPIO_Pin_7);
					}
				break;
					
				//���ƿ���
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
 
 
 





