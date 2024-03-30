/**
  ******************************************************************************
  * @file    Project/main.c 
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   Main program body
 */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "lcd.h"
#include "ABencoder.h"
#include "uarttask.h"
#include "spi_flash.h"
#include "eeprom.h"
#include "SaveConfigfile.h"
#include "uuid.h"
/* Private defines -----------------------------------------------------------*/

#define NOREPLSYTIMES 5
#define MAXSTARTDELAYTIME 10
#define MINSTARTDELAYTIME 3


#define EX2             0x10
#define EX3             0x20
#define EX4             0x40


sbit LED = P2^1;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void InitLcd(void);

 
 
u8 UsbCmdUpdate=0;
u8 usbCmdCode=0;
u8 usbWRAddr,usbWRData;
//UARTST_PKG  uart1rcpkg;
UARTST_PKG uart2rcpkg;
//UARTST_PKG uart3rcpkg;
UARTST4_PKG uart4rcpkg;

uint8_t LCD_DISPiD=0;
uint8_t NeedUpdateLEDValue=0,NeedUpdateConnect=0,NeedSaveData=0;

u8 DriverBoardConnetStatus=0;
u8 DriverBoardNoReplyCounter=0;
u8 LedShareCounter=0;
u8 SelectData=0;
u8 UpdateTHCounter=0;
u8 RunModel=0;
u8 StartDelayCounter=0;
u8 SaveEEpromDelaycounter=0;
u8 Temperature=0,Humidity=0;
u8 GetDriverBoardReturn=0;

u8 ABEncoderACCCounter=0;
u8 ABEncoderAAC=1;


u8 SelectChDelayCounter=0;
u8 AutoOnLedCh=0;
u8 MovetoOtherLedch=0;

void delay_us(uint16_t dt) 
{
	dt = dt<<1;
	while (--dt) WDTRST;
}
  


void delay_ms(uint16_t dt)
{
 while (dt>0)
 	{
 	delay_us(400);
 	dt--;
 	}
}

void ClearMemory(uint8_t * ptr,uint8_t len)
{
	uint8_t i;
	 for (i=0;i<len;i++)	
	 	{
	 	*ptr=0;
		ptr++;
	 	}
}

void SetMemory(uint8_t * ptr,uint8_t len)
{
	uint8_t i;
	 for (i=0;i<len;i++)	
	 	{
	 	*ptr=0xff;
		ptr++;
	 	}
}

void Memcpy2(u8* src, u8* dec,u8 len)
{
	uint8_t i;
	 for (i=0;i<len;i++)	
	 	{
	 	*dec=*src;
		src++;
		dec++;
	 	}
}

u8 cmpMemory(u8* ptr, u8* dec ,u8 len)
{
	uint8_t i;
	 for (i=0;i<len;i++)	
	 	{
	 	if ((*dec)!=(*ptr)) return 0;
		ptr++;
		dec++;
	 	}
	 return 1;
}





void Gpio_Init(void)
{
	
	P26=1; P27=1;P00=1;P01=1; // 按键1-4

	
	P32=1;P33=1;P36=0;P37=1;//编码器输入
	P20=1; //speaker
	LED=0; //LED
	
	P0M0=0;
	P0M1=0;
	
	P1M0=0;
	P1M1=0;+
	
	P2M0=0x2E;
	P2M1=0;

	P3M0=0X40;
	P3M1=0;

	P5M0=0X10;
	P5M1=0;

	IT1 = 1;                                    //使能INT1 下降沿中断
    EX1 = 1;                                    //使能INT1中断
	INTCLKO = EX3;                              //使能INT3中断

}

void Beep(void)
{
	P20=0;  //SPK
	delay_ms(100);
	P20=1;
}

void Timer0Init(void)		//20毫秒@33.1776MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x00;		//设置定时初值
	TH0 = 0x28;		//设置定时初值
	ET0 = 1;                                   
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
}

void UpdateSelectedLedOnOff(u8 nonoff)
{
	if (nonoff==0) MovetoOtherLedch=0;
	SaveLedData0.LedOnOff=nonoff;
	UpdateLCDLEDCloum(SaveLedData0.SelectedLedChanel,SaveLedData0.LedPower[SaveLedData0.SelectedLedChanel],SaveLedData0.LedOnOff);
}


void UpdateSelcetedLed(u8 newSelCh)
{

	if (SaveLedData0.SelectedLedChanel!=newSelCh)
	{
		UpdateLCDLEDCloum(SaveLedData0.SelectedLedChanel,SaveLedData0.LedPower[SaveLedData0.SelectedLedChanel],0);
		UpdateLCDLEDSelected(SaveLedData0.SelectedLedChanel,0);	
		SaveLedData0.SelectedLedChanel=newSelCh;
		UpdateLCDLEDSelected(SaveLedData0.SelectedLedChanel,1);	
		UpdateSelectedLedOnOff(SaveLedData0.LedOnOff);
	}
}



