#include "NodeAP.h"

extern uint16_t	  Mode_TimerCounter;
extern uint8_t		SwStatus;
extern uint8_t		Mode_Node;
extern uint8_t LedStatus_flag;

extern strSensor SensorVal;
extern StructNodeConfig NodeConfig;
extern TimePara TimeDisco;

//extern uint8_t	GetNTP_Status;

/*---  Mode Process -------------------------------------------------------*/
void Mode_Process(void)
{
  switch (Mode_Node)
  {
    case MODE_SEFE:                   //Mode 0
      SwStatus = Mode_Sefe();
      SensorVal.SW1 = SwStatus;
    break;

    case MODE_TIMER:                 //Mode 1
      if(Mode_Timer(NodeConfig.TimerSetup) == 1)
      {
        LedStatus_flag = 1;

        SwStatus = 1;
        SensorVal.SW1 = SwStatus;
      }
      else
      {
        LedStatus_flag = 0;

        SwStatus = 0;
        SensorVal.SW1 = SwStatus;
      }
    break;

    case MODE_LIGHT:                //Mode 2
      if(Mode_Light(NodeConfig.LuxSetup) == 1)
      {
        LedStatus_flag = 1;

        SwStatus = 1;
        SensorVal.SW1 = SwStatus;
      }
      else
      {
        LedStatus_flag = 0;

        SwStatus = 0;
        SensorVal.SW1 = SwStatus;
      }
    break;

    case MODE_SCHE:                 //Mode 3
      if(Mode_Schedule(NodeConfig.TimeOnSetupSum, NodeConfig.TimeOffSetupSum) == 1)
      {
        LedStatus_flag = 1;

        SwStatus = 1;
        SensorVal.SW1 = SwStatus;
      }
      else
      {
        LedStatus_flag = 0;

        SwStatus = 0;
        SensorVal.SW1 = SwStatus;
      }
    break;

    case MODE_SCHE_LIGHT:           //Mode 4
      if(Mode_Schedule(NodeConfig.TimeOnSetupSum, NodeConfig.TimeOffSetupSum) == 1)
      {
        if(Mode_Light(NodeConfig.LuxSetup) == 1)
        {
          SwStatus = 1;
          SensorVal.SW1 = SwStatus;
        }
        else
        {
          SwStatus = 0;
          SensorVal.SW1 = SwStatus;
        }
      }
      else
      {
        SwStatus = 0;
        SensorVal.SW1 = SwStatus;
      }
    break;

    case MODE_TIMER_LIGHT:           //Mode 5
      if(Mode_Timer(NodeConfig.TimerSetup) == 1)
      {
        if(Mode_Light(NodeConfig.LuxSetup) == 1)
        {
          SwStatus = 1;
          SensorVal.SW1 = SwStatus;
        }
        else
        {
          SwStatus = 0;
          SensorVal.SW1 = SwStatus;
        }
      }
      else
      {
        SwStatus = 0;
        SensorVal.SW1 = SwStatus;
      }
    break;

    case MODE_SOIL_HU:           //Mode 6
      if(Mode_SoilHumi(SensorVal.soil_hu, NodeConfig.SoilHumiDry) == 1)
      {
        LedStatus_flag = 1;

        SwStatus = 1;
        SensorVal.SW1 = SwStatus;
      }
      else
      {
        LedStatus_flag = 0;
        SwStatus = 0;
        SensorVal.SW1 = SwStatus;
      }
    break;

    default:
      break;
  }
}

/*--- Mode Sefe -----------------------------------------------------------*/
uint8_t Mode_Sefe(void)
{
  uint8_t Mode_Output;

  Mode_Output = 1;

  return Mode_Output;
}

uint8_t Mode_Timer(uint16_t SetTimer)
{
  uint8_t Mode_Output;

  if(Mode_TimerCounter > SetTimer)
  {
    Mode_Output = 0;
  }
  else
  {
    Mode_Output = 1;
  }

  return Mode_Output;
}

/*--- Mode Light ---------------------------------------------------------*/
uint8_t Mode_Light(uint16_t Light)
{
  uint8_t Mode_Output;

	if(SensorVal.light < Light)
	{
		Mode_Output = 1;
	}
	else
	{
		Mode_Output = 0;
	}

  return Mode_Output;
}

/*--- Mode Schedule Light ------------------------------------------------*/
uint8_t Mode_Schedule(uint16_t TimeOn, uint16_t TimeOff)
{
  uint8_t Mode_Output;

	if((TimeDisco.Sum_min >= TimeOn) and (TimeDisco.Sum_min < TimeOff))
	{
		Mode_Output = 1;
  }
	else
	{
		Mode_Output = 0;
	}

	//SensorVal.SW1 = SwStatus;
  return Mode_Output;
}

/*--- Mode Soil Humidity light status  ------------------------------------*/
uint8_t	Mode_SoilHumi(float SoilHumi, float SoilHumiDry)
{
  uint8_t Mode_Output;

  if(SoilHumi < SoilHumiDry)  //soil dry
  {
    Mode_Output = 1;
  }
  else
  {
    Mode_Output = 0;
  }

  return Mode_Output;
}
