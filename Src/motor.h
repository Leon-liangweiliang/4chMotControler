#ifndef __MOTOR_H
#define __MOTOR_H
#include "stc8Ax.h"


//����ļ��ٶ�
#define MOTORACCSPEED  1

typedef enum {
	MOTOR_ST_STOP   =((uint8_t)0x00),
	MOTOR_ST_ACC ,
	MOTOR_ST_CON,
	MOTOR_ST_DEC
}MOTOR_SPEED_STATUS;  //���ĵ�ǰ״̬



typedef enum {
	MOTOR_CCW_IDLE        =((uint8_t)0x00),
	MOTOR_CW       ,   //��ǰע��
	MOTOR_CCW         //����
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
	MOTOR_DIRECTION MotorDir;    //������еķ���
	MOTOR_SPEED_STATUS MotorSpeedStatus;  //�����ٶ�״̬
    uint16_t MotorRuningSpeed;   //��ﵱǰ���е��ٶ�
    uint16_t Motor2Speed;     //���ﵽ��ת���ٶ�
    uint8_t  PreAUpdate;   //֮ǰ��Aֵ���±仯
    uint8_t  PreBUpdate;   //֮ǰ��Aֵ���±仯
    uint8_t  PreABStatus;   //֮ǰ��ABֵ
    //uint8_t  PreABStatus[8];   //֮ǰ��ABֵ
    //uint8_t  PreABstnum;   //֮ǰ��ABֵbun
    uint16_t PositionABCounter; //����ĵ�ǰλ�ü��� 
    uint16_t PrePositionABCounter; //�������һ����ǰλ�ü��� 
    uint16_t Motor2MoveStep;  //���Ҫ���еĲ���
    uint8_t ABEncoder_Error;  //�������־
}MOTOR_PARAM;


typedef enum{
	MOTOR_CMD_NULL   =((uint8_t)0x00),
	MOTOR_CMD_HSTOP,     
	MOTOR_CMD_SSTOP,
	MOTOR_CW_ONESTEP,
	MOTOR_CONT_CW, 
	MOTOR_CCW_ONESTEP,
	MOTOR_CONT_CCW, 
	MOTOR_MOVE_STEP  //�˶�����
}MOTOR_CTRL_COMMAND;  //����������



extern MOTOR_PARAM xdata motor1Params;
extern MOTOR_CTRL_COMMAND motor1Cmd;

void Motor_init(void);
uint8_t MotorACCChangeSpSub(void);
void MotorMoveStep(MOTOR_DIRECTION ccw, MOTOR_SPEED speed, uint16_t MStep);

void Motor_HStop(void);
void Motor_SStop(void);
void Motor_Const_Move(MOTOR_DIRECTION ccw,MOTOR_SPEED Tospeed);


uint8_t ReadABOpticalEncoder(void);
//����˶�1��
uint8_t MotorMoveOneStep(MOTOR_DIRECTION ccw);
void ProcessMotorThread(void);

#endif 