//time0 定时超时判断
void Time0OutDelaySub(void)
{

	if (uart2rcpkg.RecStatus!=REC_HEAD1)
		{
		uart2Memory.UartPkgTimeoutCounter++;
		if (uart2Memory.UartPkgTimeoutCounter>50)
		  {
		  uart2Memory.UartPkgTimeoutCounter=0;
		  uart2rcpkg.RecStatus=REC_HEAD1;
		  }
		}
		
	if (uart4rcpkg.RecStatus!=REC2_HEAD1)
		{
		uart4rcpkg.UartPkgTimeoutCounter++;
		if (uart4rcpkg.UartPkgTimeoutCounter>50)
		  {
		  uart4rcpkg.UartPkgTimeoutCounter=0;
		  uart4rcpkg.RecStatus=REC2_HEAD1;
		  }
		}

	if (uart4rcpkg.UsbCmdStatus!=REC4_HEAD1)
		{
		uart4rcpkg.UartPkgTimeoutCounter++;
		if (uart4rcpkg.UartPkgTimeoutCounter>50)
		  {
		  uart4rcpkg.UartPkgTimeoutCounter=0;
		  uart4rcpkg.UsbCmdStatus=REC4_HEAD1;
		  }
		}

	if (SaveLedData0.Saveed==0 &&  ABEncoderDir==0) 
		{
		if (SaveEEpromDelaycounter>=150)   //3秒
			{
			NeedSaveData=1;
			}
		else
			{
			SaveEEpromDelaycounter++;
			}
		}
	else 
		{
		SaveEEpromDelaycounter=0;
		}
	

	if (ABEncoderAAC!=1) ABEncoderACCCounter++;

	if (ABEncoderACCCounter>=15)
		{
		ABEncoderACCCounter=0;
		ABEncoderAAC=1;
		}
	

	if ((LCD_DISPiD==1) && (SaveLedData0.LedOnOff==0) && (MovetoOtherLedch==1)) SelectChDelayCounter++;//自动
		if(SaveLedData0.SelectedLedChanel==1&&SaveLedData0.PurpleLamp==0)
		{
	  if (SelectChDelayCounter>75)  
	  {
		SelectChDelayCounter=0;
		AutoOnLedCh=0;
		MovetoOtherLedch=0;
	   }
    }
		else
		{
		if (SelectChDelayCounter>75)  
	  {
		SelectChDelayCounter=0;
		AutoOnLedCh=1;
		MovetoOtherLedch=0;
	   }
		}
	

	LedShareCounter++;
	if (LedShareCounter==25)   //0.5秒
		{
		LED=!LED;
		
		}
	else if (LedShareCounter>=50)  //1秒
		{
		LedShareCounter=0;
		NeedUpdateLEDValue=1;
		
		LED=!LED;
		if (StartDelayCounter<MAXSTARTDELAYTIME)  StartDelayCounter++;
		
 	if (GetDriverBoardReturn==1) 
		{
			DriverBoardNoReplyCounter=0;
			if (DriverBoardConnetStatus==0) NeedUpdateConnect=1;
			DriverBoardConnetStatus=1;
			
		}
	else 
		{
			if (DriverBoardConnetStatus==1) DriverBoardNoReplyCounter++;
			if (DriverBoardNoReplyCounter>NOREPLSYTIMES)  
				{
				DriverBoardNoReplyCounter=0;
				DriverBoardConnetStatus=0;
				NeedUpdateConnect=1;
				}
			//GetDriverBoardReturn=0;
		}	
		}
}

void InitLcd(void)
{
	if (GetDriverBoardReturn==0)  //驱动板没有返回
		{
		DisplayDisconnect();
		}
	else 
		{
		LCD_DrawWorkScreen();
		}
}





void ReturnWrUart4CmdUsb(u8 Wraddr,u8 WrData)
{
	 u8 addrsum=0;
	 
	 while (uart4rcpkg.busy==1) WDTRST;    
	 uart4rcpkg.busy=1;
	 S4BUF=0x69;
	 while (uart4rcpkg.busy==1) WDTRST;    
	 uart4rcpkg.busy=1;
	 S4BUF=0x96;
	 while (uart4rcpkg.busy==1) WDTRST;    
	 uart4rcpkg.busy=1;
	 S4BUF=usbCmdCode;
	 addrsum+=usbCmdCode;
	  while (uart4rcpkg.busy==1) WDTRST;    
	 uart4rcpkg.busy=1;
	 S4BUF=Wraddr;
	 addrsum+=Wraddr;
	  while (uart4rcpkg.busy==1) WDTRST;    
	 uart4rcpkg.busy=1;
	 S4BUF=WrData;
	 addrsum+=WrData;
	 while (uart4rcpkg.busy==1) WDTRST;    
	 uart4rcpkg.busy=1;
	 S4BUF=addrsum;
	 
}


