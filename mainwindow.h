#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QtEndian>
#include <QByteArray>
#include <QBitArray>
#include <QCheckBox>
#include <QScrollArea>
#include <QLineEdit>
#include <QDateTime>
#include <QFile>
#include <QSignalMapper>
#include <QTextStream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    void openSerialPort();
    void closeSerialPort();
    void connectSignalsAndSlots();
    void parseRxMsg(quint8 bMsgLen, QByteArray _baRxMsg);
    void parseExtFrame(quint32 _lCanId, QByteArray _baRxMsg);
    void parseStdFrame(quint16 _wCanAddr, QByteArray _baRxMsg);
    void handleReadyRead();
    void handleConnectSerial();
    void populateLiveTable();
    void populateLogTable();
    void checkboxClicked(quint16 _wSigIdx);
    void readVirtualMsg();
    void toggleVirtualRead();
    void decodeOBIIMsg(quint16 _wPid, QByteArray _baRxMsg);
    void decodePropMsg(quint16 _wCanAddr, QByteArray _baRxMsg);
    void handleActivate();
    void handleLog();
    void logMsg(bool bExt, quint32 _lCanId, QByteArray _baRxMsg);
    void selectLogFile();
};

static const quint8 bUNITS_LEN_MAX = 6U;
static const quint8 bPGN_NAME_LEN_MAX = 35U;
static const quint8 bMSG_SIG_CNT_MAX = 10U;
static const quint8 bLOG_LEN_MAX = 50U;
static const quint8 bBYTE_BIT_LEN = 8U;
static const quint8 bSERIAL_MSG_LEN = 13U; //Serial msg: ext 0 or 1, 4 bytes can_id, 8 bytes data}

typedef struct
{
    quint16 wPgn; //Parent message PGN code
    quint8 bSourceAddress; //Byte after PGN in Can_ID that tells where the signal came from, 0x00 looks like data and 0x21 some request/response
    char cName[bPGN_NAME_LEN_MAX]; //TODO check if this is long enough, longest I've seen is 31 characters
    quint8 bStartBit;
    quint8 bLengthInBits;
    quint8 bBigEndian; //1 for BigEndian, 0 for LittleEndian
    float fScale;
    float fOffset;
    float fMin;
    float fMax;
    char cUnits[bUNITS_LEN_MAX]; //eg rpm or kPa
}sJ1939_Signal_t; //Includes info about parent message

typedef struct
{
    quint16 wPid;
    char cName[bPGN_NAME_LEN_MAX]; //TODO check if this is long enough, longest I've seen is 31 characters
    float fMin;
    float fMax;
    char cUnits[bUNITS_LEN_MAX]; //eg rpm or kPa
}sOBDII_Signal_t;

typedef enum
{

}eOBDII_Signal_t;

typedef enum
{
    MODE_NONE = 0,
    MODE_J1939,
    MODE_OBDII,
    MODE_CANB
}eAppMode_t;

static const quint16  PID_ENGINE_SPEED = 0x0C;
static const quint16  PID_VEHICLE_SPEED = 0x0D;
static const quint16  PID_ENGINE_LOAD = 0x04;
static const quint16  PID_COOLANT_TEMP = 0x05;
static const quint16  PID_INTAKE_MANIFOLD_ABS_PRESS = 0x0B;
static const quint16  PID_ABS_BAROMETRIC_PRESS = 0x33;
static const quint16  PID_REL_THROTTLE_POS = 0x45;
static const quint16  PID_AMB_AIR_TEMP = 0x46;
static const quint16  PID_ACCEL_POS_D = 0x49;
static const quint16  PID_ACCEL_POS_E = 0x4A;
static const quint16  PID_MAF_RATE = 0x10;
static const quint16  PID_THROTTLE_POS = 0x11;
static const quint16  PID_ENGINE_OIL_TEMP = 0x5C;
static const quint16 PID_ENGINE_RUNTIME = 0x1F;
static const quint16 PID_FUEL_RAIL_GAUGE_PRESS = 0x23;
static const quint16 PID_CTL_MODULE_VOLT = 0x42;
static const quint16 PID_REL_ACCEL_POS = 0x5A;

static const quint16 PID22_FORD_BAT_CHARGE = 0x4028;


