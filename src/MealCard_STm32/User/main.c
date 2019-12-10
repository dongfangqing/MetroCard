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

#include <stdio.h>
#include "Led_Driver.h"
#include "SysTick_Driver.h"
#include "Beep_Driver.h"
#include "Uart_Driver.h"
#include "rc522_driver.h"
#include "spi.h"
#include "Key_Driver.h"
#include "Esp_Ap.h"

//�ƹ���ʾ
void Led_San(void)
{

	uint8_t i;
	for(i = 0;i < 4;i++)
	{
			Led_Ctl( RED_LED, LED_ON);
			delay_ms(20);
			Led_Ctl( RED_LED, LED_OFF);
			delay_ms(20);
	    Beep_Ctl(BEEP_ON);
			Led_Ctl( GREEN_LED, LED_ON);
			delay_ms(20);
			Led_Ctl( GREEN_LED, LED_OFF);
			delay_ms(20);
		  Beep_Ctl(BEEP_OFF);
	}

}

void System_Init(void)
{

	Led_Init();				//���ݵĳ�ʼ��		
	SysTick_Init();         //��ʱ���ĳ�ʼ��
	Beep_Init();            //�������ĳ�ʼ��
	USARTx_GPIO_InitConfig() ;          //�������ŵĳ�ʼ��
	USARTx_IintConfig(USART1 , 115200); //���ڳ�ʼ��     
   NVIC_USARTx_Init();   //�жϵĳ�ʼ��
	 Key_Init();
	USARTx_IintConfig( USART2 , 115200);  //����2�ĳ�ʼ��
	Esp_Tcp_AP();                //��ʼ��wifi
}


