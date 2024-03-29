#include "main.h"
#include "eeprom.h"
#include "SaveConfigfile.h"

//SAVEEEPROMACODE SaveeepromCode;
//SAVEEEPROMDATA SaveeepromData;
SAVEDATAPARAMS SaveLedData0;
//LEDUSEDTIMES ledusedtimes;



void ResetSaveData(void)
{
	SaveLedData0.Saveed=1;
	SaveLedData0.LedOnOff=0;
	SaveLedData0.LedALLChanels=4;
	SaveLedData0.DriverType=1;
	SaveLedData0.LedPower[0]=1;
	SaveLedData0.LedPower[1]=1;
	SaveLedData0.LedPower[2]=1;
	SaveLedData0.LedPower[3]=1;
	SaveLedData0.LedPower[4]=1;
	SaveLedData0.SelectedLedChanel=1;
	SaveLedData0.LEDMotorLocation[0]=0;
	SaveLedData0.LEDMotorLocation[1]=0;
	SaveLedData0.LEDMotorLocation[2]=25;
	SaveLedData0.LEDMotorLocation[3]=50;
	SaveLedData0.LEDMotorLocation[4]=75;
	SaveLedData0.MotorStepPluses=90;
	SaveLedData0.DriverFanOnOff=1;	
	SaveLedData0.PurpleLamp=1;
}


void WriteEEpromSaveData(void)
{
	IapErase(0);
	IapProgramArea(0,(u8*)(&SaveLedData0),sizeof(SAVEDATAPARAMS));
}

void ReadEEpromSaveData(void)
{
	IapReadArea(0, (u8*)(&SaveLedData0), sizeof(SAVEDATAPARAMS));

	if (SaveLedData0.SelectedLedChanel>5 ||
		SaveLedData0.LedOnOff>1   ||
		SaveLedData0.DriverType>1 ||
		SaveLedData0.DriverFanOnOff>1)
		{
		ResetSaveData();
		WriteEEpromSaveData();
		}
}


