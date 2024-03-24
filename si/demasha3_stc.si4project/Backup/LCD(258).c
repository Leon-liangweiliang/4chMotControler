//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ���������ɣ��������������κ���;
//����Ӳ����STC51
//1.8��TFT_ST7735Һ������
//********************************************************************************
//V1.1�޸�˵��
//���������ʾ���룬ֱ���ڳ�ʼ�����л�������ģʽ
//////////////////////////////////////////////////////////////////////////////////
#include "main.h"
#include "intrins.h"
#include "lcd.h"
#include "spi_flash.h"
#include "saveconfigfile.h"
#include <string.h>

#define USE_LANDSCAPE//���Գ���������л��������뿪���˺꣬����������
#define nopsw _nop_(); //_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();

//---------------------------Һ��������˵��-------------------------------------//
//����ǰ��ο�Һ����˵�����10ҳ���Ŷ���
sbit bl        =P3^4;//��ģ��BL���ţ�������Բ���IO���ƻ���PWM���ƣ�Ҳ����ֱ�ӽӵ��ߵ�ƽ����
sbit scl       =P1^2;//��ģ��CLK����,������Pin9_SCL
sbit sda       =P1^3;//��ģ��DIN/MOSI���ţ�������Pin8_SDA
sbit rs        =P1^5;//��ģ��D/C���ţ�������Pin7_A0
sbit cs        =P3^5;//��ģ��CE���ţ�������Pin12_CS
sbit reset     =P1^4;//��ģ��RST���ţ�������Pin6_RES
//---------------------------End ofҺ��������---------------------------------//



//��SPI���ߴ���һ��8λ����
void  SPI_WriteData(uchar Data2)
{
	unsigned char i=0;
	for(i=8;i>0;i--)
	{
		if(Data2&0x80)	
		sda=1; //�������
		else sda=0;
		nopsw;
		scl=0;
		nopsw;
		scl=1;
		Data2<<=1;
	}
}
//��Һ����дһ��8λָ��
void  Lcd_WriteIndex(uchar Data2)
{
		
		cs=0;
		rs=0;
		SPI_WriteData(Data2); 		
		cs=1;
}
//��Һ����дһ��8λ����
void  Lcd_WriteData(uchar Data2)
{	
		unsigned char i=0;
		cs=0;
		rs=1;
		SPI_WriteData(Data2); 	
		cs=1;
}
//��Һ����дһ��16λ����
void  LCD_WriteData_16Bit(unsigned int Data2)
{
	unsigned char i=0;
	cs=0;
	rs=1;
	SPI_WriteData(Data2>>8); 	//д���8λ����
	SPI_WriteData(Data2); 			//д���8λ����
	cs=1;

}

void LCD_Reset()
{
    reset=0;
    delay_ms(100);
    reset=1;
    delay_ms(100);
}


void CtrlLcdBL(u8 cl)
{
	if (cl==1) bl=1;
	else bl=0;
}


