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
#include "rc522_driver.h"
#include "stm32f10x.h"
#include <string.h> 

#include "SysTick_Driver.h"
#include "Beep_Driver.h"
#include "Uart_Driver.h"

#include "spi.h"


/****************************************************
**********************引脚连接***********************
**                                                 **
**    RFID RC522                  STM32F103RCT6    **
**           VCC     <-------->       GND          **
**           GND     <-------->       3.3V         **
**			 SPI_NSS     <-------->			  PA12		     **
**			 SPI_CLK     <-------->				PA11		     **
**			SPI_MISO     <-------->				PC2			     **
**			SPI_MOSI     <-------->				PB8          **
**	  			 IRQ     <-------->				PB12         **
**	  			 RST     <-------->				PC13         **
*****************************************************/

#define MAXRLEN 18 

/*
**函数功能：RIFD-RC522端口初始化
*/
void MF522_io_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	/*GPIOC13 输出 RST*/
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		    // 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	MF522_RST_HIGH();
}

/****************************************************
**函数功能：用于读/写一字节数据
**参	数：dat
**返回	值：读取得的数据/状态 
*****************************************************/
u8 SPI_MRC_RW(u8 dat)
{  	
	return RFID_SPI_WriterByte(dat);
}

/******************************************
//功    能：读RC522寄存器
//参数说明：Address[IN]:寄存器地址
//返    回：读出的值
*******************************************/
u8 ReadRawRC(unsigned char Address)
{
	u8   ucAddr;
	u8   ucResult=0;

	MF522_NSS_LOW();	   //取消片选R522
	ucAddr = ((Address<<1)&0x7E)|0x80;
	
	SPI_MRC_RW(ucAddr);
	ucResult = SPI_MRC_RW(0); 	//从SPI2FIFO读取数据
	MF522_NSS_HIGH();		//打开片选

//	printf("ucResult = %d\r\n",ucResult);
	return ucResult;
}

/**************************************************************
**功    能：写RC522寄存器
**参数说明：Address[IN]:寄存器地址
**          value[IN]:写入的值
**************************************************************/
void WriteRawRC(unsigned char Address, unsigned char value)
{  
    u8   ucAddr;

	MF522_NSS_LOW();	   //取消片选R522
    ucAddr = ((Address<<1)&0x7E);

	SPI_MRC_RW(ucAddr);
	SPI_MRC_RW(value);
	MF522_NSS_HIGH();		//打开片选
}

/****************************************************
**功    能：置RC522寄存器位
**参数说明：reg[IN]:寄存器地址
**          mask[IN]:置位值
*****************************************************/
void SetBitMask(unsigned char reg,unsigned char mask)  
{
    char tmp = 0x0;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg,tmp | mask);  // set bit mask
}

/******************************************************
**功    能：清RC522寄存器位
**参数说明：reg[IN]:寄存器地址
**          mask[IN]:清位值
*******************************************************/
void ClearBitMask(unsigned char reg,unsigned char mask)  
{
    char tmp = 0x0;
    tmp = ReadRawRC(reg);
    WriteRawRC(reg, tmp & ~mask);  // clear bit mask
} 	

/********************************************************
**函数功能：开启天线 
**			每次启动或关闭天险发射之间应至少有1ms的间隔
*********************************************************/
void PcdAntennaOn(void)
{
    unsigned char i;

    i = ReadRawRC(TxControlReg);

    if (!(i & 0x03))
    {
        SetBitMask(TxControlReg, 0x03);
    }
}

/********************************************************
**函数功能：关闭天线  
**			每次启动或关闭天险发射之间应至少有1ms的间隔
*********************************************************/
void PcdAntennaOff(void)
{
    ClearBitMask(TxControlReg, 0x03);
}

