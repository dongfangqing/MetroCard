#include "Key_Driver.h"
#include "SysTick_Driver.h"

 void Key_Init(void)
 {
	 
	 GPIO_InitTypeDef GPIO_InitStruct;
 
		// 1) ��ʱ�� (���ñ��˵ĺ���)
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
 
	 GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;           //��������
	 GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;    //���ø�������
	 GPIO_Init(GPIOA , &GPIO_InitStruct);
	
 }
 


/****************************************************
 ** ������ : scanKey
 **  �� �� : ���ư���
 **   ���� : ��
 **  ����ֵ: ������״̬
 **
******************************************************/ 
 uint8_t scanKey(void)
 {				    
	
	if( GPIO_ReadInputDataBit( GPIOA,GPIO_Pin_0) == 0)  //�жϰ����Ƿ���
	{
			delay_ms(30);   //����
		if( GPIO_ReadInputDataBit( GPIOA,GPIO_Pin_0) == 0)//�ٴ��ж�
		{
	//			while( GPIO_ReadInputDataBit( GPIOA,GPIO_Pin_0) == 0); //�ȴ�����̧��
			  return 1;   //��ʾ�ɹ�
		}
	}
		return 0;
 }
 
 
 