//////////////////////////////////////////////////////////////////////////////////////////////
//Һ������ʼ�� for S6D02A1
void lcd_initial()
{	
	LCD_Reset();//Reset before LCD Init.	
	//LCD Init For 1.44Inch LCD Panel with ST7735R.
	Lcd_WriteIndex(0x11);//Sleep exit 
	delay_ms (120);
		
	//ST7735R Frame Rate
	Lcd_WriteIndex(0xB1); 
	Lcd_WriteData(0x01); 
	Lcd_WriteData(0x2C); 
	Lcd_WriteData(0x2D); 

	Lcd_WriteIndex(0xB2); 
	Lcd_WriteData(0x01); 
	Lcd_WriteData(0x2C); 
	Lcd_WriteData(0x2D); 

	Lcd_WriteIndex(0xB3); 
	Lcd_WriteData(0x01); 
	Lcd_WriteData(0x2C); 
	Lcd_WriteData(0x2D); 
	Lcd_WriteData(0x01); 
	Lcd_WriteData(0x2C); 
	Lcd_WriteData(0x2D); 
	
	Lcd_WriteIndex(0xB4); //Column inversion 
	Lcd_WriteData(0x07); 
	
	//ST7735R Power Sequence
	Lcd_WriteIndex(0xC0); 
	Lcd_WriteData(0xA2); 
	Lcd_WriteData(0x02); 
	Lcd_WriteData(0x84); 
	Lcd_WriteIndex(0xC1); 
	Lcd_WriteData(0xC5); 

	Lcd_WriteIndex(0xC2); 
	Lcd_WriteData(0x0A); 
	Lcd_WriteData(0x00); 

	Lcd_WriteIndex(0xC3); 
	Lcd_WriteData(0x8A); 
	Lcd_WriteData(0x2A); 
	Lcd_WriteIndex(0xC4); 
	Lcd_WriteData(0x8A); 
	Lcd_WriteData(0xEE); 
	
	Lcd_WriteIndex(0xC5); //VCOM 
	Lcd_WriteData(0x0E); 
	
	Lcd_WriteIndex(0x36); //MX, MY, RGB mode 
//	Lcd_WriteData(0xC0); 
#ifdef USE_LANDSCAPE
	Lcd_WriteData(0x68); //����C8 ����08 A8
#else
	Lcd_WriteData(0xC8); //����C8 ����08 A8
#endif		*/
	//ST7735R Gamma Sequence
	Lcd_WriteIndex(0xe0); 
	Lcd_WriteData(0x0f); 
	Lcd_WriteData(0x1a); 
	Lcd_WriteData(0x0f); 
	Lcd_WriteData(0x18); 
	Lcd_WriteData(0x2f); 
	Lcd_WriteData(0x28); 
	Lcd_WriteData(0x20); 
	Lcd_WriteData(0x22); 
	Lcd_WriteData(0x1f); 
	Lcd_WriteData(0x1b); 
	Lcd_WriteData(0x23); 
	Lcd_WriteData(0x37); 
	Lcd_WriteData(0x00); 	
	Lcd_WriteData(0x07); 
	Lcd_WriteData(0x02); 
	Lcd_WriteData(0x10); 

	Lcd_WriteIndex(0xe1); 
	Lcd_WriteData(0x0f); 
	Lcd_WriteData(0x1b); 
	Lcd_WriteData(0x0f); 
	Lcd_WriteData(0x17); 
	Lcd_WriteData(0x33); 
	Lcd_WriteData(0x2c); 
	Lcd_WriteData(0x29); 
	Lcd_WriteData(0x2e); 
	Lcd_WriteData(0x30); 
	Lcd_WriteData(0x30); 
	Lcd_WriteData(0x39); 
	Lcd_WriteData(0x3f); 
	Lcd_WriteData(0x00); 
	Lcd_WriteData(0x07); 
	Lcd_WriteData(0x03); 
	Lcd_WriteData(0x10);  
	
	Lcd_WriteIndex(0x2a);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x02);
	Lcd_WriteData(0x02);
	Lcd_WriteData(0x80+2);

	Lcd_WriteIndex(0x2b);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0x00+3);
	Lcd_WriteData(0x00);
	Lcd_WriteData(0xA0+3);
	
	Lcd_WriteIndex(0xF0); //Enable test command  
	Lcd_WriteData(0x01); 
	Lcd_WriteIndex(0xF6); //Disable ram power save mode 
	Lcd_WriteData(0x00); 
	
	Lcd_WriteIndex(0x3A); //65k mode 
	Lcd_WriteData(0x05); 
	
	Lcd_WriteIndex(0x29);//Display on

	CtrlLcdBL(0);
}



/*************************************************
��������LCD_Set_Region
���ܣ�����lcd��ʾ�����ڴ�����д�������Զ�����
��ڲ�����xy�����յ�
����ֵ����
*************************************************/
void Lcd_SetRegion(unsigned int x_start,unsigned int y_start,unsigned int x_end,unsigned int y_end)
{	

//#ifdef USE_LANDSCAPE//ʹ�ú���ģʽ
	Lcd_WriteIndex(0x2a);
	Lcd_WriteData(0x0);
	Lcd_WriteData(x_start);
	Lcd_WriteData(0x00);
	Lcd_WriteData(x_end);

	Lcd_WriteIndex(0x2b);
	Lcd_WriteData(0x0);
	Lcd_WriteData(y_start);
	Lcd_WriteData(0x0);
	Lcd_WriteData(y_end);
/*
#else//����ģʽ	
	Lcd_WriteIndex(0x2a);
	Lcd_WriteData(0x02);
	Lcd_WriteData(x_start+2);
	Lcd_WriteData(0x02);
	Lcd_WriteData(x_end+2);

	Lcd_WriteIndex(0x2b);
	Lcd_WriteData(0x01);
	Lcd_WriteData(y_start+1);
	Lcd_WriteData(0x01);
	Lcd_WriteData(y_end+1);	
#endif
*/
	Lcd_WriteIndex(0x2c);
}