void DoUart4CmdPro(void)
{
if (usbCmdCode==0x80)  //write
	{
		if (usbWRAddr==0x10)
		{
		if (usbWRData==0) usbWRData=1;
		else if (usbWRData>4) usbWRData=4;
		if (SaveLedData0.LedALLChanels!=usbWRData)
			{
				SaveLedData0.LedALLChanels=usbWRData;
				NeedSaveData=1;  //需要保存
				SaveLedData0.SelectedLedChanel=1;
				WriteDriverAddressValue2(0X11,SaveLedData0.SelectedLedChanel, SaveLedData0.LedOnOff);
				if (LCD_DISPiD==1)
				{
					LCD_DrawWorkScreen();
				}	
				else 
				{
					DispSettingNumber(2,SaveLedData0.LedALLChanels);
				}
				SaveLedData0.Saveed=0;
			}
		}
		else if (usbWRAddr==0x11)
		{
			if (usbWRData==0) usbWRData=1; 
			else if (usbWRData>4) usbWRData=4;
			if (SaveLedData0.SelectedLedChanel!=usbWRData)
				{
				if (LCD_DISPiD==1)
					{
					UpdateSelcetedLed(usbWRData);
					}
				else 
					{
					SaveLedData0.SelectedLedChanel=usbWRData;
					}
				WriteDriverAddressValue2(0X11,SaveLedData0.SelectedLedChanel, SaveLedData0.LedOnOff);
				SaveLedData0.Saveed=0;
				}
		}
		else if (usbWRAddr==0x12)
		{
			if (usbWRData>1) usbWRData=1;
			
			if (SaveLedData0.LedOnOff!=usbWRData)
				{
				if (LCD_DISPiD==1)
					{
					UpdateSelectedLedOnOff(usbWRData);
					}
				else 
					{
					SaveLedData0.LedOnOff=usbWRData;
					}
				WriteDriverAddressValue(0X12,SaveLedData0.LedOnOff);
				SaveLedData0.Saveed=0;
				}
		}
		else if (usbWRAddr>=0x14 && usbWRAddr<=0x17)
		{
			if (usbWRData>100) usbWRData=100;
			if (SaveLedData0.LedPower[usbWRAddr-0x13]!=usbWRData)
				{
				SaveLedData0.LedPower[usbWRAddr-0x13]=usbWRData;
				if (LCD_DISPiD==1)
					{
					UpdateLcdPowerDisplay(usbWRAddr-0x13,usbWRData,SaveLedData0.LedOnOff);
					}
				WriteDriverAddressValue(usbWRAddr,usbWRData);
				SaveLedData0.Saveed=0;
				}
		}
		else if (usbWRAddr>=0x19 && usbWRAddr<=0x1c)
		{
			if (SaveLedData0.LEDMotorLocation[usbWRAddr-0x18]!=usbWRData)
				{
				SaveLedData0.LEDMotorLocation[usbWRAddr-0x18]=usbWRData;
				if (LCD_DISPiD>1)
					{
					DispSettingNumber((usbWRAddr-0x19)+4,usbWRData);
					}
				WriteDriverAddressValue(usbWRAddr,usbWRData);
				SaveLedData0.Saveed=0;
				}
		}
		else if (usbWRAddr==0x1d)
		{
			if (SaveLedData0.MotorStepPluses!=usbWRData)
				{
				SaveLedData0.MotorStepPluses=usbWRData;
				if (LCD_DISPiD>1)
					{
					DispSettingNumber(3,SaveLedData0.MotorStepPluses);
					}
				WriteDriverAddressValue(0X1d,usbWRData);
				SaveLedData0.Saveed=0;
				}
		}
	}
else  if (uart4rcpkg.pic_N==0x81)  //read
	{
	if (usbWRAddr==0x10)
		{
		usbWRData=SaveLedData0.LedALLChanels;
		}
		else if (usbWRAddr==0x11)
		{
		usbWRData=SaveLedData0.SelectedLedChanel;
		}
		else if (usbWRAddr==0x12)
		{
		usbWRData=SaveLedData0.LedOnOff;
		}
		else if (usbWRAddr>=0x14 && usbWRAddr<=0x17)
		{
		usbWRData=SaveLedData0.LedPower[usbWRAddr-0x13];
		}
		else if (usbWRAddr>=0x19 && usbWRAddr<=0x1c)
		{
		usbWRData=SaveLedData0.LEDMotorLocation[usbWRAddr-0x18];
		}
		else if (usbWRAddr==0x1d)
		{
		usbWRData=SaveLedData0.MotorStepPluses;
		}
	}

	ReturnWrUart4CmdUsb(usbWRAddr,usbWRData);
}


