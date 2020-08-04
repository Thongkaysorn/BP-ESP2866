#include <NodeAP.h>
#include "thingProperties.h"

extern strSensor SensorVal;
extern StructAddrConfig AddrConfig;
extern StructNodeConfig NodeConfig;
extern char ShowPara[256];

void setup()
{
  Node_AP_Init();

    // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  
  /*
     The following function allows you to obtain more information
     related to the state of network and IoT Cloud connection and errors
     the higher number the more granular information you’ll get.
     The default is 0 (only errors).
     Maximum is 4
 */
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

}

void loop()
{
  StartToConfig();        //Enter "conf" to config menu
  Routine();
  //WebServerLoop();
  Mode_Process();

  ArduinoCloud.update();
  onAirTempChange();
  onAirHumiChange();
  onSoilHumiChange();
  onLightChange();
  onStatusChange();
}

/*--- ArduinoIoTCloud function ------------------------------*/

void onAirTempChange() {
  airTemp = SensorVal.air_temp;
}

void onAirHumiChange() {
  airHumi = SensorVal.air_hu;
}

void onSoilHumiChange()
{
  soilHumi = SensorVal.soil_hu;
}

void onLightChange()
{
  light = SensorVal.light;
}

void onSwitch1Change(){
}

void onTextChange() {
}

void onStatusChange() {
  status = ShowPara;
}

void onSoilHumiDryChange() {
  NodeConfig.SoilHumiDry = soilHumiDry;
  Serial.println(NodeConfig.SoilHumiDry);

  EEPROM.write(AddrConfig.SoilHumiDry, NodeConfig.SoilHumiDry);
  EEPROM.commit();
}