void LCD_Clear(u16 color)
{
 	u8 i,j;
	Lcd_SetRegion(0,0,130,132);
 	for (i=0;i<130;i++)
    	for (j=0;j<132;j++)
        	LCD_WriteData_16Bit(color);
}

void PutPixel(u8 x_start,u8 y_start,u16 color)
{
	Lcd_SetRegion(x_start,y_start,x_start+1,y_start+1);
	LCD_WriteData_16Bit(color);
	
}


/*****************************************************************************
 * @name       :void Gui_Drawbmp16(u16 x,u16 y,const unsigned char *p)
 * @date       :2018-08-09 
 * @function   :Display a 16-bit BMP image
 * @parameters :x:the bebinning x coordinate of the BMP image
                y:the bebinning y coordinate of the BMP image
								p:the start address of image array
 * @retvalue   :None
******************************************************************************/ 
void Gui_Drawbmp16(u8 x,u8 y,const unsigned char *p) //��ʾͼƬ
{
  u16 Ki; 
	unsigned char picW,picH; 
	u16 sendv;
	PICHEADBYTE* picHead=  (PICHEADBYTE*) p;
	p+=sizeof(PICHEADBYTE);
	picW=picHead->wl;
	picH=picHead->hl;
	Lcd_SetRegion(x,y,x+picW-1,y+picH-1);//��������
	Ki=picW*picH;
  while (Ki>0)
	{	
		sendv=*(p+1);	
		sendv=sendv<<8;
	 	sendv|=*(p);	//���ݵ�λ��ǰ
		LCD_WriteData_16Bit(sendv);  		
		p+=2;
		Ki--;
	}	
//	Lcd_SetRegion(0,0,LCD_W-1,LCD_H-1);//�ָ���ʾ����Ϊȫ��	
}



/*****************************************************************************
 * @name       :void Gui_Drawbmp16(u16 x,u16 y,const unsigned char *p)
 * @date       :2018-08-09 
 * @function   :Display a 16-bit BMP image
 * @parameters :x:the bebinning x coordinate of the BMP image
                y:the bebinning y coordinate of the BMP image
								p:the start address of image array
 * @retvalue   :None
******************************************************************************/ 
void Gui_DrawFlashbmp16(u8 x,u8 y,u32 FlashAddr) //��ʾFlashͼƬ
{
	u8 flashdata[256];
	unsigned char *p;
	u16 picSize;
	u16 sendvty;
	u16 sendN;
	PICHEADBYTE picHead;
	SPI_FLASH_BufferRead(&(picHead.scan),FlashAddr,sizeof(PICHEADBYTE));

	if (picHead.wl>128 || picHead.hl>128) return;
	
	FlashAddr+=sizeof(PICHEADBYTE);
	picSize=picHead.wl*picHead.hl*2;
	
	Lcd_SetRegion(x,y,x+picHead.wl-1,y+picHead.hl-1);//��������
	
	while(picSize>0)
	{
		if (picSize>256)
			{
			sendN=256- (FlashAddr&0xff);
			SPI_FLASH_BufferRead(flashdata,FlashAddr,sendN);
			FlashAddr+=sendN;
			picSize-=sendN;
			}
		else 
			{
			sendN=picSize;	
			SPI_FLASH_BufferRead(flashdata,FlashAddr,picSize);
			picSize=0;
			}

		p=flashdata;
		while (sendN>0)
		{	
			sendvty=*(p+1);	
			sendvty=sendvty<<8;
			sendvty|=(*(p));	//���ݵ�λ��ǰ
			LCD_WriteData_16Bit(sendvty);  		
			p+=2;
			sendN-=2;
		}	
	}
}