void HandleLCD(void)
{
static u8 ReadBytest=0;
	if (NeedUpdateLEDValue)
		{
		NeedUpdateLEDValue=0;
		GetDriverBoardReturn=0;
		if (ReadBytest==0)
			{
			ReadDriverAddressValue(0x10,1); //心跳包
			ReadBytest=1;
			}
		else if (ReadBytest==1)
			{
			ReadDriverAddressValue(0x11,1); //心跳包
			ReadBytest=2;
			}
		else
			{
			ReadDriverAddressValue(0x12,1); //心跳包
			ReadBytest=0;
			}
		}
	
	if (NeedUpdateConnect)
		{
		NeedUpdateConnect=0;
		WriteDriverSqAddressValue(0X10,&(SaveLedData0.DriverType),sizeof(Run_PARAMS));
		InitLcd();
		Beep();
		}

	if (NeedSaveData)
		{
		NeedSaveData=0;
		SaveLedData0.Saveed=1;
		WriteEEpromSaveData();
		}

	if (AutoOnLedCh)//自动
	{
		AutoOnLedCh=0;
		if (LCD_DISPiD==1 && SaveLedData0.LedOnOff==0)
			{
			UpdateSelectedLedOnOff(1); 
			WriteDriverAddressValue2(0X11,SaveLedData0.SelectedLedChanel,SaveLedData0.LedOnOff);
			}
	}
}


void DoRsUart2Cmd(void)
{
	u8 i=0;
	u8 addrecode=uart2rcpkg.UartRunBuf[0];
	u8 datalen=uart2rcpkg.pklen-1;
	DriverBoardNoReplyCounter=0;
	GetDriverBoardReturn=1;
	if (DriverBoardConnetStatus==0)
		{
		DriverBoardConnetStatus=1;
		NeedUpdateConnect=1;
		}

	if (addrecode==0x10)  
		{
			if (SaveLedData0.DriverType!=uart2rcpkg.UartRunBuf[1])
				{
				WriteDriverAddressValue(0X10,SaveLedData0.DriverType);
				}
		}
	else if (addrecode==0x11) 
		{
		if (SaveLedData0.SelectedLedChanel!=uart2rcpkg.UartRunBuf[1])
			{
			if (LCD_DISPiD==1)
				{
				/*
				if (SaveLedData0.DriverType==0)
					{
					UpdateSelcetedLed(uart2rcpkg.UartRunBuf[1]);
					}
				else*/
					{
					WriteDriverAddressValue(0X11,SaveLedData0.SelectedLedChanel);
					}
				}
			}
		}
	else if (addrecode==0x12)

		{
		if (SaveLedData0.LedOnOff!=uart2rcpkg.UartRunBuf[1])
			{
			WriteDriverAddressValue(0X12,SaveLedData0.LedOnOff);
			}
		}
}


u8 DoUart2Pkg(void)
{
	u8 temp;
	
	if (uart2Memory.RxByteSize==0) return 0;

	temp=uart2Memory.RxBuffer[uart2Memory.ReadCounter];
	if (uart2Memory.ReadCounter<(RxBufferSize-1)) uart2Memory.ReadCounter++; else uart2Memory.ReadCounter=0;
	if(uart2Memory.RxByteSize>0)   uart2Memory.RxByteSize--;
	switch (uart2rcpkg.RecStatus)
	{
		case REC_HEAD1:
			if (temp==0X5A) uart2rcpkg.RecStatus=REC_HEAD2;
		break;
		case REC_HEAD2:
			if (temp==0XA5) uart2rcpkg.RecStatus=REC_RKLEN;
			else uart2rcpkg.RecStatus=REC_HEAD1;
		break;
		case REC_RKLEN:
		if (temp>24|| temp<1) 
				uart2rcpkg.RecStatus=REC_HEAD1;
			else 
				{
				uart2rcpkg.pklen=(uint8_t)(temp-1);
				uart2rcpkg.RecStatus=REC_CMD;
				}
		break;
		case REC_CMD:
			if (temp!=0x82)
				uart2rcpkg.RecStatus=REC_HEAD1;
			else 
				{
				uart2rcpkg.cmd=temp;
				uart2rcpkg.bufwpoint=0;
				uart2rcpkg.RecStatus=REC_DATA;
				}
			
		break;
		case REC_DATA:
			uart2rcpkg.UartRunBuf[uart2rcpkg.bufwpoint]=temp;
			uart2rcpkg.bufwpoint++;
			if (uart2rcpkg.bufwpoint>=uart2rcpkg.pklen)
				{
					DoRsUart2Cmd();
					uart2rcpkg.RecStatus=REC_HEAD1;
					return 1;
				}
		break;
		
	}
	return 2;
}

