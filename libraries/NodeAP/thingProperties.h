#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

#define SECRET_SSID "BaanPloyK12"
#define SECRET_PASS "Ploii087"
#define SECRET_DEVICE_KEY "WTWSL2RXKMNBELWNBYVO"

const char THING_ID[]           = "e870dc75-8e32-4f6a-8589-9d57e22f3fce";
const char DEVICE_LOGIN_NAME[]  = "da303923-f207-4963-a4b7-4f13abb403f8";

const char SSID[]               = SECRET_SSID;    // Network SSID (name)
const char PASS[]               = SECRET_PASS;    // Network password (use for WPA, or use as key for WEP)
const char DEVICE_KEY[]  = SECRET_DEVICE_KEY;    // Secret device password

void onStatusChange();
void onSoilHumiDryChange();
void onSwitch1Change();

String status;
CloudTemperature airTemp;
int soilHumiDry;
float light;
float airHumi;
float soilHumi;
bool switch1;

void initProperties(){

  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);
  ArduinoCloud.setThingId(THING_ID);
  ArduinoCloud.addProperty(status, READWRITE, ON_CHANGE, onStatusChange);
  ArduinoCloud.addProperty(airTemp, READ, 60 * SECONDS, NULL);
  ArduinoCloud.addProperty(soilHumiDry, READWRITE, ON_CHANGE, onSoilHumiDryChange);
  ArduinoCloud.addProperty(light, READ, 60 * SECONDS, NULL);
  ArduinoCloud.addProperty(airHumi, READ, 60 * SECONDS, NULL);
  ArduinoCloud.addProperty(soilHumi, READ, 60 * SECONDS, NULL);
  ArduinoCloud.addProperty(switch1, READWRITE, ON_CHANGE, onSwitch1Change);

}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);