//��ʾһ��Ӣ���ַ�
void Display_ASCII8X16(u8 x0,u8 y0,uchar *s,u8 strglen,u16 color)
{
	u8 i,j,k,x,y,xx;
	
	unsigned char qm;
	
	u16 ulOffset;
	
	char  ywbuf[32];

	for(i = 0; i<strglen;i++)
	{
		if(((unsigned char)(*(s+i))) >= 161)
		{
			return;
		}
		
		else
		{
			qm = *(s+i);

			ulOffset = (u16)(qm) <<4;
			
      for (j = 0; j < 16; j ++)
      {
				ywbuf[j]=Zk_ASCII8X16[ulOffset+j];
      }
             
      for(y = 0;y < 16;y++)
      {
	        for(x=0;x<8;x++) 
	        {
             k=x &0x07;
                	
				  	if(ywbuf[y]&(0x80 >> k))
				   	{
				  		xx=x0+x+(i<<3);
				    	PutPixel(xx,y+y0,color);
				  	}
							
			   	}
      }

		}
	}     	
}



/*******************************************************************
 * @name       :void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
 * @date       :2018-08-09 
 * @function   :fill the specified area
 * @parameters :sx:the bebinning x coordinate of the specified area
                sy:the bebinning y coordinate of the specified area
								ex:the ending x coordinate of the specified area
								ey:the ending y coordinate of the specified area
								color:the filled color value
 * @retvalue   :None
********************************************************************/
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{  	
	u16 i,j;			
	u16 width=ex-sx+1; 		//�õ����Ŀ���
	u16 height=ey-sy+1;		//�߶�
	Lcd_SetRegion(sx,sy,ex,ey);//������ʾ����
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		LCD_WriteData_16Bit(color);	//д������ 	 
	}
}

/*******************************************************************
 * @name       :void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
 * @date       :2018-08-09 
 * @function   :Draw a line between two points
 * @parameters :x1:the bebinning x coordinate of the line
                y1:the bebinning y coordinate of the line
								x2:the ending x coordinate of the line
								y2:the ending y coordinate of the line
 * @retvalue   :None
********************************************************************/
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		PutPixel(uRow,uCol,color);//���� 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
} 

/*****************************************************************************
 * @name       :void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
 * @date       :2018-08-09 
 * @function   :Draw a rectangle
 * @parameters :x1:the bebinning x coordinate of the rectangle
                y1:the bebinning y coordinate of the rectangle
								x2:the ending x coordinate of the rectangle
								y2:the ending y coordinate of the rectangle
 * @retvalue   :None
******************************************************************************/
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	LCD_DrawLine(x1,y1,x2,y1,color);
	LCD_DrawLine(x1,y1,x1,y2,color);
	LCD_DrawLine(x1,y2,x2,y2,color);
	LCD_DrawLine(x2,y1,x2,y2,color);
}  

void UpdateLCDLEDSelected(u8 chN, u8 Selected)
{
	u8 Rw=18,Rh=100;
	u8 Rx,Ry=5,RWh=110;
	u8 Dxw=LCD_W /SaveLedData0.LedALLChanels;
	if (chN==0) return;

	Rx =((Dxw-20)>>1)+Dxw*(chN-1)+1;
	if (Selected) LCD_DrawRectangle(Rx-7,RWh-1,Rx+27,RWh+16,BLACK);
	else LCD_DrawRectangle(Rx-7,RWh-1,Rx+27,RWh+16,BACKGROUNDCOLOR);
}

void UpdateLCDLEDCloum(u8 chN, u8 Power, u8 onoff)
{
	u8 Rw=18,Rh=100;
	u8 Rx,Ry=5;
	u8 Dxw=LCD_W /SaveLedData0.LedALLChanels;
	u16 color;
	if (chN==0) return;
	
	 if (chN==1) color=PURPLE;
	 else if (chN==2) color=BLUE;
	 else if (chN==3) color=GREEN;
	 else color=RED;
	 
	Rx =((Dxw-20)>>1)+Dxw*(chN-1)+1;
	//���
	LCD_Fill(Rx,Ry,Rx+Rw,Ry+Rh-Power,BACKGROUNDCOLOR);
	if (onoff) 
		LCD_Fill(Rx,Ry+Rh-Power,Rx+Rw,Rh+Ry,color);
	else
		LCD_Fill(Rx,Ry+Rh-Power,Rx+Rw,Rh+Ry,GRAY);
}


