/**
  ******************************************************************************
  * @file     stm8s_it.c
  * @author   MCD Application Team
  * @version  V2.2.0
  * @date     30-September-2014
  * @brief    Main Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
//#include "stm8s_it.h"
#include "main.h"
#include "uarttask.h"
#include "ABencoder.h"
#include "spi_flash.h"
#include "SaveConfigfile.h"


#define ADC1SUMNUM 128
#define chushu 7

#define ONDelay 200
#define OFFDelay 200



/** @addtogroup UART1_Interrupt
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/



void INT3_Isr() interrupt 11
{
	delay_us(5);
	if (P37==0 && P33==1)
		{
		//if(ABEncoderDir==0) 
			ABEncoderDir=1;
		}
		
}

void INT1_Isr() interrupt 2
{
	delay_us(5);
  	if (P33==0 && P37==1)
		{
		//if(ABEncoderDir==0) 
			ABEncoderDir=0xff;
  		}	
	/*
	else if (P33==1 && P37==1) 
		{
		ABEncoderDir=0;
		}
	*/	
}

/*
void common_isr() interrupt 13
{
	unsigned char psw2_st;
	u8 intf;
	psw2_st = P_SW2;
	P_SW2 |= 0x80;
	 intf = P3INTF;
	 P3INTF = 0x00;
 	P_SW2 = psw2_st;
 if ((intf & 0x08) && (ABEncoderDir==0) )
 	{
 	ABEncoderDir=1;
 	}
 else if ((intf & 0x80) && (ABEncoderDir==0) )
 	{
 	ABEncoderDir=0xff;
 	}

}
*/
//20ms 
void TM0_Isr() interrupt 1 
{
  /* In order to detect unexpected events during development,
     it is recommended to set a breakpoint on the following instruction.
  */
  	Time0OutDelaySub();
}
/*
void UART1_Isr() interrupt 4
{
    if (TI)
    {
        TI = 0;                                 //清中断标志
        uart1Memory.busy = 0;
    }
    if (RI)
    {
        RI = 0;                                 //清中断标志
        uart1Memory.UartPkgTimeoutCounter=0;
		uart1Memory.RxBuffer[uart1Memory.RxCounter]=SBUF;
		uart1Memory.RxByteSize++;
		if (uart1Memory.RxCounter<(RxBufferSize-1)) uart1Memory.RxCounter++; else uart1Memory.RxCounter=0;
    }
}
*/



void DoRsUart4Cmd(void)
{
	u32 WrAddr;
	static u32 SectorAddr=0;
	if (uart4rcpkg.pic_N==1)  //LOGO PICURE
		{
			if (uart4rcpkg.pkg_num==0) 
				{
					SectorAddr=0;
					SPI_FLASH_SectorErase(0);
				}
			WrAddr=(uart4rcpkg.pkg_num)<<8;
			if (SectorAddr!=(WrAddr/SPI_FLASH_SectorSize))
				{
				SPI_FLASH_SectorErase(WrAddr);
				SectorAddr=WrAddr/SPI_FLASH_SectorSize;
				}
			SPI_FLASH_BufferWrite(uart4rcpkg.UartRunBuf,WrAddr,uart4rcpkg.pklen+1);
			S4BUF=0x01;
		}
	else 
		{
		S4BUF=0xfe;
		}
}



void TranUart4Cmd_usb(void)
{
	
	usbCmdCode=uart4rcpkg.pic_N;
	usbWRAddr=uart4rcpkg.pklen;
	usbWRData=uart4rcpkg.UartRunBuf[0];	
	if (uart4rcpkg.pic_N==0x80) UsbCmdUpdate=1; else UsbCmdUpdate=2;
	
	
}


