#include "NodeAP.h"

/*--- Mode parameter -----------------------------------------------------*/
uint16_t	Mode_TimerCounter	= 0;
uint8_t		Mode_Node					= 0;
uint8_t		NTP_Status			= 0;

extern strSensor SensorVal;
extern TimePara TimeDisco;

extern StructNodeConfig NodeConfig;
extern StructAddrConfig AddrConfig;

/*--- Counter -------------------------------------------------------------*/
uint8_t Sec_5_flag 		= 0;
uint8_t Sec_10_flag		= 0;
uint8_t Sec_30_flag		= 0;
uint8_t Min_1_flag		= 0;
uint8_t Min_5_flag		= 0;
uint8_t Min_10_flag		= 0;
uint8_t Min_30_flag		= 0;
uint8_t Min_60_flag		= 0;
uint8_t	Hr_12_flag		= 0;

uint8_t SqlUpTime_flag = 0;
uint8_t SqlUpTime_count = 0;

uint8_t LedStatus_flag = 0;

uint16_t MsCounter 		= 0;
uint16_t SecCounter 	= 0;

uint8_t		SwStatus = 0;
extern uint8_t	SwPin;

char ShowPara[250];

/*--- Time counter --------------------------------------------------------*/
void TimerLoop()
{
	MsCounter++;

	if((MsCounter % 1000) == 0)
	{
		SecCounter++;
		MsCounter = 0;

		if((SecCounter % 5) == 0) {
			Sec_5_flag = 1;
		}

		if((SecCounter % 10) == 0) {
			Sec_10_flag = 1;
		}

		if((SecCounter % 30) == 0) {
			Sec_30_flag = 1;
		}

		if((SecCounter % 60) ==0) {
			Min_1_flag = 1;
			SqlUpTime_flag = 1;
		}

		if((SecCounter % 300) ==0) {
			Min_5_flag = 1;
		}

		if((SecCounter % 600) ==0) {
			Min_10_flag = 1;
		}

		if((SecCounter % 1800) ==0) {
			Min_30_flag = 1;
		}

		if((SecCounter % 3600) ==0) {
			Min_60_flag = 1;
		}

		if((SecCounter % 43200) ==0) {
			Hr_12_flag = 1;
		}
	}

	if(TimeDisco.ms == 999)
	{
		if(TimeDisco.sec == 59)
		{
			if(TimeDisco.min == 59)
			{
				if(TimeDisco.hour == 23)
				{
					TimeDisco.hour = 0;
				}
				else
				{
					TimeDisco.hour++;
				}
				TimeDisco.min = 0;
			}
			else
			{
				TimeDisco.min++;
			}

			TimeDisco.sec = 0;
		}
		else
		{
			TimeDisco.sec++;
		}

		TimeDisco.ms = 0;
	}
	else
	{
		TimeDisco.ms++;
	}

	TimeDisco.Sum_min = (TimeDisco.hour * 60) + TimeDisco.min;

	SwDriver();

	if(LedStatus_flag == 1)
	{
		LedWarning(50, 950);
	}
	else
	{
		digitalWrite(BOARD_LED, HIGH);
	}
}

void Sql_Loop(uint8_t SqlTime)
{
	if(SqlUpTime_flag == 1)
	{
		if(SqlUpTime_count == SqlTime)
		{
				UrlPacking_log();
				SQLweather();

				SqlUpTime_count = 0;
		}

		SqlUpTime_count++;
	}

	SqlUpTime_flag = 0;
}

void Sec_5_Loop()
{
	if(Sec_5_flag == 1)
	{
		//ReadSensors();
		//Parameter_print();
		 //WriteSchedul();

		Sec_5_flag = 0;
	}
}

void Sec_10_Loop(void)
{
	if(Sec_10_flag == 1)
	{
		BH1750_Read();
		ReadSoilHumi();
		Parameter_print();
		Sec_10_flag = 0;
	}
}

void Sec_30_Loop(void)
{
	if(Sec_30_flag == 1)
	{
		//Serial.println("Sec_30_Loop");
		Sec_30_flag = 0;
	}
}

void Min_1_Loop(void)
{
	if(Min_1_flag == 1)
	{
		Mode_TimerCounter++;
		DHT11_Read();
		Min_1_flag = 0;
	}
}

void Min_5_Loop(void)
{
	if(Min_5_flag == 1)
	{
		if(NTP_loop() == 1)
		{
			NTP_Status = 1;
		}
		else
		{
			NTP_Status = 0;
		}
		//Serial.println("Min_5_Loop");
		Min_5_flag = 0;
	}
}

void Min_10_Loop(void)
{
	if(Min_10_flag == 1)
	{
		//Serial.println("Min_10_Loop");
  	//UrlPacking_log();
		//SQLweather();

		Min_10_flag = 0;
	}
}

void Min_30_Loop(void)
{
	if(Min_30_flag == 1)
	{
		//Serial.println("Min_30_Loop");
		Min_30_flag = 0;
	}
}

void Min_60_Loop(void)
{
	if(Min_60_flag == 1)
	{
		//Serial.println("Min_60_Loop");

		Min_60_flag = 0;
		//SecCounter = 0;
	}
}

void Hr_12_Loop(void)
{
	if(Hr_12_flag == 1)
	{
		//Serial.println("Hr_12_Loop");
		SQLnode();

		Hr_12_flag = 0;
		SecCounter = 0;
	}
}

/*--- Switch driver -------------------------------------------------------*/
void SwDriver(void)
{
	if(SwStatus == 1)
	{
		digitalWrite(SwPin, HIGH);
	}
	else if(SwStatus == 0)
	{
		digitalWrite(SwPin, LOW);
	}
}

void Parameter_print(void)
{
	TimeDisplay();
	Serial.print("Mode:");		Serial.print(NodeConfig.NodeMode);
	Serial.print("|Timer:");	Serial.print(Mode_TimerCounter);
	Serial.print("|ID:");			Serial.print(NodeConfig.NodeId);
	Serial.print("|AT:");			Serial.print(SensorVal.air_temp);
	Serial.print("|AH:");			Serial.print(SensorVal.air_hu);
	//Serial.print("|AP:");			Serial.print(SensorVal.air_pressure);
	Serial.print("|ST:");			Serial.print(SensorVal.soil_temp);
	Serial.print("|SH:");			Serial.print(SensorVal.soil_hu);
	Serial.print("|LT:");			Serial.print(SensorVal.light);
	//Serial.print("|PH:");			Serial.print(SensorVal.PH);
	//Serial.print("|IT:");			Serial.print(SensorVal.board_temp);
	Serial.print("|VB:");			Serial.print(SensorVal.voltage);
	Serial.print("|CB:");			Serial.print(SensorVal.current);
	Serial.print("|SW:");			Serial.print(SensorVal.SW1); //SwStatus
	Serial.println();

	sprintf(ShowPara, "Mode:%d|ID:%d|AT:%.1f|AH:%.1f|ST:%.1f|SH:%.1f|LT:%.1f|VB:%.1f|CB:%.1f|SW1=%d\n", NodeConfig.NodeMode, NodeConfig.NodeId, SensorVal.air_temp, SensorVal.air_hu, SensorVal.soil_temp, SensorVal.soil_hu, SensorVal.light, SensorVal.voltage, SensorVal.current, SensorVal.SW1);
}

void NodeReset()
{
	wdt_disable();
  wdt_enable(WDTO_15MS);
  while (1) {}
}
