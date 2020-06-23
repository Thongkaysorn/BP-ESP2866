#include "NodeAP.h"

/*--- Mode parameter -----------------------------------------------------*/
extern uint8_t		Mode_Node;

/*--- Pin board design ---------------------------------------------------*/
uint8_t		SwPin 			= 16;
/*
uint8_t		ModePin1		= PIN_D6;
uint8_t		ModePin2		=	PIN_D7;
uint8_t		ModePin3		= PIN_D5;
*/
//uint8_t		ModeSet 		= 0;

uint16_t	Packet_Index = 50;
uint8_t		Sub_Packet_Index = 0;
uint8_t		Packet_Counter = 0;
bool			Unpack_done = false;

char URLweather[250];
char URLnode[250];
char URL_ReadNode[250];

Ticker blinker;		//Ticker timer
StructAddrConfig AddrConfig;

uint16_t LedStatusCounter = 0;

extern StructNodeConfig NodeConfig;

/*--- extern parameters from Node_Wifi -----------------------------------*/
extern char IPwifi[16];
extern char subnetwifi[16];
extern char gatewaywifi[16];
extern int  node_signal;
extern int  WifiTimeOut;
extern unsigned int localPort;
extern StructNodeConfig NodeConfig;
extern StructNodeParaString NodeParaString;

int httpCode;

String rcv=""; //Variable in which the server response is recorded.

HTTPClient http;

String payload;
String Packet_NodeConfig;
//String NodeConfig;
String HttpPacket;
strSensor SensorVal;

extern ESP8266WiFiMulti wifiMulti;

/*=== Node AP Initial =======================================================*/
void Node_AP_Init(void)
{
	delay(200);
	//pinMode(LED_BUILTIN, OUTPUT);
	pinMode(BOARD_LED, OUTPUT);
	PinSetup();
	Serial.begin(115200);		Serial.println("\n\n\n");
													Serial.println("----------- Welcome to Node AP -----------");
													Serial.println(">> Serial port initial         Done");
	Wire.begin();						Serial.println(">> I2C initial                 Done");
	EEPROM.begin(512);			Serial.println(">> EEPROM initial              Done");
	BH1750_initial();				Serial.println(">> Light sensor initial        Done");
	BH1750_Read();
	DHT11_initial();				Serial.println(">> Air Temp/Humi initial       Done");
	DHT11_Read();
	ReadSoilHumi();

	LoadNodeConfig();
	StartToConfig();				//Enter "conf" to config menu

	delay(50);

	Mode_Node = NodeConfig.NodeMode;

	if(WifiInit() == true)
	{
		Serial.println(">> Wifi initial : Done \n");
	}
	else
	{
		Serial.println("! Wifi initial : false");
	}

	delay(50);

	UrlPacking_log();
	UrlPacking_ReadNode();

	SQLweather();		Serial.println(">> SQL weather 1st connected                    OK\n");
	delay(50);
	SQLnode();			Serial.println(">> SQL node 1st connected                       OK\n");
	delay(50);

	//blinker.attach(1.0, TimerLoop); //Use <strong>attach_ms</strong> if you need time in ms
	blinker.attach_ms(1.0, TimerLoop); //Use <strong>attach_ms</strong> if you need time in ms
	NTP_loop();
	delay(50);
}

/*=== Send weather data to database ======================================*/
void Routine(void)
{
	Sec_5_Loop();
	Sec_10_Loop();
	Sec_30_Loop();
	Min_1_Loop();
	Min_5_Loop();
	Min_10_Loop();
	Min_30_Loop();
	Min_60_Loop();
	Sql_Loop(NodeConfig.SqlUpTime);
}

/*--- URL packing to send to sever ---------------------------------------*/
void UrlPacking_log(void)
{
	sprintf(URLweather, "http://192.168.1.111:81/weather.php?&node_id=%d&air_temp=%f&air_hu=%f&air_pressure=%f&soil_temp=%f&soil_hu=%f&light=%f&PH=%f&board_temp=%f&voltage=%f&current=%f&SW1=%d&SW2=%d", NodeConfig.NodeId, SensorVal.air_temp, SensorVal.air_hu, SensorVal.air_pressure, SensorVal.soil_temp, SensorVal.soil_hu, SensorVal.light, SensorVal.PH, SensorVal.board_temp, SensorVal.voltage, SensorVal.current, SensorVal.SW1, SensorVal.SW2);

	sprintf(URLnode, "http://192.168.1.111:81/node.php?&node_id=%d&node_mode=%d&lux_setup=%d&timer_setup=%d&time_on_setup_hh=%d&time_on_setup_mm=%d&time_off_setup_hh=%d&time_off_setup_mm=%d&soil_humi_dry=%d&sql_uptime=%d", NodeConfig.NodeId, NodeConfig.NodeMode, NodeConfig.LuxSetup, NodeConfig.TimerSetup, NodeConfig.TimeOnSetuphh, NodeConfig.TimeOnSetupmm, NodeConfig.TimeOffSetuphh, NodeConfig.TimeOffSetupmm, NodeConfig.SoilHumiDry, NodeConfig.SqlUpTime);

	Serial.println(URLweather);
	Serial.println(URLnode);
}

/*--- URL packing Node config --------------------------------------------*/
void UrlPacking_ReadNode(void)
{
	sprintf(URL_ReadNode, "http://192.168.1.111:81/read_sql.php?");
	Serial.println(URL_ReadNode);
}

/*--- Send data to database ------------------------------------------------*/
void SQLweather(void)
{
	if((wifiMulti.run() == WL_CONNECTED))
  	{
    	//HTTPClient http;
    	Serial.println("HTTPClient");
			Serial.println(URLweather);
    	http.begin(URLweather); //HTTP

    	httpCode = http.GET();

    if(httpCode > 0)
    {
      	Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      	if(httpCode == HTTP_CODE_OK)
      	{
        	payload = http.getString();
        	Serial.println(payload);
      	}
     }
     else
     {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
     }
    http.end();
   }
}

/* SQL node upload status --------------------------------------------------*/
void SQLnode(void)
{
	if((wifiMulti.run() == WL_CONNECTED))
  	{
    	//HTTPClient http;
    	Serial.println("HTTPClient");
    	Serial.println(URLnode);
    	http.begin(URLnode); //HTTP

    	httpCode = http.GET();

    if(httpCode > 0)
    {
      	Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      	if(httpCode == HTTP_CODE_OK)
      	{
        	payload = http.getString();
        	Serial.println(payload);
      	}
     }
     else
     {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
     }
    http.end();
   }
}

/*--- Switch pin setup ---------------------------------------------------*/
void PinSetup(void)
{
	pinMode(SwPin, OUTPUT);					digitalWrite(SwPin, LOW);
}

/*--- LED Status ----------------------------------------------------------*/
void LedWarning(uint16_t MsOn, uint16_t MsOff)
{
	if(LedStatusCounter <= MsOn)
	{
		digitalWrite(BOARD_LED, LOW);
	}
	else if(LedStatusCounter <= MsOff)
	{
		digitalWrite(BOARD_LED, HIGH);
	}
	else if(LedStatusCounter > MsOff)
	{
		LedStatusCounter = 0;
	}

	LedStatusCounter++;
}
