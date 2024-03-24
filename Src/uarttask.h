#ifndef __UARTTASK_H
#define __UARTTASK_H




//extern UART_Memory uart1Memory;
extern UART_Memory uart2Memory;
//extern UART_Memory uart3Memory;
//extern UART_Memory uart4Memory;


//void Uart1Init(void);

void Uart2Init(void);

//void Uart3Init(void);

void Uart4Init(void);

void UpdateLcdShare(u8 share);
void UpdateTemperatureHumidity(uint8_t temperature,u8 humidity);
void SetAddressValue(uint16_t add,uint16_t va);
void ReadDispId(void);

void ChangeDispId(uint16_t picid);
void ReadDate(void);

void SetSeqAddressValue(uint16_t add,uint8_t* va,uint8_t num);
void SetSeqAddressIntValue(uint16_t add,uint8_t* va,uint8_t num);

void ClearSeqAddressValue(uint16_t add,uint8_t num);

void SetAddressU32Value(uint16_t add,u32 va1);

void Cutpicute(uint16_t addr,uint16_t cupPID, uint16_t x_org_lu,uint16_t y_org_lu,uint16_t x_org_rd,uint16_t y_org_rd,uint16_t x_pas_lu,uint16_t y_pas_lu);

//void Cutpicute(void);

void WriteSeqAddressValue(uint8_t add,uint8_t* va,uint8_t num);
void ReadDriverAddressValue(uint8_t add,uint8_t num);
void WriteDriverAddressValue(uint8_t add,uint8_t va);
void WriteDriverAddressValue2(uint8_t add,uint8_t va1,uint8_t va2);

void WriteDriverSqAddressValue(uint8_t add,uint8_t* va,uint8_t len);



void ReturnRecUart2(uint8_t add,uint8_t* va,uint8_t len);
void SetAddressValueU2(uint16_t add,uint8_t va);

#endif




