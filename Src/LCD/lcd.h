#ifndef __LCD_H
#define __LCD_H		

/////////////////////////////////////�û�������///////////////////////////////////	 
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////	  
//����LCD�ĳߴ�
#define LCD_W 128
#define LCD_H 128


typedef struct 
{
   unsigned char scan;
   unsigned char gray;
   unsigned char wl;
   unsigned char wh;
   unsigned char hl;
   unsigned char hh;
   unsigned char is565;
   unsigned char rgb;
}PICHEADBYTE;

//LCD����
void CtrlLcdBL(u8);

void lcd_initial(void); 
void LCD_Clear(u16 Color);
void Gui_Drawbmp16(u8 x,u8 y,const unsigned char *p);
void Gui_DrawFlashbmp16(u8 x,u8 y,u32 FlashAddr); //��ʾFlashͼƬ
void Display_ASCII8X16(u8 x0,u8 y0,uchar *s,u8 strglen,u16 color);
void DisplayDisconnect(void);
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);
void LCD_DrawWorkScreen(void);
void UpdateLCDLEDSelected(u8 chN, u8 Selected);
void UpdateLCDLEDCloum(u8 chN, u8 Power, u8 onoff);
void UpdateLcdPowerDisplay(u8 chN, u8 Power,u8 onoff);
void DisplaySetting(void);
void DispSettingRectangle(void);
void DispSettingNumber(u8 location,u8 numberv);
void DisplaySetting2(void);






//������ɫ
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define PURPLE           0xA254
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
//GUI��ɫ

#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ
//������ɫΪPANEL����ɫ 
 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ

#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)




#define BACKGROUNDCOLOR   LGRAY


extern uchar code Zk_ASCII8X16[];
extern  u8 SettingP;

#endif  
	 
	 