//Generated with signalparser.py
static const quint16 J1939_PGN_EEC1 =  0xF004;
static const quint16 J1939_PGN_EEC2 =  0xF003;
static const quint16 J1939_PGN_IC1 =  0xFEF6;
static const quint16 J1939_PGN_IC2 =  0xFDD0;
static const quint16 J1939_PGN_IMT1 =  0xFEA6;
static const quint16 J1939_PGN_IMT2 =  0xFEA5;
static const quint16 J1939_PGN_EFL_P1 =  0xFEEF;
static const quint16 J1939_PGN_ET1 =  0xFEEE;
static const quint16 J1939_PGN_ETC2 =  0xF005;
static const quint16 J1939_PGN_LFE =  0xFEF2;
static const quint16 J1939_PGN_EBC2 =  0xFEBF;
static const quint16 J1939_PGN_VD =  0xFEE0;
static const quint16 J1939_PGN_LFC =  0xFEE9;
static const quint16 J1939_PGN_DM4 =  0xFECD;
static const quint16 J1939_PGN_GPM13 =  0xFE5F;
static const quint16 J1939_PGN_EEC7 =  0xFD94;
//Ones to check(Leave out those that end with 21):
/*
 * fedb01, FEDB
 * cff0381, Proprietary B
 * 18f00e3d, Aftertreatment 1 Intake Gas 1
 * 18ff3000, Proprietary B
 * 18f00f3d, Aftertreatment 1 Outlet Gas 1
 * 14ffa000, Proprietary B
 * 18a9fe31, Forward Lane Image Command
 * 14fda421, PTO Drive Engagement
 * 18fef500, Ambient Conditions
 * 18fef117, Cruise Control/Vehicle Speed 1
 * 18fedf00, Electronic Engine Controller 3
 * 10ff2121, Proprietary B
 * 14fef031, Power Takeoff Information
 * 18f0233d, Aftertreatment 1 SCR Dosing System Information 1
 * etc...
*/
//Generated with signalparser.py
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
  EngCoolantFilterDiffPress= 22,// IC1
  EngExhaustGasTemp= 23,// IC1
  EngAirFilter1DiffPress= 24,// IC1
  EngIntakeManifold1Temp= 25,// IC1
  EngIntakeManifold1Press= 26,// IC1
  EngDslPrtclateFilterIntakePress= 27,// IC1
  EngAirIntakePress= 28,// IC1
  EngAirFilter4DiffPress= 29,// IC2
  EngAirFilter3DiffPress= 30,// IC2
  EngAirFilter2DiffPress= 31,// IC2
  EngIntakeManifold1AbsPress= 32,// IC2
  EngIntakeManifold2AbsPress= 33,// IC2
  EngIntakeManifold2Press= 34,// IC2
  EngIntakeManifold1AbsPressHiRes= 35,// IC2
  EngTurbo4BoostPress= 36,// IMT1
  EngTurbo3BoostPress= 37,// IMT1
  EngTurbo2BoostPress= 38,// IMT1
  EngTurbo1BoostPress= 39,// IMT1
  EngIntakeManifold6Temp= 40,// IMT2
  EngIntakeManifold5Temp= 41,// IMT2
  EngIntakeManifold4Temp= 42,// IMT2
  EngIntakeManifold3Temp= 43,// IMT2
  EngIntakeManifold2Temp= 44,// IMT2
  EngCoolantLevel= 45,// EFL_P1
  EngCoolantPress= 46,// EFL_P1
  EngCrankcasePress= 47,// EFL_P1
  EngOilPress= 48,// EFL_P1
  EngOilLevel= 49,// EFL_P1
  EngExCrankcaseBlowbyPress= 50,// EFL_P1
  EngFuelDeliveryPress= 51,// EFL_P1
  EngFuelTemp1= 52,// ET1
  EngIntercoolerThermostatOpening= 53,// ET1
  EngIntercoolerTemp= 54,// ET1
  EngTurboOilTemp= 55,// ET1
  EngOilTemp1= 56,// ET1
  EngCoolantTemp= 57,// ET1
  TransCurrentRange= 58,// ETC2
  TransRqedRange= 59,// ETC2
  TransCurrentGear= 60,// ETC2
  TransActualGearRatio= 61,// ETC2
  TransSelectedGear= 62,// ETC2
  EngAverageFuelEconomy= 63,// LFE
  EngInstantaneousFuelEconomy= 64,// LFE
  EngFuelRate= 65,// LFE
  EngThrottleValve2Pos= 66,// LFE
  EngThrottleValve1Pos= 67,// LFE
  RlativeSpeedRearAxle2RightWheel= 68,// EBC2
  RelativeSpeedRearAxle2LeftWheel= 69,// EBC2
  RlativeSpeedRearAxle1RightWheel= 70,// EBC2
  RelativeSpeedRearAxle1LeftWheel= 71,// EBC2
  RlativeSpeedFrontAxleRightWheel= 72,// EBC2
  RelativeSpeedFrontAxleLeftWheel= 73,// EBC2
  FrontAxleSpeed= 74,// EBC2
  TotalVehicleDistance= 75,// VD
  TripDistance= 76,// VD
  EngTotalFuelUsed= 77,// LFC
  EngTripFuel= 78,// LFC
  EngIntakeManifold1PressI= 79,// DM4
  FreezeFrameLength= 80,// DM4
  EngSpeedI= 81,// DM4
  EngTorqueModeI= 82,// DM4
  WheelBasedVehicleSpeed= 83,// DM4
  EngPercentLoadAtCurrentSpeedI= 84,// DM4
  EngineCoolantTemperature= 85,// DM4
  EngineTorqueMode= 86,// GPM13
  VehicleSpeed= 87,// GPM13
  PercentLoadAtCurrentSpeed= 88,// GPM13
  EngineSpeed= 89,// GPM13
  ActualEngine_PercTorque= 90,// GPM13
  DriversDemandEngine_PercTorque= 91,// GPM13
  EngineRunning= 92,// GPM13
  EngineControlAllowed= 93,// GPM13
  EngExhstGsRcrculation1Valve2Pos= 94,// EEC7
  EngExhstGsRcirculation1ValvePos= 95,// EEC7
  EngCrnkcsBrthrOilSeparatorSpeed= 96,// EEC7
  CmdedEngIntakeManifoldPress= 97,// EEC7
}eJ1939_Signal_t;