void UpdateLcdPowerDisplay(u8 chN, u8 Power,u8 onoff)
{
	 u8 Rw=18,Rh=100;
	 u8 Rx,Ry=5,RWh=110;
	 u8 NumberStr[4];
	 u8 Dxw=LCD_W /SaveLedData0.LedALLChanels;
	 if (chN==0) return;
	Rx =((Dxw-20)>>1)+Dxw*(chN-1)+1;
	//���
	UpdateLCDLEDCloum( chN,  Power,onoff);
	

//���������ʾ
	LCD_Fill(Rx-6,RWh,Rx+26,RWh+15,BACKGROUNDCOLOR);
//	if (Selected) LCD_DrawRectangle(Rx-9,RWh-1,Rx+38,RWh+18,BLACK);
	
	if (Power==100)
		{
		Rx-=6;
		NumberStr[0]='1';
		NumberStr[1]='0';
		NumberStr[2]='0';
		NumberStr[3]='%';
		Display_ASCII8X16(Rx,RWh,NumberStr,4,BLACK);
		}
	else if (Power>=10) 
		{
		NumberStr[0]='0'+Power/10;  
		NumberStr[1]='0'+Power%10;
		NumberStr[2]='%';
		Display_ASCII8X16(Rx,RWh,NumberStr,3,BLACK);
		}
	else 
		{
		NumberStr[0]='0'+Power%10;
		NumberStr[1]='%';
		Display_ASCII8X16(Rx,RWh,NumberStr,2,BLACK);
		}
}

void LCD_DrawWorkScreen(void)
{
 u8 Rw=20,Rh=102;
 u8 Rx,Ry=4;
 u8 Dxw=LCD_W /SaveLedData0.LedALLChanels; 
 u8 onoff=0;
	LCD_Clear(BACKGROUNDCOLOR);
	Rx =(Dxw-Rw)>>1;
	LCD_DrawRectangle(Rx,Ry,Rx+Rw,Rh+Ry,PURPLE);
	if (SaveLedData0.SelectedLedChanel==1 && SaveLedData0.LedOnOff==1) onoff=1;else onoff=0;
	UpdateLcdPowerDisplay(1, SaveLedData0.LedPower[1],onoff);
	if (SaveLedData0.LedALLChanels>=2)
		{
		Rx +=Dxw;
		LCD_DrawRectangle(Rx,4,Rx+Rw,Rh+4,BLUE);
		if (SaveLedData0.SelectedLedChanel==2 && SaveLedData0.LedOnOff==1) onoff=1;else onoff=0;
		UpdateLcdPowerDisplay(2, SaveLedData0.LedPower[2],onoff);
		if (SaveLedData0.LedALLChanels>=3)
			{
			Rx +=Dxw;
			LCD_DrawRectangle(Rx,4,Rx+Rw,Rh+4,GREEN);
			if (SaveLedData0.SelectedLedChanel==3 && SaveLedData0.LedOnOff==1) onoff=1;else onoff=0;
			UpdateLcdPowerDisplay(3, SaveLedData0.LedPower[3],onoff);
			
			
			if (SaveLedData0.LedALLChanels>=4)
				{
				Rx +=Dxw;
				LCD_DrawRectangle(Rx,4,Rx+Rw,Rh+4,RED);
				if (SaveLedData0.SelectedLedChanel==4 && SaveLedData0.LedOnOff==1) onoff=1;else onoff=0;
				UpdateLcdPowerDisplay(4, SaveLedData0.LedPower[4],onoff);
				}
			}
		}

	if (SaveLedData0.SelectedLedChanel==1)
	{
	UpdateLCDLEDSelected(1,1);
	}
	else if (SaveLedData0.SelectedLedChanel==2)
	{
	UpdateLCDLEDSelected(2,1);
	}
	else if (SaveLedData0.SelectedLedChanel==3)
	{
	UpdateLCDLEDSelected(3,1);
	}
	else if (SaveLedData0.SelectedLedChanel==4)
	{
	UpdateLCDLEDSelected(4,1);
	}
	LCD_DISPiD=1;
}