/*******************************************************************************
**函数功能：用MF522计算CRC16函数
********************************************************************************/
void CalulateCRC(unsigned char *pIndata,unsigned char len,unsigned char *pOutData)
{
    unsigned char i,n;

    ClearBitMask(DivIrqReg,0x04);
    WriteRawRC(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);

    for (i=0; i<len; i++)
    {   
		WriteRawRC(FIFODataReg, *(pIndata+i));   
	}
    WriteRawRC(CommandReg, PCD_CALCCRC);

    i = 0xFF;

    do 
    {
        n = ReadRawRC(DivIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x04));

    pOutData[0] = ReadRawRC(CRCResultRegL);
    pOutData[1] = ReadRawRC(CRCResultRegM);
}


/******************************
//功    能：复位RC522
//返    回: 成功返回MI_OK
*******************************/
signed char PcdReset(void)
{
	MF522_RST_HIGH();	//复位引脚置位
	delay_us(1);
	MF522_RST_LOW();	//复位引脚清0
	delay_us(1);
	MF522_RST_HIGH();	//复位引脚置位
	delay_us(1);
	WriteRawRC(CommandReg,PCD_RESETPHASE);				 //启动卡操作
	delay_us(1);
	
	WriteRawRC(ModeReg,0x3D);            //和Mifare卡通讯，CRC初始值0x6363
	WriteRawRC(TReloadRegL,30);           
	WriteRawRC(TReloadRegH,0);
	WriteRawRC(TModeReg,0x8D);			
	WriteRawRC(TPrescalerReg,0x3E);
	WriteRawRC(TxAutoReg,0x40);  
	   
	return MI_OK;
}

/****************************************************
**函数功能：设置RC522的工作方式 
****************************************************/
char M500PcdConfigISOType(unsigned char type)
{
	if (type == 'A')                     //ISO14443_A
	{ 
		ClearBitMask(Status2Reg,0x08);
		
		/*     WriteRawRC(CommandReg,0x20);    //as default   
		WriteRawRC(ComIEnReg,0x80);     //as default
		WriteRawRC(DivlEnReg,0x0);      //as default
		WriteRawRC(ComIrqReg,0x04);     //as default
		WriteRawRC(DivIrqReg,0x0);      //as default
		WriteRawRC(Status2Reg,0x0);//80    //trun off temperature sensor
		WriteRawRC(WaterLevelReg,0x08); //as default
		WriteRawRC(ControlReg,0x20);    //as default
		WriteRawRC(CollReg,0x80);    //as default
		*/
		WriteRawRC(ModeReg,0x3D);//3F
		/*	   WriteRawRC(TxModeReg,0x0);      //as default???
		WriteRawRC(RxModeReg,0x0);      //as default???
		WriteRawRC(TxControlReg,0x80);  //as default???
		
		WriteRawRC(TxSelReg,0x10);      //as default???
		*/
		WriteRawRC(RxSelReg,0x86);//84
		//      WriteRawRC(RxThresholdReg,0x84);//as default
		//      WriteRawRC(DemodReg,0x4D);      //as default
		
		//      WriteRawRC(ModWidthReg,0x13);//26
		WriteRawRC(RFCfgReg,0x7F);   //4F
		/*   WriteRawRC(GsNReg,0x88);        //as default???
		WriteRawRC(CWGsCfgReg,0x20);    //as default???
		WriteRawRC(ModGsCfgReg,0x20);   //as default???
		*/
		WriteRawRC(TReloadRegL,30);//tmoLength);// TReloadVal = 'h6a =tmoLength(dec) 
		WriteRawRC(TReloadRegH,0);
		WriteRawRC(TModeReg,0x8D);
		WriteRawRC(TPrescalerReg,0x3E);
		
	
		//     PcdSetTmo(106);
		delay_ms(10);
		PcdAntennaOn();
	}
	else
	{ 
		return (char)MI_NOTAGERR; 
	}
	
	return MI_OK;
}

