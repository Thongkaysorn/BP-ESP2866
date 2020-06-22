#include "NodeAP.h"
#include <BH1750FVI.h>

extern strSensor SensorVal;

BH1750FVI LightSensor(BH1750FVI::k_DevModeContHighRes);

void BH1750_initial(void)
{
    LightSensor.begin();
}

void BH1750_Read(void)
{
    SensorVal.light = LightSensor.GetLightIntensity();
    SensorVal.light = SensorVal.light/1000;
    //Serial.print("Light: ");
    //Serial.println(SensorVal.light);
    //delay(250);
}
/*--- BH1750 Overview ---------------------------------------------------------------------------
  This example uses only 4 wires to test the BH1750 Light sensor

  Connecting the sensor to a NodeMCU ESP8266:
  VCC  <-> 3V3
  GND  <-> GND
  SDA  <-> D2
  SCL  <-> D1

      k_DevModeContHighRes     = 0x10,
      k_DevModeContHighRes2    = 0x11,
      k_DevModeContLowRes      = 0x13,
      k_DevModeOneTimeHighRes  = 0x20,
      k_DevModeOneTimeHighRes2 = 0x21,
      k_DevModeOneTimeLowRes   = 0x23

Each mode has three different precisions:

Low Resolution Mode - (4 lx precision, 16ms measurement time)
High Resolution Mode - (1 lx precision, 120ms measurement time)
High Resolution Mode 2 - (0.5 lx precision, 120ms measurement time)

Low Resolution Mode - (generic range: 0.0 up to 54612.5 lux)
High Resolution Mode - (generic range: 0.0 up to 54612.5 lux)
High Resolution Mode 2 - (generic range: 0.0 up to 27306.25 lux)

------------------------------------------------------------------------------------------------*/
