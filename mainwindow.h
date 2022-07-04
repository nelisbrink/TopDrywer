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
#include <QFileDialog>
#include <QFileInfo>

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
    void parseLogExtFrame(double _dTimestamp, quint32 _lCanId, QByteArray _baRxMsg);
    void parseLogStdFrame(double _dTimestamp, quint16 _wCanAddr, QByteArray _baRxMsg);
    void handleReadyRead();
    void handleConnectSerial();
    void clearLiveTable();
    void clearLogTable();
    void checkboxClickedLive(quint16 _wSigIdx);
    void checkboxClickedLog(quint16 _wSigIdx);
    void readVirtualMsg();
    void toggleVirtualRead();
    void decodeOBDIIMsg(quint16 _wPid, QByteArray _baRxMsg);
    void decodeOBDIILogMsg(double _dTimestamp, quint16 _wPid, QByteArray _baRxMsg);
    float mapOBDIIToVal(quint16 _wPid, float _fA, float _fB);
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
    bool bSourceAddressFilter; //True if we want to filter messages based on source addres
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
    MODE_NONE = 0,
    MODE_J1939,
    MODE_OBDII,
    MODE_CANB
}eAppMode_t;


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

#endif // MAINWINDOW_H
