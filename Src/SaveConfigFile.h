#ifndef _SAVECONFIGFILE_
#define _SAVECONFIGFILE_
	

typedef struct {
	u8 DriverType; //0X10   //0 hand,  1 stepmotor
	u8 TOSelectedLed;
	u8 LedOnOff;  
	u8 LedPower[5]; //0X13---0X17
	u8 LedLocation[5];   //0X18---0X1c
	u8 MotorStepPluse;
	u8 DriverFanOnOff;
	u8 PurpleLamp;
}Run_PARAMS;



typedef struct {
	u8 Saveed;
	u8 DriverType; //0X10   //0 hand,  1 stepmotor
	u8 SelectedLedChanel;
	u8 LedOnOff;
	u8 LedPower[5]; //0X13---0X17
	u8 LEDMotorLocation[5];  //0X18---0X1c
	u8 MotorStepPluses;
	u8 DriverFanOnOff;
	u8 LedALLChanels;
	u8 PurpleLamp;
}SAVEDATAPARAMS;


/*

typedef struct {
	u8 RedLedMax;
	u8 RedLedMin;
	u8 GreenLedMax;
	u8 GreenLedMin;
	u8 BlueLedMax;
	u8 BlueLedMin;
	u8 UVLedMax;
	u8 UvLedMin;
	u8 BtPower;
	u8 FanSpeed1;
	u8 FanSpeedTempsure1;
	u8 FanSpeedTempsure2;
	u8 FanSpeedTempsure3;
	u8 FanSpeed2;
	u8 FanSpeed3;
}SAVEEEPROMDATA;

typedef struct{	
	u8 StartDate[16];
	u8 FinishDate[16];
	u8 AuthorizationCode[16];
}SAVEEEPROMACODE;
*/
/*
typedef struct{
	u32 RedUseHours;
	u32 GreenUseHours;
	u32 BlueUseHours;
	u32 UVUseHours;
	u8 RedUseMinutes;
	u8 GreenUseMinutes;
	u8 BlueUseMinutes;
	u8 UVUseMinutes;
} LEDUSEDTIMES;
*/

//extern SAVEEEPROMACODE SaveeepromCode;
//extern SAVEEEPROMDATA SaveeepromData;
extern SAVEDATAPARAMS SaveLedData0;
//extern LEDUSEDTIMES ledusedtimes;

//	void WriteAuthorizationCode(void);
//	void ReadAuthorizationCode(void);
	
	void WriteEEpromSaveData(void);
	void ReadEEpromSaveData(void);
	
//	void WriteSaveCWData2Data0_12(u8 DataCh);
//	void ReadCWDataFData0_12(u8 DataCh);
//	u8 GetData0_12Used(u8 DataCh);

//	void WirteLEDUsedHours(void);
//	void WirteLEDUsedMinutes(void);
//	void ReadLEDUsedTime(void);
	
#endif

