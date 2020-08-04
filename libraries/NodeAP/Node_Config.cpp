#include "NodeAP.h"

StructNodeParaString	NodeParaString;
StructNodeCommand 		NodeCommand;
StructNodeConfig 			NodeConfig;
extern StructAddrConfig AddrConfig;

uint8_t	GetDataFlag = 0;
uint8_t	EndConfigFlag = 0;
String StringTemp = "";

void LoadNodeConfig()
{
	/*--- First write EEPROM before record config --------------------------*/
	FirstEEPROMwrite();

	NodeConfig.NodeId		= EEPROM.read(AddrConfig.NodeId);
	NodeConfig.NodeMode = EEPROM.read(AddrConfig.NodeMode);

	NodeConfig.LuxSetup = EEPROM.read(AddrConfig.LuxSetupHbyte);
	NodeConfig.LuxSetup = NodeConfig.LuxSetup << 8;
	NodeConfig.LuxSetup = NodeConfig.LuxSetup | EEPROM.read(AddrConfig.LuxSetupLbyte);

	NodeConfig.TimerSetup = EEPROM.read(AddrConfig.TimerSetupHbyte);
	NodeConfig.TimerSetup = NodeConfig.TimerSetup << 8;
	NodeConfig.TimerSetup = NodeConfig.TimerSetup | EEPROM.read(AddrConfig.TimerSetupLbyte);

	NodeConfig.TimeOnSetuphh = EEPROM.read(AddrConfig.TimeOnSetuphh);
	NodeConfig.TimeOnSetupmm = EEPROM.read(AddrConfig.TimeOnSetupmm);
	NodeConfig.TimeOnSetupSum = ((NodeConfig.TimeOnSetuphh *60) + NodeConfig.TimeOnSetupmm);

	NodeConfig.TimeOffSetuphh = EEPROM.read(AddrConfig.TimeOffSetuphh);
	NodeConfig.TimeOffSetupmm = EEPROM.read(AddrConfig.TimeOffSetupmm);
	NodeConfig.TimeOffSetupSum = ((NodeConfig.TimeOffSetuphh *60) + NodeConfig.TimeOffSetupmm);

	NodeParaString.NodeName 		= ReadEepromString(AddrConfig.NodeName);
	NodeParaString.WifiSSID 		= ReadEepromString(AddrConfig.WifiSSID);
	NodeParaString.WifiPassword = ReadEepromString(AddrConfig.WifiPassword);
	NodeParaString.SqlIP 			= ReadEepromString(AddrConfig.SqlIP);
	NodeParaString.WifiSSID2 	= ReadEepromString(AddrConfig.WifiSSID2);
	NodeParaString.WifiPassword2 	= ReadEepromString(AddrConfig.WifiPassword2);

	NodeConfig.SoilHumiDry = EEPROM.read(AddrConfig.SoilHumiDry);
	NodeConfig.SqlUpTime	 = EEPROM.read(AddrConfig.SqlUpTime);

	PrintConfig();
}

void PrintConfig()
{
	Serial.print("\n----- Load configuration from EEPROM -----");
	Serial.print("\nNodeId : ");			Serial.print(NodeConfig.NodeId);
	Serial.print("\nNodeMode : ");		Serial.print(NodeConfig.NodeMode);
	Serial.print("\nLuxSetup : ");		Serial.print(NodeConfig.LuxSetup);
	Serial.print("\nTimerSetup : ");	Serial.print(NodeConfig.TimerSetup);
	Serial.print("\nTimeOnSetup : "); Serial.print(NodeConfig.TimeOnSetuphh);
	Serial.print(".");								Serial.print(NodeConfig.TimeOnSetupmm);
	Serial.print("\nTimeOffSetup : "); Serial.print(NodeConfig.TimeOffSetuphh);
	Serial.print(".");								Serial.print(NodeConfig.TimeOffSetupmm);
	Serial.print("\nSoil Humidiry setup : "); Serial.print(NodeConfig.SoilHumiDry);
	Serial.print("\nSqlUpTime : "); 	Serial.print(NodeConfig.SqlUpTime);

	Serial.print("\nNodeName : ");		Serial.print(NodeParaString.NodeName);
	Serial.print("WifiSSID : ");		Serial.print(NodeParaString.WifiSSID);
	Serial.print("WifiPassword : ");		Serial.print(NodeParaString.WifiPassword);
	Serial.print("SqlIP : ");			Serial.print(NodeParaString.SqlIP);
	Serial.print("WifiSSID2 : ");	Serial.print(NodeParaString.WifiSSID2);
	Serial.print("WifiPassword2 : ");	Serial.print(NodeParaString.WifiPassword2);
	Serial.print("--------------------------------------------");
	Serial.println();
}

