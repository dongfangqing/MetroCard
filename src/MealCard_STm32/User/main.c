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

#include <stdio.h>
#include "Led_Driver.h"
#include "SysTick_Driver.h"
#include "Beep_Driver.h"
#include "Uart_Driver.h"
#include "rc522_driver.h"
#include "spi.h"
#include "Key_Driver.h"
#include "Esp_Ap.h"

//灯光提示
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

	Led_Init();				//灯泡的初始化		
	SysTick_Init();         //定时器的初始化
	Beep_Init();            //报警器的初始化
	USARTx_GPIO_InitConfig() ;          //串口引脚的初始化
	USARTx_IintConfig(USART1 , 115200); //串口初始化     
   NVIC_USARTx_Init();   //中断的初始化
	 Key_Init();
	USARTx_IintConfig( USART2 , 115200);  //串口2的初始化
	Esp_Tcp_AP();                //初始化wifi
}


int main(void)
{
	static unsigned char g_ucTempbuf[9]  = {1,2,3,4,5,6,7,8};
  static unsigned char r_ucTempbuf[9];
	
	unsigned char temp1 = 0;    //定义变量
	unsigned char temp2 = 0;
	static unsigned char money = 0;
	static uint8_t  buf[3] = {0};
	static uint8_t  cbuf[3] = {0};
	char  str[10] = {'0','0',};
	
	int i = 0;
	int ret,status ;
	static int time = 0;

	System_Init();         //系统初始化
	
	SPI_InitConfig();
	RC522_Init();                //射频识别的初始化
	
	Clean_Usart1_Buf();   //清空串口1的数组
	

while(1)
{
	

	
		 
#if 1
	
		for(i = 0; i < 10;i++)
	{
	//		printf("buf[%d] = %d\n",i,usart2_buf[i]);
	
		//解手机发送过来的数据
		if(usart2_buf[i] == 35 && i < 8)
		{
				buf[0] = usart2_buf[i] ;
				buf[1] = usart2_buf[i+1];
			  buf[2] = usart2_buf[i+2];		
				usart2_buf[i] = 0; 
		}
				
		if(buf[0] == 35)     //#钱 扣钱
		{			
	//	printf("%d   %d  \n",buf[1],buf[2]);
			
			temp1 = buf[1]  - '0';     //获得钱数
			temp2 = buf[2]  - '0';
			
			money = temp1*10 + temp2;         //把钱转换成数字
			
		status = Card_handle(g_ucTempbuf,NULL);//扫描卡等等
		if(status == 5)		//只有扫描成功······一直到密码验证成功，才进行信息的写入
		{

			//获取原来的卡的钱
			if(Card_Look_User(r_ucTempbuf) == 1)
			{
				 printf("\n");
					printf("原来卡中余额 = %d\n",r_ucTempbuf[0]);
				
							//减去扣除的钱
			if(r_ucTempbuf[0] > money)    //只有余额大于扣款时，才扣款
			{
			    money = r_ucTempbuf[0] - money;
				
					Write_Card2(&money);//充卡成功
			}else{
					printf("余额不足，请尽快充值\n");
			}
				
			{					
			   if(Card_Look_User(r_ucTempbuf) == 1)
				 {
						printf("卡中总额为 = %d\n",r_ucTempbuf[0]);
						buf[0] = 0;   //扣款成功才清除标志位
					 
					 sprintf(str, "%d", r_ucTempbuf[0]); //将数字转为字符串输入到 p 中
						if(r_ucTempbuf[0] > 99)
						{
							 //与手机实时显示
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
						Led_San();       //灯光提示
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
	

  //冲钱 	
	//获取串口一的数据
	for(i = 0; i < 10;i++)
	{
	//		printf("buf[%d] = %d\n",i,usart1_buf[i]);
	
		//解手机发送过来的数据
		if((usart1_buf[i] == 36 || usart1_buf[i] == 35 )&& i < 8)
		{
				cbuf[0] = usart1_buf[i] ;
				cbuf[1] = usart1_buf[i+1];
			  cbuf[2] = usart1_buf[i+2];		
				usart1_buf[i] = 0; 
		}
		if(cbuf[0] == 35)   //#钱数
		{
				temp1 = cbuf[1]  - '0';     //获得钱数
				temp2 = cbuf[2]  - '0';
			
			money = temp1*10 + temp2;         //把钱转换成数字
			
		status = Card_handle(g_ucTempbuf,NULL);//扫描卡等等
		if(status == 5)		//只有扫描成功······一直到密码验证成功，才进行信息的写入
		{

			//获取原来的卡的钱
			if(Card_Look_User(r_ucTempbuf) == 1)
			{
					printf("原来卡中余额 = %d\n",r_ucTempbuf[0]);
				
							//减去扣除的钱
			if(r_ucTempbuf[0] > money)    //只有余额大于扣款时，才扣款
			{
			    money = r_ucTempbuf[0] - money;
					Write_Card2(&money);//充卡成功
			}else{
					printf("余额不足，请尽快充值\n");
			}
		
			
			{					
			   if(Card_Look_User(r_ucTempbuf) == 1)
				 {
						printf("卡中总额为 = %d\n",r_ucTempbuf[0]);
						cbuf[0] = 0;   //扣款成功才清除标志位
					 
           sprintf(str, "%d", r_ucTempbuf[0]); //将数字转为字符串输入到 p 中
						
					  if(r_ucTempbuf[0] > 99)
						{
							 //与手机实时显示
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
							Led_San();       //灯光提示
				 }
			}
					
			}

			
		}	
		
		}
		
		//判别数据头是否正确
		if(cbuf[0] == 36)   //$钱数
		{

			temp1 = cbuf[1]  - '0';     //获得钱数
			temp2 = cbuf[2]  - '0';
			
			money = temp1*10 + temp2;         //把钱转换成数字
			

			//获取原来的卡的钱
			if(Card_Look_User(r_ucTempbuf) == 1)
			{
					printf("原来卡中余额 = %d\n",r_ucTempbuf[0]);
				
							//加上现在的钱
			money = r_ucTempbuf[0] + money;
		
			Write_Card(&money);//充卡成功
			
			{					
			   if(Card_Look_User(r_ucTempbuf) == 1)
				 {
						printf("卡中总额为 = %d\n",r_ucTempbuf[0]);
						cbuf[0] = 0;     //等充卡成功，清除标志位
					 
					  sprintf(str, "%d", r_ucTempbuf[0]); //将数字转为字符串输入到 p 中
					 
						if(r_ucTempbuf[0] > 99)
						{
							 //与手机实时显示
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
						
					Led_San();       //灯光提示
						
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
	
	
	
	//查看余额
	status = Card_handle(g_ucTempbuf,NULL);//扫描卡等等
		if(status == 5)		//只有扫描成功······一直到密码验证成功，才进行信息的写入
		{

			//获取原来的卡的钱
			if(Card_Look_User(r_ucTempbuf) == 1)
			{
				 printf("\n");
					printf("卡中余额 = %d\n",r_ucTempbuf[0]);
				
				 sprintf(str, "%d", r_ucTempbuf[0]); //将数字转为字符串输入到 p 中
										if(r_ucTempbuf[0] > 99)
						{
							 //与手机实时显示
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

