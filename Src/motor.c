#include "main.h"
#include "motor.h"

#define FAST_SPEED_DUTY  99//62//50
#define NORMAL_SPEED_DUTY 90//56// 35
#define SLOW_SPEED_DUTY  85//50//20
#define Back_SPEED_DUTY  99//71//60
#define HMove_SPEED_DUTY   99

#define TIM1_PERIOD 100 

volatile MOTOR_PARAM motor1Params;

MOTOR_CTRL_COMMAND motor1Cmd;


void TIM4_Config(void)  //691微秒@22.1184MHz
{
	  /* 
	  TIM4 configuration:
	 - TIM4CLK is set to 16 MHz, the TIM2 Prescaler is equal to 256 so the TIM2 counter
	 clock used is 16 MHz / 128 = 125 000 Hz
	- In this example we need to generate a time base equal to 1 ms   r: 1/16*11.0592=0.6912ms
    so TIM4_PERIOD = (0.001 * 125000 - 1) = 124 */
	 /*
	TIM4_DeInit();
	TIM4_TimeBaseInit(TIM4_PRESCALER_128,  124);
	TIM4_ClearFlag(TIM4_FLAG_UPDATE);
	TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);//使能中断
	TIM4_Cmd(DISABLE);
	*/
	
    T4T3M &= 0x7f;                               //定时器停止
	T4L = 0x06;		//设置定时初值
	T4H = 0xFB;		//设置定时初值
    IE2 |= ET4;                                  //使能定时器中断

}



/**
  * @brief  Configure TIM1 to generate 7 PWM signals with 4 different duty cycles
  * @param  None
  * @retval None
  */
void PWM_Config(void)
{
#if 0
   TIM1_DeInit();

  /* Time Base configuration */
    /* 
    TIM1 configuration:
   - TIM1CLK is set to 16 MHz, the TIM2 Prescaler is equal to 1 so the TIM2 counter
   clock used is 16 MHz / 8 = 2MHz
  - In this example we need to generate a time base equal to 20KHZ
   so TIM1_PERIOD = (2MHz/ 20KHZ - 1) = 99 
   */
   
  /*
  TIM1_Period = 99
  TIM1_Prescaler =7
  TIM1_CounterMode = TIM1_COUNTERMODE_UP
  TIM1_RepetitionCounter = 0
  */

  TIM1_TimeBaseInit(7, TIM1_COUNTERMODE_UP, TIM1_PERIOD, 0);

  /* Channel 1, 2,3 and 4 Configuration in PWM mode */
  
  /*
  TIM1_OCMode = TIM1_OCMODE_PWM2
  TIM1_OutputState = TIM1_OUTPUTSTATE_ENABLE
  TIM1_OutputNState = TIM1_OUTPUTNSTATE_ENABLE
  TIM1_Pulse = CCR1_Val
  TIM1_OCPolarity = TIM1_OCPOLARITY_LOW
  TIM1_OCNPolarity = TIM1_OCNPOLARITY_HIGH
  TIM1_OCIdleState = TIM1_OCIDLESTATE_SET
  TIM1_OCNIdleState = TIM1_OCIDLESTATE_RESET
  /*TIM1_Pulse = CCR1_Val*/
  TIM1_OC1Init(TIM1_OCMODE_PWM2, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_ENABLE,
               0, TIM1_OCPOLARITY_LOW, TIM1_OCNPOLARITY_HIGH, TIM1_OCIDLESTATE_SET,
               TIM1_OCNIDLESTATE_RESET); 

  TIM1_OC2Init(TIM1_OCMODE_PWM2, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_ENABLE, 0,
               TIM1_OCPOLARITY_LOW, TIM1_OCNPOLARITY_HIGH, TIM1_OCIDLESTATE_SET, 
               TIM1_OCNIDLESTATE_RESET);	


  /* TIM1 counter enable */
  TIM1_Cmd(ENABLE);

  /* TIM1 Main Output Enable */
  TIM1_CtrlPWMOutputs(ENABLE);
#endif

    P_SW2 = 0x80;
    PWMCKS = 10;                              // PWM时钟为系统时钟/11
    PWMC = TIM1_PERIOD;                              //设置PWM周期为100个PWM时钟
    
	PWM1T1= 0;                             //PWM1在计数值为0H地方输出低电平
    PWM1T2= 0;                             //PWM1在计数值为0H地方输出高电平
    
    PWM2T1= 0;                             //PWM1在计数值为0H地方输出低电平
    PWM2T2= 0;                             //PWM1在计数值为0H地方输出高电平
    
    PWM1CR= 0x80;                               //使能PWM0输出
    PWM2CR= 0x80;                               //使能PWM1输出
    P_SW2 = 0x00;

    PWMCR = 0x80;                               //启动PWM模块
}



