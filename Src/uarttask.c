#include "main.h"
#include "uarttask.h"


#define FOSC     33117600UL       //22118400UL

#define BRT_9600             (65536 - FOSC / 9600 / 4)
#define BRT_250K             (65536 - FOSC / 250000 / 4)
#define BRT_115200           (65536 - FOSC / 115200 / 4)
#define BRT_57600             (65536 - FOSC / 57600 / 4)


#define ES2 0x01
#define ES3 0x08
#define ES4 0x10
//UART_Memory uart1Memory;
UART_Memory uart2Memory;
//UART_Memory uart3Memory;
//UART_Memory uart4Memory;


void Uart2Init(void)		//115200bps@33.1776MHz
{
    S2CON |= 0x10;
    T2L = BRT_9600;                                
    T2H = BRT_9600>>8;
    AUXR |= 0x14;                                //?????
    IE2 |= ES2;                                  //??????
    ClearMemory((uint8_t*)&uart2Memory,sizeof(UART_Memory));
	ClearMemory((u8*)&uart2rcpkg,sizeof(UARTST_PKG));
}


void Uart4Init(void)		//115200bps@33.1776MHz
{
	S4CON = 0x10;		//8位数据,可变波特率
	S4CON |= 0x40;		//串口4选择定时器4为波特率发生器
	T4T3M |= 0x20;		//定时器4时钟为Fosc,即1T
	T4L = 0xB8;		//设定定时初值
	T4H = 0xFF;		//设定定时初值
	T4T3M |= 0x80;		//启动定时器4	
	IE2 |= ES4;                                  //??????
	ClearMemory((u8*)&uart4rcpkg,sizeof(UARTST4_PKG));
}

void Uart4Init_9600(void)		//9600bps@33.1776MHz
{
	S4CON = 0x10;		//8位数据,可变波特率
	S4CON |= 0x40;		//串口4选择定时器4为波特率发生器
	T4T3M |= 0x20;		//定时器时钟1T模式
	T4L = 0xA0;		//设置定时初始值
	T4H = 0xFC;		//设置定时初始值
	T4T3M |= 0x80;		//定时器4开始计时
	IE2 |= ES4;                                  //??????
	ClearMemory((u8*)&uart4rcpkg,sizeof(UARTST4_PKG));
}




/*

uint8_t CompString(uint8_t *str1,uint8_t *str2,uint8_t len)
{
	while (len>0)
	{
		if ((*str1)!=(*str2)) return 0;
		len--;
		str1++;
		str2++;
	}
	return 1;
}

int32_t String2Number(uint8_t *str1)
{
   int32_t ttm=0;
	 int8_t zf=1;
	while (1)
	{
		if ((*str1)>='0' && (*str1)<='9')
			{
			ttm=ttm*10+(*str1)-'0';
			}
		else if ((*str1)=='-')
			{
			 zf=-1;
			}
		else break;
		str1++;
	}
	return zf*ttm;
}
*/