/*******************************************
**功    能：命令卡片进入休眠状态
**返    回: 成功返回MI_OK
********************************************/
char PcdHalt(void)
{
    char status;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_HALT;
    ucComMF522Buf[1] = 0;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    return status;
}

/**********************************************************
**功    能：写数据到M1卡一块
**参数说明: addr[IN]：块地址
**          pData[IN]：写入的数据，16字节
**返    回: 成功返回MI_OK
**********************************************************/                  
char PcdWrite(unsigned char addr,unsigned char *pData)
{
    char status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_WRITE;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = (char)MI_ERR;   }
        
    if (status == MI_OK)
    {
        //memcpy(ucComMF522Buf, pData, 16);
        for (i=0; i<16; i++)
        {    ucComMF522Buf[i] = *(pData+i);   }
        CalulateCRC(ucComMF522Buf,16,&ucComMF522Buf[16]);

        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,18,ucComMF522Buf,&unLen);
        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {   status = (char)MI_ERR;   }
    }
    
    return status;
}

/**********************************************************
**功    能：读取M1卡一块数据
**参数说明: addr[IN]：块地址
**          pData[OUT]：读出的数据，16字节
**返    回: 成功返回MI_OK
***********************************************************/ 
char PcdRead(unsigned char addr,unsigned char *pData)
{
    char status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_READ;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
   
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    if ((status == MI_OK) && (unLen == 0x90))
 //   {   memcpy(pData, ucComMF522Buf, 16);   }
    {
        for (i=0; i<16; i++)
        {    *(pData+i) = ucComMF522Buf[i];   }
    }
    else
    {   status = (char)MI_ERR;   }
    
    return status;
}




/*****************************************************************/





/*****************************************************************
**功    能：寻卡  /扫描卡
**参数说明: req_code[IN]:寻卡方式
**                0x52 = 寻感应区内所有符合14443A标准的卡
**                0x26 = 寻未进入休眠状态的卡
**          pTagType[OUT]：卡片类型代码
**                0x4400 = Mifare_UltraLight
**                0x0400 = Mifare_One(S50)
**                0x0200 = Mifare_One(S70)
**                0x0800 = Mifare_Pro(X)
**                0x4403 = Mifare_DESFire
**返    回: 成功返回MI_OK
******************************************************************/
char PcdRequest(unsigned char req_code,unsigned char *pTagType)
{
	char status;  
	unsigned int  unLen;
	unsigned char ucComMF522Buf[MAXRLEN]; 
	unsigned char xTest ;
	
	
	ClearBitMask(Status2Reg,0x08);
	WriteRawRC(BitFramingReg,0x07);
/*****************调试***********************/	
//	xTest = ReadRawRC(BitFramingReg);
//	if(xTest == 0x07 )
//	{ 
//		printf("PcdRequest WRITE and READ SUCCEED!!\r\n");
////		while(1);
//	}
//	else 
//	{
//		printf("PcdRequest WRITE and READ ERROR!!\r\n");
//		while(1);
//	}
/********************************************/

	SetBitMask(TxControlReg,0x03);
	
	ucComMF522Buf[0] = req_code;
	
	status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,1,ucComMF522Buf,&unLen);

/**************************调试****************************/
//	if(status  == MI_OK )
//	{ 
//		printf("eturn status OK!!\r\n");
////		while(1);
//	}
//	else 
//	{
//		printf("return status ERROR!!\r\n");
////		while(1);
//	}
/******************************************************/
	if ((status == MI_OK) && (unLen == 0x10))
	{    
		*pTagType     = ucComMF522Buf[0];
		*(pTagType+1) = ucComMF522Buf[1];
	}
	else
	{   
		status = (char)MI_ERR;   
	}
	
	return status;
}


