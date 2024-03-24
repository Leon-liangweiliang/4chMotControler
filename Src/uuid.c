#include "main.h"
#include "uuid.h"

#define UID_WRITE_FLASHADDRESS        0x7123   //ÉÕÐ´Æ÷·ÅÖÃµÄµØÖ·
#define UID_ROM_ADDR 				  0x7ff9


#define PWDXORBYTE  55
#define LEFTMOVBIT  1
#define ADDBYTE  33
#define RIGHTMOVBIT  1

uint8_t UUDIERROR=0;
uint8_t STC8A_ID[7];



//Get STM8S Unique ID

void Get_STC8A_UniqueID(void)
{ 
    uint8_t i=0;
	char *ID;
	 ID = (char idata *)0xf1;
    for(i=0; i<7; i++) 
        STC8A_ID[i] = ID[i]; 
} 

/*
void SaveUID2DataFlash(void)
{

	u8 i,j; 
	for (i=0;i<12;i+=2)
		{
		for (j=0;j<2;j++)
			{
			DATAFLSH_ID[i+j]=STM8S_ID[i+j]^DATAEEPPWD[j];
			FLASH_ProgramByte(UID_WRITE_FLASHADDRESS+i+j, DATAFLSH_ID[i+j]);
			 Wait until End of high voltage flag is set
		    while (FLASH_GetFlagStatus(FLASH_FLAG_HVOFF) == RESET);
			}
		}
}
*/


uint8_t CmpDataFlashUuid(void)
{
	uint8_t i;
	uint8_t * IDWrFlash;
	uint8_t tempv;
	Get_STC8A_UniqueID();
	IDWrFlash=(uint8_t code *) UID_WRITE_FLASHADDRESS;
	for (i=0;i<7;i++)
		{
			tempv=(STC8A_ID[i]^PWDXORBYTE);
			tempv=tempv<<LEFTMOVBIT;
			tempv=tempv+ADDBYTE;
			tempv=tempv>>RIGHTMOVBIT;
			
		if (IDWrFlash[i]!=tempv)
			{
			UUDIERROR=1;
			return 0;
			}
		}
	UUDIERROR=0;
	return 1;
}


#if 0
void SaveUID2AT24C02(void)
{
	u8 i,j;
	for (i=0;i<12;i+=4)
		{
		for (j=0;j<4;j++)
		EEP_ID[i+j]=STM8S_ID[i+j]^AT24EEPPWD[j];
		}
	
	ISendStr( UID_WRITE_EEPADDRESS, EEP_ID,12); 
}

u8 Cmp24c02Flash(void)
{
	u8 i,j;
	IRcvStr( UID_WRITE_EEPADDRESS, EEP_ID,12);
	
	for (i=0;i<12;i+=4)
		{
		for (j=0;j<4;j++)
			{
			if (EEP_ID[i+j]!=(STM8S_ID[i+j]^AT24EEPPWD[j])) return 0;
			}
		}
	return 1;
}

#define ERRORUSETIMES 20

u8 GetDownCounter(void)
{
	u8 losttemp=ERRORUSETIMES;
	if (Cmp24c02Flash()==0)
		{
		AT24UUDIERROR=1;
		losttemp=FLASH_ReadByte(UID_WRITE_FLASHADDRESS+123);
		if (losttemp>ERRORUSETIMES) losttemp=ERRORUSETIMES;
		else if (losttemp>0) losttemp--;
		FLASH_ProgramByte(UID_WRITE_FLASHADDRESS+123, losttemp);
		/* Wait until End of high voltage flag is set*/
		while (FLASH_GetFlagStatus(FLASH_FLAG_HVOFF) == RESET);
		}
	else 
		{	
		if (FLASH_ReadByte(UID_WRITE_FLASHADDRESS+123)!=ERRORUSETIMES)
			{
			losttemp=ERRORUSETIMES;
			FLASH_ProgramByte(UID_WRITE_FLASHADDRESS+123, losttemp);
			/* Wait until End of high voltage flag is set*/
			while (FLASH_GetFlagStatus(FLASH_FLAG_HVOFF) == RESET);
			}
		}
	return losttemp;
}




