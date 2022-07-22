#ifndef SIGNALS_J1939_H
#define SIGNALS_J1939_H
#include <mainwindow.h>


static const quint16 J1939_PGN_EEC1 =  0xF004;
static const quint16 J1939_PGN_EEC2 =  0xF003;
static const quint16 J1939_PGN_EEC3 =  0xFEDF;
static const quint16 J1939_PGN_EEC4 =  0xFEBE;
static const quint16 J1939_PGN_EEC6 =  0xFDA3;
static const quint16 J1939_PGN_EEC7 =  0xFD94;
static const quint16 J1939_PGN_IC1 =  0xFEF6;
static const quint16 J1939_PGN_IC2 =  0xFDD0;
static const quint16 J1939_PGN_IMT1 =  0xFEA6;
static const quint16 J1939_PGN_IMT2 =  0xFEA5;
static const quint16 J1939_PGN_EFL_P1 =  0xFEEF;
static const quint16 J1939_PGN_ET1 =  0xFEEE;
static const quint16 J1939_PGN_ET2 =  0xFEA4;
static const quint16 J1939_PGN_ETC2 =  0xF005;
static const quint16 J1939_PGN_LFE =  0xFEF2;
static const quint16 J1939_PGN_EBC2 =  0xFEBF;
static const quint16 J1939_PGN_VD =  0xFEE0;
static const quint16 J1939_PGN_LFC =  0xFEE9;
static const quint16 J1939_PGN_DM4 =  0xFECD;
static const quint16 J1939_PGN_GPM13 =  0xFE5F;
static const quint16 J1939_PGN_TCO1 =  0xFE6C;
static const quint16 J1939_PGN_TCW =  0xFE96;
static const quint16 J1939_PGN_VEP1 =  0xFEF7;
static const quint16 J1939_PGN_CCVS =  0xFEF1;
static const quint16 J1939_PGN_CCSS =  0xFEED;
static const quint16 J1939_PGN_TSC1 =  0xFE; //Comes in as 0000
static const quint16 J1939_PGN_AMB =  0xFEF5;
static const quint16 J1939_PGN_TFAC =  0xF01A;
static const quint16 J1939_PGN_VDC2 =  0xF009;
static const quint16 J1939_PGN_VDC1 =  0xFE4F;
static const quint16 J1939_PGN_EBC1 =  0xF001;
static const quint16 J1939_PGN_EngTrbCmprssorBypassInformation =  0xFCF3;
static const quint16 J1939_PGN_ERC1 =  0xF000;