/****************************************************************************
**功    能：扣款和充值
**参数说明: dd_mode[IN]：命令字
**               0xC0 = 扣款
**               0xC1 = 充值
**          addr[IN]：钱包地址
**          pValue[IN]：4字节增(减)值，低位在前
**返    回: 成功返回MI_OK
*****************************************************************************/                 
char PcdValue(unsigned char dd_mode,unsigned char addr,unsigned char *pValue)
{
    char status;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = dd_mode;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = (char)MI_ERR;   }
        
    if (status == (char)MI_OK)
    {
        memcpy(ucComMF522Buf, pValue, 4);
 //       for (i=0; i<16; i++)
 //       {    ucComMF522Buf[i] = *(pValue+i);   }
        CalulateCRC(ucComMF522Buf,4,&ucComMF522Buf[4]);
        unLen = 0;
        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,6,ucComMF522Buf,&unLen);
        if (status != (char)MI_ERR)
        {    status = (char)MI_OK;    }
    }
    
    if (status == (char)MI_OK)
    {
        ucComMF522Buf[0] = PICC_TRANSFER;
        ucComMF522Buf[1] = addr;
        CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]); 
   
        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

        if ((status != (char)MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {  
			 status = (char)MI_ERR;   
		
		}
    }
    return status;
}

/***************************************************************
**功    能：备份钱包
**参数说明: sourceaddr[IN]：源地址
**          goaladdr[IN]：目标地址
**返    回: 成功返回MI_OK
*****************************************************************/
char PcdBakValue(unsigned char sourceaddr, unsigned char goaladdr)
{
    char status;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_RESTORE;
    ucComMF522Buf[1] = sourceaddr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   
		status = (char)MI_ERR;   
	}
    
    if (status == MI_OK)
    {
        ucComMF522Buf[0] = 0;
        ucComMF522Buf[1] = 0;
        ucComMF522Buf[2] = 0;
        ucComMF522Buf[3] = 0;
        CalulateCRC(ucComMF522Buf,4,&ucComMF522Buf[4]);
 
        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,6,ucComMF522Buf,&unLen);
        if (status != (char)MI_ERR)
        {    
			status = (char)MI_OK;    
		}
    }
    
    if (status != MI_OK)
    {    
	 return (char)MI_ERR;  
	 
	}
    
    ucComMF522Buf[0] = PICC_TRANSFER;
    ucComMF522Buf[1] = goaladdr;

    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   
		status = (char)MI_ERR;  
	 
	 }

    return status;
}

/************************************************************************
**功    能：验证卡片密码
**参数说明: auth_mode[IN]: 密码验证模式
**                 0x60 = 验证A密钥
**                 0x61 = 验证B密钥 
**          addr[IN]：块地址
**          pKey[IN]：密码
**          pSnr[IN]：卡片序列号，4字节
**返    回: 成功返回MI_OK
*************************************************************************/               
char PcdAuthState(unsigned char auth_mode,unsigned char addr,unsigned char *pKey,unsigned char *pSnr)
{
    char status;
    unsigned int  unLen;
    unsigned char i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = auth_mode;
    ucComMF522Buf[1] = addr;
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+2] = *(pKey+i);   }
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+8] = *(pSnr+i);   }
 //   memcpy(&ucComMF522Buf[2], pKey, 6); 
 //   memcpy(&ucComMF522Buf[8], pSnr, 4); 
    
    status = PcdComMF522(PCD_AUTHENT,ucComMF522Buf,12,ucComMF522Buf,&unLen);
    if ((status != MI_OK) || (!(ReadRawRC(Status2Reg) & 0x08)))
    {   status = (char)MI_ERR;   }
    	status=0x00;
    return status;
}

/********************************************************************
**功    能：选定卡片
**参数说明: pSnr[IN]:卡片序列号，4字节
**返    回: 成功返回MI_OK
***********************************************************************/
char PcdSelect(unsigned char *pSnr)
{
    char status;
    unsigned char i;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i=0; i<4; i++)
    {
    	ucComMF522Buf[i+2] = *(pSnr+i);
    	ucComMF522Buf[6]  ^= *(pSnr+i);
    }
    CalulateCRC(ucComMF522Buf,7,&ucComMF522Buf[7]);
  
    ClearBitMask(Status2Reg,0x08);

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,9,ucComMF522Buf,&unLen);
    
    if ((status == MI_OK) && (unLen == 0x18))
    {   status = MI_OK;  }
    else
    {   status = (char)MI_ERR;    }

    return status;
}