//Generated with signalparser.py
static const sJ1939_Signal_t sJ1939_Signals[] =
{
    //EEC1
    {J1939_PGN_EEC1,0x00,"SrcAddrssOfCtrllngDvcFrEngCntrl",40,8,0,1,0,0,255,""},
    {J1939_PGN_EEC1,0x00,"ActlEngPrcntTrqueHighResolution",4,4,0,0.125,0,0,0.875,"%"},
    {J1939_PGN_EEC1,0x00,"EngDemandPercentTorque",56,8,0,1,-125,-125,125,"%"},
    {J1939_PGN_EEC1,0x00,"EngStarterMode",48,4,0,1,0,0,15,""},
    {J1939_PGN_EEC1,0x00,"EngSpeed",24,16,0,0.125,0,0,8031.875,"rpm"},
    {J1939_PGN_EEC1,0x00,"ActualEngPercentTorque",16,8,0,1,-125,-125,125,"%"},
    {J1939_PGN_EEC1,0x00,"DriversDemandEngPercentTorque",8,8,0,1,-125,-125,125,"%"},
    {J1939_PGN_EEC1,0x00,"EngTorqueMode",0,4,0,1,0,0,15,""},
    //EEC2
    {J1939_PGN_EEC2,0x00,"SCRThermalManagementActive",46,2,0,1,0,0,3,""},
    {J1939_PGN_EEC2,0x00,"MmntryEngMaxPowerEnableFeedback",42,2,0,1,0,0,3,""},
    {J1939_PGN_EEC2,0x00,"EstPumpingPercentTorque",56,8,0,1,-125,-125,125,"%"},
    {J1939_PGN_EEC2,0x00,"DPFThermalManagementActive",44,2,0,1,0,0,3,""},
    {J1939_PGN_EEC2,0x00,"ActlMxAvailableEngPercentTorque",48,8,0,0.4,0,0,100,"%"},
    {J1939_PGN_EEC2,0x00,"AccelPedalPos2",32,8,0,0.4,0,0,100,"%"},
    {J1939_PGN_EEC2,0x00,"VhclAccelerationRateLimitStatus",40,2,0,1,0,0,3,""},
    {J1939_PGN_EEC2,0x00,"EngPercentLoadAtCurrentSpeed",16,8,0,1,0,0,250,"%"},
    {J1939_PGN_EEC2,0x00,"AccelPedal2LowIdleSwitch",6,2,0,1,0,0,3,""},
    {J1939_PGN_EEC2,0x00,"RoadSpeedLimitStatus",4,2,0,1,0,0,3,""},
    {J1939_PGN_EEC2,0x00,"AccelPedalKickdownSwitch",2,2,0,1,0,0,3,""},
    {J1939_PGN_EEC2,0x00,"AccelPedal1LowIdleSwitch",0,2,0,1,0,0,3,""},
    {J1939_PGN_EEC2,0x00,"RemoteAccelPedalPos",24,8,0,0.4,0,0,100,"%"},
    {J1939_PGN_EEC2,0x00,"AccelPedalPos1",8,8,0,0.4,0,0,100,"%"},
    //IC1
    {J1939_PGN_IC1,0x00,"EngCoolantFilterDiffPress",56,8,0,0.5,0,0,125,"kPa"},
    {J1939_PGN_IC1,0x00,"EngExhaustGasTemp",40,16,0,0.03125,-273,-273,1734.96875,"deg C"},
    {J1939_PGN_IC1,0x00,"EngAirFilter1DiffPress",32,8,0,0.05,0,0,12.5,"kPa"},
    {J1939_PGN_IC1,0x00,"EngIntakeManifold1Temp",16,8,0,1,-40,-40,210,"deg C"},
    {J1939_PGN_IC1,0x00,"EngIntakeManifold1Press",8,8,0,2,0,0,500,"kPa"},
    {J1939_PGN_IC1,0x00,"EngDslPrtclateFilterIntakePress",0,8,0,0.5,0,0,125,"kPa"},
    {J1939_PGN_IC1,0x00,"EngAirIntakePress",24,8,0,2,0,0,500,"kPa"},
    //IC2
    {J1939_PGN_IC2,0x00,"EngAirFilter4DiffPress",16,8,0,0.05,0,0,12.5,"kPa"},
    {J1939_PGN_IC2,0x00,"EngAirFilter3DiffPress",8,8,0,0.05,0,0,12.5,"kPa"},
    {J1939_PGN_IC2,0x00,"EngAirFilter2DiffPress",0,8,0,0.05,0,0,12.5,"kPa"},
    {J1939_PGN_IC2,0x00,"EngIntakeManifold1AbsPress",32,8,0,0.1,0,0,25.5,"kPa"},
    {J1939_PGN_IC2,0x00,"EngIntakeManifold2AbsPress",56,8,0,2,0,0,500,"kPa"},
    {J1939_PGN_IC2,0x00,"EngIntakeManifold2Press",24,8,0,2,0,0,500,"kPa"},
    {J1939_PGN_IC2,0x00,"EngIntakeManifold1AbsPressHiRes",40,16,0,0.1,0,0,6425.5,"kPa"},
    //IMT1
    {J1939_PGN_IMT1,0x00,"EngTurbo4BoostPress",48,16,0,0.125,0,0,8031.875,"kPa"},
    {J1939_PGN_IMT1,0x00,"EngTurbo3BoostPress",32,16,0,0.125,0,0,8031.875,"kPa"},
    {J1939_PGN_IMT1,0x00,"EngTurbo2BoostPress",16,16,0,0.125,0,0,8031.875,"kPa"},
    {J1939_PGN_IMT1,0x00,"EngTurbo1BoostPress",0,16,0,0.125,0,0,8031.875,"kPa"},
    //IMT2
    {J1939_PGN_IMT2,0x00,"EngIntakeManifold6Temp",32,8,0,1,-40,-40,210,"deg C"},
    {J1939_PGN_IMT2,0x00,"EngIntakeManifold5Temp",24,8,0,1,-40,-40,210,"deg C"},
    {J1939_PGN_IMT2,0x00,"EngIntakeManifold4Temp",16,8,0,1,-40,-40,210,"deg C"},
    {J1939_PGN_IMT2,0x00,"EngIntakeManifold3Temp",8,8,0,1,-40,-40,210,"deg C"},
    {J1939_PGN_IMT2,0x00,"EngIntakeManifold2Temp",0,8,0,1,-40,-40,210,"deg C"},
    //EFL_P1
    {J1939_PGN_EFL_P1,0x00,"EngCoolantLevel",56,8,0,0.4,0,0,100,"%"},
    {J1939_PGN_EFL_P1,0x00,"EngCoolantPress",48,8,0,2,0,0,500,"kPa"},
    {J1939_PGN_EFL_P1,0x00,"EngCrankcasePress",32,16,0,0.0078125,-250,-250,251.99,"kPa"},
    {J1939_PGN_EFL_P1,0x00,"EngOilPress",24,8,0,4,0,0,1000,"kPa"},
    {J1939_PGN_EFL_P1,0x00,"EngOilLevel",16,8,0,0.4,0,0,100,"%"},
    {J1939_PGN_EFL_P1,0x00,"EngExCrankcaseBlowbyPress",8,8,0,0.05,0,0,12.5,"kPa"},
    {J1939_PGN_EFL_P1,0x00,"EngFuelDeliveryPress",0,8,0,4,0,0,1000,"kPa"},
    //ET1
    {J1939_PGN_ET1,0x00,"EngFuelTemp1",8,8,0,1,-40,-40,210,"deg C"},
    {J1939_PGN_ET1,0x00,"EngIntercoolerThermostatOpening",56,8,0,0.4,0,0,100,"%"},
    {J1939_PGN_ET1,0x00,"EngIntercoolerTemp",48,8,0,1,-40,-40,210,"deg C"},
    {J1939_PGN_ET1,0x00,"EngTurboOilTemp",32,16,0,0.03125,-273,-273,1734.96875,"deg C"},
    {J1939_PGN_ET1,0x00,"EngOilTemp1",16,16,0,0.03125,-273,-273,1734.96875,"deg C"},
    {J1939_PGN_ET1,0x00,"EngCoolantTemp",0,8,0,0.03125,-273,-273,-265.03125,"deg C"},
    //ETC2
    {J1939_PGN_ETC2,0x00,"TransCurrentRange",48,16,0,1,0,0,255,""},
    {J1939_PGN_ETC2,0x00,"TransRqedRange",32,16,0,1,0,0,255,"per"},
    {J1939_PGN_ETC2,0x00,"TransCurrentGear",24,8,0,1,-125,-125,125,""},
    {J1939_PGN_ETC2,0x00,"TransActualGearRatio",8,16,0,0.001,0,0,64.255,""},
    {J1939_PGN_ETC2,0x00,"TransSelectedGear",0,8,0,1,-125,-125,125,""},
    //LFE
    {J1939_PGN_LFE,0x00,"EngAverageFuelEconomy",32,16,0,0.001953125,0,0,125.5,"km/L"},
    {J1939_PGN_LFE,0x00,"EngInstantaneousFuelEconomy",16,16,0,0.001953125,0,0,125.5,"km/L"},
    {J1939_PGN_LFE,0x00,"EngFuelRate",0,16,0,0.05,0,0,3212.75,"L/h"},
    {J1939_PGN_LFE,0x00,"EngThrottleValve2Pos",56,8,0,0.4,0,0,100,"%"},
    {J1939_PGN_LFE,0x00,"EngThrottleValve1Pos",48,8,0,0.4,0,0,100,"%"},
    //EBC2
    {J1939_PGN_EBC2,0x00,"RlativeSpeedRearAxle2RightWheel",56,8,0,0.0625,-7.8125,-7.8125,7.8125,"km/h"},
    {J1939_PGN_EBC2,0x00,"RelativeSpeedRearAxle2LeftWheel",48,8,0,0.0625,-7.8125,-7.8125,7.8125,"km/h"},
    {J1939_PGN_EBC2,0x00,"RlativeSpeedRearAxle1RightWheel",40,8,0,0.0625,-7.8125,-7.8125,7.8125,"km/h"},
    {J1939_PGN_EBC2,0x00,"RelativeSpeedRearAxle1LeftWheel",32,8,0,0.0625,-7.8125,-7.8125,7.8125,"km/h"},
    {J1939_PGN_EBC2,0x00,"RlativeSpeedFrontAxleRightWheel",24,8,0,0.0625,-7.8125,-7.8125,7.8125,"km/h"},
    {J1939_PGN_EBC2,0x00,"RelativeSpeedFrontAxleLeftWheel",16,8,0,0.0625,-7.8125,-7.8125,7.8125,"km/h"},
    {J1939_PGN_EBC2,0x00,"FrontAxleSpeed",0,16,0,0.00390625,0,0,250.996,"km/h"},
    //VD
    {J1939_PGN_VD,0x00,"TotalVehicleDistance",32,32,0,0.125,0,0,526385151.9,"km"},
    {J1939_PGN_VD,0x00,"TripDistance",0,32,0,0.125,0,0,526385151.9,"km"},
    //LFC
    {J1939_PGN_LFC,0x00,"EngTotalFuelUsed",32,32,0,0.5,0,0,2105540607.5,"L"},
    {J1939_PGN_LFC,0x00,"EngTripFuel",0,32,0,0.5,0,0,2105540607.5,"L"},
    //DM4
    {J1939_PGN_DM4,0x00,"EngIntakeManifold1PressI",48,8,0,2,0,0,500,"kPa"},
    {J1939_PGN_DM4,0x00,"FreezeFrameLength",0,8,0,1,0,0,255,"Byte"},
    {J1939_PGN_DM4,0x00,"EngSpeedI",56,16,0,0.125,0,0,8031.875,"rpm"},
    {J1939_PGN_DM4,0x00,"EngTorqueModeI",40,4,0,1,0,0,15,""},
    {J1939_PGN_DM4,0x00,"WheelBasedVehicleSpeed",88,16,0,0.00390625,0,0,250.996,"km/h"},
    {J1939_PGN_DM4,0x00,"EngPercentLoadAtCurrentSpeedI",72,8,0,1,0,0,250,"%"},
    {J1939_PGN_DM4,0x00,"EngineCoolantTemperature",80,8,0,1,-40,-40,210,"∞C"},
    //GPM13
    {J1939_PGN_GPM13,0x00,"EngineTorqueMode",0,4,0,1,0,0,15,""},
    {J1939_PGN_GPM13,0x00,"VehicleSpeed",48,16,0,0.00390625,0,0,250.996,"km/hr"},
    {J1939_PGN_GPM13,0x00,"PercentLoadAtCurrentSpeed",40,8,0,1,0,0,125,"%"},
    {J1939_PGN_GPM13,0x00,"EngineSpeed",24,16,0,0.125,0,0,8031.88,"rpm"},
    {J1939_PGN_GPM13,0x00,"ActualEngine_PercTorque",16,8,0,1,-125,-125,125,"%"},
    {J1939_PGN_GPM13,0x00,"DriversDemandEngine_PercTorque",8,8,0,1,-125,-125,125,"%"},
    {J1939_PGN_GPM13,0x00,"EngineRunning",6,2,0,1,0,0,1,""},
    {J1939_PGN_GPM13,0x00,"EngineControlAllowed",4,2,0,1,0,0,1,""},
    //EEC7
    {J1939_PGN_EEC7,0x00,"EngExhstGsRcrculation1Valve2Pos",16,16,0,0.0025,0,0,160.6375,"%"},
    {J1939_PGN_EEC7,0x00,"EngExhstGsRcirculation1ValvePos",0,16,0,0.0025,0,0,160.6375,"%"},
    {J1939_PGN_EEC7,0x00,"EngCrnkcsBrthrOilSeparatorSpeed",32,16,0,1,0,0,64255,"rpm"},
    {J1939_PGN_EEC7,0x00,"CmdedEngIntakeManifoldPress",48,16,0,0.125,0,0,8031.875,"kPa"},
};

