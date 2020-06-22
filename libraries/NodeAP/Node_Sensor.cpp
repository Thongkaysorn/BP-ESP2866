#include "NodeAP.h"

/*--- Extern structure parameters -----------------------------------------*/
extern strSensor SensorVal;

int 	soil_hu_raw		= 0;

/*--- Read Temperature -------------------------------------------------------------------------------*/
void ReadAirTempHu()
{
	//SensorVal.air_temp = SHT21.getHumidity();
	//SensorVal.air_hu = SHT21.getTemperature();
/*
	while(isnan(SensorVal.air_temp) || isnan(SensorVal.air_hu))
	{
		//delay(1000);
		SensorVal.air_temp = dht.readTemperature();
		SensorVal.air_hu = dht.readHumidity();

		Serial.println("Failed to read from DHT sensor!");
	}
*/
	//dtostrf(SensorVal.air_temp, 3, 1, LogText.air_temp);
	//dtostrf(SensorVal.air_hu, 3, 1, LogText.air_hu);
}

/*--- Read Humidity ----------------------------------------------------------------------------------*/
void ReadSoilHumi()
{
	for(int i = 0; i < 10; i++)
	{
		soil_hu_raw += analogRead(A0);
		delay(50);
	}
	//SensorVal.soil_hu = soil_hu_raw;
	SensorVal.soil_hu = (soil_hu_raw / 100);
	SensorVal.soil_hu = map(SensorVal.soil_hu, 0, 100, 100, 0);
	//SensorVal.soil_hu = (SensorVal.soil_hu/10);

	//dtostrf(SensorVal.soil_hu, 3, 1, LogText.soil_hu);

	soil_hu_raw = 0;
}