u8 DoUart4Pkg(u8 rxByte)  //USB
{
	uart4rcpkg.UartPkgTimeoutCounter=0;
	
	if ((uart4rcpkg.UsbCmdStatus==REC4_HEAD1) && (uart4rcpkg.RecStatus)==REC2_HEAD1)
	{
		if (rxByte==0X5A) uart4rcpkg.RecStatus=REC2_HEAD2;
		else if (rxByte==0x69) uart4rcpkg.UsbCmdStatus=REC4_HEAD2;
	}

	else 
	{
		if (uart4rcpkg.UsbCmdStatus!=REC4_HEAD1)
		{
			switch (uart4rcpkg.UsbCmdStatus)
			{
				case REC4_HEAD2:
					if (rxByte==0x96) uart4rcpkg.UsbCmdStatus=REC4_CMD;
					else uart4rcpkg.UsbCmdStatus=REC4_HEAD1;
				break;
				case REC4_CMD:
						uart4rcpkg.pic_N=rxByte;
						uart4rcpkg.UsbCmdStatus=REC4_ADDR;
						uart4rcpkg.Addsum=rxByte;
				break;
				
				case REC4_ADDR:
						uart4rcpkg.pklen =rxByte;
						uart4rcpkg.UsbCmdStatus=REC4_DATA_RLEN;
						uart4rcpkg.Addsum +=rxByte;
				break;

				case REC4_DATA_RLEN:
						uart4rcpkg.bufwpoint=1;
						uart4rcpkg.UartRunBuf[0]=rxByte;
						uart4rcpkg.Addsum +=rxByte;
						uart4rcpkg.UsbCmdStatus=REC4_ADDSUM;
				break;
				case REC4_ADDSUM:
					if (uart4rcpkg.Addsum==rxByte) // check addsum
					{
						TranUart4Cmd_usb();
						uart4rcpkg.UsbCmdStatus=REC4_HEAD1;
						return 1;
					}
					else
					{
						uart4rcpkg.UsbCmdStatus=REC2_HEAD1;
					}
			}
			
		}
		else 
		{	
			switch (uart4rcpkg.RecStatus)
			{
				case REC4_HEAD2:
					if (rxByte==0XA5) uart4rcpkg.RecStatus=REC2_PIC_N;
					else uart4rcpkg.RecStatus=REC2_HEAD1;
				break;
				case REC2_PIC_N:
						uart4rcpkg.pic_N=rxByte;
						uart4rcpkg.RecStatus=REC2_PKG_N_H;
				break;
				
				case REC2_PKG_N_H:
						uart4rcpkg.pkg_num =rxByte;
						uart4rcpkg.pkg_num=(uart4rcpkg.pkg_num<<8);
						uart4rcpkg.RecStatus=REC2_PKG_N_L;
				break;

				case REC2_PKG_N_L:
						uart4rcpkg.pkg_num |=rxByte;
						uart4rcpkg.RecStatus=REC2_RKLEN;
				break;
				
				case REC2_RKLEN:
						uart4rcpkg.pklen=rxByte;
						uart4rcpkg.bufwpoint=0;
						uart4rcpkg.RecStatus=REC2_DATA;
						uart4rcpkg.Addsum=0;
				break;
				case REC2_DATA:
					uart4rcpkg.UartRunBuf[uart4rcpkg.bufwpoint]=rxByte;
					uart4rcpkg.Addsum+=rxByte;
					if (uart4rcpkg.bufwpoint>=uart4rcpkg.pklen)
						{
							uart4rcpkg.RecStatus=REC2_ADDSUM;
						}
					else 
						{
							uart4rcpkg.bufwpoint++;
						}
					
				break;
				case REC2_ADDSUM:
					if (uart4rcpkg.Addsum==rxByte) // check addsum
					{
						DoRsUart4Cmd();
						uart4rcpkg.RecStatus=REC2_HEAD1;
						return 1;
					}
					else
					{
						S4BUF=0xff;
						uart4rcpkg.RecStatus=REC2_HEAD1;
					}
			}
			
		}
	}
	
	return 0xff;



}



void Uart2Isr() interrupt 8 
{

    if (S2CON & 0x02)
    {
        S2CON &= ~0x02;
        uart2Memory.busy = 0;
    }
		
    if (S2CON & 0x01)
    {
        S2CON &= ~0x01;
		uart2Memory.UartPkgTimeoutCounter=0;
		uart2Memory.RxBuffer[uart2Memory.RxCounter]=S2BUF;
		uart2Memory.RxByteSize++;
		if (uart2Memory.RxCounter<(RxBufferSize-1)) uart2Memory.RxCounter++; else uart2Memory.RxCounter=0;
		
		
    }
}
 /*
void UART3_Isr() interrupt 17
{
    if (S3CON & 0x02)
    {
        S3CON &= ~0x02;                         //清中断标志
        uart3Memory.busy = 0;                        
    }
    if (S3CON & 0x01)
    {
        S3CON &= ~0x01;                         //清中断标志
        uart3Memory.UartPkgTimeoutCounter=0;
		uart3Memory.RxBuffer[uart3Memory.RxCounter]=S3BUF;
		uart3Memory.RxByteSize++;
		if (uart3Memory.RxCounter<(RxBufferSize-1)) uart3Memory.RxCounter++; else uart3Memory.RxCounter=0;
    }
}
*/
void UART4_Isr() interrupt 18
{
		u8 rect;
    if (S4CON & 0x02)
    {
        S4CON &= ~0x02;                         //清中断标志
        uart4rcpkg.busy = 0;                      
    }
    if (S4CON & 0x01)
    {
        S4CON &= ~0x01;                         //清中断标志
			 rect=S4BUF;
        DoUart4Pkg(rect);
    }
}


 
