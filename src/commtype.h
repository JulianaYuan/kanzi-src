#include "Any.h"
enum MsgType
{
	MSG_ID_ContentType = 1,
	MSG_ID_TurnLeftLight = 2,
	MSG_ID_TurnRightLight = 3,
	MSG_ID_RotateValue = 4,
	MSG_ID_SpeedValue = 5,
	MSG_ID_SongName = 6,
    MSG_ID_SingerName = 7,
    MSG_ID_TurnRoundDistance =8,
	MSG_ID_TargetDistance = 9,
	MSG_ID_TopTime = 10,
	MSG_ID_DTEDistance = 11,
	MSG_ID_BatteryLevel = 12,
	MSG_ID_ODODistance = 13,
	MSG_ID_GearType = 14,
	MSG_ID_CallName = 15,
	MSG_ID_CallState = 16,
	MSG_ID_Carc_doorFL = 17,
	MSG_ID_Carc_doorBL = 18,
	MSG_ID_Carc_doorBR = 19,
	MSG_ID_Carc_doorFR = 20,
	MSG_ID_MusicState = 21,
	MSG_ID_ABSLight = 22,
	MSG_ID_AirBagLight = 23,
	MSG_ID_BatteryLight = 24,
	MSG_ID_BrakePadLight = 25,
	MSG_ID_BrakePedalNotDePress = 26,
	MSG_ID_CarDoorLight = 27,
	MSG_ID_DippedHeadLight = 28,
	MSG_ID_EngineWarmUpLight = 29,
	MSG_ID_EPCLight = 30,
	MSG_ID_ESPLight = 31,
	MSG_ID_FogFrontLight = 32,
	MSG_ID_FogRearLight = 33,
	MSG_ID_HeatingFrontLight = 34,
	MSG_ID_HeatingRearLight = 35,
	MSG_ID_ImmobiliserLight = 36,
	MSG_ID_LowCleanWaterLight = 37,
	MSG_ID_LowFuelLight = 38,
	MSG_ID_LowOilPressure = 39,
	MSG_ID_LowPressureLight = 40,
	MSG_ID_MotorDetectLight = 41,
	MSG_ID_ParkingBrake = 42,
	MSG_ID_SafetyLight = 43,
	MSG_ID_SteeringWheelLight = 44,
	MSG_ID_WidthLight = 45,
	MSG_ID_AnimationStart = 46,
	MSG_ID_iMusicMoveState = 47,
	MSG_ID_bMachineStart = 48,
	MSG_ID_TopContentType = 49,
	MSG_ID_sNaviDirection = 50,
	MSG_ID_sTimeValue = 51,
	MSG_ID_iTimePeriod = 52
	
};

struct CommMsg
{
	int m_msgType;
	Any m_data;
	std::string Stringvalue;
	int intValue;
	bool bool_value;
};

struct DoorMsg
{
	bool m_door_LB;
	bool m_door_LF;
	bool m_door_RB;
	bool m_door_RF;
};

struct SpeedMsg
{
	int m_speed;
};

struct RotateMsg
{
	int m_rotate;
};

struct FuelMsg
{
	float m_fuel;
};

struct GearMsg
{
	int m_gear;
};

struct TotalMileageMsg
{
	int m_totalMileage;
};

struct TripMileageMsg
{
	int m_tripMileage;
};

struct InstantFuelMsg
{
	int m_instantFuel;
};

struct WaterTemptureMsg
{
	float m_tempture;
};