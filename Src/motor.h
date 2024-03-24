#ifndef __MOTOR_H
#define __MOTOR_H
#include "stc8Ax.h"


//电机的加速度
#define MOTORACCSPEED  1

typedef enum {
	MOTOR_ST_STOP   =((uint8_t)0x00),
	MOTOR_ST_ACC ,
	MOTOR_ST_CON,
	MOTOR_ST_DEC
}MOTOR_SPEED_STATUS;  //马达的当前状态



typedef enum {
	MOTOR_CCW_IDLE        =((uint8_t)0x00),
	MOTOR_CW       ,   //向前注射
	MOTOR_CCW         //后退
} MOTOR_DIRECTION;

typedef enum {
	MOTOR_STOP        =((uint8_t)0x00),
	MOTOR_FAST,
	MOTOR_NORMAL,
	MOTOR_SLOW,
	MOTOR_BACK,
	MOTOR_HANDMOVE
} MOTOR_SPEED;


typedef struct {
	MOTOR_DIRECTION MotorDir;    //马达运行的方向
	MOTOR_SPEED_STATUS MotorSpeedStatus;  //马达的速度状态
    uint16_t MotorRuningSpeed;   //马达当前运行的速度
    uint16_t Motor2Speed;     //马达达到运转的速度
    uint8_t  PreAUpdate;   //之前的A值更新变化
    uint8_t  PreBUpdate;   //之前的A值更新变化
    uint8_t  PreABStatus;   //之前的AB值
    //uint8_t  PreABStatus[8];   //之前的AB值
    //uint8_t  PreABstnum;   //之前的AB值bun
    uint16_t PositionABCounter; //电机的当前位置计数 
    uint16_t PrePositionABCounter; //电机的上一个当前位置计数 
    uint16_t Motor2MoveStep;  //马达要运行的步数
    uint8_t ABEncoder_Error;  //马达错误标志
}MOTOR_PARAM;


typedef enum{
	MOTOR_CMD_NULL   =((uint8_t)0x00),
	MOTOR_CMD_HSTOP,     
	MOTOR_CMD_SSTOP,
	MOTOR_CW_ONESTEP,
	MOTOR_CONT_CW, 
	MOTOR_CCW_ONESTEP,
	MOTOR_CONT_CCW, 
	MOTOR_MOVE_STEP  //运动距离
}MOTOR_CTRL_COMMAND;  //马达控制命令



extern MOTOR_PARAM xdata motor1Params;
extern MOTOR_CTRL_COMMAND motor1Cmd;

void Motor_init(void);
uint8_t MotorACCChangeSpSub(void);
void MotorMoveStep(MOTOR_DIRECTION ccw, MOTOR_SPEED speed, uint16_t MStep);

void Motor_HStop(void);
void Motor_SStop(void);
void Motor_Const_Move(MOTOR_DIRECTION ccw,MOTOR_SPEED Tospeed);


uint8_t ReadABOpticalEncoder(void);
//马达运动1步
uint8_t MotorMoveOneStep(MOTOR_DIRECTION ccw);
void ProcessMotorThread(void);

#endif 


