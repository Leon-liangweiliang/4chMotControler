#include "main.h"
#include "intrins.h"

void IapIdle()
{
    IAP_CONTR = 0;                              //¹Ø±ÕIAP¹¦ÄÜ
    IAP_CMD = 0;                                //Çå³ıÃüÁî¼Ä´æÆ÷
    IAP_TRIG = 0;                               //Çå³ı´¥·¢¼Ä´æÆ÷
    IAP_ADDRH = 0x80;                           //½«µØÖ·ÉèÖÃµ½·ÇIAPÇøÓò
    IAP_ADDRL = 0;
}

u8 IapRead(int addr)
{
    u8 dat;

    IAP_CONTR = 0x80;                         //Ê¹ÄÜIAP
    IAP_TPS   = 33;
    IAP_CMD = 1;                                //ÉèÖÃIAP¶ÁÃüÁî
    IAP_ADDRL = addr;                           //ÉèÖÃIAPµÍµØÖ·
    IAP_ADDRH = addr >> 8;                      //ÉèÖÃIAP¸ßµØÖ·
    IAP_TRIG = 0x5a;                            //Ğ´´¥·¢ÃüÁî(0x5a)
    IAP_TRIG = 0xa5;                            //Ğ´´¥·¢ÃüÁî(0xa5)
    _nop_();
    dat = IAP_DATA;                             //¶ÁIAPÊı¾İ
    IapIdle();                                  //¹Ø±ÕIAP¹¦ÄÜ

    return dat;
}

void IapProgram(int addr, u8 dat)
{
    IAP_CONTR = 0X80;                         //Ê¹ÄÜIAP
    IAP_TPS   = 33;
    IAP_CMD = 2;                                //ÉèÖÃIAPĞ´ÃüÁî
    IAP_ADDRL = addr;                           //ÉèÖÃIAPµÍµØÖ·
    IAP_ADDRH = addr >> 8;                      //ÉèÖÃIAP¸ßµØÖ·
    IAP_DATA = dat;                             //Ğ´IAPÊı¾İ
    IAP_TRIG = 0x5a;                            //Ğ´´¥·¢ÃüÁî(0x5a)
    IAP_TRIG = 0xa5;                            //Ğ´´¥·¢ÃüÁî(0xa5)
    _nop_();
    IapIdle();                                  //¹Ø±ÕIAP¹¦ÄÜ
}

void IapErase(int addr)
{
    IAP_CONTR = 0X80;                         //Ê¹ÄÜIAP
    IAP_TPS   = 33;
    IAP_CMD = 3;                                //ÉèÖÃIAP²Á³ıÃüÁî
    IAP_ADDRL = addr;                           //ÉèÖÃIAPµÍµØÖ·
    IAP_ADDRH = addr >> 8;                      //ÉèÖÃIAP¸ßµØÖ·
    IAP_TRIG = 0x5a;                            //Ğ´´¥·¢ÃüÁî(0x5a)
    IAP_TRIG = 0xa5;                            //Ğ´´¥·¢ÃüÁî(0xa5)
    _nop_();                                    //
    IapIdle();                                  //¹Ø±ÕIAP¹¦ÄÜ
}

void IapProgramArea(int addr, u8*datap, int len)
{
	while (len>0)
		{
		IapProgram(addr,*datap);
		datap++;
		addr++;
		len--;
		}
}


void IapReadArea(int addr, u8*datap, int len)
{
	while (len>0)
		{
		*datap=IapRead(addr);
		datap++;
		addr++;
		len--;
		}
}


