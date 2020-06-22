#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266mDNS.h>
#include <Wire.h>
#include <SPI.h>
#include <EEPROM.h>
#include <DS3231.h>
#include <Ticker.h>

/*--- DNS ----------------------------------------------------------------*/
#include <ESP8266mDNS.h>
#include <WiFiClient.h>

/*--- NTP ----------------------------------------------------------------*/
#include <TimeLib.h>
#include <WiFiUdp.h>

#define MODE_SEFE					0		//Turn OFF all devices
#define MODE_TIMER				1		//Turn OFF when Time system coundown
#define MODE_LIGHT				2		//Active with light Lux
#define MODE_SCHE					3		//Active with schedul time setup
#define MODE_SCHE_LIGHT		4		//Active schedul on light
#define MODE_TIMER_LIGHT	5		//Active timer on light
#define MODE_SOIL_HU			6		//Active with soil humidity for light status

/*--- Pin mapping define --------------------------------------------------*/
#define I2C_SCL					5		//I2C
#define I2C_SDA					4		//I2C

#define PIN_D0					16		//Sw can used on devlop

#define BOARD_LED				D4	//Led status
#define DHTPIN					14	//Mode
#define PIN_D6					12	//Mode
#define PIN_D7					13	//Mode

#define PIN_A0					A0	//Input sens

//#define NO_SENS					0
//#define TIME_SENS				1
//#define LIGHT_SENS			2

/*--- Function ------------------------------------------------------------*/
void Mode_Process(void);								//Process in all of mode function
uint8_t Mode_Sefe(void);								//Back to sefe mode if have some problem
uint8_t Mode_Timer(uint16_t SetTimer);
uint8_t Mode_Light(uint16_t Light);
uint8_t Mode_Schedule(uint16_t TimeOn, uint16_t TimeOff);
uint8_t	Mode_SoilHumi(float SoilHumi, float SoilHumiDry);

void Parameter_print(void);

void Node_AP_Init(void);
void LedWarning(uint16_t MsOn, uint16_t MsOff);

void ReadAirTempHu(void);
void ReadSoilHumi(void);
void WifiStatus(void);

void InitRtc(void);
void RtcGetTime(void);

bool WifiInit(void);
void SQLweather(void);
void SQLnode(void);

void UrlPacking_log(void);
void UrlPacking_ReadNode(void);
void Routine(void);

//void SwitchDrive(uint8_t SW1_Status);
void SwDriver(void);

/*--- Timer Loop function -------------------------------------------------*/
void TimerLoop(void);
void Sec_5_Loop(void);
void Sec_10_Loop(void);
void Sec_30_Loop(void);
void Min_1_Loop(void);
void Min_5_Loop(void);
void Min_10_Loop(void);
void Min_30_Loop(void);
void Min_60_Loop(void);
void Hr_12_Loop(void);
void Sql_Loop(uint8_t SqlTime);

void PinSetup(void);
void NodeReset(void);

/*--- NTP ---------------------------------------------------------------*/
uint8_t NTP_loop(void);
time_t getNtpTime();
void NTP_SaveTime();
void printDigits(int digits);
void sendNTPpacket(IPAddress &address);
void TimeDisplay(void);

/*--- BH1750 -------------------------------------------------------------*/
void BH1750_initial(void);
void BH1750_Read(void);

void DHT11_initial(void);
void DHT11_Read(void);

/*--- Sensor sensitivity input -------------------------------------------*/
//uint16_t SensorSens(uint8_t SensPara);

void WriteSchedul(void);

void Read_IP_Addess(void);
void Write_IP_Address(void);

/*--- Node configuration function -----------------------------------------*/
void StartToConfig(void);
void PrintConfig(void);
String GetDataConf(void);
void LoadNodeConfig(void);
void FirstEEPROMwrite(void);

/*--- EEPROM --------------------------------------------------------------*/
void WriteEepromString(char addr,String data);
String ReadEepromString(char addr);

typedef struct
{
	int ms;
	int sec;
	int min;
	int hour;
	int day;
	int month;
	int year;
	int Sum_min;
}TimePara;

