#ifndef AT24C02_H
#define AT24C02_H
#include "main.h"
void Init_I2c_GPIO(void);
uint8_t ISendStr(uint8_t WriteAddr,uint8_t *s,uint8_t NumByteToWrite);
uint8_t IRcvStr(uint8_t ReadAddr,uint8_t *s,uint8_t NumByteToRead);
#endif