void CheckButton(void)
{
	u8 tpv;
	u16 timout=0;
  u8 n1=0,n2=0;
  if (GetDriverBoardReturn==0) return;

	if (ABEncoderDir==1)
	{
		ABEncoderDir=0;
		if (LCD_DISPiD==1)
		{
			if (SaveLedData0.LedOnOff==1)
			{
				tpv=SaveLedData0.LedPower[SaveLedData0.SelectedLedChanel];
				if (tpv<(100-ABEncoderAAC)) tpv+=ABEncoderAAC; else tpv=100;
				ABEncoderACCCounter=0;
				if (ABEncoderAAC<10) ABEncoderAAC++;
				SaveLedData0.LedPower[SaveLedData0.SelectedLedChanel]=tpv;
				SaveLedData0.Saveed=0;
				UpdateLcdPowerDisplay(SaveLedData0.SelectedLedChanel,tpv,1);
				WriteDriverAddressValue(0x13+SaveLedData0.SelectedLedChanel,tpv);
			}
			else 
			{	
				SelectChDelayCounter=0;
				tpv=SaveLedData0.SelectedLedChanel;
				UpdateLCDLEDSelected(tpv,0);
				if (tpv<(SaveLedData0.LedALLChanels)) tpv++; else tpv=1;
				SaveLedData0.SelectedLedChanel=tpv;
				UpdateLCDLEDSelected(tpv,1);
				MovetoOtherLedch=1;
			}
		}
		else 
		{
			if (LCD_DISPiD==2)
			{
				if (SaveLedData0.LedALLChanels<4) SaveLedData0.LedALLChanels++; 
				DispSettingNumber(LCD_DISPiD,SaveLedData0.LedALLChanels);
				SaveLedData0.SelectedLedChanel=1;
			}
			else if (LCD_DISPiD==3)
			{
				tpv=SaveLedData0.MotorStepPluses;
				if (tpv<(255-ABEncoderAAC)) tpv+=ABEncoderAAC; else tpv=255;
				SaveLedData0.MotorStepPluses=tpv;
				ABEncoderACCCounter=0;
				if (ABEncoderAAC<10) ABEncoderAAC++;
				DispSettingNumber(LCD_DISPiD,tpv);
				WriteDriverAddressValue(0X1d,tpv);
			}
			
			else if (LCD_DISPiD>3 && LCD_DISPiD<=7)
			{
				tpv=SaveLedData0.LEDMotorLocation[LCD_DISPiD-3];
				if (tpv<254) tpv+=1; else tpv=255;
				//if (tpv<(255-ABEncoderAAC)) tpv+=ABEncoderAAC; else tpv=255;
				//ABEncoderACCCounter=0;
				//if (ABEncoderAAC<10) ABEncoderAAC++;
				SaveLedData0.LEDMotorLocation[LCD_DISPiD-3]=tpv;
				DispSettingNumber(LCD_DISPiD,tpv);
				WriteDriverAddressValue(0X16+LCD_DISPiD-1,tpv);
				WriteDriverAddressValue2(0X11,LCD_DISPiD-3,1); //选择LED，点亮LED
			}
			else if (LCD_DISPiD==8)
			{
				SaveLedData0.DriverFanOnOff=1;
				DispSettingNumber(2,1);
				WriteDriverAddressValue(0X1e,1);
			}
			
			 else if (LCD_DISPiD==9)
			{
				SaveLedData0.PurpleLamp=1;
				DispSettingNumber(3,1);
				WriteDriverAddressValue(0X20,1);
			}
			
		}

	}
	
	else if (ABEncoderDir==0xff)
	{
		ABEncoderDir=0;
		if (LCD_DISPiD==1)
		{
			
			if (SaveLedData0.LedOnOff==1)
			{
				tpv=SaveLedData0.LedPower[SaveLedData0.SelectedLedChanel];
				if (tpv>ABEncoderAAC) tpv-=ABEncoderAAC; else tpv=1;
				ABEncoderACCCounter=0;
				if (ABEncoderAAC<10) ABEncoderAAC++;
				SaveLedData0.LedPower[SaveLedData0.SelectedLedChanel]=tpv;
				SaveLedData0.Saveed=0;
				UpdateLcdPowerDisplay(SaveLedData0.SelectedLedChanel,tpv,1);
				WriteDriverAddressValue(0x13+SaveLedData0.SelectedLedChanel,tpv);
			}
			else 
			{
				SelectChDelayCounter=0;
				tpv=SaveLedData0.SelectedLedChanel;
				UpdateLCDLEDSelected(tpv,0);
				if (tpv>1) tpv--; else tpv=SaveLedData0.LedALLChanels;
				SaveLedData0.SelectedLedChanel=tpv;
				UpdateLCDLEDSelected(tpv,1);
				MovetoOtherLedch=1;
			}
		}
		else 
		{
			if (LCD_DISPiD==2)
			{
				if (SaveLedData0.LedALLChanels>1) SaveLedData0.LedALLChanels--; 
				DispSettingNumber(LCD_DISPiD,SaveLedData0.LedALLChanels);
			}
			else if (LCD_DISPiD==3)
			{
				tpv=SaveLedData0.MotorStepPluses;
		
				if (tpv>(ABEncoderAAC+1)) tpv-=ABEncoderAAC; else tpv=1;
				SaveLedData0.MotorStepPluses=tpv;
				ABEncoderACCCounter=0;
				if (ABEncoderAAC<10) ABEncoderAAC++;
				DispSettingNumber(LCD_DISPiD,tpv);
				WriteDriverAddressValue(0X1d,tpv);  //设置LED 每步脉冲
			}
			else if (LCD_DISPiD>3 && LCD_DISPiD<=7)
			{
				tpv=SaveLedData0.LEDMotorLocation[LCD_DISPiD-3];
				if (tpv>0) tpv-=1; else tpv=0;
			//	if (tpv>ABEncoderAAC) tpv-=ABEncoderAAC; else tpv=0;
			//	ABEncoderACCCounter=0;
			//	if (ABEncoderAAC<10) ABEncoderAAC++;
				SaveLedData0.LEDMotorLocation[LCD_DISPiD-3]=tpv;
				DispSettingNumber(LCD_DISPiD,tpv);
				WriteDriverAddressValue(0X16+LCD_DISPiD-1,tpv);//发送led 位置信息
				WriteDriverAddressValue2(0X11,LCD_DISPiD-3,1);//发送选择的led 和 开灯
		
			}
			else if (LCD_DISPiD==8)
			{
				SaveLedData0.DriverFanOnOff=0;
				DispSettingNumber(2,0);
				WriteDriverAddressValue(0X1e,0);
			}
			
				else if (LCD_DISPiD==9)
			{
				SaveLedData0.PurpleLamp=0;
				DispSettingNumber(3,0);
				WriteDriverAddressValue(0X20,0);
			}
			
		}
	}

	if (P32==0)//编码器按键
		{
		delay_ms(5);
		if (P32==0)
		{
		/*
			for (tpv=0;tpv<100;tpv++)
				{
				delay_ms(30);
				if (P32==1) break;
				}
			if(P32==0)
				{
				DisplaySetting();
				Beep();  //进入设置界面
				}
			else*/
				{
				Beep();
				if (LCD_DISPiD==1)// && GetDriverBoardReturn==1)
					{
					if (SaveLedData0.LedOnOff)	UpdateSelectedLedOnOff(0); else UpdateSelectedLedOnOff(1); 
					WriteDriverAddressValue2(0X11,SaveLedData0.SelectedLedChanel,SaveLedData0.LedOnOff);
					}
				else if (LCD_DISPiD>=2)
					{
					LCD_DISPiD++; 
					if (LCD_DISPiD==3) 
						{
						DispSettingRectangle();
						WriteDriverAddressValue(0X1d,SaveLedData0.MotorStepPluses);  //设置LED 每步脉冲
						}
					else if (LCD_DISPiD<=7) 
						{
						DispSettingRectangle();
						WriteDriverAddressValue2(0X11,LCD_DISPiD-3,1);// update led selection and on off
						}
					else if (LCD_DISPiD==8) //driver fan onoff
						{
						  DisplaySetting1();							
						}
						else if(LCD_DISPiD==9)
						{
							DisplaySetting2();
						}
					else 
						{
						LCD_DISPiD=1;
						LCD_DrawWorkScreen();
						NeedSaveData=1;  //需要保存
						WriteDriverAddressValue2(0X11,SaveLedData0.SelectedLedChanel,SaveLedData0.LedOnOff);
						}
					
					}
				}

			while (P32==0) 
				{
				WDTRST;
				while (P32==0)  delay_ms(5);
				}
		}
	}


/*
	
	if (ABEncoderDir==1)
	{
		ABEncoderDir=2;
		
	}
	else if (ABEncoderDir==0xff)
	{
		ABEncoderDir=2;
		
	}
   */

//   ABEncoderStep=;
   if ((P3&0X88)==0x88) ABEncoderDir=0;


	

	if (P26==0)
	{
		delay_ms(5);
		if (P26==0)
		{
			Beep();
				if (LCD_DISPiD==1)
				 {
					if (SaveLedData0.SelectedLedChanel==1)
					{
						if (SaveLedData0.LedOnOff)	UpdateSelectedLedOnOff(0); else UpdateSelectedLedOnOff(1); 
						WriteDriverAddressValue2(0X11,SaveLedData0.SelectedLedChanel,SaveLedData0.LedOnOff);
					}
					else
					{
						UpdateSelcetedLed(1);
						UpdateSelectedLedOnOff(1); 
						WriteDriverAddressValue2(0X11,SaveLedData0.SelectedLedChanel, SaveLedData0.LedOnOff);
					}
				 }
			
				while (P26==0) 
				{
				WDTRST;
				while (P26==0)  
					{
					
					if (P01==0)
						{
						for (tpv=0;tpv<100;tpv++)
						{
							delay_ms(30);
							if (P26!=0 || P01!=0) break;
							}
							if (P26==0 && P01==0)
								{
								DisplaySetting();
								Beep();  //进入设置界面
								}
							while (P26==0 || P01==0) 
							{
							WDTRST;
							while (P26==0 || P01==0)  delay_ms(5);
						}
						}
					else 
						delay_ms(5);
					}
				}
			}
	}
	
	if (P27==0)
	{
		delay_ms(5);
		if (P27==0)
		{
			Beep();
			if (LCD_DISPiD==1)
			 {
				if (SaveLedData0.SelectedLedChanel==2)
				{
					if (SaveLedData0.LedOnOff)	UpdateSelectedLedOnOff(0); else UpdateSelectedLedOnOff(1); 
					WriteDriverAddressValue2(0X11,SaveLedData0.SelectedLedChanel,SaveLedData0.LedOnOff);
				}
				else  if (SaveLedData0.LedALLChanels>1)
				{
					UpdateSelcetedLed(2);
					UpdateSelectedLedOnOff(1); 
					WriteDriverAddressValue2(0X11,SaveLedData0.SelectedLedChanel, SaveLedData0.LedOnOff);
				}
			}
			 else
			 {
			 if (LCD_DISPiD==2)
			   {
				if (SaveLedData0.LedALLChanels<4) SaveLedData0.LedALLChanels++; 
				DispSettingNumber(LCD_DISPiD,SaveLedData0.LedALLChanels);
				SaveLedData0.SelectedLedChanel=1;
			   }
				 else if (LCD_DISPiD==3)
			{
				tpv=SaveLedData0.MotorStepPluses;
				if (tpv<(255-ABEncoderAAC)) tpv+=ABEncoderAAC; else tpv=255;
				SaveLedData0.MotorStepPluses=tpv;
				ABEncoderACCCounter=0;
				if (ABEncoderAAC<10) ABEncoderAAC++;
				DispSettingNumber(LCD_DISPiD,tpv);
				WriteDriverAddressValue(0X1d,tpv);
			}
			
			else if (LCD_DISPiD>3 && LCD_DISPiD<=7)
			{
				tpv=SaveLedData0.LEDMotorLocation[LCD_DISPiD-3];
				if (tpv<254) tpv+=1; else tpv=255;
				//if (tpv<(255-ABEncoderAAC)) tpv+=ABEncoderAAC; else tpv=255;
				//ABEncoderACCCounter=0;
				//if (ABEncoderAAC<10) ABEncoderAAC++;
				SaveLedData0.LEDMotorLocation[LCD_DISPiD-3]=tpv;
				DispSettingNumber(LCD_DISPiD,tpv);
				WriteDriverAddressValue(0X16+LCD_DISPiD-1,tpv);
				WriteDriverAddressValue2(0X11,LCD_DISPiD-3,1); //选择LED，点亮LED
			}
			else if (LCD_DISPiD==8)
			{
				SaveLedData0.DriverFanOnOff=1;
				DispSettingNumber(2,1);
				WriteDriverAddressValue(0X1e,1);
			}
			
			 else if (LCD_DISPiD==9)
			{
				SaveLedData0.PurpleLamp=1;
				DispSettingNumber(3,1);
				WriteDriverAddressValue(0X20,1);
			}
		   }
			while (P27==0) 
				{
				WDTRST;
				while (P27==0)  delay_ms(5);
				}
		}
	}

	if (P00==0)
	{
		delay_ms(5);
		if (P00==0)
		{
			Beep();
			if (LCD_DISPiD==1)
			{
				if (SaveLedData0.SelectedLedChanel==3)
				{
					if (SaveLedData0.LedOnOff)	UpdateSelectedLedOnOff(0); else UpdateSelectedLedOnOff(1); 
					WriteDriverAddressValue2(0X11,SaveLedData0.SelectedLedChanel,SaveLedData0.LedOnOff);
				}
				else if (SaveLedData0.LedALLChanels>2)
				{
					UpdateSelcetedLed(3);
					UpdateSelectedLedOnOff(1); 
					WriteDriverAddressValue2(0X11,SaveLedData0.SelectedLedChanel, SaveLedData0.LedOnOff);
				}
			}
			else 
		{
			if (LCD_DISPiD==2)
			{
				if (SaveLedData0.LedALLChanels>1) SaveLedData0.LedALLChanels--; 
				DispSettingNumber(LCD_DISPiD,SaveLedData0.LedALLChanels);
			}
			else if (LCD_DISPiD==3)
			{
				tpv=SaveLedData0.MotorStepPluses;
		
				if (tpv>(ABEncoderAAC+1)) tpv-=ABEncoderAAC; else tpv=1;
				SaveLedData0.MotorStepPluses=tpv;
				ABEncoderACCCounter=0;
				if (ABEncoderAAC<10) ABEncoderAAC++;
				DispSettingNumber(LCD_DISPiD,tpv);
				WriteDriverAddressValue(0X1d,tpv);  //设置LED 每步脉冲
			}
			else if (LCD_DISPiD>3 && LCD_DISPiD<=7)
			{
				tpv=SaveLedData0.LEDMotorLocation[LCD_DISPiD-3];
				if (tpv>0) tpv-=1; else tpv=0;
			//	if (tpv>ABEncoderAAC) tpv-=ABEncoderAAC; else tpv=0;
			//	ABEncoderACCCounter=0;
			//	if (ABEncoderAAC<10) ABEncoderAAC++;
				SaveLedData0.LEDMotorLocation[LCD_DISPiD-3]=tpv;
				DispSettingNumber(LCD_DISPiD,tpv);
				WriteDriverAddressValue(0X16+LCD_DISPiD-1,tpv);//发送led 位置信息
				WriteDriverAddressValue2(0X11,LCD_DISPiD-3,1);//发送选择的led 和 开灯		
			}
			else if (LCD_DISPiD==8)
			{
				SaveLedData0.DriverFanOnOff=0;
				DispSettingNumber(2,0);
				WriteDriverAddressValue(0X1e,0);
			}
			
				else if (LCD_DISPiD==9)
			{
				SaveLedData0.PurpleLamp=0;
				DispSettingNumber(3,0);
				WriteDriverAddressValue(0X20,0);
			}
		}
			while (P00==0) 
				{
				WDTRST;
				while (P00==0)  delay_ms(5);
				}
		}
	}

	if (P01==0)
	{
		delay_ms(5);
		if (P01==0)
		{
			Beep();
			
			if (LCD_DISPiD==1)
			{
				if (SaveLedData0.SelectedLedChanel==4)
				{
					if (SaveLedData0.LedOnOff)	UpdateSelectedLedOnOff(0); else UpdateSelectedLedOnOff(1); 
					WriteDriverAddressValue2(0X11,SaveLedData0.SelectedLedChanel,SaveLedData0.LedOnOff);
				}
				else if (SaveLedData0.LedALLChanels>3)
				{
					UpdateSelcetedLed(4);
					UpdateSelectedLedOnOff(1); 
					WriteDriverAddressValue2(0X11,SaveLedData0.SelectedLedChanel, SaveLedData0.LedOnOff);
				}
			}
						else if (LCD_DISPiD>=2)
					{
					LCD_DISPiD++; 
					if (LCD_DISPiD==3) 
						{
						DispSettingRectangle();
						WriteDriverAddressValue(0X1d,SaveLedData0.MotorStepPluses);  //设置LED 每步脉冲
						}
					else if (LCD_DISPiD<=7) 
						{
						DispSettingRectangle();
						WriteDriverAddressValue2(0X11,LCD_DISPiD-3,1);// update led selection and on off
						}
					else if (LCD_DISPiD==8) //driver fan onoff
						{
						  DisplaySetting1();							
						}
						else if(LCD_DISPiD==9)
						{
							DisplaySetting2();
						}
						else 
						{
						LCD_DISPiD=1;
						LCD_DrawWorkScreen();
						NeedSaveData=1;  //需要保存
						WriteDriverAddressValue2(0X11,SaveLedData0.SelectedLedChanel,SaveLedData0.LedOnOff);
						}
					}
			while (P01==0) 
				{
				WDTRST;
				while (P01==0) 
					{
					if (P26==0)
						{
						for (tpv=0;tpv<100;tpv++)
						{
						delay_ms(30);
						if (P26!=0 || P01!=0) break;
						}
						if (P26==0 && P01==0)
							{
							DisplaySetting();
							Beep();  //进入设置界面
							}
						while (P26==0 || P01==0) 
							{
							WDTRST;
							while (P26==0 || P01==0)  delay_ms(5);
							}
						}
					else 
						delay_ms(5);
					}
				}
		}
	}


	if (UsbCmdUpdate!=0)
	{
		UsbCmdUpdate=0;
		DoUart4CmdPro();
	}
}

