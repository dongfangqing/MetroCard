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

/****************************************************
**********************��������***********************
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
**������: Esp_Init
**��  ��: ��ʼ��wifiģ��
**��  ��: ��
**����ֵ: ��
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
**������: Esp_Enable
**��  ��: ʹ��wifiģ��
**��  ��: ��
**����ֵ: ��
******************************************************/
void Esp_Enable(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_9);
}

/****************************************************
**������: Esp_Disable
**��  ��: ʧ��wifiģ��
**��  ��: ��
**����ֵ: ��
******************************************************/
void Esp_Disable(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
}


/****************************************************
**������: Esp_SendDate
**��  ��: ��wifiģ�鷢������
**��  ��: ��
**����ֵ: ��
******************************************************/
void Esp_SendDate(uint8_t *str)
{
	USART2_PutStr(str);
}


/****************************************************
**������: Esp_RetString_Compare
**��  ��: wifiģ�鷵��ֵ�ȽϺ������������OK����ʾ�ɹ�������ʧ��
**��  ��: wifi���ص��ַ���
**����ֵ: SUCCESS or ERROR
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
**������: Esp_Test
**��  ��: ���wifiģ���Ƿ�����
**��  ��: ��
**����ֵ: SUCCESS or ERROR
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
**������: Esp_To_Configure
**��  ��: ��wifi����ATָ����ж�wifiģ������úͲ���
**��  ��: Ҫ���͵�ATָ��
**����ֵ: SUCCESS or ERROR
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
**������: Esp_Tcp_AP
**��  ��: ���ó�APģʽ���ɹ�����SUCCESS��ʧ�ܷ���ERROR
**��  ��: ��
**����ֵ: SUCCESS or ERROR
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
			
			//�������Ӻ󣬲鿴AP��IP��ַ
			Esp_To_Configure("AT+CIFSR\r\n") ;
			
			delay_ms(1000);
		 //����������
			if(Esp_To_Configure("AT+CIPMUX=1\r\n") == SUCCESS)
			{			 	
  			delay_ms(1000);
				//����������
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











