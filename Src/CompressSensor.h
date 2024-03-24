#ifndef _COMPRESSSENSOR_H_
#define _COMPRESSSENSOR_H_

#define THROATHPRES 450
#define THROATHPRES_test 700

	extern uint8_t PressProSt,PressIsArrive;
	extern volatile uint16_t  ReleaseCompressV;

	void SensorInit(void);
	void ReadReleaseCompressV(void);
	void ControlCompress(void);
	void CtrlCompress(uint8_t ct);
	void Tim3TimeroutSub(void);
	void TIM3_Init(void);
#endif