typedef enum
{
  SrcAddrssOfCtrllngDvcFrEngCntrl= 0,// EEC1
  ActlEngPrcntTrqueHighResolution= 1,// EEC1
  EngDemandPercentTorque= 2,// EEC1
  EngStarterMode= 3,// EEC1
  EngSpeed= 4,// EEC1
  ActualEngPercentTorque= 5,// EEC1
  DriversDemandEngPercentTorque= 6,// EEC1
  EngTorqueMode= 7,// EEC1
  SCRThermalManagementActive= 8,// EEC2
  MmntryEngMaxPowerEnableFeedback= 9,// EEC2
  EstPumpingPercentTorque= 10,// EEC2
  DPFThermalManagementActive= 11,// EEC2
  ActlMxAvailableEngPercentTorque= 12,// EEC2
  AccelPedalPos2= 13,// EEC2
  VhclAccelerationRateLimitStatus= 14,// EEC2
  EngPercentLoadAtCurrentSpeed= 15,// EEC2
  AccelPedal2LowIdleSwitch= 16,// EEC2
  RoadSpeedLimitStatus= 17,// EEC2
  AccelPedalKickdownSwitch= 18,// EEC2
  AccelPedal1LowIdleSwitch= 19,// EEC2
  RemoteAccelPedalPos= 20,// EEC2
  AccelPedalPos1= 21,// EEC2
  EngsDsrdOprtngSpdAsymmtryAdjstmn= 22,// EEC3
  Aftrtratment1ExhaustGasMassFlow= 23,// EEC3
  Aftertreatment2IntakeDewPoint= 24,// EEC3
  Aftertreatment2ExhaustDewPoint= 25,// EEC3
  Aftertreatment1IntakeDewPoint= 26,// EEC3
  Aftertreatment1ExhaustDewPoint= 27,// EEC3
  EstEngPrsticLossesPercentTorque= 28,// EEC3
  EngsDesiredOperatingSpeed= 29,// EEC3
  NominalFrictionPercentTorque= 30,// EEC3
  EngRotationDirection= 31,// EEC4
  CrnkAttmptCntOnPrsntStrtAttempt= 32,// EEC4
  EngIntakeManifoldPressCtrlMode= 33,// EEC4
  EngRatedSpeed= 34,// EEC4
  EngRatedPower= 35,// EEC4
  EngVrableGeometryTurboActuator1= 36,// EEC6
  EngTrbCmprssrBypssActtr1TmpStts= 37,// EEC6
  EngTrbCmprssrBypssActtr1PrlmnryF= 38,// EEC6
  EngTrbCmprssrBypassActuator2Cmd= 39,// EEC6
  EngTrbCmprssrBypassActuator1Pos= 40,// EEC6
  EngTrbCmprssrBypassActuator1Cmd= 41,// EEC6
  EngDsrdTrbCmprssrBypssActtr1Pos= 42,// EEC6
  EngExhstGsRcrculation1Valve2Pos= 43,// EEC7
  EngExhstGsRcirculation1ValvePos= 44,// EEC7
  EngCrnkcsBrthrOilSeparatorSpeed= 45,// EEC7
  CmdedEngIntakeManifoldPress= 46,// EEC7
  EngCoolantFilterDiffPress= 47,// IC1
  EngExhaustGasTemp= 48,// IC1
  EngAirFilter1DiffPress= 49,// IC1
  EngIntakeManifold1Temp= 50,// IC1
  EngIntakeManifold1Press= 51,// IC1
  EngDslPrtclateFilterIntakePress= 52,// IC1
  EngAirIntakePress= 53,// IC1
  EngAirFilter4DiffPress= 54,// IC2
  EngAirFilter3DiffPress= 55,// IC2
  EngAirFilter2DiffPress= 56,// IC2
  EngIntakeManifold1AbsPress= 57,// IC2
  EngIntakeManifold2AbsPress= 58,// IC2
  EngIntakeManifold2Press= 59,// IC2
  EngIntakeManifold1AbsPressHiRes= 60,// IC2
  EngTurbo4BoostPress= 61,// IMT1
  EngTurbo3BoostPress= 62,// IMT1
  EngTurbo2BoostPress= 63,// IMT1
  EngTurbo1BoostPress= 64,// IMT1
  EngIntakeManifold6Temp= 65,// IMT2
  EngIntakeManifold5Temp= 66,// IMT2
  EngIntakeManifold4Temp= 67,// IMT2
  EngIntakeManifold3Temp= 68,// IMT2
  EngIntakeManifold2Temp= 69,// IMT2
  EngCoolantLevel= 70,// EFL_P1
  EngCoolantPress= 71,// EFL_P1
  EngCrankcasePress= 72,// EFL_P1
  EngOilPress= 73,// EFL_P1
  EngOilLevel= 74,// EFL_P1
  EngExCrankcaseBlowbyPress= 75,// EFL_P1
  EngFuelDeliveryPress= 76,// EFL_P1
  EngFuelTemp1= 77,// ET1
  EngIntercoolerThermostatOpening= 78,// ET1
  EngIntercoolerTemp= 79,// ET1
  EngTurboOilTemp= 80,// ET1
  EngOilTemp1= 81,// ET1
  EngCoolantTemp= 82,// ET1
  EngExhstGsRcrculation1DiffPress= 83,// ET2
  EngExhaustGasRecirculation1Temp= 84,// ET2
  EngECUTemp= 85,// ET2
  EngOilTemp2= 86,// ET2
  TransCurrentRange= 87,// ETC2
  TransRqedRange= 88,// ETC2
  TransCurrentGear= 89,// ETC2
  TransActualGearRatio= 90,// ETC2
  TransSelectedGear= 91,// ETC2
  EngAverageFuelEconomy= 92,// LFE
  EngInstantaneousFuelEconomy= 93,// LFE
  EngFuelRate= 94,// LFE
  EngThrottleValve2Pos= 95,// LFE
  EngThrottleValve1Pos= 96,// LFE
  RlativeSpeedRearAxle2RightWheel= 97,// EBC2
  RelativeSpeedRearAxle2LeftWheel= 98,// EBC2
  RlativeSpeedRearAxle1RightWheel= 99,// EBC2
  RelativeSpeedRearAxle1LeftWheel= 100,// EBC2
  RlativeSpeedFrontAxleRightWheel= 101,// EBC2
  RelativeSpeedFrontAxleLeftWheel= 102,// EBC2
  FrontAxleSpeed= 103,// EBC2
  TotalVehicleDistance= 104,// VD
  TripDistance= 105,// VD
  EngTotalFuelUsed= 106,// LFC
  EngTripFuel= 107,// LFC
  EngIntakeManifold1PressI= 108,// DM4
  FreezeFrameLength= 109,// DM4
  EngSpeedI= 110,// DM4
  EngTorqueModeI= 111,// DM4
  WheelBasedVehicleSpeed= 112,// DM4
  EngPercentLoadAtCurrentSpeedI= 113,// DM4
  EngineCoolantTemperature= 114,// DM4
  EngineTorqueMode= 115,// GPM13
  VehicleSpeed= 116,// GPM13
  PercentLoadAtCurrentSpeed= 117,// GPM13
  EngineSpeed= 118,// GPM13
  ActualEngine_PercTorque= 119,// GPM13
  DriversDemandEngine_PercTorque= 120,// GPM13
  EngineRunning= 121,// GPM13
  EngineControlAllowed= 122,// GPM13
  VehicleOverspeed= 123,// TCO1
  VehicleMotion= 124,// TCO1
  TachographVehicleSpeed= 125,// TCO1
  TachographOutputShaftSpeed= 126,// TCO1
  DirectionIndicator= 127,// TCO1
  TachographPerformance= 128,// TCO1
  HandlingInformation= 129,// TCO1
  SystemEvent= 130,// TCO1
  DriverCardDriver2= 131,// TCO1
  Driver2TimeRelatedStates= 132,// TCO1
  DriverCardDriver1= 133,// TCO1
  Driver1TimeRelatedStates= 134,// TCO1
  Driver2WorkingState= 135,// TCO1
  Driver1WorkingState= 136,// TCO1
  EngTrbWstgteActuator1TempStatus= 137,// TCW
  EngTrbWstgtActtr1PreliminaryFMI= 138,// TCW
  EngDsrdTrbWastegateActuator2Pos= 139,// TCW
  EngDsrdTrbWastegateActuator1Pos= 140,// TCW
  EngTurboWastegateActuator4Pos= 141,// TCW
  EngTurboWastegateActuator3Pos= 142,// TCW
  EngTurboWastegateActuator2Pos= 143,// TCW
  EngTurboWastegateActuator1Pos= 144,// TCW
  EngTrbWstgtActuatorCtrlAirPress= 145,// TCW
  ChargingSystemPotential= 146,// VEP1
  AltCurrent= 147,// VEP1
  NetBatteryCurrent= 148,// VEP1
  KeyswitchBatteryPotential= 149,// VEP1
  BatteryPotential_PowerInput1= 150,// VEP1
  ParkBrakeReleaseInhibitRq= 151,// CCVS
  PTOGovernorState= 152,// CCVS
  EngShutdownOverrideSwitch= 153,// CCVS
  EngTestModeSwitch= 154,// CCVS
  EngIdleDecrementSwitch= 155,// CCVS
  EngIdleIncrementSwitch= 156,// CCVS
  CruiseCtrlStates= 157,// CCVS
  CruiseCtrlSetSpeed= 158,// CCVS
  CruiseCtrlAccelerateSwitch= 159,// CCVS
  CruiseCtrlResumeSwitch= 160,// CCVS
  CruiseCtrlCoastSwitch= 161,// CCVS
  CruiseCtrlSetSwitch= 162,// CCVS
  ClutchSwitch= 163,// CCVS
  BrakeSwitch= 164,// CCVS
  CruiseCtrlEnableSwitch= 165,// CCVS
  CruiseCtrlActive= 166,// CCVS
  WheelBasedVehicleSpeedI= 167,// CCVS
  CruiseCtrlPauseSwitch= 168,// CCVS
  ParkingBrakeSwitch= 169,// CCVS
  TwoSpeedAxleSwitch= 170,// CCVS
  CruiseCtrlLowSetLimitSpeed= 171,// CCSS
  CruiseCtrlHighSetLimitSpeed= 172,// CCSS
  MaxVehicleSpeedLimit= 173,// CCSS
  MessageCounter= 174,// TSC1
  ControlPurpose= 175,// TSC1
  TransmissionRate= 176,// TSC1
  EngRqedTorque_TorqueLimit= 177,// TSC1
  EngRqedSpeed_SpeedLimit= 178,// TSC1
  OverrideCtrlModePriority= 179,// TSC1
  EngRqedSpeedCtrlConditions= 180,// TSC1
  EngOverrideCtrlMode= 181,// TSC1
  MessageChecksum= 182,// TSC1
  EngineRequestedTorqueHiRes= 183,// TSC1
  EngAirIntakeTemp= 184,// AMB
  RoadSurfaceTemp= 185,// AMB
  AmbientAirTemp= 186,// AMB
  CabInteriorTemp= 187,// AMB
  BarometricPress= 188,// AMB
  EngThrottleActuator2CtrlCmd= 189,// TFAC
  EngThrottleActuator1CtrlCmd= 190,// TFAC
  EngFuelActuator2CtrlCmd= 191,// TFAC
  EngFuelActuator1CtrlCmd= 192,// TFAC
  SteerWheelAngle= 193,// VDC2
  LongitudinalAcceleration= 194,// VDC2
  LateralAcceleration= 195,// VDC2
  YawRate= 196,// VDC2
  SteerWheelAngleSensorType= 197,// VDC2
  SteerWheelTurnCounter= 198,// VDC2
  YCBrakeCtrlActive= 199,// VDC1
  YCEngCtrlActive= 200,// VDC1
  ROPBrakeCtrlActive= 201,// VDC1
  ROPEngCtrlActive= 202,// VDC1
  VDCBrakeLightRq= 203,// VDC1
  VDCFullyOperational= 204,// VDC1
  VDCInformationSignal= 205,// VDC1
  EngAuxShutdownSwitch= 206,// EBC1
  SrcAddrssOfCtrllngDvcFrBrkCntrl= 207,// EBC1
  BrakePedalPos= 208,// EBC1
  TrctrMntdTrilerABSWarningSignal= 209,// EBC1
  TrailerABSStatus= 210,// EBC1
  HaltBrakeSwitch= 211,// EBC1
  ATC_ASRInformationSignal= 212,// EBC1
  ABS_EBSAmberWarningSignal= 213,// EBC1
  EBSRedWarningSignal= 214,// EBC1
  ABSFullyOperational= 215,// EBC1
  EngRetarderSelection= 216,// EBC1
  RemoteAccelEnableSwitch= 217,// EBC1
  EngDerateSwitch= 218,// EBC1
  AccelInterlockSwitch= 219,// EBC1
  TractionCtrlOverrideSwitch= 220,// EBC1
  ASRHillHolderSwitch= 221,// EBC1
  ASROffroadSwitch= 222,// EBC1
  ABSOffroadSwitch= 223,// EBC1
  EBSBrakeSwitch= 224,// EBC1
  AntiLockBrakingActive= 225,// EBC1
  ASRBrakeCtrlActive= 226,// EBC1
  ASREngCtrlActive= 227,// EBC1
  EngTrbCmprssrBypssActtr2OprtnStt= 228,// EngTrbCmprssorBypassInformation
  EngTrbCmprssrBypssActtr1OprtnStt= 229,// EngTrbCmprssorBypassInformation
  EngTrbCmprssrBypssActtr2TmpStts= 230,// EngTrbCmprssorBypassInformation
  EngTrbCmprssrBypssActtr2PrlmnryF= 231,// EngTrbCmprssorBypassInformation
  EngDsrdTrbCmprssrBypssActtr2Pos= 232,// EngTrbCmprssorBypassInformation
  EngTrbCmprssrBypassActuator2Pos= 233,// EngTrbCmprssorBypassInformation
  RetarderSelectionNonEng= 234,// ERC1
  SrcAddrssOfCtrllngDvcFrRtrdrCntr= 235,// ERC1
  RetarderRoadSpeedLimitSwitch= 236,// ERC1
  RetarderRoadSpeedExceededStatus= 237,// ERC1
  ActlMxAvlblRtarderPercentTorque= 238,// ERC1
  DrvrsDmandRetarderPercentTorque= 239,// ERC1
  RetarderRqingBrakeLight= 240,// ERC1
  EngCoolantLoadIncrease= 241,// ERC1
  IntendedRetarderPercentTorque= 242,// ERC1
  ActualRetarderPercentTorque= 243,// ERC1
  RetarderEnableShiftAssistSwitch= 244,// ERC1
  RetarderEnableBrakeAssistSwitch= 245,// ERC1
  RetarderTorqueMode= 246,// ERC1
}eJ1939_Signal_t;