/********************************************
**功    能：防冲撞
**参数说明: pSnr[OUT]:卡片序列号，4字节
**返    回: 成功返回MI_OK
*********************************************/  
char PcdAnticoll(unsigned char *pSnr)
{
    char status;
    unsigned char i,snr_check=0;
    unsigned int  unLen;
    unsigned char ucComMF522Buf[MAXRLEN]; 
    

    ClearBitMask(Status2Reg,0x08);
    WriteRawRC(BitFramingReg,0x00);
    ClearBitMask(CollReg,0x80);
 
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x20;

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen);

    if (status == MI_OK)
    {
    	 for (i=0; i<4; i++)
         {   
             *(pSnr+i)  = ucComMF522Buf[i];
             snr_check ^= ucComMF522Buf[i];
         }
         if (snr_check != ucComMF522Buf[i])
         {   status = (char)MI_ERR;    }
    }
    
    SetBitMask(CollReg,0x80);
    return status;
}

/*******************************************************************
//功    能：通过RC522和ISO14443卡通讯
//参数说明：Command[IN]:RC522命令字
//          pInData[IN]:通过RC522发送到卡片的数据
//          InLenByte[IN]:发送数据的字节长度
//          pOutData[OUT]:接收到的卡片返回数据
//          *pOutLenBit[OUT]:返回数据的位长度
********************************************************************/
char PcdComMF522(unsigned char Command, 
                 unsigned char *pInData, 
                 unsigned char InLenByte,
                 unsigned char *pOutData, 
                 unsigned int  *pOutLenBit)
{
	char status = (char)MI_ERR;
	unsigned char irqEn   = 0x00;
	unsigned char waitFor = 0x00;
	unsigned char lastBits;
	unsigned char n;
	unsigned int i;
	switch (Command)
	{
		case PCD_AUTHENT:
			irqEn   = 0x12;
			waitFor = 0x10;
			break;
		case PCD_TRANSCEIVE:
			irqEn   = 0x77;
			waitFor = 0x30;
			break;
			default:
		break;
	}
	
	WriteRawRC(ComIEnReg,irqEn|0x80);
	ClearBitMask(ComIrqReg,0x80);
	WriteRawRC(CommandReg,PCD_IDLE);
	SetBitMask(FIFOLevelReg,0x80);
		
	for (i=0; i<InLenByte; i++)
	{   
		WriteRawRC(FIFODataReg, pInData[i]);    
	}

	WriteRawRC(CommandReg, Command);
	
	
	if (Command == PCD_TRANSCEIVE)
	{    
		SetBitMask(BitFramingReg,0x80);  }
	
		//    i = 600;//根据时钟频率调整，操作M1卡最大等待时间25ms
		i = 2000;
	do 
	{
		n = ReadRawRC(ComIrqReg);
		i--;
	}
	while ((i!=0) && !(n&0x01) && !(n&waitFor));
	ClearBitMask(BitFramingReg,0x80);
	
	if (i!=0)
	{    
		if(!(ReadRawRC(ErrorReg)&0x1B))
		{
			status = (char)MI_OK;
			if (n & irqEn & 0x01)
			{   
				status = (char)MI_NOTAGERR;   
			}

			if (Command == PCD_TRANSCEIVE)
			{
				n = ReadRawRC(FIFOLevelReg);
				lastBits = ReadRawRC(ControlReg) & 0x07;
				if (lastBits)
				{   
					*pOutLenBit = (n-1)*8 + lastBits;   
				}
				else
				{   
					*pOutLenBit = n*8;   
				}

				if (n == 0)
				{   
					n = 1;    
				}

				if (n > MAXRLEN)
				{   
					n = MAXRLEN;   
				}

				for (i=0; i<n; i++)
				{   
					pOutData[i] = ReadRawRC(FIFODataReg);    
				}
			}
		}
		else
		{  
			status = (char)MI_ERR;  
		}
	}
	
	SetBitMask(ControlReg,0x80);           // stop timer now
	WriteRawRC(CommandReg,PCD_IDLE); 
	
	return status;
} 