void DisplayDisconnect(void)
{
	LCD_Clear(BACKGROUNDCOLOR);
	LCD_DrawRectangle(5,5,123,123,BLACK);
	Display_ASCII8X16(33,16,"Warnning",8,RED);
	Display_ASCII8X16(13,48,"Driver board",12,RED);
	Display_ASCII8X16(13,64,"disconnect!",11,RED);
	LCD_DISPiD=0xff;
}

void DispSettingRectangle(void)
{
	LCD_DrawRectangle(95,7+(LCD_DISPiD-3)*20,126,25+(LCD_DISPiD-3)*20,BACKGROUNDCOLOR);
	LCD_DrawRectangle(95,7+(LCD_DISPiD-2)*20,126,25+(LCD_DISPiD-2)*20,BLACK);
}

void DispSettingNumber(u8 location,u8 numberv)
{
	u8 Num[3];
	u8 y;
	y=8+20*(location-2);
	//���������ʾ
	LCD_Fill(95,y,126,y+16,BACKGROUNDCOLOR);
	Num[0]='0'+numberv/100;
	Num[1]='0'+numberv%100/10;
	Num[2]='0'+numberv%10;
	Display_ASCII8X16(96,y,Num,3,BLACK);
}


void DisplaySetting(void)
{

	LCD_DISPiD=2;
	LCD_Clear(BACKGROUNDCOLOR);
	Display_ASCII8X16(8,8,"LED Number:",11,BLACK);
	DispSettingNumber(2,SaveLedData0.LedALLChanels);
	LCD_DrawRectangle(95,7,126,25,BLACK);

	Display_ASCII8X16(8,28,"MotorStep:",10,BLACK);
	DispSettingNumber(3,SaveLedData0.MotorStepPluses);

	Display_ASCII8X16(8,48,"LED1 Lot:",9,BLACK);
	DispSettingNumber(4,SaveLedData0.LEDMotorLocation[1]);
	
	Display_ASCII8X16(8,68,"LED2 Lot:",9,BLACK);
	DispSettingNumber(5,SaveLedData0.LEDMotorLocation[2]);


	Display_ASCII8X16(8,88,"LED3 Lot:",9,BLACK);
	DispSettingNumber(6,SaveLedData0.LEDMotorLocation[3]);
	
	Display_ASCII8X16(8,108,"LED4 Lot:",9,BLACK);
	DispSettingNumber(7,SaveLedData0.LEDMotorLocation[4]);

	
}


#if 1