static const sJ1939_Signal_t sJ1939_Signals[] =
{
//EEC1
{J1939_PGN_EEC1,1,0x00,"SrcAddrssOfCtrllngDvcFrEngCntrl",40,8,0,1,0,0,255,""},
{J1939_PGN_EEC1,1,0x00,"ActlEngPrcntTrqueHighResolution",4,4,0,0.125,0,0,0.875,"%"},
{J1939_PGN_EEC1,1,0x00,"EngDemandPercentTorque",56,8,0,1,-125,-125,125,"%"},
{J1939_PGN_EEC1,1,0x00,"EngStarterMode",48,4,0,1,0,0,15,""},
{J1939_PGN_EEC1,1,0x00,"EngSpeed",24,16,0,0.125,0,0,8031.875,"rpm"},
{J1939_PGN_EEC1,1,0x00,"ActualEngPercentTorque",16,8,0,1,-125,-125,125,"%"},
{J1939_PGN_EEC1,1,0x00,"DriversDemandEngPercentTorque",8,8,0,1,-125,-125,125,"%"},
{J1939_PGN_EEC1,1,0x00,"EngTorqueMode",0,4,0,1,0,0,15,""},
//EEC2
{J1939_PGN_EEC2,0,0x00,"SCRThermalManagementActive",46,2,0,1,0,0,3,""},
{J1939_PGN_EEC2,0,0x00,"MmntryEngMaxPowerEnableFeedback",42,2,0,1,0,0,3,""},
{J1939_PGN_EEC2,0,0x00,"EstPumpingPercentTorque",56,8,0,1,-125,-125,125,"%"},
{J1939_PGN_EEC2,0,0x00,"DPFThermalManagementActive",44,2,0,1,0,0,3,""},
{J1939_PGN_EEC2,0,0x00,"ActlMxAvailableEngPercentTorque",48,8,0,0.4,0,0,100,"%"},
{J1939_PGN_EEC2,0,0x00,"AccelPedalPos2",32,8,0,0.4,0,0,100,"%"},
{J1939_PGN_EEC2,0,0x00,"VhclAccelerationRateLimitStatus",40,2,0,1,0,0,3,""},
{J1939_PGN_EEC2,0,0x00,"EngPercentLoadAtCurrentSpeed",16,8,0,1,0,0,250,"%"},
{J1939_PGN_EEC2,0,0x00,"AccelPedal2LowIdleSwitch",6,2,0,1,0,0,3,""},
{J1939_PGN_EEC2,0,0x00,"RoadSpeedLimitStatus",4,2,0,1,0,0,3,""},
{J1939_PGN_EEC2,0,0x00,"AccelPedalKickdownSwitch",2,2,0,1,0,0,3,""},
{J1939_PGN_EEC2,0,0x00,"AccelPedal1LowIdleSwitch",0,2,0,1,0,0,3,""},
{J1939_PGN_EEC2,0,0x00,"RemoteAccelPedalPos",24,8,0,0.4,0,0,100,"%"},
{J1939_PGN_EEC2,0,0x00,"AccelPedalPos1",8,8,0,0.4,0,0,100,"%"},
//EEC3
{J1939_PGN_EEC3,0,0x00,"EngsDsrdOprtngSpdAsymmtryAdjstmn",24,8,0,1,0,0,250,""},
{J1939_PGN_EEC3,0,0x00,"Aftrtratment1ExhaustGasMassFlow",40,16,0,0.2,0,0,12851,"kg/h"},
{J1939_PGN_EEC3,0,0x00,"Aftertreatment2IntakeDewPoint",60,2,0,1,0,0,3,""},
{J1939_PGN_EEC3,0,0x00,"Aftertreatment2ExhaustDewPoint",62,2,0,1,0,0,3,""},
{J1939_PGN_EEC3,0,0x00,"Aftertreatment1IntakeDewPoint",56,2,0,1,0,0,3,""},
{J1939_PGN_EEC3,0,0x00,"Aftertreatment1ExhaustDewPoint",58,2,0,1,0,0,3,""},
{J1939_PGN_EEC3,0,0x00,"EstEngPrsticLossesPercentTorque",32,8,0,1,-125,-125,125,"%"},
{J1939_PGN_EEC3,0,0x00,"EngsDesiredOperatingSpeed",8,16,0,0.125,0,0,8031.875,"rpm"},
{J1939_PGN_EEC3,0,0x00,"NominalFrictionPercentTorque",0,8,0,1,-125,-125,125,"%"},
//EEC4
{J1939_PGN_EEC4,0,0x00,"EngRotationDirection",32,2,0,1,0,0,3,""},
{J1939_PGN_EEC4,0,0x00,"CrnkAttmptCntOnPrsntStrtAttempt",40,8,0,1,0,0,250,""},
{J1939_PGN_EEC4,0,0x00,"EngIntakeManifoldPressCtrlMode",34,2,0,1,0,0,3,""},
{J1939_PGN_EEC4,0,0x00,"EngRatedSpeed",16,16,0,0.125,0,0,8031.875,"rpm"},
{J1939_PGN_EEC4,0,0x00,"EngRatedPower",0,16,0,0.5,0,0,32127.5,"kW"},
//EEC6
{J1939_PGN_EEC6,0,0x00,"EngVrableGeometryTurboActuator1",16,8,0,0.4,0,0,100,"%"},
{J1939_PGN_EEC6,0,0x00,"EngTrbCmprssrBypssActtr1TmpStts",61,3,0,1,0,0,7,""},
{J1939_PGN_EEC6,0,0x00,"EngTrbCmprssrBypssActtr1PrlmnryF",56,5,0,1,0,0,31,""},
{J1939_PGN_EEC6,0,0x00,"EngTrbCmprssrBypassActuator2Cmd",32,16,0,0.0025,0,0,160.6375,"%"},
{J1939_PGN_EEC6,0,0x00,"EngTrbCmprssrBypassActuator1Pos",24,8,0,0.4,0,0,100,"%"},
{J1939_PGN_EEC6,0,0x00,"EngTrbCmprssrBypassActuator1Cmd",0,16,0,0.0025,0,0,160.6375,"%"},
{J1939_PGN_EEC6,0,0x00,"EngDsrdTrbCmprssrBypssActtr1Pos",48,8,0,0.4,0,0,100,"%"},
//EEC7
{J1939_PGN_EEC7,0,0x00,"EngExhstGsRcrculation1Valve2Pos",16,16,0,0.0025,0,0,160.6375,"%"},
{J1939_PGN_EEC7,0,0x00,"EngExhstGsRcirculation1ValvePos",0,16,0,0.0025,0,0,160.6375,"%"},
{J1939_PGN_EEC7,0,0x00,"EngCrnkcsBrthrOilSeparatorSpeed",32,16,0,1,0,0,64255,"rpm"},
{J1939_PGN_EEC7,0,0x00,"CmdedEngIntakeManifoldPress",48,16,0,0.125,0,0,8031.875,"kPa"},
//IC1
{J1939_PGN_IC1,0,0x00,"EngCoolantFilterDiffPress",56,8,0,0.5,0,0,125,"kPa"},
{J1939_PGN_IC1,0,0x00,"EngExhaustGasTemp",40,16,0,0.03125,-273,-273,1734.96875,"deg C"},
{J1939_PGN_IC1,0,0x00,"EngAirFilter1DiffPress",32,8,0,0.05,0,0,12.5,"kPa"},
{J1939_PGN_IC1,0,0x00,"EngIntakeManifold1Temp",16,8,0,1,-40,-40,210,"deg C"},
{J1939_PGN_IC1,0,0x00,"EngIntakeManifold1Press",8,8,0,2,0,0,500,"kPa"},
{J1939_PGN_IC1,0,0x00,"EngDslPrtclateFilterIntakePress",0,8,0,0.5,0,0,125,"kPa"},
{J1939_PGN_IC1,0,0x00,"EngAirIntakePress",24,8,0,2,0,0,500,"kPa"},
//IC2
{J1939_PGN_IC2,0,0x00,"EngAirFilter4DiffPress",16,8,0,0.05,0,0,12.5,"kPa"},
{J1939_PGN_IC2,0,0x00,"EngAirFilter3DiffPress",8,8,0,0.05,0,0,12.5,"kPa"},
{J1939_PGN_IC2,0,0x00,"EngAirFilter2DiffPress",0,8,0,0.05,0,0,12.5,"kPa"},
{J1939_PGN_IC2,0,0x00,"EngIntakeManifold1AbsPress",32,8,0,0.1,0,0,25.5,"kPa"},
{J1939_PGN_IC2,0,0x00,"EngIntakeManifold2AbsPress",56,8,0,2,0,0,500,"kPa"},
{J1939_PGN_IC2,0,0x00,"EngIntakeManifold2Press",24,8,0,2,0,0,500,"kPa"},
{J1939_PGN_IC2,0,0x00,"EngIntakeManifold1AbsPressHiRes",40,16,0,0.1,0,0,6425.5,"kPa"},
//IMT1
{J1939_PGN_IMT1,0,0x00,"EngTurbo4BoostPress",48,16,0,0.125,0,0,8031.875,"kPa"},
{J1939_PGN_IMT1,0,0x00,"EngTurbo3BoostPress",32,16,0,0.125,0,0,8031.875,"kPa"},
{J1939_PGN_IMT1,0,0x00,"EngTurbo2BoostPress",16,16,0,0.125,0,0,8031.875,"kPa"},
{J1939_PGN_IMT1,0,0x00,"EngTurbo1BoostPress",0,16,0,0.125,0,0,8031.875,"kPa"},
//IMT2
{J1939_PGN_IMT2,0,0x00,"EngIntakeManifold6Temp",32,8,0,1,-40,-40,210,"deg C"},
{J1939_PGN_IMT2,0,0x00,"EngIntakeManifold5Temp",24,8,0,1,-40,-40,210,"deg C"},
{J1939_PGN_IMT2,0,0x00,"EngIntakeManifold4Temp",16,8,0,1,-40,-40,210,"deg C"},
{J1939_PGN_IMT2,0,0x00,"EngIntakeManifold3Temp",8,8,0,1,-40,-40,210,"deg C"},
{J1939_PGN_IMT2,0,0x00,"EngIntakeManifold2Temp",0,8,0,1,-40,-40,210,"deg C"},
//EFL_P1
{J1939_PGN_EFL_P1,0,0x00,"EngCoolantLevel",56,8,0,0.4,0,0,100,"%"},
{J1939_PGN_EFL_P1,0,0x00,"EngCoolantPress",48,8,0,2,0,0,500,"kPa"},
{J1939_PGN_EFL_P1,0,0x00,"EngCrankcasePress",32,16,0,0.0078125,-250,-250,251.99,"kPa"},
{J1939_PGN_EFL_P1,0,0x00,"EngOilPress",24,8,0,4,0,0,1000,"kPa"},
{J1939_PGN_EFL_P1,0,0x00,"EngOilLevel",16,8,0,0.4,0,0,100,"%"},
{J1939_PGN_EFL_P1,0,0x00,"EngExCrankcaseBlowbyPress",8,8,0,0.05,0,0,12.5,"kPa"},
{J1939_PGN_EFL_P1,0,0x00,"EngFuelDeliveryPress",0,8,0,4,0,0,1000,"kPa"},
//ET1
{J1939_PGN_ET1,0,0x00,"EngFuelTemp1",8,8,0,1,-40,-40,210,"deg C"},
{J1939_PGN_ET1,0,0x00,"EngIntercoolerThermostatOpening",56,8,0,0.4,0,0,100,"%"},
{J1939_PGN_ET1,0,0x00,"EngIntercoolerTemp",48,8,0,1,-40,-40,210,"deg C"},
{J1939_PGN_ET1,0,0x00,"EngTurboOilTemp",32,16,0,0.03125,-273,-273,1734.96875,"deg C"},
{J1939_PGN_ET1,0,0x00,"EngOilTemp1",16,16,0,0.03125,-273,-273,1734.96875,"deg C"},
{J1939_PGN_ET1,0,0x00,"EngCoolantTemp",0,8,0,0.03125,-273,-273,-265.03125,"deg C"},
//ET2
{J1939_PGN_ET2,0,0x00,"EngExhstGsRcrculation1DiffPress",32,16,0,0.0078125,-250,-250,251.99,"kPa"},
{J1939_PGN_ET2,0,0x00,"EngExhaustGasRecirculation1Temp",48,16,0,0.03125,-273,-273,1734.96875,"deg C"},
{J1939_PGN_ET2,0,0x00,"EngECUTemp",16,16,0,0.03125,-273,-273,1734.96875,"deg C"},
{J1939_PGN_ET2,0,0x00,"EngOilTemp2",0,16,0,0.03125,-273,-273,1734.96875,"deg C"},
//ETC2
{J1939_PGN_ETC2,0,0x00,"TransCurrentRange",48,16,0,1,0,0,255,""},
{J1939_PGN_ETC2,0,0x00,"TransRqedRange",32,16,0,1,0,0,255,"per"},
{J1939_PGN_ETC2,0,0x00,"TransCurrentGear",24,8,0,1,-125,-125,125,""},
{J1939_PGN_ETC2,0,0x00,"TransActualGearRatio",8,16,0,0.001,0,0,64.255,""},
{J1939_PGN_ETC2,0,0x00,"TransSelectedGear",0,8,0,1,-125,-125,125,""},
//LFE
{J1939_PGN_LFE,0,0x00,"EngAverageFuelEconomy",32,16,0,0.001953125,0,0,125.5,"km/L"},
{J1939_PGN_LFE,0,0x00,"EngInstantaneousFuelEconomy",16,16,0,0.001953125,0,0,125.5,"km/L"},
{J1939_PGN_LFE,0,0x00,"EngFuelRate",0,16,0,0.05,0,0,3212.75,"L/h"},
{J1939_PGN_LFE,0,0x00,"EngThrottleValve2Pos",56,8,0,0.4,0,0,100,"%"},
{J1939_PGN_LFE,0,0x00,"EngThrottleValve1Pos",48,8,0,0.4,0,0,100,"%"},
//EBC2
{J1939_PGN_EBC2,0,0x00,"RlativeSpeedRearAxle2RightWheel",56,8,0,0.0625,-7.8125,-7.8125,7.8125,"km/h"},
{J1939_PGN_EBC2,0,0x00,"RelativeSpeedRearAxle2LeftWheel",48,8,0,0.0625,-7.8125,-7.8125,7.8125,"km/h"},
{J1939_PGN_EBC2,0,0x00,"RlativeSpeedRearAxle1RightWheel",40,8,0,0.0625,-7.8125,-7.8125,7.8125,"km/h"},
{J1939_PGN_EBC2,0,0x00,"RelativeSpeedRearAxle1LeftWheel",32,8,0,0.0625,-7.8125,-7.8125,7.8125,"km/h"},
{J1939_PGN_EBC2,0,0x00,"RlativeSpeedFrontAxleRightWheel",24,8,0,0.0625,-7.8125,-7.8125,7.8125,"km/h"},
{J1939_PGN_EBC2,0,0x00,"RelativeSpeedFrontAxleLeftWheel",16,8,0,0.0625,-7.8125,-7.8125,7.8125,"km/h"},
{J1939_PGN_EBC2,0,0x00,"FrontAxleSpeed",0,16,0,0.00390625,0,0,250.996,"km/h"},
//VD
{J1939_PGN_VD,0,0x00,"TotalVehicleDistance",32,32,0,0.125,0,0,526385151.9,"km"},
{J1939_PGN_VD,0,0x00,"TripDistance",0,32,0,0.125,0,0,526385151.9,"km"},
//LFC
{J1939_PGN_LFC,0,0x00,"EngTotalFuelUsed",32,32,0,0.5,0,0,2105540607.5,"L"},
{J1939_PGN_LFC,0,0x00,"EngTripFuel",0,32,0,0.5,0,0,2105540607.5,"L"},
//DM4
{J1939_PGN_DM4,0,0x00,"EngIntakeManifold1PressI",48,8,0,2,0,0,500,"kPa"},
{J1939_PGN_DM4,0,0x00,"FreezeFrameLength",0,8,0,1,0,0,255,"Byte"},
{J1939_PGN_DM4,0,0x00,"EngSpeedI",56,16,0,0.125,0,0,8031.875,"rpm"},
{J1939_PGN_DM4,0,0x00,"EngTorqueModeI",40,4,0,1,0,0,15,""},
{J1939_PGN_DM4,0,0x00,"WheelBasedVehicleSpeed",88,16,0,0.00390625,0,0,250.996,"km/h"},
{J1939_PGN_DM4,0,0x00,"EngPercentLoadAtCurrentSpeedI",72,8,0,1,0,0,250,"%"},
{J1939_PGN_DM4,0,0x00,"EngineCoolantTemperature",80,8,0,1,-40,-40,210,"∞C"},
//GPM13
{J1939_PGN_GPM13,0,0x00,"EngineTorqueMode",0,4,0,1,0,0,15,""},
{J1939_PGN_GPM13,0,0x00,"VehicleSpeed",48,16,0,0.00390625,0,0,250.996,"km/hr"},
{J1939_PGN_GPM13,0,0x00,"PercentLoadAtCurrentSpeed",40,8,0,1,0,0,125,"%"},
{J1939_PGN_GPM13,0,0x00,"EngineSpeed",24,16,0,0.125,0,0,8031.88,"rpm"},
{J1939_PGN_GPM13,0,0x00,"ActualEngine_PercTorque",16,8,0,1,-125,-125,125,"%"},
{J1939_PGN_GPM13,0,0x00,"DriversDemandEngine_PercTorque",8,8,0,1,-125,-125,125,"%"},
{J1939_PGN_GPM13,0,0x00,"EngineRunning",6,2,0,1,0,0,1,""},
{J1939_PGN_GPM13,0,0x00,"EngineControlAllowed",4,2,0,1,0,0,1,""},
//TCO1
{J1939_PGN_TCO1,0,0x00,"VehicleOverspeed",14,2,0,1,0,0,3,""},
{J1939_PGN_TCO1,0,0x00,"VehicleMotion",6,2,0,1,0,0,3,""},
{J1939_PGN_TCO1,0,0x00,"TachographVehicleSpeed",48,16,0,0.00390625,0,0,250.996,"km/h"},
{J1939_PGN_TCO1,0,0x00,"TachographOutputShaftSpeed",32,16,0,0.125,0,0,8031.875,"rpm"},
{J1939_PGN_TCO1,0,0x00,"DirectionIndicator",30,2,0,1,0,0,3,""},
{J1939_PGN_TCO1,0,0x00,"TachographPerformance",28,2,0,1,0,0,3,""},
{J1939_PGN_TCO1,0,0x00,"HandlingInformation",26,2,0,1,0,0,3,""},
{J1939_PGN_TCO1,0,0x00,"SystemEvent",24,2,0,1,0,0,3,""},
{J1939_PGN_TCO1,0,0x00,"DriverCardDriver2",20,2,0,1,0,0,3,""},
{J1939_PGN_TCO1,0,0x00,"Driver2TimeRelatedStates",16,4,0,1,0,0,15,""},
{J1939_PGN_TCO1,0,0x00,"DriverCardDriver1",12,2,0,1,0,0,3,""},
{J1939_PGN_TCO1,0,0x00,"Driver1TimeRelatedStates",8,4,0,1,0,0,15,""},
{J1939_PGN_TCO1,0,0x00,"Driver2WorkingState",3,3,0,1,0,0,7,""},
{J1939_PGN_TCO1,0,0x00,"Driver1WorkingState",0,3,0,1,0,0,7,""},
//TCW
{J1939_PGN_TCW,0,0x00,"EngTrbWstgteActuator1TempStatus",53,3,0,1,0,0,7,""},
{J1939_PGN_TCW,0,0x00,"EngTrbWstgtActtr1PreliminaryFMI",48,5,0,1,0,0,31,""},
{J1939_PGN_TCW,0,0x00,"EngDsrdTrbWastegateActuator2Pos",56,8,0,0.4,0,0,100,"%"},
{J1939_PGN_TCW,0,0x00,"EngDsrdTrbWastegateActuator1Pos",40,8,0,0.4,0,0,100,"%"},
{J1939_PGN_TCW,0,0x00,"EngTurboWastegateActuator4Pos",24,8,0,0.4,0,0,100,"%"},
{J1939_PGN_TCW,0,0x00,"EngTurboWastegateActuator3Pos",16,8,0,0.4,0,0,100,"%"},
{J1939_PGN_TCW,0,0x00,"EngTurboWastegateActuator2Pos",8,8,0,0.4,0,0,100,"%"},
{J1939_PGN_TCW,0,0x00,"EngTurboWastegateActuator1Pos",0,8,0,0.4,0,0,100,"%"},
{J1939_PGN_TCW,0,0x00,"EngTrbWstgtActuatorCtrlAirPress",32,8,0,4,0,0,1000,"kPa"},
//VEP1
{J1939_PGN_VEP1,0,0x00,"ChargingSystemPotential",16,16,0,0.05,0,0,3212.75,"V"},
{J1939_PGN_VEP1,0,0x00,"AltCurrent",8,8,0,1,0,0,250,"A"},
{J1939_PGN_VEP1,0,0x00,"NetBatteryCurrent",0,8,0,1,-125,-125,125,"A"},
{J1939_PGN_VEP1,0,0x00,"KeyswitchBatteryPotential",48,16,0,0.05,0,0,3212.75,"V"},
{J1939_PGN_VEP1,0,0x00,"BatteryPotential_PowerInput1",32,16,0,0.05,0,0,3212.75,"V"},
//CCVS
{J1939_PGN_CCVS,0,0x00,"ParkBrakeReleaseInhibitRq",6,2,0,1,0,0,3,""},
{J1939_PGN_CCVS,0,0x00,"PTOGovernorState",48,5,0,1,0,0,31,""},
{J1939_PGN_CCVS,0,0x00,"EngShutdownOverrideSwitch",62,2,0,1,0,0,3,""},
{J1939_PGN_CCVS,0,0x00,"EngTestModeSwitch",60,2,0,1,0,0,3,""},
{J1939_PGN_CCVS,0,0x00,"EngIdleDecrementSwitch",58,2,0,1,0,0,3,""},
{J1939_PGN_CCVS,0,0x00,"EngIdleIncrementSwitch",56,2,0,1,0,0,3,""},
{J1939_PGN_CCVS,0,0x00,"CruiseCtrlStates",53,3,0,1,0,0,7,""},
{J1939_PGN_CCVS,0,0x00,"CruiseCtrlSetSpeed",40,8,0,1,0,0,250,"km/h"},
{J1939_PGN_CCVS,0,0x00,"CruiseCtrlAccelerateSwitch",38,2,0,1,0,0,3,""},
{J1939_PGN_CCVS,0,0x00,"CruiseCtrlResumeSwitch",36,2,0,1,0,0,3,""},
{J1939_PGN_CCVS,0,0x00,"CruiseCtrlCoastSwitch",34,2,0,1,0,0,3,""},
{J1939_PGN_CCVS,0,0x00,"CruiseCtrlSetSwitch",32,2,0,1,0,0,3,""},
{J1939_PGN_CCVS,0,0x00,"ClutchSwitch",30,2,0,1,0,0,3,""},
{J1939_PGN_CCVS,0,0x00,"BrakeSwitch",28,2,0,1,0,0,3,""},
{J1939_PGN_CCVS,0,0x00,"CruiseCtrlEnableSwitch",26,2,0,1,0,0,3,""},
{J1939_PGN_CCVS,0,0x00,"CruiseCtrlActive",24,2,0,1,0,0,3,""},
{J1939_PGN_CCVS,0,0x00,"WheelBasedVehicleSpeedI",8,16,0,0.00390625,0,0,250.996,"km/h"},
{J1939_PGN_CCVS,0,0x00,"CruiseCtrlPauseSwitch",4,2,0,1,0,0,3,""},
{J1939_PGN_CCVS,0,0x00,"ParkingBrakeSwitch",2,2,0,1,0,0,3,""},
{J1939_PGN_CCVS,0,0x00,"TwoSpeedAxleSwitch",0,2,0,1,0,0,3,""},
//CCSS
{J1939_PGN_CCSS,0,0x00,"CruiseCtrlLowSetLimitSpeed",16,8,0,1,0,0,250,"km/h"},
{J1939_PGN_CCSS,0,0x00,"CruiseCtrlHighSetLimitSpeed",8,8,0,1,0,0,250,"km/h"},
{J1939_PGN_CCSS,0,0x00,"MaxVehicleSpeedLimit",0,8,0,1,0,0,250,"km/h"},
//TSC1
{J1939_PGN_TSC1,0,0x00,"MessageCounter",56,4,0,1,0,0,15,""},
{J1939_PGN_TSC1,0,0x00,"ControlPurpose",35,5,0,1,0,0,31,""},
{J1939_PGN_TSC1,0,0x00,"TransmissionRate",32,3,0,1,0,0,7,""},
{J1939_PGN_TSC1,0,0x00,"EngRqedTorque_TorqueLimit",24,8,0,1,-125,-125,125,"%"},
{J1939_PGN_TSC1,0,0x00,"EngRqedSpeed_SpeedLimit",8,16,0,0.125,0,0,8031.875,"rpm"},
{J1939_PGN_TSC1,0,0x00,"OverrideCtrlModePriority",4,2,0,1,0,0,3,""},
{J1939_PGN_TSC1,0,0x00,"EngRqedSpeedCtrlConditions",2,2,0,1,0,0,3,""},
{J1939_PGN_TSC1,0,0x00,"EngOverrideCtrlMode",0,2,0,1,0,0,3,""},
{J1939_PGN_TSC1,0,0x00,"MessageChecksum",60,4,0,1,0,0,15,""},
{J1939_PGN_TSC1,0,0x00,"EngineRequestedTorqueHiRes",40,4,0,0.125,0,0,0.875,"%"},
//AMB
{J1939_PGN_AMB,0,0x00,"EngAirIntakeTemp",40,8,0,1,-40,-40,210,"deg C"},
{J1939_PGN_AMB,0,0x00,"RoadSurfaceTemp",48,16,0,0.03125,-273,-273,1734.96875,"deg C"},
{J1939_PGN_AMB,0,0x00,"AmbientAirTemp",24,16,0,0.03125,-273,-273,1734.96875,"deg C"},
{J1939_PGN_AMB,0,0x00,"CabInteriorTemp",8,16,0,0.03125,-273,-273,1734.96875,"deg C"},
{J1939_PGN_AMB,0,0x00,"BarometricPress",0,8,0,0.5,0,0,125,"kPa"},
//TFAC
{J1939_PGN_TFAC,0,0x00,"EngThrottleActuator2CtrlCmd",16,16,0,0.0025,0,0,160.6375,"%"},
{J1939_PGN_TFAC,0,0x00,"EngThrottleActuator1CtrlCmd",0,16,0,0.0025,0,0,160.6375,"%"},
{J1939_PGN_TFAC,0,0x00,"EngFuelActuator2CtrlCmd",48,16,0,0.0025,0,0,160.6375,"%"},
{J1939_PGN_TFAC,0,0x00,"EngFuelActuator1CtrlCmd",32,16,0,0.0025,0,0,160.6375,"%"},
//VDC2
{J1939_PGN_VDC2,0,0x00,"SteerWheelAngle",0,16,0,0.0009765625,-31.374,-31.374,31.374,"rad"},
{J1939_PGN_VDC2,0,0x00,"LongitudinalAcceleration",56,8,0,0.1,-12.5,-12.5,12.5,"m/s2"},
{J1939_PGN_VDC2,0,0x00,"LateralAcceleration",40,16,0,0.00048828125,-15.687,-15.687,15.687,"m/s2"},
{J1939_PGN_VDC2,0,0x00,"YawRate",24,16,0,0.0001220703125,-3.92,-3.92,3.92,"rad/s"},
{J1939_PGN_VDC2,0,0x00,"SteerWheelAngleSensorType",22,2,0,1,0,0,3,""},
{J1939_PGN_VDC2,0,0x00,"SteerWheelTurnCounter",16,6,0,1,-32,-32,29,"turns"},
//VDC1
{J1939_PGN_VDC1,0,0x00,"YCBrakeCtrlActive",14,2,0,1,0,0,3,""},
{J1939_PGN_VDC1,0,0x00,"YCEngCtrlActive",12,2,0,1,0,0,3,""},
{J1939_PGN_VDC1,0,0x00,"ROPBrakeCtrlActive",10,2,0,1,0,0,3,""},
{J1939_PGN_VDC1,0,0x00,"ROPEngCtrlActive",8,2,0,1,0,0,3,""},
{J1939_PGN_VDC1,0,0x00,"VDCBrakeLightRq",4,2,0,1,0,0,3,""},
{J1939_PGN_VDC1,0,0x00,"VDCFullyOperational",2,2,0,1,0,0,3,""},
{J1939_PGN_VDC1,0,0x00,"VDCInformationSignal",0,2,0,1,0,0,3,""},
//EBC1
{J1939_PGN_EBC1,0,0x00,"EngAuxShutdownSwitch",28,2,0,1,0,0,3,""},
{J1939_PGN_EBC1,0,0x00,"SrcAddrssOfCtrllngDvcFrBrkCntrl",48,8,0,1,0,0,255,""},
{J1939_PGN_EBC1,0,0x00,"BrakePedalPos",8,8,0,0.4,0,0,100,"%"},
{J1939_PGN_EBC1,0,0x00,"TrctrMntdTrilerABSWarningSignal",62,2,0,1,0,0,3,""},
{J1939_PGN_EBC1,0,0x00,"TrailerABSStatus",60,2,0,1,0,0,3,""},
{J1939_PGN_EBC1,0,0x00,"HaltBrakeSwitch",58,2,0,1,0,0,3,""},
{J1939_PGN_EBC1,0,0x00,"ATC_ASRInformationSignal",46,2,0,1,0,0,3,""},
{J1939_PGN_EBC1,0,0x00,"ABS_EBSAmberWarningSignal",44,2,0,1,0,0,3,""},
{J1939_PGN_EBC1,0,0x00,"EBSRedWarningSignal",42,2,0,1,0,0,3,""},
{J1939_PGN_EBC1,0,0x00,"ABSFullyOperational",40,2,0,1,0,0,3,""},
{J1939_PGN_EBC1,0,0x00,"EngRetarderSelection",32,8,0,0.4,0,0,100,"%"},
{J1939_PGN_EBC1,0,0x00,"RemoteAccelEnableSwitch",30,2,0,1,0,0,3,""},
{J1939_PGN_EBC1,0,0x00,"EngDerateSwitch",26,2,0,1,0,0,3,""},
{J1939_PGN_EBC1,0,0x00,"AccelInterlockSwitch",24,2,0,1,0,0,3,""},
{J1939_PGN_EBC1,0,0x00,"TractionCtrlOverrideSwitch",22,2,0,1,0,0,3,""},
{J1939_PGN_EBC1,0,0x00,"ASRHillHolderSwitch",20,2,0,1,0,0,3,""},
{J1939_PGN_EBC1,0,0x00,"ASROffroadSwitch",18,2,0,1,0,0,3,""},
{J1939_PGN_EBC1,0,0x00,"ABSOffroadSwitch",16,2,0,1,0,0,3,""},
{J1939_PGN_EBC1,0,0x00,"EBSBrakeSwitch",6,2,0,1,0,0,3,""},
{J1939_PGN_EBC1,0,0x00,"AntiLockBrakingActive",4,2,0,1,0,0,3,""},
{J1939_PGN_EBC1,0,0x00,"ASRBrakeCtrlActive",2,2,0,1,0,0,3,""},
{J1939_PGN_EBC1,0,0x00,"ASREngCtrlActive",0,2,0,1,0,0,3,""},
//EngTrbCmprssorBypassInformation
{J1939_PGN_EngTrbCmprssorBypassInformation,0,0x00,"EngTrbCmprssrBypssActtr2OprtnStt",28,4,0,1,0,0,15,""},
{J1939_PGN_EngTrbCmprssorBypassInformation,0,0x00,"EngTrbCmprssrBypssActtr1OprtnStt",24,4,0,1,0,0,15,""},
{J1939_PGN_EngTrbCmprssorBypassInformation,0,0x00,"EngTrbCmprssrBypssActtr2TmpStts",21,3,0,1,0,0,7,""},
{J1939_PGN_EngTrbCmprssorBypassInformation,0,0x00,"EngTrbCmprssrBypssActtr2PrlmnryF",16,5,0,1,0,0,31,""},
{J1939_PGN_EngTrbCmprssorBypassInformation,0,0x00,"EngDsrdTrbCmprssrBypssActtr2Pos",8,8,0,0.4,0,0,100,"%"},
{J1939_PGN_EngTrbCmprssorBypassInformation,0,0x00,"EngTrbCmprssrBypassActuator2Pos",0,8,0,0.4,0,0,100,"%"},
//ERC1
{J1939_PGN_ERC1,0,0x00,"RetarderSelectionNonEng",48,8,0,0.4,0,0,100,"%"},
{J1939_PGN_ERC1,0,0x00,"SrcAddrssOfCtrllngDvcFrRtrdrCntr",32,8,0,1,0,0,255,""},
{J1939_PGN_ERC1,0,0x00,"RetarderRoadSpeedLimitSwitch",28,2,0,1,0,0,3,""},
{J1939_PGN_ERC1,0,0x00,"RetarderRoadSpeedExceededStatus",30,2,0,1,0,0,3,""},
{J1939_PGN_ERC1,0,0x00,"ActlMxAvlblRtarderPercentTorque",56,8,0,1,-125,-125,125,"%"},
{J1939_PGN_ERC1,0,0x00,"DrvrsDmandRetarderPercentTorque",40,8,0,1,-125,-125,125,"%"},
{J1939_PGN_ERC1,0,0x00,"RetarderRqingBrakeLight",26,2,0,1,0,0,3,""},
{J1939_PGN_ERC1,0,0x00,"EngCoolantLoadIncrease",24,2,0,1,0,0,3,""},
{J1939_PGN_ERC1,0,0x00,"IntendedRetarderPercentTorque",16,8,0,1,-125,-125,125,"%"},
{J1939_PGN_ERC1,0,0x00,"ActualRetarderPercentTorque",8,8,0,1,-125,-125,125,"%"},
{J1939_PGN_ERC1,0,0x00,"RetarderEnableShiftAssistSwitch",6,2,0,1,0,0,3,""},
{J1939_PGN_ERC1,0,0x00,"RetarderEnableBrakeAssistSwitch",4,2,0,1,0,0,3,""},
{J1939_PGN_ERC1,0,0x00,"RetarderTorqueMode",0,4,0,1,0,0,15,""},
};
#endif // SIGNALS_J1939_H