/***********************************
**函数功能：RC522初始化
***********************************/
void RC522_Init(void)
{
	MF522_io_init(); 	//端口初始化
	PcdReset(); 	//复位RC522
	PcdAntennaOff(); 	//关闭天线 
	PcdAntennaOn();  	//打开天线
	M500PcdConfigISOType( 'A' );	//设置RC522的工作方式 
	delay_ms(100);	
	delay_ms(100);

}



/*************************************************************************************/
/*************************************应用程序实现************************************/
/*************************************************************************************/

/**************************************************************
**函数功能：操作卡--扫描卡、防冲撞、选卡、验证A、B区密码
**参		数：g_ucTempbuf---------卡片类型代码
**				serial_number-------卡片序列号存储
**返回	值：ret == 5时，所有操作正确，否则都是错误操作
**************************************************************/
int Card_handle(unsigned char* g_ucTempbuf,unsigned char* serial_number)
{
	u8 status;
	u8 temp;
	int i = 0;
	int ret = 0;
	unsigned char  DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	
	status = PcdRequest(PICC_REQALL,g_ucTempbuf);/*扫描卡*/
	if(status==0)	//扫描成功
	{	
//		printf("卡的类型:");
		for(i=0;i<2;i++)
		{	
			temp=g_ucTempbuf[i];
			printf("%X",temp);					
		}
		ret = 1;
	}
	else
	{
		//再扫描一次
		status = PcdRequest(PICC_REQALL,g_ucTempbuf);/*扫描卡*/
		if(status==0)	//扫描成功
		{	
	//		printf("卡的类型:");
			for(i=0;i<2;i++)
			{	
				temp=g_ucTempbuf[i];
//				printf("%X",temp);					
			}
			ret = 1;
		}
		else
		{
//			printf("\r\n扫描失败\r\n");
			ret = -1;
		}
	}
		
	status = PcdAnticoll(g_ucTempbuf);/*防冲撞*/ 
	if(status == 0)	//读序列号成功
	{ 
	//	printf("\r\n卡的序号");
		for(i=0;i<4;i++)
		{
			temp=g_ucTempbuf[i];
//			printf("%X",temp);		
			if(serial_number != NULL)
			{
				serial_number[i] = temp;
			}
		}
		ret += 1;
	}
	else
	{
		ret -= 1;
//		printf("\r\n防冲撞失败\r\n");
	}
	
	status = PcdSelect(g_ucTempbuf);//选定卡片
	if(status == 0)
	{
		ret += 1;
//		printf("\r\n卡选定成功\n");
	} 
	else 
	{
//		printf("\r\n卡选定失败\r\n");
		ret -= 1;
	}
	
	if(ret == 3)
	{
		//校验A密码
		status = PcdAuthState(PICC_AUTHENT1A, 1, DefaultKey, g_ucTempbuf);//验证卡片密码    
		if (status == MI_OK)
		{   
			ret += 1;
//			printf("校验A密码成功\r\n");
		}
		else 
		{
			ret -= 1;
//			printf("校验A密码失败\r\n");
		}
		
		//校验B密码
		status = PcdAuthState(PICC_AUTHENT1B, 1, DefaultKey, g_ucTempbuf);//验证卡片密码    
		if (status == MI_OK)
		{   
			ret += 1;
//			printf("校验B密码成功\r\n");
		}
		else 
		{
			ret -= 1;
//			printf("校验B密码失败\r\n");
		}
	}
	
	return  ret;
}

