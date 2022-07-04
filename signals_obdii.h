#ifndef SIGNALS_OBDII_H
#define SIGNALS_OBDII_H
#include <mainwindow.h>

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

typedef enum
{

}eOBDII_Signal_t;

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

#endif // SIGNALS_OBDII_H