String GetDataConf()
{
	String StringGetData = "";
	String StringReturn = "";

	while (GetDataFlag == 0) {
		while(Serial.available() > 0){
			StringGetData = Serial.readString();
			Serial.print(StringGetData);
		}
		if (StringGetData.substring(0) == NodeCommand.Save) {
			GetDataFlag = 1;
		}
		else{
			StringReturn = StringGetData;
		}
	}
	GetDataFlag = 0;
	return StringReturn;
}

void StartToConfig()
{
	while(Serial.available() > 0 )
	{
		String str = Serial.readString();

		if(str.substring(0) == NodeCommand.Conf)
		{
			Serial.println("\n\n>>Configuration menu");
			Serial.println("Enter command menu <Enter help for menu command list>");

			while (EndConfigFlag == 0)
			{
				while (Serial.available() > 0)
				{
					StringTemp = Serial.readString();

					if (StringTemp.substring(0) == NodeCommand.NodeName)
					{
						Serial.print("Enter node name / Enter save for record and exit\n");
						NodeParaString.NodeName = GetDataConf();
						Serial.print("Node name is : ");
						Serial.print(NodeParaString.NodeName);

						WriteEepromString(AddrConfig.NodeName,NodeParaString.NodeName);
					}
					else if (StringTemp.substring(0) == NodeCommand.NodeId)
					{
						Serial.print("Enter node ID / Enter save for record and exit\n");
						NodeParaString.NodeId = GetDataConf();
						NodeConfig.NodeId = (NodeParaString.NodeId).toInt();
						Serial.print("Node ID is : ");
						Serial.println(NodeConfig.NodeId);

						EEPROM.write(AddrConfig.NodeId, NodeConfig.NodeId);
						EEPROM.commit();
					}
					else if (StringTemp.substring(0) == NodeCommand.NodeMode)
					{
						Serial.print("Enter Node Mode / Enter save for record and exit\n");
						NodeParaString.NodeMode = GetDataConf();
						NodeConfig.NodeMode = (NodeParaString.NodeMode).toInt();
						Serial.print("Node Mode is : ");
						Serial.println(NodeConfig.NodeMode);

						EEPROM.write(AddrConfig.NodeMode, NodeConfig.NodeMode);
						EEPROM.commit();
					}
					else if (StringTemp.substring(0) == NodeCommand.WifiSSID)
					{
						Serial.print("Enter Wifi SSID / Enter save for record and exit\n");
						NodeParaString.WifiSSID = GetDataConf();
						Serial.print("Wifi SSID is : ");
						Serial.print(NodeParaString.WifiSSID);

						WriteEepromString(AddrConfig.WifiSSID,NodeParaString.WifiSSID);
					}
					else if (StringTemp.substring(0) == NodeCommand.WifiPassword)
					{
						Serial.print("Enter Wifi Password / Enter save for record and exit\n");
						NodeParaString.WifiPassword = GetDataConf();
						Serial.print("Wifi Password is : ");
						Serial.print(NodeParaString.WifiPassword);

						WriteEepromString(AddrConfig.WifiPassword,NodeParaString.WifiPassword);
					}
					else if (StringTemp.substring(0) == NodeCommand.SqlIP) {
						Serial.print("Enter Wifi IP / Enter save for record and exit\n");
						NodeParaString.SqlIP = GetDataConf();
						Serial.print("Wifi IP is : ");
						Serial.print(NodeParaString.SqlIP);

						WriteEepromString(AddrConfig.SqlIP,NodeParaString.SqlIP);
					}
					else if (StringTemp.substring(0) == NodeCommand.WifiSSID2)
					{
						Serial.print("Enter Wifi Subnet / Enter save for record and exit\n");
						NodeParaString.WifiSSID2 = GetDataConf();
						Serial.print("Wifi Subnet is : ");
						Serial.print(NodeParaString.WifiSSID2);

						WriteEepromString(AddrConfig.WifiSSID2,NodeParaString.WifiSSID2);
					}
					else if (StringTemp.substring(0) == NodeCommand.WifiPassword2)
					{
						Serial.print("Enter Wifi Gateway / Enter save for record and exit\n");
						NodeParaString.WifiPassword2 = GetDataConf();
						Serial.print("Wifi Gateway is : ");
						Serial.print(NodeParaString.WifiPassword2);

						WriteEepromString(AddrConfig.WifiPassword2,NodeParaString.WifiPassword2);
					}
					else if (StringTemp.substring(0) == NodeCommand.LuxSetup)
					{
						Serial.print("Enter Lux Setup / Enter save for record and exit\n");
						NodeParaString.LuxSetup = GetDataConf();
						NodeConfig.LuxSetup = (NodeParaString.LuxSetup).toInt();
						Serial.print("Lux Setup is : ");
						Serial.print(NodeConfig.LuxSetup);

						EEPROM.write(AddrConfig.LuxSetupHbyte, (NodeConfig.LuxSetup >> 8));
						EEPROM.write(AddrConfig.LuxSetupLbyte, (NodeConfig.LuxSetup & 0x00FF));
						EEPROM.commit();
					}
					else if (StringTemp.substring(0) == NodeCommand.TimerSetup)
					{
						Serial.print("Enter Timer Setup (min) / Enter save for record and exit\n");
						NodeParaString.TimerSetup = GetDataConf();
						NodeConfig.TimerSetup = (NodeParaString.TimerSetup).toInt();
						Serial.print("Timer Setup is : ");
						Serial.print(NodeConfig.TimerSetup);

						EEPROM.write(AddrConfig.TimerSetupHbyte, (NodeConfig.TimerSetup >> 8));
						EEPROM.write(AddrConfig.TimerSetupLbyte, (NodeConfig.TimerSetup & 0x00FF));
						EEPROM.commit();
					}
					else if (StringTemp.substring(0) == NodeCommand.TimeOnSetup)
					{
						Serial.print("Enter Time On Setup (hhmm) / Enter save for record and exit\n");
						NodeParaString.TimeOnSetup = GetDataConf();
						NodeParaString.TimeOnSetuphh = (NodeParaString.TimeOnSetup).substring(0,2);
						NodeParaString.TimeOnSetupmm = (NodeParaString.TimeOnSetup).substring(2,4);

						NodeConfig.TimeOnSetuphh = (NodeParaString.TimeOnSetuphh).toInt();
						NodeConfig.TimeOnSetupmm = (NodeParaString.TimeOnSetupmm).toInt();

						Serial.print("Time On Setup is : ");
						Serial.print(NodeConfig.TimeOnSetuphh);
						Serial.print(NodeConfig.TimeOnSetupmm);

						NodeConfig.TimeOnSetupSum = ((NodeConfig.TimeOnSetuphh *60) + NodeConfig.TimeOnSetupmm);

						EEPROM.write(AddrConfig.TimeOnSetuphh, NodeConfig.TimeOnSetuphh);
						EEPROM.write(AddrConfig.TimeOnSetupmm, NodeConfig.TimeOnSetupmm);
						EEPROM.commit();
					}
					else if (StringTemp.substring(0) == NodeCommand.TimeOffSetup)
					{
						Serial.print("Enter Time Off Setup (hhmm) / Enter save for record and exit\n");
						NodeParaString.TimeOffSetup = GetDataConf();
						NodeParaString.TimeOffSetuphh = (NodeParaString.TimeOffSetup).substring(0,2);
						NodeParaString.TimeOffSetupmm = (NodeParaString.TimeOffSetup).substring(2,4);

						NodeConfig.TimeOffSetuphh = (NodeParaString.TimeOffSetuphh).toInt();
						NodeConfig.TimeOffSetupmm = (NodeParaString.TimeOffSetupmm).toInt();

						NodeConfig.TimeOffSetupSum = ((NodeConfig.TimeOffSetuphh *60) + NodeConfig.TimeOffSetupmm);

						Serial.print("Time Off Setup is : ");
						Serial.print(NodeConfig.TimeOffSetuphh);
						Serial.print(NodeConfig.TimeOffSetupmm);

						EEPROM.write(AddrConfig.TimeOffSetuphh, NodeConfig.TimeOffSetuphh);
						EEPROM.write(AddrConfig.TimeOffSetupmm, NodeConfig.TimeOffSetupmm);
						EEPROM.commit();
					}
					else if (StringTemp.substring(0) == NodeCommand.SoilHumiDry)
					{
						Serial.print("Enter Soil Humidity Dry Setup (%) / Enter save for record and exit\n");
						NodeParaString.SoilHumiDry = GetDataConf();
						NodeParaString.SoilHumiDry = (NodeParaString.SoilHumiDry).substring(0,2);

						NodeConfig.SoilHumiDry = (NodeParaString.SoilHumiDry).toInt();

						Serial.print("Soil Humidity Dry Setup is : ");
						Serial.print(NodeConfig.SoilHumiDry);

						EEPROM.write(AddrConfig.SoilHumiDry, NodeConfig.SoilHumiDry);
						EEPROM.commit();
					}
					else if (StringTemp.substring(0) == NodeCommand.SqlUpTime)
					{
						Serial.print("Enter SQL update time (Min) / Enter save for record and exit\n");
						NodeParaString.SqlUpTime = GetDataConf();
						NodeParaString.SqlUpTime = (NodeParaString.SqlUpTime).substring(0,2);

						NodeConfig.SqlUpTime = (NodeParaString.SqlUpTime).toInt();

						Serial.print("SQL update time is : ");
						Serial.print(NodeConfig.SqlUpTime);

						EEPROM.write(AddrConfig.SqlUpTime, NodeConfig.SqlUpTime);
						EEPROM.commit();
					}
					else if(StringTemp.substring(0) == NodeCommand.End)
					{
						EndConfigFlag = 1;
					}
					else if (StringTemp.substring(0) == NodeCommand.Help)
					{
						Serial.print("---------------Command list---------------\n");
						Serial.print("nodename : Add/Edit Node name\n");
						Serial.print("nodeid : Add/Edit Node ID\n");
						Serial.print("nodemode : Edit node mode\n");
						Serial.print("wifissid : Add/Edit WiFi SSID\n");
						Serial.print("wifipassword : Add/Edit WiFi Password\n");
						Serial.print("SqlIP : Add/Edit WiFi network IP\n");
						Serial.print("WifiSSID2 : Add/Edit WiFi network Subnet\n");
						Serial.print("WifiPassword2 : Add/Edit WiFi network Gateway\n");
						Serial.print("luxsetup : Edit Lux setup sensor\n");
						Serial.print("timersetup : Edit Timer Setup\n");
						Serial.print("timeonsetup : Edit Time On Setup\n");
						Serial.print("timeoffsetup : Edit Time Off Setup\n");
						Serial.print("soilhumidry : Edit Soil humidity setup\n");
						Serial.print("save : Save parameters\n");
						Serial.print("help : To configuration menu command\n");
						Serial.print("end : End of configuration\n");
						Serial.print("conf : Access to config menu\n");
						Serial.print("node : Show node configuration parameters\n");
						Serial.print("sqluptime : Setup update SQL server time period \n");
						Serial.print("reset : Reset Node\n");
						Serial.print("modeinfo : Mode detail\n");
					}
				}
				delay(100);
			}
		}
		else if (str.substring(0) == NodeCommand.Node)
		{
			PrintConfig();
		}
		else if (str.substring(0) == NodeCommand.ModeInfo)
		{
			Serial.print("*------------------------------------------*\n");
			Serial.print("mode 0 : Turn OFF all devices\n");
			Serial.print("mode 1 : Turn OFF when Time system coundown\n");
			Serial.print("mode 2 : Active with light Lux\n");
			Serial.print("mode 3 : Active with schedul time setup\n");
			Serial.print("mode 4 : Active schedul on light\n");
			Serial.print("mode 5 : Active timer on light\n");
			Serial.print("mode 6 : Active with soil humidity\n");
		}
		else if (str.substring(0) == NodeCommand.NodeReset)
		{
			Serial.print("Reboot Node\n");
			NodeReset();
		}
		else {
			Serial.println(">>If you need config Please enter: conf ");
		}

		Serial.println("-----------End of configuration-----------");
	}

	EndConfigFlag = 0;
}