typedef struct
{
	String NodeName 		= "";
	String NodeId				= "";
	String NodeMode 		= "";
	String WifiSSID			= "";
	String WifiPassword = "";
	String SqlIP				= "";
	String WifiSSID2		= "";
	String WifiPassword2	= "";
	String LuxSetup 		= "";
	String TimerSetup	  = "";
	String TimeOnSetup	= "";
	String TimeOnSetuphh = "";
	String TimeOnSetupmm = "";
	String TimeOffSetup = "";
	String TimeOffSetuphh = "";
	String TimeOffSetupmm = "";
	String SoilHumiDry		= "";
	String SqlUpTime 			= "";
}StructNodeParaString;

typedef struct
{
	String NodeName 		= "nodename\n";
	String NodeId				= "nodeid\n";
	String NodeMode			= "nodemode\n";
	String WifiSSID			= "wifissid\n";
	String WifiPassword = "wifipassword\n";
	String SqlIP				= "SqlIP\n";
	String WifiSSID2   = "WifiSSID2\n";
	String WifiPassword2  = "WifiPassword2\n";
	String LuxSetup			= "luxsetup\n";
	String TimerSetup 	= "timersetup\n";
	String TimeOnSetup 	= "timeonsetup\n";
	String TimeOffSetup = "timeoffsetup\n";
	String SoilHumiDry	= "soilhumidry\n";
	String Save 	  		= "save\n";
	String Help 				= "help\n";
	String End					= "end\n";
	String Conf					= "conf\n";
	String Node					= "node\n";
	String SqlUpTime		= "sqluptime\n";
	String NodeReset		= "reset\n";
	String ModeInfo			= "modeinfo\n";
}StructNodeCommand;

typedef struct
{
	uint8_t NodeId 						= 0;
	uint8_t NodeMode 					= 0;
	uint16_t LuxSetup 				= 0;
	uint16_t TimerSetup 			= 0;
	uint8_t TimeOnSetuphh			= 0;
	uint8_t TimeOnSetupmm			= 0;
	uint16_t TimeOnSetupSum		= 0;
	uint8_t TimeOffSetuphh 		= 0;
	uint8_t TimeOffSetupmm		= 0;
	uint16_t TimeOffSetupSum 	= 0;
	uint8_t SchUpdate					= 0;
	uint8_t SoilHumiDry				= 0;
	uint8_t SqlUpTime 				= 0;
}StructNodeConfig;

/*--- EEPROM address -----------------------------------------------------*/
typedef struct
{
	uint8_t NodeId 					= 1;
	uint8_t	NodeMode 				= 2;
	uint8_t	LuxSetupHbyte 	= 3;
	uint8_t LuxSetupLbyte 	= 4;
	uint8_t TimerSetupHbyte	= 5;
	uint8_t TimerSetupLbyte = 6;
	uint8_t TimeOnSetuphh 	= 7;
	uint8_t TimeOnSetupmm 	= 8;
	uint8_t TimeOffSetuphh 	= 9;
	uint8_t TimeOffSetupmm 	= 10;
	uint8_t	SoilHumiDry			= 11;
	uint8_t SqlUpTime 			= 12;

	uint8_t NodeName 			= 100;
	uint8_t WifiSSID 			= 120;
	uint8_t WifiPassword  = 140;
	uint8_t SqlIP 				= 160;
	uint8_t WifiSSID2 		= 180;
	uint8_t WifiPassword2 	= 200;
}StructAddrConfig;

/*--- string parameter structure ------------------------------------------*/
typedef struct
{
	float air_temp			= 0;
	float air_hu				= 0;
	float air_pressure	= 0;
	float soil_temp			= 0;
	float soil_hu 			= 0;
	float light 				= 0;
	float PH						= 0;
	float flow_rate 		= 0;
	float voltage 			= 0;
	float current 			= 0;
	bool SW1 						= 0;
	bool SW2 						= 0;
	float board_temp		= 0;
}strSensor;
