#ifndef _RC522_H_
#define _RC522_H_


#include "stm32f10x.h"



//片选引脚
#define MF522_NSS_HIGH()      GPIO_SetBits(GPIOA, GPIO_Pin_12)
#define MF522_NSS_LOW()       GPIO_ResetBits(GPIOA, GPIO_Pin_12)		        //csn置低
//复位引脚
#define MF522_RST_HIGH()      GPIO_SetBits(GPIOC, GPIO_Pin_13)
#define MF522_RST_LOW()       GPIO_ResetBits(GPIOC, GPIO_Pin_13)	


/***************************************
//和MF522通讯时返回的错误代码
****************************************/
#define MI_OK                          (0)
#define MI_NOTAGERR                    (-1)
#define MI_ERR                         (-2)


void RC522_Init(void);		  //RC522初始化

void MF522_io_init(void);		//SPI2配置初始化
u8 SPI_MRC_RW(u8 dat);			//用于读/写一字节数据
u8 ReadRawRC(unsigned char Address);	//读RC522寄存器
void WriteRawRC(unsigned char Address, unsigned char value);//写RC522寄存器
void SetBitMask(unsigned char reg,unsigned char mask);//置RC522寄存器位
void ClearBitMask(unsigned char reg,unsigned char mask);//清RC522寄存器位
void PcdAntennaOn(void);		//开启天线 
void PcdAntennaOff(void);		//关闭天线 
//用MF522计算CRC16函数 
void CalulateCRC(unsigned char *pIndata,unsigned char len,unsigned char *pOutData);
signed char PcdReset(void);		//复位RC522
char M500PcdConfigISOType(unsigned char type);//设置RC522的工作方式 
char PcdHalt(void);			//命令卡片进入休眠状态



char PcdComMF522(unsigned char Command, 
                 unsigned char *pInData, 
                 unsigned char InLenByte,
                 unsigned char *pOutData, 
                 unsigned int  *pOutLenBit);  //通过RC522和ISO14443卡通讯

char PcdRequest(unsigned char req_code,unsigned char *pTagType);//寻卡
//扣款和充值
char PcdValue(unsigned char dd_mode,unsigned char addr,unsigned char *pValue);
char PcdBakValue(unsigned char sourceaddr, unsigned char goaladdr);//备份钱包
char PcdWrite(unsigned char addr,unsigned char *pData);//写数据到M1卡一块
char PcdRead(unsigned char addr,unsigned char *pData);//读取M1卡一块数据
char PcdAuthState(unsigned char auth_mode,unsigned char addr,
					unsigned char *pKey,unsigned char *pSnr);	//验证卡片密码
char PcdSelect(unsigned char *pSnr);		//选定卡片
char PcdAnticoll(unsigned char *pSnr);	//防冲撞


int Write_Card(unsigned char* g_ucTempbuf);
int Write_Card2(unsigned char* g_ucTempbuf);


/******************卡的上层操作函数********************************/
//操作卡--扫描卡、防冲撞、选卡、验证A、B区密码
int Card_handle(unsigned char* g_ucTempbuf,unsigned char* serial_number);
//办理新卡
int Card_New(unsigned char* g_ucTempbuf,unsigned char* papers,
						unsigned char* money,unsigned char* serial_number);
int Card_Topup(unsigned char* g_ucTempbuf,int money);		//充值卡
int Card_deduct_money(unsigned char* g_ucTempbuf,int money);//刷卡扣款
int Card_Look_Balance(unsigned char* g_ucTempbuf);	//查看余额
int Card_Look_User(unsigned char* g_ucTempbuf);			//读出用户编号





/*******************************************************************
//MF522命令字
*******************************************************************/
#define PCD_IDLE              0x00               //取消当前命令
#define PCD_AUTHENT           0x0E               //验证密钥
#define PCD_RECEIVE           0x08               //接收数据
#define PCD_TRANSMIT          0x04               //发送数据
#define PCD_TRANSCEIVE        0x0C               //发送并接收数据
#define PCD_RESETPHASE        0x0F               //复位
#define PCD_CALCCRC           0x03               //CRC计算