/***********************************************************************************************
*函数名 ：Motor_init
*函数功能描述 ：包括选择电机芯片初始化和电机SPI初始化
*函数参数 ： 
*函数返回值 ：
***********************************************************************************************/
void Motor_init(void)
{
   //pwm
   //GPIO_Init(GPIOC, (GPIO_Pin_TypeDef)(GPIO_PIN_1| GPIO_PIN_2), GPIO_MODE_OUT_PP_LOW_FAST);
	
   
   //AB编码器输入
   //GPIO_Init(GPIOD, (GPIO_Pin_TypeDef)(GPIO_PIN_3| GPIO_PIN_4), GPIO_MODE_IN_PU_IT);
   //EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD, EXTI_SENSITIVITY_RISE_FALL);
   //INT0,INT1;
    PX0=1;     //INT0,INT1 最高级优先
    IPH|=PX0H;    
	PX1=1;
    IPH|=PX1H;    
	IT0 = 0;                                    //使能INT0上升沿和下降沿中断
    EX0 = 1;                                    //使能INT0中断
    IT1 = 0;                                    //使能INT1上升沿和下降沿中断
    EX1 = 1;                                    //使能INT1中断
   motor1Params.MotorDir= MOTOR_CCW_IDLE;
   motor1Params.MotorSpeedStatus=MOTOR_ST_STOP;
   motor1Params.Motor2Speed=0;
   motor1Params.MotorRuningSpeed=0;
   motor1Params.PositionABCounter=0;
   motor1Params.PrePositionABCounter=0;
   motor1Params.MotorDir=MOTOR_CCW_IDLE;
//	 motor1Params.PreABstnum=0;
//   motor1Params.PreABStatus[0]=ReadABOpticalEncoder();
  // motor1Params.PreABStatus=ReadABOpticalEncoder();
   motor1Params.ABEncoder_Error=0;
   motor1Params.Motor2MoveStep=0;
   motor1Cmd=MOTOR_CMD_NULL;
  
   PWM_Config();
   TIM4_Config();
  
}

void MotorMoveSpeedUpdate(MOTOR_DIRECTION CCW, uint16_t speed)
{
	
	if (CCW==MOTOR_CW)
		{
		//TIM1_SetCompare2(0);
		///TIM1_SetCompare1(speed);
		P_SW2 = 0x80;
		PWM2T1= 0;                             //PWM1在计数值为0H地方输出低电
	    PWM1T1= speed;                             //PWM1在计数值为0H地方输出低电平
	    P_SW2 = 0x00;
		}
	else if (CCW==MOTOR_CCW)
		{
		//TIM1_SetCompare1(0);
		//TIM1_SetCompare2(speed);
		P_SW2 = 0x80;
		PWM1T1= 0;                             //PWM1在计数值为0H地方输出低电
	    PWM2T1= speed;                             //PWM1在计数值为0H地方输出低电平
	    P_SW2 = 0x00;
		}
	else
		{
		//TIM1_SetCompare1(0);
		//TIM1_SetCompare2(0);
		P_SW2 = 0x80;
		PWM1T1= 0;                             //PWM1在计数值为0H地方输出低电
		PWM2T1= 0;                             //PWM1在计数值为0H地方输出低电
		P_SW2 = 0x00;
		}
	
}

