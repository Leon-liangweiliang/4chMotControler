#ifndef _EEPROM_H
#define _EEPROM_H

#include "main.h"
u8 IapRead(int addr);
void IapProgram(int addr, u8 dat);
void IapErase(int addr);
void IapProgramArea(int addr, u8*datap, int len);
void IapReadArea(int addr, u8*datap, int len);

#endif
