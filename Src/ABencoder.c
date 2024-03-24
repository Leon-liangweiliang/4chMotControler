#include "main.h"


u8 ABEncoderStep=0X88;
u8 PreABEncoderStep=0X88;
u8 ABEncoderDir=0;


//¶ÁÈ¡±àÂëÆ÷µÄ×´Ì¬£¬·µ»Ø¶ÁÈ¡Öµ£¬AB
//0x00,0x08,0x88,0x80  

void ReadABOpticalEncoder(void)
{
//	u8 pv;
	ABEncoderStep=P3&0X88;
//	ABEncoderStep=(ret>>2)&0X3;
if (ABEncoderStep==0X08)
/*	if (
		(ABEncoderStep==0X08 && PreABEncoderStep==0X88)
		||(ABEncoderStep==0X80 && PreABEncoderStep==0X00)
		||(ABEncoderStep==0X88 && PreABEncoderStep==0X80)
	//	||(ABEncoderStep==0X00 && PreABEncoderStep==0X08)
		)*/
	{
		ABEncoderDir=1;
	}
	
else if (ABEncoderStep==0X80)
/*	else if (
		(ABEncoderStep==0X80 && PreABEncoderStep==0X88)
		||(ABEncoderStep==0X08 && PreABEncoderStep==0X00)
//		||(ABEncoderStep==0X00 && PreABEncoderStep==0X80)
		||(ABEncoderStep==0X88 && PreABEncoderStep==0X08)
		)*/
	{
		ABEncoderDir=0xff;
	}
//	PreABEncoderStep=ABEncoderStep;
}