uint16_t Get2MoveSpeed(MOTOR_SPEED mop)
{
	if (mop==MOTOR_STOP)
		return 0;
	else if (mop==MOTOR_FAST)
		return FAST_SPEED_DUTY;
	else if (mop==MOTOR_NORMAL)
		return NORMAL_SPEED_DUTY;
	else if (mop==MOTOR_SLOW)
		return SLOW_SPEED_DUTY;
	else if (mop==MOTOR_BACK)
		return Back_SPEED_DUTY;
	else if (mop==MOTOR_HANDMOVE)
		return HMove_SPEED_DUTY;
	return 0;
}



void MotorMoveStep(MOTOR_DIRECTION ccw, MOTOR_SPEED Tospeed, uint16_t MStep)
{
	motor1Params.MotorDir=ccw;
	motor1Params.Motor2MoveStep=MStep;
	motor1Params.PositionABCounter=0;
	motor1Params.PrePositionABCounter=0;
	motor1Params.Motor2Speed=Get2MoveSpeed(Tospeed);
	motor1Cmd=MOTOR_MOVE_STEP;
	
	//TIM1_CtrlPWMOutputs(ENABLE);
	//TIM4_Cmd(ENABLE);  
	P_SW2 = 0x80;
	PWM1CR= 0x80;                               //使能PWM0输出
    PWM2CR= 0x80;                               //使能PWM1输出
    P_SW2 = 0x00;
	T4T3M |= 0x80;		//定时器4开始计时
}



void Motor_Const_Move(MOTOR_DIRECTION ccw,MOTOR_SPEED Tospeed)
{
	motor1Params.MotorDir=ccw;
	motor1Params.Motor2Speed=Get2MoveSpeed(Tospeed);
	//TIM1_CtrlPWMOutputs(ENABLE);
	//TIM4_Cmd(ENABLE);
	P_SW2 = 0x80;
	PWM1CR= 0x80;                               //使能PWM0输出
    PWM2CR= 0x80;                               //使能PWM1输出
    P_SW2 = 0x00;
	T4T3M |= 0x80;		//定时器4开始计时
}


void Motor_HStop(void)
{
	//TIM4_Cmd(DISABLE);
	//TIM1_CtrlPWMOutputs(DISABLE);
	//GPIO_WriteLow(GPIOC, (GPIO_Pin_TypeDef)(GPIO_PIN_1|GPIO_PIN_2));
	
	T4T3M &= 0x7f;                               //定时器停止
	P_SW2 = 0x80;
	PWM1CR= 0x00;                               //使能PWM0输出
    PWM2CR= 0x00;                               //使能PWM1输出
    P_SW2 = 0x00;
	P21=0;P22=0;
	
	motor1Params.Motor2Speed=0;
	motor1Params.MotorRuningSpeed=0;
	motor1Params.MotorSpeedStatus=MOTOR_ST_STOP;
	MotorMoveSpeedUpdate(MOTOR_CCW_IDLE,0);
	
}

void Motor_SStop(void)
{
	motor1Params.Motor2Speed=0;
	motor1Cmd=MOTOR_CMD_SSTOP;
	//TIM1_CtrlPWMOutputs(ENABLE);
	//TIM4_Cmd(ENABLE);
	P_SW2 = 0x80;
	PWM1CR= 0x80;                               //使能PWM0输出
    PWM2CR= 0x80;                               //使能PWM1输出
    P_SW2 = 0x00;
	T4T3M |= 0x80;		//定时器4开始计时
}