void CaculatedAuthorizationCode(u8 *Startdate,u8* FinishDate,u8* ACode)
{
	u16 UseDates=0;
	u8 UseDateNum[3];
	u16 SYear,FYear;
	u8 SMonth,SDate,FMonth,FDate;
	SYear=(Startdate[0]-'0')*1000;
	SYear+=(Startdate[1]-'0')*100;
	SYear+=(Startdate[2]-'0')*10;
	SYear+=(Startdate[3]-'0');
	
	SMonth=(Startdate[5]-'0')*10;
	SMonth+=(Startdate[6]-'0');

	SDate=(Startdate[8]-'0')*10;
	SDate+=(Startdate[9]-'0');

	FYear=(FinishDate[0]-'0')*1000;
	FYear+=(FinishDate[1]-'0')*100;
	FYear+=(FinishDate[2]-'0')*10;
	FYear+=(FinishDate[3]-'0');


	FMonth=(FinishDate[5]-'0')*10;
	FMonth+=(FinishDate[6]-'0');

	FDate=(FinishDate[8]-'0')*10;
	FDate+=(FinishDate[9]-'0');

	while (SYear<FYear)
	{
		while (SMonth<=12)
		{
			if (SMonth==2 )
			{
				if (SYear%4==0) UseDates+=(29-SDate)+1; else  UseDates+=(28-SDate)+1;
				
			}
			else if (SMonth==1|| SMonth==3 || SMonth==5 || SMonth==7 ||SMonth==8 ||SMonth==10 || SMonth==12)
			{
				UseDates+=(31-SDate)+1;
			}
			else 
			{
				UseDates+=(30-SDate)+1;
			}
			SMonth++;
			SDate=1;
		}
		SMonth=1;
		SYear++;
	}

	while (SMonth<FMonth)
		{
			if (SMonth==2 )
			{
				if (SYear%4==0) UseDates+=(29-SDate)+1; else  UseDates+=(28-SDate)+1;
				
			}
			else if (SMonth==1|| SMonth==3 || SMonth==5 || SMonth==7 ||SMonth==8 ||SMonth==10 || SMonth==12)
			{
				UseDates+=(31-SDate)+1;
			}
			else 
			{
				UseDates+=(30-SDate)+1;
			}
			SMonth++;
			SDate=1;
		}
	
	UseDates+=(FDate-SDate)+1;

	UseDateNum[0]=UseDates/100%10;
	UseDateNum[1]=UseDates/10%10;
	UseDateNum[2]=UseDates%10;

	ACode[0]=((Startdate[0]-'0') ^ 7)+'0';
	ACode[1]=((Startdate[1]-'0') ^ UseDateNum[2])+'0';
	ACode[2]=((Startdate[2]-'0') ^ UseDateNum[1])+'0';
	ACode[3]=((Startdate[3]-'0') ^ UseDateNum[0])+'0';
	ACode[4]=((Startdate[5]-'0') ^ (Startdate[9]-'0')) +'0' ;
	ACode[5]=((Startdate[6]-'0') ^ (Startdate[8]-'0')) +'0';
	
}

u8 CmpFinishDate(u8 *FinishDate , u8* NowDate)
{
	u16 FYear,NYear;
	u8 FMonth,FDate,NMonth,NDate;

	FYear=(FinishDate[0]-'0')*1000;
	FYear+=(FinishDate[1]-'0')*100;
	FYear+=(FinishDate[2]-'0')*10;
	FYear+=(FinishDate[3]-'0');


	FMonth=(FinishDate[5]-'0')*10;
	FMonth+=(FinishDate[6]-'0');

	FDate=(FinishDate[8]-'0')*10;
	FDate+=(FinishDate[9]-'0');

	NYear=2000+ NowDate[0];
	NMonth=NowDate[1];
	NDate=NowDate[2];

	if (NYear>FYear)  return 0;
	if ((FYear==NYear) && (NMonth>FMonth)) return 0;
	if ((FYear==NYear) && (FMonth==NMonth) && (NDate>FDate)) return 0;
	return 1;
}
#endif