/*
  
void SetAddressValue(uint16_t add,uint16_t va)
{
	UARTST_PKG sendpkg;
	sendpkg.pklen=0x05;
	sendpkg.cmd=0x82;
	sendpkg.UartRunBuf[0]=add>>8;  //Addr_h
	sendpkg.UartRunBuf[1]=add&0xff;	//Addr_l
	sendpkg.UartRunBuf[2]=va>>8;	//data1
	sendpkg.UartRunBuf[3]=va&0xff;	//data1
	SendToLcd(&sendpkg);
}

void SetAddressU32Value(uint16_t add,u32 va)
{
	UARTST_PKG sendpkg;
	sendpkg.pklen=0x07;
	sendpkg.cmd=0x82;
	sendpkg.UartRunBuf[0]=add>>8;  //Addr_h
	sendpkg.UartRunBuf[1]=add&0xff;	//Addr_l
	sendpkg.UartRunBuf[2]=va>>24;	//data1
	sendpkg.UartRunBuf[3]=va>>16;	//data2
	sendpkg.UartRunBuf[4]=va>>8;	//data3
	sendpkg.UartRunBuf[5]=va;	//data4
	SendToLcd(&sendpkg);
}



void ReadDispId(void)
{
	UARTST_PKG sendpkg;
	
	sendpkg.pklen=0x03;
	sendpkg.cmd=0x81;
	sendpkg.UartRunBuf[0]=0x03;  //Addr
	sendpkg.UartRunBuf[1]=2;	//data0
	SendToLcd(&sendpkg);
}


void ChangeDispId(uint16_t picid)
{
	UARTST_PKG sendpkg;
	
	sendpkg.pklen=0x04;
	sendpkg.cmd=0x80;
	sendpkg.UartRunBuf[0]=0x03;  //Addr
	sendpkg.UartRunBuf[1]=picid>>8;	//data0
	sendpkg.UartRunBuf[2]=picid&0xff;	//data1
	SendToLcd(&sendpkg);
	LCD_DISPiD=picid;
}


void SetSeqAddressValue(uint16_t add,uint8_t* va,uint8_t num)
{
	UARTST_PKG sendpkg;
	uint8_t i;
	sendpkg.pklen=5+num;
	sendpkg.cmd=0x82;
	sendpkg.UartRunBuf[0]=add>>8;  //Addr_h
	sendpkg.UartRunBuf[1]=add&0xff;	//Addr_l
	for (i=0;i<num;i++)
		{
		sendpkg.UartRunBuf[2+i]=(*va);	//data
		va++;
		}

	SendToLcd(&sendpkg);
}

void SetSeqAddressIntValue(uint16_t add,uint8_t* va,uint8_t num)
{
	UARTST_PKG sendpkg;
	uint8_t i;
	num=num<<1;
	sendpkg.pklen=5+num;
	sendpkg.cmd=0x82;
	sendpkg.UartRunBuf[0]=add>>8;  //Addr_h
	sendpkg.UartRunBuf[1]=add&0xff;	//Addr_l
	for (i=0;i<num;i+=2)
		{
		sendpkg.UartRunBuf[2+i]=0;	
		sendpkg.UartRunBuf[3+i]=(*va);	//data
		
		va++;
		}

	SendToLcd(&sendpkg);
}

/*
void ClearSeqAddressValue(uint16_t add,uint8_t num)
{
	UARTST_PKG sendpkg;
	uint8_t i;
	num=num<<1;
	sendpkg.pklen=5+num;
	sendpkg.cmd=0x82;
	sendpkg.UartRunBuf[0]=add>>8;  //Addr_h
	sendpkg.UartRunBuf[1]=add&0xff;	//Addr_l
	for (i=0;i<num;i++)
		{
		sendpkg.UartRunBuf[2+i]=0;	//data
		}

	SendToLcd(&sendpkg);
}
*/

void SendToUart2(UARTST_PKG * sendpkg)
{
	uint8_t i;

    while (uart2Memory.busy==1) WDTRST;                      //清看门狗,否则系统复位
    uart2Memory.busy = 1;
    S2BUF = 0X5A;

    while (uart2Memory.busy==1) WDTRST;                      //清看门狗,否则系统复位
    uart2Memory.busy = 1;
    S2BUF = 0XA5;

    while (uart2Memory.busy) WDTRST;                      //清看门狗,否则系统复位
    uart2Memory.busy = 1;
    S2BUF = sendpkg->pklen;

    while (uart2Memory.busy) WDTRST;                      //清看门狗,否则系统复位
    uart2Memory.busy = 1;
    S2BUF = sendpkg->cmd;
	
	for (i=0;i<(sendpkg->pklen-1);i++)
	{
        while (uart2Memory.busy) WDTRST;                      //清看门狗,否则系统复位
	    uart2Memory.busy = 1;
	    S2BUF = sendpkg->UartRunBuf[i];
	}
	

}