void ExClockConfig(void)
{
// u32 timeoutcounter=0;
	P_SW2 = 0x80;
	XOSCCR = 0xc0;								//???ˉía2??§??
	while (!(XOSCCR & 1) ) 
		{
		WDTRST;			   //μè′yê±?ó?è?¨
		}
	CLKDIV = 0x00;								 //ê±?ó2?·??μ
	CKSEL = 0x01;								//????ía2??§??
	P_SW2 = 0x00;
}


void main()
{
	
	ExClockConfig();
	if(CmpDataFlashUuid()==0) while (1);
	Gpio_Init();
	Uart2Init();   //DRIVER BOARD
	Uart4Init();  //USB
	lcd_initial();
	Timer0Init();
	SPI_FLASH_Init();
	delay_ms(100);
	Gui_DrawFlashbmp16(0,0,0);
//	
//	Display_ASCII8X16(50,16,"EasyLWL",7,BLUE);
	CtrlLcdBL(1);
	EA=1;
	ReadDriverAddressValue(0x10,1);
	Beep();
	ReadEEpromSaveData();
	ReadDriverAddressValue(0x10,1);

	//开机加载画
	while (StartDelayCounter<MAXSTARTDELAYTIME)  //等待开机停留时间, lcd 应答
		{
		WDTRST;  
		if ((StartDelayCounter>=MINSTARTDELAYTIME) &&(GetDriverBoardReturn!=0)) break;
		if (NeedUpdateLEDValue)
		{
		NeedUpdateLEDValue=0;
		GetDriverBoardReturn=0;
		ReadDriverAddressValue(0x10,1); //心跳包
		}
		DoUart2Pkg();  //driver board
		}
	
	NeedUpdateConnect=0;
	InitLcd();
	WriteDriverSqAddressValue(0X10,&(SaveLedData0.DriverType),sizeof(Run_PARAMS));
  /* Infinite loop */
  while (1)
  {
    WDTRST;                      //清看门狗,否则系统复位
    DoUart2Pkg();  //driver board
    //DoUart4Pkg();   //usb uart4 interrupt sub run 
	CheckButton();
	HandleLCD();
  }
}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