uchar code Zk_ASCII8X16[]=
 {
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x18,0x08,0xc8,0x08,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x7f,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,0xe0,
  0x60,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x18,0x10,
  0x10,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x08,0x08,0x08,0x0c,0x0c,0x0c,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x6c,0x38,0x10,0x00,0x7c,0xc6,0x70,0x1c,
  0xc6,0x7c,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x38,0x6c,0x00,0x78,
  0x0c,0x7c,0xcc,0xcc,0x76,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x7c,0xc6,0x70,0xc0,0xc6,0x7c,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x6c,0x96,0x7c,0xd0,0xd6,0x6c,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7c,0xc6,0x06,0x06,0xc6,
  0x7c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x6c,0xc6,0xc6,0xfe,
  0xc6,0xc6,0x6c,0x38,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x10,0x38,0x28,0x6c,0x44,0xc6,0xc6,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x7c,0xc6,0x06,0xfe,0xc6,0x7c,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xdc,0x66,0x66,0x66,0xf6,0x06,
  0x06,0x66,0x3c,0x00,0x00,0x00,0x00,0x6c,0x38,0xf8,0x0c,0x7e,0xc6,
  0xc6,0xc6,0x7c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0e,0x1b,0x18,
  0x18,0x18,0x18,0x18,0x18,0x18,0xd8,0x70,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0xfe,0x86,0x0c,0x18,0x0c,0x06,0x06,0xc6,0x7c,0x00,0x00,
  0x00,0x00,0x76,0xdc,0x00,0x00,0x78,0x0c,0x7c,0xcc,0xcc,0x76,0x00,
  0x00,0x00,0x00,0x00,0x00,0x76,0xdc,0x00,0x00,0x7c,0xc6,0x06,0x06,
  0xc6,0x7c,0x00,0x00,0x00,0x00,0x00,0x00,0x76,0xdc,0x00,0x00,0xdc,
  0x66,0x66,0x66,0x66,0x66,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x76,0x99,0x9f,0x98,0x99,0x6f,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0c,0x06,0x06,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3c,
  0x66,0x66,0x3c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x3c,
  0x3c,0x3c,0x18,0x18,0x00,0x18,0x18,0x00,0x00,0x00,0x00,0x00,0x00,
  0xc6,0xc6,0xc6,0x44,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x6c,0x6c,0xfe,0x6c,0x6c,0x6c,0xfe,0x6c,0x6c,
  0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x7c,0xc6,0xc2,0xc0,0x7c,0x06,
  0x86,0xc6,0x7c,0x18,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xc3,
  0xc6,0x0c,0x18,0x30,0x63,0xc3,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x38,0x6c,0x6c,0x38,0x76,0xdc,0xcc,0xcc,0x76,0x00,0x00,0x00,0x00,
  0x00,0x00,0x30,0x30,0x30,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x30,0x60,0x60,0x60,0x60,0x60,
  0x30,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x0c,0x06,0x06,
  0x06,0x06,0x06,0x0c,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x6c,0x38,0xfe,0x38,0x6c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x18,0x18,0x18,0x7e,0x18,0x18,0x18,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,
  0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xfe,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x02,0x06,0x0c,0x18,0x30,0x60,0xc0,0x80,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x7c,0xc6,0xce,0xde,0xf6,0xe6,0xc6,0xc6,0x7c,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x38,0x78,0x18,0x18,0x18,
  0x18,0x18,0x7e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7c,0xc6,0x06,
  0x0c,0x18,0x30,0x60,0xc6,0xfe,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x7c,0xc6,0x06,0x06,0x3c,0x06,0x06,0xc6,0x7c,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x0c,0x1c,0x3c,0x6c,0xcc,0xfe,0x0c,0x0c,0x1e,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0xfe,0xc0,0xc0,0xc0,0xfc,0x06,0x06,
  0xc6,0x7c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x60,0xc0,0xc0,
  0xfc,0xc6,0xc6,0xc6,0x7c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xfe,
  0xc6,0x06,0x0c,0x18,0x30,0x30,0x30,0x30,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x7c,0xc6,0xc6,0xc6,0x7c,0xc6,0xc6,0xc6,0x7c,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x7c,0xc6,0xc6,0xc6,0x7e,0x06,0x06,0x0c,
  0x78,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,
  0x00,0x18,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,
  0x18,0x00,0x00,0x00,0x18,0x18,0x30,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x06,0x0c,0x18,0x30,0x60,0x30,0x18,0x0c,0x06,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7e,0x00,0x00,0x7e,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x30,0x18,0x0c,0x06,0x0c,
  0x18,0x30,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7c,0xc6,0xc6,
  0x0c,0x18,0x18,0x00,0x18,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x7c,0xc6,0xc6,0xde,0xde,0xde,0xdc,0xc0,0x7c,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x10,0x38,0x6c,0xc6,0xc6,0xfe,0xc6,0xc6,0xc6,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0xfc,0x66,0x66,0x66,0x7c,0x66,0x66,
  0x66,0xfc,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3c,0x66,0xc2,0xc0,
  0xc0,0xc0,0xc2,0x66,0x3c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xf8,
  0x6c,0x66,0x66,0x66,0x66,0x66,0x6c,0xf8,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0xfe,0x66,0x62,0x68,0x78,0x68,0x62,0x66,0xfe,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0xfe,0x66,0x62,0x68,0x78,0x68,0x60,0x60,
  0xf0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3c,0x66,0xc2,0xc0,0xc0,
  0xde,0xc6,0x66,0x3a,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xc6,0xc6,
  0xc6,0xc6,0xfe,0xc6,0xc6,0xc6,0xc6,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x3c,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x3c,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x1e,0x0c,0x0c,0x0c,0x0c,0x0c,0xcc,0xcc,0x78,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe6,0x66,0x6c,0x6c,0x78,0x6c,
  0x6c,0x66,0xe6,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xf0,0x60,0x60,
  0x60,0x60,0x60,0x62,0x66,0xfe,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0xc6,0xee,0xfe,0xd6,0xc6,0xc6,0xc6,0xc6,0xc6,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0xc6,0xe6,0xf6,0xfe,0xde,0xce,0xc6,0xc6,0xc6,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x38,0x6c,0xc6,0xc6,0xc6,0xc6,0xc6,
  0x6c,0x38,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xfc,0x66,0x66,0x66,
  0x7c,0x60,0x60,0x60,0xf0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7c,
  0xc6,0xc6,0xc6,0xd6,0xde,0x7c,0x0c,0x0e,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0xfc,0x66,0x66,0x66,0x7c,0x6c,0x66,0x66,0xe6,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x7c,0xc6,0xc6,0x60,0x38,0x0c,0xc6,0xc6,
  0x7c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7e,0x5a,0x18,0x18,0x18,
  0x18,0x18,0x18,0x3c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xc6,0xc6,
  0xc6,0xc6,0xc6,0xc6,0xc6,0xc6,0x7c,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0xc6,0xc6,0xc6,0xc6,0xc6,0xc6,0x6c,0x38,0x10,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0xc6,0xc6,0xc6,0xc6,0xc6,0xd6,0xfe,0xee,0xc6,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xc6,0xc6,0xc6,0x6c,0x38,0x6c,
  0xc6,0xc6,0xc6,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xc6,0xc6,0xc6,
  0x6c,0x38,0x38,0x38,0x38,0x7c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0xfe,0xc6,0x8c,0x18,0x30,0x60,0xc2,0xc6,0xfe,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x3c,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x3c,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xc0,0xe0,0x70,0x38,0x1c,0x0e,
  0x06,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3c,0x0c,0x0c,0x0c,
  0x0c,0x0c,0x0c,0x0c,0x3c,0x00,0x00,0x00,0x00,0x00,0x10,0x38,0x6c,
  0xc6,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xfe,
  0x00,0x00,0x00,0x00,0x30,0x30,0x18,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x78,0x0c,
  0x7c,0xcc,0xcc,0x76,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe0,0x60,
  0x60,0x78,0x6c,0x66,0x66,0x66,0xdc,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x7c,0xc6,0xc0,0xc0,0xc6,0x7c,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x1c,0x0c,0x0c,0x3c,0x6c,0xcc,0xcc,0xcc,0x76,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7c,0xc6,0xfe,
  0xc0,0xc6,0x7c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x36,0x32,
  0x30,0x7c,0x30,0x30,0x30,0x78,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x76,0xcc,0xcc,0xcc,0x7c,0x0c,0xcc,0x78,0x00,0x00,
  0x00,0x00,0x00,0xe0,0x60,0x60,0x6c,0x76,0x66,0x66,0x66,0xe6,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x38,0x18,0x18,0x18,
  0x18,0x3c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x06,0x00,0x0e,
  0x06,0x06,0x06,0x06,0x66,0x66,0x3c,0x00,0x00,0x00,0x00,0x00,0xe0,
  0x60,0x60,0x66,0x6c,0x78,0x6c,0x66,0xe6,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x38,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x3c,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x44,0xfe,0xd6,0xd6,0xd6,
  0xd6,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xdc,0x66,
  0x66,0x66,0x66,0x66,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x7c,0xc6,0xc6,0xc6,0xc6,0x7c,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0xdc,0x66,0x66,0x66,0x7c,0x60,0x60,0xf0,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x76,0xcc,0xcc,0xcc,0x7c,0x0c,
  0x0c,0x1e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xdc,0x76,0x66,
  0x60,0x60,0xf0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x7c,0xc6,0x70,0x1c,0xc6,0x7c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x10,0x30,0x30,0xfc,0x30,0x30,0x30,0x36,0x1c,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0xcc,0xcc,0xcc,0xcc,0xcc,0x76,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xc6,0xc6,0xc6,0x6c,
  0x38,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xc6,
  0xc6,0xc6,0xd6,0xfe,0x6c,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0xc6,0x6c,0x38,0x38,0x6c,0xc6,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0xc6,0xc6,0xc6,0xc6,0x7e,0x06,0x0c,0x78,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xfe,0xcc,0x18,0x30,0x66,
  0xfe,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0e,0x18,0x18,0x18,0x70,
  0x18,0x18,0x18,0x0e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,
  0x18,0x18,0x00,0x18,0x18,0x18,0x18,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x70,0x18,0x18,0x18,0x0e,0x18,0x18,0x18,0x70,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x76,0xdc,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
  };



#endif
