#ifndef MAIN_H
#define MAIN_H
#include "stc8gxx.h"
#include "intrins.h"

#define uint8_t unsigned char
#define u8 unsigned char
#define uchar unsigned char


#define uint16_t unsigned int
#define u16 unsigned int

#define uint32_t unsigned long
#define u32 unsigned long

#define WDTRST WDT_CONTR |= 0x10 

#define RxBufferSize   32

typedef enum
{
	REC_HEAD1=0,
	REC_HEAD2,
	REC_RKLEN,
	REC_CMD,
	REC_DATA,
	REC_ADDSUM
}UARTST_ENUM;


typedef struct
{
	uint8_t pklen;
	uint8_t cmd;
	uint8_t UartRunBuf[8];
	uint8_t bufwpoint;
	uint8_t Addsum;
	UARTST_ENUM RecStatus;
}UARTST_PKG;


typedef enum
{
	REC2_HEAD1=0,
	REC2_HEAD2,
	REC2_PIC_N,
	REC2_PKG_N_H,
	REC2_PKG_N_L,
	REC2_RKLEN,
	REC2_DATA,
	REC2_ADDSUM
}UARTST2_ENUM;


typedef enum
{
	REC4_HEAD1=0,
	REC4_HEAD2,
	REC4_CMD,
	REC4_ADDR,
	REC4_DATA_RLEN,
	REC4_ADDSUM
}UARTST4_ENUM;


typedef struct
{
	uint8_t UartPkgTimeoutCounter;
	uint8_t busy;
	UARTST2_ENUM RecStatus;
	UARTST4_ENUM UsbCmdStatus;
	uint8_t pic_N;
	uint16_t pkg_num;
	uint8_t pklen;
	uint8_t bufwpoint;
	uint8_t UartRunBuf[256];
	uint8_t Addsum;
}UARTST4_PKG;

typedef struct {
	uint8_t RxCounter;
	uint8_t ReadCounter;
	uint8_t RxByteSize;
	uint8_t UartPkgTimeoutCounter;
	uint8_t busy;
	uint8_t RxBuffer[RxBufferSize];
}UART_Memory;

extern uint8_t NeedBeep;
extern uint8_t LCD_DISPiD;
//extern UARTST_PKG	uart1rcpkg;
extern UARTST_PKG	uart2rcpkg;
//extern UARTST_PKG	uart3rcpkg;
extern UARTST4_PKG	uart4rcpkg;
extern u8 UsbCmdUpdate;
 
extern u8 usbWRAddr,usbWRData;
extern u8 usbCmdCode;

void delay_us(uint16_t dt);
void delay_ms(uint16_t dt);
void Time0OutDelaySub(void);
void ClearMemory(uint8_t * ptr,uint8_t len);


#endif
