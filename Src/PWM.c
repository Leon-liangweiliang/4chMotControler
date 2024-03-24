#include "main.h"
#include "pwm.h"
#include "intrins.h"

void PWMInit(void)
{
	P21=1;
	P22=1;
    P_SW2 = 0x80;
    PWMCKS = 0x00;                              // PWMʱ��Ϊϵͳʱ��
    PWMC = 26;                              //����PWM����Ϊ25��PWMʱ��
    TADCP =00;
	PWM1T1= 1; 							//PWM1�ڼ���ֵΪ30�ط�����͵�ƽ
	PWM1T2= 12;							   //PWM1�ڼ���ֵΪ0�ط�����ߵ�ƽ
	PWM2T1= 14; 							//PWM2�ڼ���ֵΪ30�ط�����͵�ƽ
	PWM2T2= 25;							   //PWM2�ڼ���ֵΪ0�ط�����ߵ�ƽ
    PWM1CR= 0x00;                               //ʹ��PWM1���,��ʼΪ�ߵ�ƽ
    PWM2CR= 0x00;                               //ʹ��PWM2���,��ʼΪ�ߵ�ƽ
    P_SW2 = 0x00;
    PWMCR = 0x00;                               //����PWMģ��
	
}

//dutypre :0-20
/*
void SetPwmDuty(u8 dutypre)
{
	if(dutypre==0)
		{
		P_SW2 = 0x80;
		PWM1T1= 30;                             //PWM1�ڼ���ֵΪ30�ط�����͵�ƽ
	    PWM1T2= 0;                             //PWM1�ڼ���ֵΪ0�ط�����ߵ�ƽ
	    PWM2T1= 30;                             //PWM2�ڼ���ֵΪ30�ط�����͵�ƽ
	    PWM2T2= 0;                             //PWM2�ڼ���ֵΪ0�ط�����ߵ�ƽ
	    P_SW2 = 0x00;
		}
	else
		{
		P_SW2 = 0x80;
		PWM1T1= 0;                             //PWM1�ڼ���ֵΪ1�ط�����͵�ƽ
	    PWM1T2= (dutypre>>1)+1;                             //PWM1�ڼ���ֵΪ10�ط�����ߵ�ƽ
	    PWM2T1= 13;                             //PWM2�ڼ���ֵΪ11H�ط�����͵�ƽ
	    PWM2T2= ((dutypre+1)>>1)+14;                             //PWM2�ڼ���ֵΪ22�ط�����ߵ�ƽ
	    P_SW2 = 0x00;
		}
}
*/



void BeginPwm()
{
	P_SW2 = 0x80;
	PWM1CR= 0xC0;                               //ʹ��PWM1���,��ʼΪ�ߵ�ƽ
    PWM2CR= 0xC0;                               //ʹ��PWM2���,��ʼΪ�ߵ�ƽ
    P_SW2 = 0x00;
    PWMCR = 0x80;                               //����PWMģ��
}


