
#include "main.h"
#include "SaveConfigFile.h"
#include "CompressSensor.h"



volatile uint16_t  ReleaseCompressV=0;
extern volatile uint16_t CurrentCompressV;
extern volatile uint8_t CompressValueUpdate;
extern volatile uint8_t AdcSumCount;
extern volatile uint32_t Adc1sumv;

// uint16_t code PowerLevelReaDelay[]={110,95,80,70,60,45,30,20,10};
 uint16_t code PowerLevelReaDelay[]={110,95,80,70,60,45,30,20,10};

// uint16_t code PowerLevelChoDelay[]={400,410,420,430,440,450,460,470,480};
uint16_t code PowerLevelChoDelay[]={400,410,420,430,440,450,460,470,480};

uint8_t PressIsArrive=0;
uint8_t PressProSt=0;

uint16_t timer3_DelayCounter=0;


extern uint8_t _isOnOff;

void SensorInit(void)
{
    ADCCFG = 0x2d;   //12bit   480 cpu clocks;
    ADC_CONTR = 0x82;                           //使能并启动ADC模块  CH2
    PADC=1;
    IPH|=PADCH;
    EADC = 1;                                   //使能ADC中断
    EA = 1;
}


void WaitCompressValueUpdate(void)
{
	CompressValueUpdate=0;
	AdcSumCount=0;
	Adc1sumv=0;
	while (CompressValueUpdate==0);
}

void ReadReleaseCompressV(void)
{
	SlotCtrl(0);
	BengCtrl(1);
	delay_ms(400);
	/*Start Conversion */
	ADC_CONTR |= 0x40;                          //启动AD转换//ADC1_StartConversion();
	WaitCompressValueUpdate();
	ReleaseCompressV=CurrentCompressV;
	BengCtrl(0);
	//ADC1_Cmd(DISABLE);
	ADC_CONTR = 0x08;                           //使能并关闭ADC模块  CH8
	delay_ms(50);
	SlotCtrl(0);
}

//1ms延时
void TIM3_Config(uint16_t delayt)
{
	  /* 
	  TIM3 configuration:
	 - TIM3CLK is set to 16 MHz, the TIM2 Prescaler is equal to 256 so the TIM2 counter
	 clock used is 16 MHz / 128 = 125 000 Hz
	- In this example we need to generate a time base equal to 1 ms 
    so TIM3_PERIOD = (0.001 * 125000 - 1) = 124 */
	
	  /*
	delayt=delayt*125-1;
//	TIM3_DeInit();
	TIM3_TimeBaseInit(TIM3_PRESCALER_128,  delayt);
	TIM3_ClearFlag(TIM3_FLAG_UPDATE);
//	TIM3_ITConfig(TIM3_IT_UPDATE, ENABLE);//使能中断
	TIM3_Cmd(ENABLE);
	*/
	timer3_DelayCounter= delayt/5;
	T4T3M &= 0xFD;		//定时器时钟12T模式
	T3L = 0x20;		//设置定时初值
	T3H = 0xE7;		//设置定时初值
	AUXINTIF &= ~T3IF;                          //清中断标志
	T4T3M |= 0x08;		//定时器3开始计时
}

 

void TIM3_Init(void)//3455微秒@22.1184MHz
{
	  /* 
	  TIM3 configuration:
	 - TIM3CLK is set to 16 MHz, the TIM2 Prescaler is equal to 256 so the TIM2 counter
	 clock used is 16 MHz / 128 = 125 000 Hz
	- In this example we need to generate a time base equal to 1 ms 
    so TIM3_PERIOD = (0.001 * 125000 - 1) = 124 */
	/* 
	TIM3_DeInit();
	TIM3_ClearFlag(TIM3_FLAG_UPDATE);
	TIM3_ITConfig(TIM3_IT_UPDATE, ENABLE);//使能中断
	//TIM3_Cmd(DISABLE);
	*/

  T4T3M &= 0xfd;	//定时器时钟12T模式
	T3L = 0x20;		//设置定时初值
	T3H = 0xE7;		//设置定时初值
	IE2 |= ET3;                                  //使能定时器中断
	T4T3M &= 0xf7;                               //停止启动定时器
}

//控制压力线程
void CtrlCompress(uint8_t ct)
{
	if (ct)
		{
	//	Adc1sumv=0;
	//	CompressValueUpdate=0;
		AdcSumCount=0;
		PressIsArrive=0;  //压力到达标志
		PressProSt=0;
		BengCtrl(1);//打开气泵
		SlotCtrl(0);
		//ADC1_Cmd(ENABLE);
		//ADC1_StartConversion(); //启动ADC
		ADC_CONTR = 0x88;                           //使能并启动ADC模块  CH8
		ADC_CONTR |= 0x40;                          //启动AD转换
		TIM3_Config(500);  //500ms充气延时
		}
	else 
		{
		BengCtrl(0);
		SlotCtrl(1);
		//ADC1_Cmd(DISABLE);
		//TIM3_Cmd(DISABLE);
		ADC_CONTR = 0x08;                           //使能并启动ADC模块  CH8
		T4T3M &= 0xf7;                               //停止启动定时器
		}
}

void ControlCompress(void)
{
	uint8_t PowerLevel;
	PowerLevel=SaveConfigfile[0].sEPPowerLevel;
	if (CurrentCompressV<(THROATHPRES))  //压力阈值判断，达到阈值
		{
		PressIsArrive=1; //到达压力标志
		if (PowerLevel<10)
			{
			PressProSt=1; //压力到阈值，抽气延时
			TIM3_Config(PowerLevelChoDelay[PowerLevel-1]); //开始抽气延时，暂停ADC,
			}
		else
			{
			SlotCtrl(0);
			//ADC1_StartConversion();	
			ADC_CONTR |= 0x40;                          //启动AD转换
			TIM3_Config(500);  //500ms充气延时
			}
		}
	else //压力未到,接续ADC检测
		{
		SlotCtrl(0);
		//ADC1_StartConversion();
		ADC_CONTR |= 0x40;                          //启动AD转换
		}

}

void Tim3TimeroutSub(void)
{
	uint8_t PowerLevel;
	if (timer3_DelayCounter>0) 
		{
		timer3_DelayCounter--;
		return;
		}
	else T4T3M &= 0xf7;                               //停止启动定时器
	//TIM3_Cmd(DISABLE);
	PowerLevel=SaveConfigfile[0].sEPPowerLevel;
	if (PressProSt==1) //压力到阈值，抽气延时
	 	{
	 		SlotCtrl(1);//放气
	 		TIM3_Config(PowerLevelReaDelay[PowerLevel-1]);  //开始放气延时
			PressProSt=2;
	 	}
	else if (PressProSt==2)
		{
			PressProSt=0;
			AdcSumCount=0;
			SlotCtrl(0);
			//ADC1_StartConversion();
			ADC_CONTR |= 0x40;                          //启动AD转换
			TIM3_Config(500);  //500ms充气延时
		}
	else if (PressProSt==0)
		{
		PressIsArrive=0; //未到达压力标志
		}
	
}