int main(void)
{
	static unsigned char g_ucTempbuf[9]  = {1,2,3,4,5,6,7,8};
  static unsigned char r_ucTempbuf[9];
	
	unsigned char temp1 = 0;    //�������
	unsigned char temp2 = 0;
	static unsigned char money = 0;
	static uint8_t  buf[3] = {0};
	static uint8_t  cbuf[3] = {0};
	char  str[10] = {'0','0',};
	
	int i = 0;
	int ret,status ;
	static int time = 0;

	System_Init();         //ϵͳ��ʼ��
	
	SPI_InitConfig();
	RC522_Init();                //��Ƶʶ��ĳ�ʼ��
	
	Clean_Usart1_Buf();   //��մ���1������
	

while(1)
{
	

	
		 
#if 1
	
		for(i = 0; i < 10;i++)
	{
	//		printf("buf[%d] = %d\n",i,usart2_buf[i]);
	
		//���ֻ����͹���������
		if(usart2_buf[i] == 35 && i < 8)
		{
				buf[0] = usart2_buf[i] ;
				buf[1] = usart2_buf[i+1];
			  buf[2] = usart2_buf[i+2];		
				usart2_buf[i] = 0; 
		}
				
		if(buf[0] == 35)     //#Ǯ ��Ǯ
		{			
	//	printf("%d   %d  \n",buf[1],buf[2]);
			
			temp1 = buf[1]  - '0';     //���Ǯ��
			temp2 = buf[2]  - '0';
			
			money = temp1*10 + temp2;         //��Ǯת��������
			
		status = Card_handle(g_ucTempbuf,NULL);//ɨ�迨�ȵ�
		if(status == 5)		//ֻ��ɨ��ɹ�������������һֱ��������֤�ɹ����Ž�����Ϣ��д��
		{

			//��ȡԭ���Ŀ���Ǯ
			if(Card_Look_User(r_ucTempbuf) == 1)
			{
				 printf("\n");
					printf("ԭ��������� = %d\n",r_ucTempbuf[0]);
				
							//��ȥ�۳���Ǯ
			if(r_ucTempbuf[0] > money)    //ֻ�������ڿۿ�ʱ���ſۿ�
			{
			    money = r_ucTempbuf[0] - money;
				
					Write_Card2(&money);//�俨�ɹ�
			}else{
					printf("���㣬�뾡���ֵ\n");
			}
				
			{					
			   if(Card_Look_User(r_ucTempbuf) == 1)
				 {
						printf("�����ܶ�Ϊ = %d\n",r_ucTempbuf[0]);
						buf[0] = 0;   //�ۿ�ɹ��������־λ
					 
					 sprintf(str, "%d", r_ucTempbuf[0]); //������תΪ�ַ������뵽 p ��
						if(r_ucTempbuf[0] > 99)
						{
							 //���ֻ�ʵʱ��ʾ
							USART2_PutStr("AT+CIPSEND=0,3\r\n");
							USART2_PutStr("AT+CIPSEND=1,3\r\n");
							USART2_PutStr("AT+CIPSEND=2,3\r\n");
							delay_ms(100);
							USART2_PutStr(str);
					 	
						}else{
							USART2_PutStr("AT+CIPSEND=0,2\r\n");
							USART2_PutStr("AT+CIPSEND=1,3\r\n");
							USART2_PutStr("AT+CIPSEND=2,3\r\n");
							delay_ms(100);
							USART2_PutStr(str);
						}
						Led_San();       //�ƹ���ʾ
				 }
			}
					
			}

			
		}		
			
			
		}
		    
		  if(i == 9)
				{
							Clean_Usart2_Buf();
				}
	}
	
	
	
	delay_ms(1000);
	delay_ms(1000);
	

  //��Ǯ 	
	//��ȡ����һ������
	for(i = 0; i < 10;i++)
	{
	//		printf("buf[%d] = %d\n",i,usart1_buf[i]);
	
		//���ֻ����͹���������
		if((usart1_buf[i] == 36 || usart1_buf[i] == 35 )&& i < 8)
		{
				cbuf[0] = usart1_buf[i] ;
				cbuf[1] = usart1_buf[i+1];
			  cbuf[2] = usart1_buf[i+2];		
				usart1_buf[i] = 0; 
		}
		if(cbuf[0] == 35)   //#Ǯ��
		{
				temp1 = cbuf[1]  - '0';     //���Ǯ��
				temp2 = cbuf[2]  - '0';
			
			money = temp1*10 + temp2;         //��Ǯת��������
			
		status = Card_handle(g_ucTempbuf,NULL);//ɨ�迨�ȵ�
		if(status == 5)		//ֻ��ɨ��ɹ�������������һֱ��������֤�ɹ����Ž�����Ϣ��д��
		{

			//��ȡԭ���Ŀ���Ǯ
			if(Card_Look_User(r_ucTempbuf) == 1)
			{
					printf("ԭ��������� = %d\n",r_ucTempbuf[0]);
				
							//��ȥ�۳���Ǯ
			if(r_ucTempbuf[0] > money)    //ֻ�������ڿۿ�ʱ���ſۿ�
			{
			    money = r_ucTempbuf[0] - money;
					Write_Card2(&money);//�俨�ɹ�
			}else{
					printf("���㣬�뾡���ֵ\n");
			}
		
			
			{					
			   if(Card_Look_User(r_ucTempbuf) == 1)
				 {
						printf("�����ܶ�Ϊ = %d\n",r_ucTempbuf[0]);
						cbuf[0] = 0;   //�ۿ�ɹ��������־λ
					 
           sprintf(str, "%d", r_ucTempbuf[0]); //������תΪ�ַ������뵽 p ��
						
					  if(r_ucTempbuf[0] > 99)
						{
							 //���ֻ�ʵʱ��ʾ
							USART2_PutStr("AT+CIPSEND=0,3\r\n");
							USART2_PutStr("AT+CIPSEND=1,3\r\n");
							USART2_PutStr("AT+CIPSEND=2,3\r\n");
							delay_ms(100);
							USART2_PutStr(str);
					 	
						}else{
							USART2_PutStr("AT+CIPSEND=0,2\r\n");
							USART2_PutStr("AT+CIPSEND=1,2\r\n");
							USART2_PutStr("AT+CIPSEND=2,2\r\n");
							delay_ms(100);
							USART2_PutStr(str);
						}
							Led_San();       //�ƹ���ʾ
				 }
			}
					
			}

			
		}	
		
		}
		
		//�б�����ͷ�Ƿ���ȷ
		if(cbuf[0] == 36)   //$Ǯ��
		{

			temp1 = cbuf[1]  - '0';     //���Ǯ��
			temp2 = cbuf[2]  - '0';
			
			money = temp1*10 + temp2;         //��Ǯת��������
			

			//��ȡԭ���Ŀ���Ǯ
			if(Card_Look_User(r_ucTempbuf) == 1)
			{
					printf("ԭ��������� = %d\n",r_ucTempbuf[0]);
				
							//�������ڵ�Ǯ
			money = r_ucTempbuf[0] + money;
		
			Write_Card(&money);//�俨�ɹ�
			
			{					
			   if(Card_Look_User(r_ucTempbuf) == 1)
				 {
						printf("�����ܶ�Ϊ = %d\n",r_ucTempbuf[0]);
						cbuf[0] = 0;     //�ȳ俨�ɹ��������־λ
					 
					  sprintf(str, "%d", r_ucTempbuf[0]); //������תΪ�ַ������뵽 p ��
					 
						if(r_ucTempbuf[0] > 99)
						{
							 //���ֻ�ʵʱ��ʾ
							USART2_PutStr("AT+CIPSEND=0,3\r\n");
							USART2_PutStr("AT+CIPSEND=1,3\r\n");
							USART2_PutStr("AT+CIPSEND=2,3\r\n");
							delay_ms(100);
							USART2_PutStr(str);
					 	
						}else{
							USART2_PutStr("AT+CIPSEND=0,2\r\n");
							USART2_PutStr("AT+CIPSEND=1,2\r\n");
							USART2_PutStr("AT+CIPSEND=2,2\r\n");
							delay_ms(100);
							USART2_PutStr(str);
						}
						
					Led_San();       //�ƹ���ʾ
						
				 }
			}
			
					
			}

			
		}	
						if(i == 9)
				{
							Clean_Usart1_Buf();
				}
	}
#endif
	
	
	
	//�鿴���
	status = Card_handle(g_ucTempbuf,NULL);//ɨ�迨�ȵ�
		if(status == 5)		//ֻ��ɨ��ɹ�������������һֱ��������֤�ɹ����Ž�����Ϣ��д��
		{

			//��ȡԭ���Ŀ���Ǯ
			if(Card_Look_User(r_ucTempbuf) == 1)
			{
				 printf("\n");
					printf("������� = %d\n",r_ucTempbuf[0]);
				
				 sprintf(str, "%d", r_ucTempbuf[0]); //������תΪ�ַ������뵽 p ��
										if(r_ucTempbuf[0] > 99)
						{
							 //���ֻ�ʵʱ��ʾ
							USART2_PutStr("AT+CIPSEND=0,3\r\n");
							USART2_PutStr("AT+CIPSEND=1,3\r\n");
							USART2_PutStr("AT+CIPSEND=2,3\r\n");
							delay_ms(100);
							USART2_PutStr(str);
					 	
						}else{
							USART2_PutStr("AT+CIPSEND=0,2\r\n");
							USART2_PutStr("AT+CIPSEND=1,2\r\n");
							USART2_PutStr("AT+CIPSEND=2,2\r\n");
							delay_ms(100);
							USART2_PutStr(str);
						}
				
		 }
	 }


		 
		
	}

}