//马达运动1步
uint8_t MotorMoveOneStep(MOTOR_DIRECTION ccw)
{
	Motor_HStop();
	motor1Params.MotorRuningSpeed=motor1Params.Motor2Speed=SLOW_SPEED_DUTY;
	motor1Params.MotorSpeedStatus=MOTOR_ST_CON;
	motor1Params.PrePositionABCounter=motor1Params.PositionABCounter;
	
	while(motor1Params.PrePositionABCounter==motor1Params.PositionABCounter && motor1Params.ABEncoder_Error==0)
		{
		if (ccw==MOTOR_CW)
			{
			//GPIO_WriteHigh(GPIOC, (GPIO_Pin_TypeDef)(GPIO_PIN_1));
			P21=1;
			delay_ms(2);
			//GPIO_WriteLow(GPIOC, (GPIO_Pin_TypeDef)(GPIO_PIN_1));
			P21=0;
			
			}
		else 
			{
			//GPIO_WriteHigh(GPIOC, (GPIO_Pin_TypeDef)(GPIO_PIN_2));
			P22=1;
			delay_ms(2);
			//GPIO_WriteLow(GPIOC, (GPIO_Pin_TypeDef)(GPIO_PIN_2));
			P22=0;
			}
		}
	motor1Params.MotorSpeedStatus=MOTOR_ST_STOP;
	motor1Params.MotorRuningSpeed=motor1Params.Motor2Speed=0;
	if (motor1Params.ABEncoder_Error==1) return 0;
	return 1;
}

MOTOR_SPEED_STATUS MotorACCChangeSpSub(void)
{
	if (motor1Params.MotorRuningSpeed <motor1Params.Motor2Speed)
		{
		motor1Params.MotorRuningSpeed+=MOTORACCSPEED;
		MotorMoveSpeedUpdate(motor1Params.MotorDir,motor1Params.MotorRuningSpeed);
		return MOTOR_ST_ACC;  //加速
		}
	else if (motor1Params.MotorRuningSpeed > motor1Params.Motor2Speed)
		{
		motor1Params.MotorRuningSpeed-=MOTORACCSPEED;
		MotorMoveSpeedUpdate(motor1Params.MotorDir,motor1Params.MotorRuningSpeed);
		return MOTOR_ST_DEC;  //减速
		}	
	else if (motor1Params.MotorRuningSpeed ==0)
		return MOTOR_ST_STOP;  //停止

		return MOTOR_ST_CON;  //匀速
  
}


uint8_t DoMotorCmd(void)
{
	if (motor1Cmd==MOTOR_CMD_NULL)  return 0;

	switch (motor1Cmd)
		{
		case MOTOR_CMD_SSTOP:
			if (motor1Params.MotorSpeedStatus==MOTOR_ST_STOP)
				{
				motor1Params.MotorDir=MOTOR_CCW_IDLE;
				motor1Cmd=MOTOR_CMD_NULL;
				return 0;
				}
		break;
		case MOTOR_MOVE_STEP:
			if (motor1Params.PositionABCounter>=motor1Params.Motor2MoveStep)
				{
				Motor_HStop(); //硬件停止
				motor1Cmd=MOTOR_CMD_NULL;
				return 0;
				}
			else if ((motor1Params.PositionABCounter+50)>motor1Params.Motor2MoveStep)//接近目的位置
				{
				motor1Params.Motor2Speed=SLOW_SPEED_DUTY; //减速
				}
		break;
		}
	return 1;
}

//马达线程
void ProcessMotorThread(void)
{
	//速度调节检测
	motor1Params.MotorSpeedStatus= MotorACCChangeSpSub();
	//马达控制
	if (DoMotorCmd()==0 && (motor1Params.MotorSpeedStatus==MOTOR_ST_STOP || motor1Params.MotorSpeedStatus==MOTOR_ST_CON))
		//TIM4_Cmd(DISABLE);
		T4T3M &= 0x7f;                               //定时器停止
}