/**************************************
**只使用扇区0，块1----存放系统生成的编号
**					   块2-----存放卡中余额
**************************************/
/*************************************************
**函数功能：办理新卡
**					将生成用户编号写入扇区0
**					块1中，将第一次金额写入
**					块2中
**参		数：g_ucTempbuf---------卡片类型代码
**					papers--------------客户信息号
**					money---------------第一次充值金额
**					serial_number-------卡的序列号，返回和用户信息一起登记
**					没张IC卡都有一个唯一的序列号，作为与用户绑定
**返回	值：只有当ret == 2时，才能认为操作成功了，否则操作失败！
***************************************************/
int Card_New(unsigned char* g_ucTempbuf,unsigned char* papers,
						unsigned char* money,unsigned char* serial_number)
{
	int ret = 0;
	u8 status = 0;
	u8 buff[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	
	//扫描卡等等 ,将卡的序列号存放在serial_number
	status = Card_handle(g_ucTempbuf,serial_number);
	if(status == 5)		//只有扫描成功······一直到密码验证成功，才进行信息的写入
	{
		status = PcdWrite(1, buff);	//先将块清0
		status = PcdWrite(1, papers);	//将客户信息编号写入块1
		if(status==0)
		{
			ret += 1;
	//		printf("客户信息写入成功\r\n");
		}
		else
		{
			ret -= 1;
//			printf("客户信息写入失败\r\n");
		}
		
		status = PcdWrite(2, buff);	//先将块清0
		status = PcdWrite(2, money);	//将客户信息编号写入块1
		if(status==0)
		{
			ret += 1;

		}
		else
		{
			ret -= 1;

		}
	}
	
	return ret;
}




/*********************************
**函数功能：充值卡-----从扇区0
**					块2加值
**参		数：g_ucTempbuf---------卡片类型代码
**					money---------------充值金额
**返回	值：ret == 1时，表示充值成功
*********************************/
int Card_Topup(unsigned char* g_ucTempbuf,int money)
{
	int ret= 0;
	u8 status = 0;
	u8 raw_money[16];
	int i = 0;
	int temp = 0;
	
	PcdReset();
	while(Card_Look_Balance(g_ucTempbuf) != 1);		//将卡内余额读出
	for(i=0;i<4;i++)		//金额用到4字节
	{
		raw_money[i]=g_ucTempbuf[i];
		printf("%X",raw_money[i]);
	}	
	printf("\r\n");
	//数据处理,将读到的数据转换为一个32位的int型整数
	temp = raw_money[3];//将最高字节给temp
	temp <<= 8;			//temp左移8位
	temp |= raw_money[2];
	temp <<= 8;
	temp |= raw_money[1];
	temp <<= 8;
	temp |= raw_money[0];
	
	//数据已经还原,加上充值金额
	temp += money;
	
	//又将数据还原成4字节大小的char型数组
	raw_money[0] = 0xff & temp;
	temp >>= 8;
	raw_money[1] = 0xff & temp;
	temp >>= 8;
	raw_money[2] = 0xff & temp;
	temp >>= 8;
	raw_money[3] = 0xff & temp;
	
	//将数组没用用到的12字节配置为0
	for(i = 4;i < 16;i++)
	{
		raw_money[i] = 0;
	}
	
	//将数据写到块1中
	status = PcdWrite(2, raw_money);	//将金额写入块2
	if(status==0)
	{
		ret = 1;
		
	}
	else
	{
		ret = -1;
	
	}
	
	return ret;
}


/**********************************
**函数功能：刷卡扣款-----从扇区0
**					块2减值
**参		数：g_ucTempbuf---------卡片类型代码
**					money------------扣除金额
**返回	值：ret == 1时，表示充值成功
***********************************/
int Card_deduct_money(unsigned char* g_ucTempbuf,int money)
{
	int ret= 0;
	u8 status = 0;
	u8 raw_money[16];
	int i = 0;
	int temp = 0;
	
	PcdReset();
	while(Card_Look_Balance(g_ucTempbuf) != 1);		//将卡内余额读出
	for(i=0;i<4;i++)		//金额用到4字节
	{
		raw_money[i]=g_ucTempbuf[i];
		printf("%X",raw_money[i]);
	}	
	
	printf("\r\n");
	
	//数据处理,将读到的数据转换为一个32位的int型整数
	temp = raw_money[3];//将最高字节给temp
	temp <<= 8;			//temp左移8位
	temp |= raw_money[2];
	temp <<= 8;
	temp |= raw_money[1];
	temp <<= 8;
	temp |= raw_money[0];
	
	//数据已经还原,减去扣取金额
	temp -= money;
	
	//又将数据还原成4字节大小的char型数组
	raw_money[0] = 0xff & temp;
	temp >>= 8;
	raw_money[1] = 0xff & temp;
	temp >>= 8;
	raw_money[2] = 0xff & temp;
	temp >>= 8;
	raw_money[3] = 0xff & temp;
	
	//将数组没用用到的12字节配置为0
	for(i = 4;i < 16;i++)
	{
		raw_money[i] = 0;
	}
	
	//将数据写到块1中
	status = PcdWrite(2, raw_money);	//将金额写入块2
	if(status==0)
	{
		ret = 1;

	}
	else
	{
		ret = -1;
	
	}
	
	return ret;
}


/***********************************************************
**函数功能：查看余额
**参		数：g_ucTempbuf---------卡片类型代码
**					读成功后，余额值存放在g_ucTempbuf中，及时读取即可
**返回	值：ret == 1时，表示充值成功
***********************************************************/
int Card_Look_Balance(unsigned char* g_ucTempbuf)
{
	int ret = 0;
	u8 status  = 0;
	
	status = Card_handle(g_ucTempbuf,NULL);//扫描卡等等
	if(status == 5)		//只有扫描成功······一直到密码验证成功，才进行信息的写入
	{
		status = PcdRead(0x02, g_ucTempbuf);//读块2
		if(status==0)
		{
			ret = 1;
			printf("read成功\r\n");
		}
		else
		{
			ret = -1;
			printf("read失败\r\n");
		}
	}
	
	return ret;
}


/**********************************
**函数功能：读出用户编号
**					从扇区0块1中读取
**参		数：g_ucTempbuf---------卡片类型代码
**					读成功后，用户编号存放在g_ucTempbuf中，及时读取即可
**返回	值：ret == 1时，表示充值成功
**********************************/
int Card_Look_User(unsigned char* g_ucTempbuf)
{
	int ret = 0;
	u8 status  = 0;
	
	status = Card_handle(g_ucTempbuf,NULL);//扫描卡等等
	if(status == 5)		//只有扫描成功······一直到密码验证成功，才进行信息的写入
	{
		status = PcdRead(0x01, g_ucTempbuf);//读块1
		if(status==0)
		{
			ret = 1;
		}
		else
		{
			ret = -1;
		}
	}
	
	return ret;
}



int Write_Card(unsigned char* g_ucTempbuf)
{
	int ret= 0;
	u8 status = 0;

	//将数据写到块1中
	status = PcdWrite(1, g_ucTempbuf);	//将金额写入块2
	if(status==0)
	{
		ret = 1;
		printf("充值成功\r\n");
	}
	else
	{
		ret = -1;
	}
	
	return ret;
}


int Write_Card2(unsigned char* g_ucTempbuf)
{
	int ret= 0;
	u8 status = 0;

	//将数据写到块1中
	status = PcdWrite(1, g_ucTempbuf);	//将金额写入块2
	if(status==0)
	{
		ret = 1;
		printf("扣款成功\r\n");
	}
	else
	{
		ret = -1;
	}
	
	return ret;
}




