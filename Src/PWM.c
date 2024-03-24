#include "main.h"
#include "pwm.h"
#include "intrins.h"

void PWMInit(void)
{
	P21=1;
	P22=1;
    P_SW2 = 0x80;
    PWMCKS = 0x00;                              // PWM时钟为系统时钟
    PWMC = 26;                              //设置PWM周期为25个PWM时钟
    TADCP =00;
	PWM1T1= 1; 							//PWM1在计数值为30地方输出低电平
	PWM1T2= 12;							   //PWM1在计数值为0地方输出高电平
	PWM2T1= 14; 							//PWM2在计数值为30地方输出低电平
	PWM2T2= 25;							   //PWM2在计数值为0地方输出高电平
    PWM1CR= 0x00;                               //使能PWM1输出,初始为高电平
    PWM2CR= 0x00;                               //使能PWM2输出,初始为高电平
    P_SW2 = 0x00;
    PWMCR = 0x00;                               //启动PWM模块
	
}

//dutypre :0-20
/*
void SetPwmDuty(u8 dutypre)
{
	if(dutypre==0)
		{
		P_SW2 = 0x80;
		PWM1T1= 30;                             //PWM1在计数值为30地方输出低电平
	    PWM1T2= 0;                             //PWM1在计数值为0地方输出高电平
	    PWM2T1= 30;                             //PWM2在计数值为30地方输出低电平
	    PWM2T2= 0;                             //PWM2在计数值为0地方输出高电平
	    P_SW2 = 0x00;
		}
	else
		{
		P_SW2 = 0x80;
		PWM1T1= 0;                             //PWM1在计数值为1地方输出低电平
	    PWM1T2= (dutypre>>1)+1;                             //PWM1在计数值为10地方输出高电平
	    PWM2T1= 13;                             //PWM2在计数值为11H地方输出低电平
	    PWM2T2= ((dutypre+1)>>1)+14;                             //PWM2在计数值为22地方输出高电平
	    P_SW2 = 0x00;
		}
}
*/



void BeginPwm()
{
	P_SW2 = 0x80;
	PWM1CR= 0xC0;                               //使能PWM1输出,初始为高电平
    PWM2CR= 0xC0;                               //使能PWM2输出,初始为高电平
    P_SW2 = 0x00;
    PWMCR = 0x80;                               //启动PWM模块
}