/*******************************************************************
//Mifare_One卡片命令字
********************************************************************/
#define PICC_REQIDL           0x26               //寻天线区内未进入休眠状态
#define PICC_REQALL           0x52               //寻天线区内全部卡
#define PICC_ANTICOLL1        0x93               //防冲撞
#define PICC_ANTICOLL2        0x95               //防冲撞
#define PICC_AUTHENT1A        0x60               //验证A密钥
#define PICC_AUTHENT1B        0x61               //验证B密钥
#define PICC_READ             0x30               //读块
#define PICC_WRITE            0xA0               //写块
#define PICC_DECREMENT        0xC0               //扣款
#define PICC_INCREMENT        0xC1               //充值
#define PICC_RESTORE          0xC2               //调块数据到缓冲区
#define PICC_TRANSFER         0xB0               //保存缓冲区中数据
#define PICC_HALT             0x50               //休眠

/********************************************************************
//MF522 FIFO长度定义
********************************************************************/
#define DEF_FIFO_LENGTH       64                 //FIFO size=64byte

/*********************************************************
//MF522寄存器定义
**********************************************************/
// PAGE 0
#define     RFU00                 0x00    
#define     CommandReg            0x01    
#define     ComIEnReg             0x02    
#define     DivlEnReg             0x03    
#define     ComIrqReg             0x04    
#define     DivIrqReg             0x05
#define     ErrorReg              0x06    
#define     Status1Reg            0x07    
#define     Status2Reg            0x08    
#define     FIFODataReg           0x09
#define     FIFOLevelReg          0x0A
#define     WaterLevelReg         0x0B
#define     ControlReg            0x0C
#define     BitFramingReg         0x0D
#define     CollReg               0x0E
#define     RFU0F                 0x0F
// PAGE 1     
#define     RFU10                 0x10
#define     ModeReg               0x11
#define     TxModeReg             0x12
#define     RxModeReg             0x13
#define     TxControlReg          0x14
#define     TxAutoReg             0x15
#define     TxSelReg              0x16
#define     RxSelReg              0x17
#define     RxThresholdReg        0x18
#define     DemodReg              0x19
#define     RFU1A                 0x1A
#define     RFU1B                 0x1B
#define     MifareReg             0x1C
#define     RFU1D                 0x1D
#define     RFU1E                 0x1E
#define     SerialSpeedReg        0x1F
// PAGE 2    
#define     RFU20                 0x20  
#define     CRCResultRegM         0x21
#define     CRCResultRegL         0x22
#define     RFU23                 0x23
#define     ModWidthReg           0x24
#define     RFU25                 0x25
#define     RFCfgReg              0x26
#define     GsNReg                0x27
#define     CWGsCfgReg            0x28
#define     ModGsCfgReg           0x29
#define     TModeReg              0x2A
#define     TPrescalerReg         0x2B
#define     TReloadRegH           0x2C
#define     TReloadRegL           0x2D
#define     TCounterValueRegH     0x2E
#define     TCounterValueRegL     0x2F
// PAGE 3      
#define     RFU30                 0x30
#define     TestSel1Reg           0x31
#define     TestSel2Reg           0x32
#define     TestPinEnReg          0x33
#define     TestPinValueReg       0x34
#define     TestBusReg            0x35
#define     AutoTestReg           0x36
#define     VersionReg            0x37
#define     AnalogTestReg         0x38
#define     TestDAC1Reg           0x39  
#define     TestDAC2Reg           0x3A   
#define     TestADCReg            0x3B   
#define     RFU3C                 0x3C   
#define     RFU3D                 0x3D   
#define     RFU3E                 0x3E   
#define     RFU3F		  		  0x3F



#endif 

