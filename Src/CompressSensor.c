
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
    ADC_CONTR = 0x82;                           //ʹ�ܲ�����ADCģ��  CH2
    PADC=1;
    IPH|=PADCH;
    EADC = 1;                                   //ʹ��ADC�ж�
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
	ADC_CONTR |= 0x40;                          //����ADת��//ADC1_StartConversion();
	WaitCompressValueUpdate();
	ReleaseCompressV=CurrentCompressV;
	BengCtrl(0);
	//ADC1_Cmd(DISABLE);
	ADC_CONTR = 0x08;                           //ʹ�ܲ��ر�ADCģ��  CH8
	delay_ms(50);
	SlotCtrl(0);
}

//1ms��ʱ
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
//	TIM3_ITConfig(TIM3_IT_UPDATE, ENABLE);//ʹ���ж�
	TIM3_Cmd(ENABLE);
	*/
	timer3_DelayCounter= delayt/5;
	T4T3M &= 0xFD;		//��ʱ��ʱ��12Tģʽ
	T3L = 0x20;		//���ö�ʱ��ֵ
	T3H = 0xE7;		//���ö�ʱ��ֵ
	AUXINTIF &= ~T3IF;                          //���жϱ�־
	T4T3M |= 0x08;		//��ʱ��3��ʼ��ʱ
}

 

void TIM3_Init(void)//3455΢��@22.1184MHz
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
	TIM3_ITConfig(TIM3_IT_UPDATE, ENABLE);//ʹ���ж�
	//TIM3_Cmd(DISABLE);
	*/

  T4T3M &= 0xfd;	//��ʱ��ʱ��12Tģʽ
	T3L = 0x20;		//���ö�ʱ��ֵ
	T3H = 0xE7;		//���ö�ʱ��ֵ
	IE2 |= ET3;                                  //ʹ�ܶ�ʱ���ж�
	T4T3M &= 0xf7;                               //ֹͣ������ʱ��
}

//����ѹ���߳�
void CtrlCompress(uint8_t ct)
{
	if (ct)
		{
	//	Adc1sumv=0;
	//	CompressValueUpdate=0;
		AdcSumCount=0;
		PressIsArrive=0;  //ѹ�������־
		PressProSt=0;
		BengCtrl(1);//������
		SlotCtrl(0);
		//ADC1_Cmd(ENABLE);
		//ADC1_StartConversion(); //����ADC
		ADC_CONTR = 0x88;                           //ʹ�ܲ�����ADCģ��  CH8
		ADC_CONTR |= 0x40;                          //����ADת��
		TIM3_Config(500);  //500ms������ʱ
		}
	else 
		{
		BengCtrl(0);
		SlotCtrl(1);
		//ADC1_Cmd(DISABLE);
		//TIM3_Cmd(DISABLE);
		ADC_CONTR = 0x08;                           //ʹ�ܲ�����ADCģ��  CH8
		T4T3M &= 0xf7;                               //ֹͣ������ʱ��
		}
}

void ControlCompress(void)
{
	uint8_t PowerLevel;
	PowerLevel=SaveConfigfile[0].sEPPowerLevel;
	if (CurrentCompressV<(THROATHPRES))  //ѹ����ֵ�жϣ��ﵽ��ֵ
		{
		PressIsArrive=1; //����ѹ����־
		if (PowerLevel<10)
			{
			PressProSt=1; //ѹ������ֵ��������ʱ
			TIM3_Config(PowerLevelChoDelay[PowerLevel-1]); //��ʼ������ʱ����ͣADC,
			}
		else
			{
			SlotCtrl(0);
			//ADC1_StartConversion();	
			ADC_CONTR |= 0x40;                          //����ADת��
			TIM3_Config(500);  //500ms������ʱ
			}
		}
	else //ѹ��δ��,����ADC���
		{
		SlotCtrl(0);
		//ADC1_StartConversion();
		ADC_CONTR |= 0x40;                          //����ADת��
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
	else T4T3M &= 0xf7;                               //ֹͣ������ʱ��
	//TIM3_Cmd(DISABLE);
	PowerLevel=SaveConfigfile[0].sEPPowerLevel;
	if (PressProSt==1) //ѹ������ֵ��������ʱ
	 	{
	 		SlotCtrl(1);//����
	 		TIM3_Config(PowerLevelReaDelay[PowerLevel-1]);  //��ʼ������ʱ
			PressProSt=2;
	 	}
	else if (PressProSt==2)
		{
			PressProSt=0;
			AdcSumCount=0;
			SlotCtrl(0);
			//ADC1_StartConversion();
			ADC_CONTR |= 0x40;                          //����ADת��
			TIM3_Config(500);  //500ms������ʱ
		}
	else if (PressProSt==0)
		{
		PressIsArrive=0; //δ����ѹ����־
		}
	
}


