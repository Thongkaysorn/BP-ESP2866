#include "NodeAP.h"

/*--- Wifi -------------------------------------------------------------------------------------------*/
ESP8266WiFiMulti wifiMulti;
WiFiUDP Udp;

/*--- Wifi parameters ------------------------------------------------------*/
char IPwifi[16];
char subnetwifi[16];
char gatewaywifi[16];
char SSIDwifi[16];

int  node_signal	= 0;
int  WifiTimeOut	= 0;
unsigned int localPort	= 8888; //local port to listen for UDP packets

/*--- Wifi initial -----------------------------------------------------------------------------------*/
bool WifiInit(void)
{
		node_signal = 0;
		WifiTimeOut = 0;
		localPort 	= 8888; 									//local port to listen for UDP packets

		wifiMulti.addAP("BANN_PLOII", "Ploii79318");
  	wifiMulti.addAP("APRT", "AstonmartinDB9");
  	wifiMulti.addAP("EM-APRT", "Astonmartin");
		wifiMulti.addAP("phareuhas2G_", "27092522");

    // Static IP details...
    /*
    IPAddress ip(192, 168, 1, 45);
    IPAddress gateway(192, 168, 1, 1);
    IPAddress subnet(255, 255, 255, 0);
    IPAddress dns(8, 8, 8, 8);
    */

    // Static IP Setup Info Here...
    //WiFi.config(ip, gateway, subnet, dns); //If you need Internet Access You should Add DNS also..
	//WiFi.hostname("hostname");

  	Serial.println("Waiting for Node connecting to wifi");

  	while ((wifiMulti.run() != WL_CONNECTED) && (WifiTimeOut < 10))
  	{
    	Serial.print(".");
    	delay(500);
    	WifiTimeOut++;
  	}

  	Serial.println("Connected to wifi");
  	sprintf(IPwifi, "%d.%d.%d.%d", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3]);
  	sprintf(subnetwifi, "%d.%d.%d.%d", WiFi.subnetMask()[0], WiFi.subnetMask()[1], WiFi.subnetMask()[2], WiFi.subnetMask()[3]);
  	sprintf(gatewaywifi, "%d.%d.%d.%d", WiFi.gatewayIP()[0], WiFi.gatewayIP()[1], WiFi.gatewayIP()[2], WiFi.gatewayIP()[3]);

  	Serial.print("Status: "); 	Serial.println(WiFi.status());    // Network parameters
  	Serial.print("IP: ");     	Serial.println(IPwifi); 			//Serial.println(WiFi.localIP());
  	Serial.print("Subnet: "); 	Serial.println(subnetwifi);		//Serial.println(WiFi.subnetMask());
  	Serial.print("Gateway: "); 	Serial.println(gatewaywifi);

  	Serial.print("SSID: "); 	Serial.println(WiFi.SSID());
  	Serial.print("Signal: "); 	Serial.println(WiFi.RSSI());
  	//Serial.println(WiFi.hostname());

  	Serial.print("IP number assigned by DHCP is ");
  	Serial.println(WiFi.localIP());

  	Serial.println("Starting UDP");
  	Udp.begin(localPort);
  	Serial.print("Local port: ");
  	Serial.println(Udp.localPort());
  	Serial.println("waiting for sync");
  	setSyncProvider(getNtpTime);
  	setSyncInterval(300);

  	//WebServerSetup();
		return true;
}

void WifiStatus()
{
	node_signal = WiFi.RSSI();
}
