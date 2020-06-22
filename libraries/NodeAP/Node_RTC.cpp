#include "NodeAP.h"

char 			tmeString[] = "00000000,00:00:00";
int 			timer1_counter;

const int chipSelect = 15;
uint8_t	NewFileTimeOut	=	600;

DS3231			clock_rtc;
RTCDateTime		dt;
extern strSensor SensorVal;

/*--- RTC get time ---------------------------------------------------------*/
void RtcGetTime(void)
{
	dt = clock_rtc.getDateTime();
	/*
	Serial.print("DT:");
	Serial.print(dt.year);   Serial.print("-");
	Serial.print(dt.month);  Serial.print("-");
	Serial.print(dt.day);    Serial.print(" ");
	Serial.print(dt.hour);   Serial.print(":");
	Serial.print(dt.minute); Serial.print(":");
	Serial.print(dt.second); Serial.print(",IT");
	*/
	//clock_rtc.forceConversion();


	sprintf(tmeString, "%04d%02d%02d,%02d:%02d:%02d", dt.year, dt.month, dt.day, dt.hour, dt.minute, dt.second);// [added seconds]
	Serial.print(tmeString);

	/*--- Function for create log fileneme -----------------------------------*/
	/*
	if(FlagFirstFile == 0)
	{
		sprintf(FileName, "%02d%02d%02d%02d.csv", dt.month, dt.day, dt.hour, dt.minute);
		FlagFirstFile = 1;
	}

	if((dt.hour == 0) && (dt.minute == 0))
	{
		sprintf(FileName, "%02d%02d%02d%02d.csv", dt.month, dt.day, dt.hour, dt.minute);
		FlagNewFile++;
	}
	*/
}