static const sOBDII_Signal_t sOBDII_Signals[]
{
    {PID_ENGINE_SPEED, "Engine speed", 0, 16383.75,"rpm"},
    {PID_VEHICLE_SPEED, "Vehicle speed",0,255,"km/h"},
    {PID_FUEL_RAIL_GAUGE_PRESS,"Fuel rail gauge press", 0,655350,"kPa"},
    {PID_INTAKE_MANIFOLD_ABS_PRESS,"Manifold Intake AbsPress",0,255,"kPa"},
    {PID_ABS_BAROMETRIC_PRESS,"AbsBarPress",0,255,"kPa"},
    {PID_REL_THROTTLE_POS,"Relative throttle pos",0,100,"%"},
    {PID_AMB_AIR_TEMP,"Amb air temp",-40,215,"°C"},
    {PID_REL_ACCEL_POS,"Relative accelerator pos",0,100,"%"},
    {PID_ACCEL_POS_D,"Accelerator pos D",0,100,"%"},
    {PID_ACCEL_POS_E,"Accelerator pos E",0,100,"%"},
    {PID_THROTTLE_POS,"Throttle pos",0,100,"%"},
    {PID_ENGINE_OIL_TEMP,"Engine oil temp",-40,210,"°C"},
    {PID_ENGINE_RUNTIME,"Run time since engine start",0,65535,"s"},
    {PID_MAF_RATE,"Mass air flow rate", 0,655.35,"g/s"},
    {PID_CTL_MODULE_VOLT,"Control module voltage",0,65.535,"V"},
    {PID_ENGINE_LOAD,"Engine load",0,100,"%"},
    {PID_COOLANT_TEMP,"Engine coolant temp",-40,215,"°C"},
};

#endif // MAINWINDOW_H