void WriteDriverAddressValue(uint8_t add,uint8_t va)
{
	UARTST_PKG sendpkg;
	sendpkg.pklen=3;
	sendpkg.cmd=0x80;
	sendpkg.UartRunBuf[0]=add;  //Addr_h
	sendpkg.UartRunBuf[1]=va;	//data
	SendToUart2(&sendpkg);
}

void WriteDriverAddressValue2(uint8_t add,uint8_t va1,uint8_t va2)
{
	UARTST_PKG sendpkg;
	sendpkg.pklen=4;
	sendpkg.cmd=0x80;
	sendpkg.UartRunBuf[0]=add;  //Addr_h
	sendpkg.UartRunBuf[1]=va1;	//data
	sendpkg.UartRunBuf[2]=va2;	//data
	SendToUart2(&sendpkg);
}



void WriteDriverSqAddressValue(uint8_t add,uint8_t* va,uint8_t len)
{
	u8 i;
	
	UARTST_PKG sendpkg;
	sendpkg.pklen=2+len;
	sendpkg.cmd=0x80;
	sendpkg.UartRunBuf[0]=add;  //Addr_h
	for (i=0;i<len;i++)
		{
		sendpkg.UartRunBuf[i+1]=*va;	//data
		va++;
		}
	SendToUart2(&sendpkg);
}



void ReadDriverAddressValue(uint8_t add,uint8_t num)
{
	UARTST_PKG sendpkg;
	sendpkg.pklen=3;
	sendpkg.cmd=0x81;
	sendpkg.UartRunBuf[0]=add;  //Addr_h
	sendpkg.UartRunBuf[1]=num;	//data1
	SendToUart2(&sendpkg);
}


/*
void SendToUart2(UARTST_PKG * sendpkg)
{
	uint8_t i;

    while (uart2Memory.busy==1) WDTRST;                      //清看门狗,否则系统复位
    uart2Memory.busy = 1;
    S2BUF = 0X5A;

    while (uart2Memory.busy==1) WDTRST;                      //清看门狗,否则系统复位
    uart2Memory.busy = 1;
    S2BUF = 0XA5;

    while (uart2Memory.busy) WDTRST;                      //清看门狗,否则系统复位
    uart2Memory.busy = 1;
    S2BUF = sendpkg->pklen;

    while (uart2Memory.busy) WDTRST;                      //清看门狗,否则系统复位
    uart2Memory.busy = 1;
    S2BUF = sendpkg->cmd;
	
	for (i=0;i<(sendpkg->pklen-1);i++)
	{
        while (uart2Memory.busy) WDTRST;                      //清看门狗,否则系统复位
	    uart2Memory.busy = 1;
	    S2BUF = sendpkg->UartRunBuf[i];
	}

}

void ReturnRecUart2(uint8_t add,uint8_t* va,uint8_t len)
{
	u8 i;
	UARTST_PKG sendpkg;
	sendpkg.pklen=3+len;
	sendpkg.cmd=0x82;
	sendpkg.UartRunBuf[0]=add;	//Addr_h
	sendpkg.UartRunBuf[1]=len;
	for (i=0;i<len;i++)
		{
		sendpkg.UartRunBuf[i+2]=*va;	//data
		va++;
		}
	SendToUart2(&sendpkg);
}

void SetAddressValueU2(uint16_t add,uint8_t va)
{
	UARTST_PKG sendpkg;
//	uint8_t i;
	sendpkg.pklen=5;
	sendpkg.cmd=0x82;
	sendpkg.UartRunBuf[0]=add>>8;  //Addr_h
	sendpkg.UartRunBuf[1]=add&0xff;	//Addr_l
	sendpkg.UartRunBuf[2]=va;	//data
	SendToUart2(&sendpkg);
}
*/