void FirstEEPROMwrite()
{
	EEPROM.write(AddrConfig.NodeId, 255);
	EEPROM.write(AddrConfig.NodeMode, 6);
	EEPROM.write(AddrConfig.LuxSetupHbyte, 255);
	EEPROM.write(AddrConfig.LuxSetupLbyte, 255);
	EEPROM.write(AddrConfig.TimerSetupHbyte, 255);
	EEPROM.write(AddrConfig.TimerSetupLbyte, 255);
	EEPROM.write(AddrConfig.TimeOnSetuphh, 255);
	EEPROM.write(AddrConfig.TimeOnSetupmm, 255);
	EEPROM.write(AddrConfig.TimeOffSetuphh, 255);
	EEPROM.write(AddrConfig.TimeOffSetupmm, 255);
	EEPROM.write(AddrConfig.SoilHumiDry, 255);
	EEPROM.write(AddrConfig.SqlUpTime, 255);

	WriteEepromString(AddrConfig.NodeName,"NodeName\n");
	WriteEepromString(AddrConfig.WifiSSID,"Ploii79318\n");
	WriteEepromString(AddrConfig.WifiPassword,"password\n");
	WriteEepromString(AddrConfig.SqlIP,"192.168.1.111\n");
	WriteEepromString(AddrConfig.WifiSSID2,"255.255.255.0\n");
	WriteEepromString(AddrConfig.WifiPassword2,"192.168.1.254\n");
}
