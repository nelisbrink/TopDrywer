#include "mainwindow.h"
#include "ui_mainwindow.h"

//Global variables
QList<QList<double>> qlLogValuesJ1939; //TODO back to float?
QList<QList<double>> qlLogTimestampsJ1939; //TODO back to qint64?
QList<QList<double>> qlLogValuesOBDII;
QList<QList<double>> qlLogTimestampsOBDII;
//Generics to assign to
QFile *fileIn;
QFile *fileOut;
QTimer * timer;
QGridLayout *gridLive;
QGridLayout *gridLog;
quint16 wSignalCountLive = 0U;
quint16 wSignalCountLog = 0U;
QHash<quint16,quint16> qhGraphIdxMap;
//
quint16 wJ1939SignalCount = 0U;
quint16 wOBDIISignalCount = 0U;
quint16 wCANBSignalCount = 0U;
QFile *fileJ1939in;
QFile *fileJ1939out;
QFile *fileOBDIIin;
QFile *fileOBDIIout;
QFile *fileCANBin;
QFile *fileCANBout;
bool bVirtualReadActive = false;
bool bLoggingActive = false;
eAppMode_t eMode = MODE_J1939;
bool bReadingSerialMsg = false;
quint8 bSerialMsgLength = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("TopDrywer V" + QString(APP_VERSION));

    //For Windows TODO test
    QIcon *icon = new QIcon("../../../../TopDrywer/MyIcon.icns");
    setWindowIcon(*icon);

    //New instance of serial
    serial = new QSerialPort(this);

    //Display available serial ports
    Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts())
    {
        ui->cbSerialPorts->addItem(port.portName());
    }

    //Get the total signal counts
    wJ1939SignalCount = sizeof(sJ1939_Signals)/sizeof(sJ1939_Signal_t);
    wOBDIISignalCount = sizeof(sOBDII_Signals)/sizeof(sOBDII_Signal_t);
    wCANBSignalCount = 0; //TODO

    //Declare csv_files
    timer = new QTimer(this);

    fileJ1939in = new QFile("../../../../TopDrywer/example_logs/j1939_log_example.csv");
    fileOBDIIin = new QFile("../../../../TopDrywer/example_logs/obdii_log_example.csv");
    fileCANBin = new QFile("../../../../TopDrywer/example_logs/canb_log_example.csv");
    fileJ1939out = new QFile("../../../../TopDrywer/logs_out/" + QString(APP_VERSION) + "j1939_log_out.csv");
    fileOBDIIout = new QFile("../../../../TopDrywer/logs_out/" + QString(APP_VERSION) + "obdii_log_out.csv");
    fileCANBout = new QFile("../../../../TopDrywer/logs_out/" + QString(APP_VERSION) + "canb_log_out.csv");

    //Set up Activate checkboxes
    ui->cbActivate->addItem("J1939");
    ui->cbActivate->addItem("OBDII");
    ui->cbActivate->addItem("CANB");

    //Set up graphs
    ui->widgetPlotLive->xAxis->setLabel("Timestamp");
    ui->widgetPlotLive->xAxis->setTickLabelRotation(90);
    QSharedPointer<QCPAxisTickerDateTime> dateTimeTicker2(new QCPAxisTickerDateTime);
    ui->widgetPlotLive->xAxis->setTicker(dateTimeTicker2);
    dateTimeTicker2->setDateTimeFormat("h:m:s.z");

    ui->widgetPlotLog->xAxis->setLabel("Timestamp");
    ui->widgetPlotLog->xAxis->setTickLabelRotation(90);
    QSharedPointer<QCPAxisTickerDateTime> dateTimeTicker(new QCPAxisTickerDateTime);
    ui->widgetPlotLog->xAxis->setTicker(dateTimeTicker);
    dateTimeTicker->setDateTimeFormat("h:m:s.z");

    gridLive = new QGridLayout;
    ui->scrollAreaLiveWidget->setLayout(gridLive);

    handleActivate(); //Call the default which is J1939

    //Connect signals and slots
    connectSignalsAndSlots();
}

MainWindow::~MainWindow()
{
    closeSerialPort();
    //If files were still open
    fileJ1939in->close();
    fileOBDIIin->close();
    fileCANBin->close();
    fileJ1939out->close();
    fileOBDIIout->close();
    fileCANBout->close();

    delete ui;
}

void MainWindow::populateLiveTable()
{
    //Clear the grid
    while(gridLive->count())
    {
        QWidget* widget = gridLive->itemAt(0)->widget();
        if(widget) {
            gridLive->removeWidget(widget);
            delete widget;
        }
    }
    //Populate the grid with dummy values
    for(quint16 i=0;i<wSignalCountLive;i++)
    {
        QCheckBox *sigCheck = new QCheckBox("", ui->scrollAreaLiveWidget);
        QLineEdit *sigVal = new QLineEdit(0,ui->scrollAreaLiveWidget);
        QLabel *sigUnits = new QLabel("",ui->scrollAreaLiveWidget);
        gridLive->addWidget(sigCheck,i,0);
        gridLive->addWidget(sigVal,i,1);
        gridLive->addWidget(sigUnits,i,2);
        //Connect all the signals' checkbox events
        connect(sigCheck,&QCheckBox::stateChanged,this, [=](){
            checkboxClicked(i);
        });
    }
}

void MainWindow::populateLogTable()
{
    gridLog = new QGridLayout;
    //Populate the grid with dummy values
    for(quint16 i=0;i<wSignalCountLog;i++)
    {
        QCheckBox *sigCheck = new QCheckBox("", ui->scrollAreaLogWidget);
        gridLog->addWidget(sigCheck,i,0);
    }
//    logGrid->setColumnMinimumWidth(2,80); //TODO do this with a stretch
    ui->scrollAreaLiveWidget->setLayout(gridLog);
}

void MainWindow::connectSignalsAndSlots()
{
    connect(ui->pbOpenSerial,&QPushButton::pressed,this,&MainWindow::handleConnectSerial);
    connect(serial, &QSerialPort::readyRead, this, &MainWindow::handleReadyRead);

    connect(ui->pbVirtualRead,&QPushButton::clicked, this, &MainWindow::toggleVirtualRead);
    connect(timer, &QTimer::timeout, this, [=](){
        readVirtualMsg();
    });
    connect(ui->cbActivate,&QComboBox::textActivated, this, &MainWindow::handleActivate);
    connect(ui->pbLog,&QPushButton::clicked, this, &MainWindow::handleLog);
    connect(ui->pbLogFile, &QPushButton::clicked, this, &MainWindow::selectLogFile);
}

void::MainWindow::handleLog()
{
    bLoggingActive = !bLoggingActive;
    if(bLoggingActive)
    {
        ui->pbLog->setText("Stop");
        if(fileOut->open(QFile::WriteOnly | QFile::Truncate))
        {
            QTextStream stream(fileOut);
            stream << "Timestamp," << "CanId," << "DataBytes" << "\n";
        }
        else
        {
            QMessageBox::critical(this, tr("Error"), fileOut->errorString());
        }
    }
    else
    {
        ui->pbLog->setText("Start");
        fileOut->close();
    }
}

void MainWindow::handleActivate()
{
    QByteArray qbCmd;

    //Stop the virtual read
    ui->pbVirtualRead->setText("Start");
    timer->stop();
    if(fileIn)
    {
        fileIn->close();
    }
    //Stop the logging
    ui->pbLog->setText("Start");
    if(fileOut)
    {
        fileOut->close();
    }
    if(ui->cbActivate->currentText() == "OBDII")
    {
        ui->tabWidgetMode->setTabText(0, "OBDII");
        qbCmd = "O"; //This is letter O (0x4F) for OBDII not zero
        eMode = MODE_OBDII;
        fileIn = fileOBDIIin;
        fileOut = fileOBDIIout;
        timer->setInterval(40); //TODO change when using own log file, then 10ms or whatever we set in the serialCANrelay
        wSignalCountLive = wOBDIISignalCount;
    }
    else if(ui->cbActivate->currentText() == "CANB")
    {
        ui->tabWidgetMode->setTabText(0, "CANB");
        qbCmd = "C"; //This is letter C (0x43) for CANB not zero
        eMode = MODE_CANB;
        fileIn = fileCANBin;
        fileOut = fileCANBout;
        timer->setInterval(40);
        wSignalCountLive = wCANBSignalCount;
    }
    else //J1939
    {
        ui->tabWidgetMode->setTabText(0, "J1939");
        qbCmd = "J"; //This is letter J (0x4A) for J1939
        eMode = MODE_J1939;
        fileIn = fileJ1939in;
        fileOut = fileJ1939out;
        timer->setInterval(5);
        wSignalCountLive = wJ1939SignalCount;
    }
    if(serial->isOpen())
    {
        if(serial->write(qbCmd) < 1)
        {
            qDebug() << "Failed to write serial command\n";
        }
        else
        {
            qDebug() << "Sent serial command\n";
        }
    }
    //Set up the table
    populateLiveTable();
    //Clear the graph
    ui->widgetPlotLive->clearGraphs();
    qhGraphIdxMap.clear();
}

void MainWindow::toggleVirtualRead()
{
    bVirtualReadActive = !bVirtualReadActive;
    if(bVirtualReadActive)
    {
        ui->pbVirtualRead->setText("Stop");
        if (!fileIn->open(QIODevice::ReadOnly)) {
            qDebug() << fileIn->errorString();
        }
        else
        {
            fileIn->readLine(); //Read the header
        }
        timer->start();
    }
    else
    {
        ui->pbVirtualRead->setText("Start");
        timer->stop();
        fileIn->close();
    }
}

void MainWindow::selectLogFile()
{

}

void MainWindow::openSerialPort()
{
    serial->setPortName(ui->cbSerialPorts->currentText());
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    if (serial->open(QIODevice::ReadWrite))
    {
//        QMessageBox::information(this,tr("Success"),tr("Serial port opened successfully"));
        ui->pbOpenSerial->setText("Disconnect");
        //Read out whole buffer to clean
        serial->readAll();
        //Send the mode over
        handleActivate();
    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());
    }
}

void MainWindow::closeSerialPort()
{
    if(serial->isOpen())
    {
        serial->close();
        ui->pbOpenSerial->setText("Connect");
    }
}

void MainWindow::handleConnectSerial()
{
    if(serial->isOpen()){
        closeSerialPort();
    }
    else{
        openSerialPort();
    }
}

void MainWindow::checkboxClicked(quint16 _wSigIdx)
{
    Qt::CheckState cbState = static_cast<QCheckBox*>(gridLive->itemAtPosition(_wSigIdx,0)->widget())->checkState();
    if(cbState == Qt::Unchecked)
    {
        //Update all the other indexes of still plotted graphs
        quint16 wRemovedGraphIdx = qhGraphIdxMap.value(_wSigIdx);
        foreach(auto key, qhGraphIdxMap.keys())
        {
            if(qhGraphIdxMap[key] > wRemovedGraphIdx)
            {
                qhGraphIdxMap[key] = qhGraphIdxMap.value(key) - 1;
            }
        }
        ui->widgetPlotLive->removeGraph(qhGraphIdxMap.value(_wSigIdx));
        qhGraphIdxMap.remove(_wSigIdx);
        ui->widgetPlotLive->replot();
    }
    else
    {
         qhGraphIdxMap.insert(_wSigIdx, ui->widgetPlotLive->graphCount()); //Index before we increment with addGraph
         ui->widgetPlotLive->addGraph(); //Now add the graph at this index
    }
}

void MainWindow::handleReadyRead()
{
    quint8 bytesAvailable = serial->bytesAvailable();
    if(!bReadingSerialMsg && bytesAvailable)
    {
        bSerialMsgLength = serial->read(1U)[0]; //Assume perfect comms, first byte always length of bytes to follow
        bReadingSerialMsg = true;
        if(bytesAvailable > bSerialMsgLength) //Bigger, not equal because we already read 1
        {
            QByteArray baRxMsg = serial->read(bSerialMsgLength);
            bReadingSerialMsg = false;
            parseRxMsg(bSerialMsgLength, baRxMsg);
        }
    }
    else if(bReadingSerialMsg && (bytesAvailable >= bSerialMsgLength))
    {
        QByteArray baRxMsg = serial->read(bSerialMsgLength);
        bReadingSerialMsg = false;
        parseRxMsg(bSerialMsgLength, baRxMsg);
    }
}

void MainWindow::readVirtualMsg()
{
    if(!fileIn->atEnd())
    {
        QList<QByteArray> line = fileIn->readLine().split(';');
        QByteArray qbCanId = QByteArray::fromHex(line[2]);
        QByteArray qbData = QByteArray::fromHex(line[9]);
        quint32 lCanId;
        quint16 wCanId;
        if(eMode == MODE_J1939) //TODO this will exclude CANB data that is actually extended
        {
            quint32 lCanId = qFromBigEndian<quint32>(qbCanId);
            parseExtFrame(lCanId, qbData);
        }
        else
        {
            quint16 wCanId = qFromBigEndian<quint16>(qbCanId);
            parseStdFrame(wCanId, qbData);
        }
    }
}

void MainWindow::parseRxMsg(quint8 bMsgLen, QByteArray _baRxMsg)
{
    quint8 dataLen = bMsgLen - 4U - 1U; //Remove Can ID and ext bit
    //Print rx message
    ui->pteTerminal->appendPlainText(_baRxMsg.toHex(' ').toUpper());
    //Parse rx message
    quint8 bExt = _baRxMsg[0]; //Is this an extended message or standard frame
    if(bExt) //Extended frame like J1393
    {
        quint32 lCanId = qFromLittleEndian<quint32>(_baRxMsg.sliced(1,4));
        QByteArray baRxData = _baRxMsg.sliced(5,dataLen);
        parseExtFrame(lCanId, baRxData);
    }
    else //Standard frame like OBDII
    {
        quint16 wCanAddr = (qFromLittleEndian<quint32>(_baRxMsg.sliced(1,4)) & 0xFFFF);
        QByteArray baRxData = _baRxMsg.sliced(5,dataLen);
        parseStdFrame(wCanAddr, baRxData);
    }
}

void MainWindow::logMsg(bool bExt, quint32 _lCanId, QByteArray _baRxMsg)
{
    QTextStream stream(fileOut);
    double timeNow = (double)QDateTime::currentMSecsSinceEpoch()/1000.0;
    stream << QString::number(timeNow, 'f', 6) << "," << QString::number(_lCanId,16).toUpper() << "," << _baRxMsg.toHex(0).toUpper() << "\n";
}

void MainWindow::parseExtFrame(quint32 _lCanId, QByteArray _baRxMsg)
{
    //Save message to file
    if(bLoggingActive)
    {
        logMsg(true, _lCanId, _baRxMsg);
    }
    //Now decode message
    bool messageDecodable = false;
    quint16 wPgn = (_lCanId >> 8) & 0xFFFF;
    quint8 bSourceAddress = _lCanId & 0xFF;
    quint64 dlRawVal = qFromLittleEndian<quint64>(_baRxMsg); //Whole message is 64 bits or 8 bytes
        for(quint8 j=0;j<wJ1939SignalCount;j++)
        {
            sJ1939_Signal_t J1939_Sig = sJ1939_Signals[j];
            if((J1939_Sig.wPgn == wPgn) && (J1939_Sig.bSourceAddress == bSourceAddress))
            {
                messageDecodable = true;
                //Take only the part of the message that contains the bits for this signal
                QBitArray btaBitMask(J1939_Sig.bLengthInBits,true);
                quint32 lRawVal = (dlRawVal >> J1939_Sig.bStartBit) & btaBitMask.toUInt32(QSysInfo::LittleEndian); //No single signal is bigger than 32 bits
                float fFinalVal = lRawVal*J1939_Sig.fScale + J1939_Sig.fOffset;
//                qDebug() << J1939_Sig.cName << ":" << fFinalVal << J1939_Sig.cUnits << "\n";
                //First append to bigger list till desired index
                while(qlLogValuesJ1939.length() <= j)
                {
                    QList<double> emptyList;
                    qlLogValuesJ1939.append(emptyList);
                    QList<double> emptyList2;
                    qlLogTimestampsJ1939.append(emptyList2);
                }
                //TODO add below if the rolling buffer if we, so the size does not grow
                if(qlLogValuesJ1939[j].length() > bLOG_LEN_MAX) //Rolling buffer, if bigger then remove value from start
                {
                    qlLogValuesJ1939[j].takeFirst();//Remove element at start
                    qlLogTimestampsJ1939[j].takeFirst();
                }
                //Now append to smaller list inside bigger list
                qlLogValuesJ1939[j].append(fFinalVal);
                qlLogTimestampsJ1939[j].append(static_cast<double>(QDateTime::currentMSecsSinceEpoch())/1000.0);
                //Update the GUI with new data
                static_cast<QCheckBox*>(gridLive->itemAtPosition(j,0)->widget())->setText(J1939_Sig.cName);
                static_cast<QLineEdit*>(gridLive->itemAtPosition(j,1)->widget())->setText(QString::number(fFinalVal));
                static_cast<QLabel*>(gridLive->itemAtPosition(j,2)->widget())->setText(J1939_Sig.cUnits);
                ui->scrollAreaLiveWidget->update(); //To prevent strange screen tearing bug when scrolling
                if(qhGraphIdxMap.contains(j))
                {
                   ui->widgetPlotLive->graph(qhGraphIdxMap.value(j))->setData(qlLogTimestampsJ1939[j],qlLogValuesJ1939[j]);
                   ui->widgetPlotLive->yAxis->setLabel(J1939_Sig.cUnits);
                   ui->widgetPlotLive->xAxis->setRange(qlLogTimestampsJ1939[j].first(), qlLogTimestampsJ1939[j].last());
                   ui->widgetPlotLive->yAxis->rescale(); //Basically an autoscale
                   ui->widgetPlotLive->replot();
                }
            }
        }
        if(!messageDecodable)
        {
            QString hexCanId = QString::number( _lCanId, 16 );
            qDebug() << "Unknown extended frame message" << "CAN ID:" << hexCanId <<"\n";
        }
}

void MainWindow::parseStdFrame(quint16 _wCanAddr, QByteArray _baRxMsg)
{
    //Save message to file
    if(bLoggingActive)
    {
        logMsg(false, _wCanAddr, _baRxMsg);
    }
    qDebug() << QString::number(_wCanAddr,16) << _baRxMsg.toHex() << "\n";
    //We request from service 0x01 and 0x22
    if(_wCanAddr == 0x7E8) //ECU replying to our request
    {
        quint8 serviceCode = _baRxMsg[1];
        if(serviceCode == 0x41) //Current data 0x01 + 0x40 (see Wikipedia OBDII)
        {
            quint8 dataBytes = _baRxMsg[0] - 2; //Minus service code and 1 byte pid code
            QByteArray qbPid;
            qbPid.append(_baRxMsg[2]);//PID here is actually just 1 byte, sending as 2 to fit service 0x22 PIDs too
            qbPid.append((quint8)0x00);
            quint16 wPid = qFromLittleEndian<quint16>(qbPid);;
            decodeOBIIMsg(wPid, _baRxMsg.sliced(3,dataBytes));
        }
        else if(0x62) //Proprietary extended mode 0x22 + 0x40 (see Wikipedia OBDII)
        {
            quint8 dataBytes = _baRxMsg[0] - 3; //Minus service code and 2 byte pid code
            quint16 wPid = qFromLittleEndian<quint16>(_baRxMsg.sliced(2,2));
            decodeOBIIMsg(wPid, _baRxMsg.sliced(4,dataBytes));
        }
        else //Other services like diagnostics 05 etc.
        {
            //TODO
        }
    }
    else if(_wCanAddr == 0x7DF)
    {
        qDebug() << "Own broadcast 0x7DF msg\n";
    }
    else //Sensors sending their own data on CAN, not responses to our requests
    {
        decodePropMsg(_wCanAddr, _baRxMsg);
    }
}

void MainWindow::decodeOBIIMsg(quint16 _wPid, QByteArray _baRxMsg)
{
//    qDebug() << _baRxMsg.toHex();
    bool messageDecodable = false;
    float fA = (quint8)_baRxMsg[0]; //Force unsigned, otherwise it can intepret as int
    float fB = 0.0;
    if(_baRxMsg.count() > 1U)
    {
        fB = (quint8)_baRxMsg[1];  //Force unsigned, otherwise it can intepret as int
    }
    float fFinalVal = 0;
    quint16 wSignalIdx;
    for(quint16 i = 0U; i < wOBDIISignalCount;i++)
    {
        if(sOBDII_Signals[i].wPid == _wPid)
        {
            wSignalIdx = i;
            messageDecodable = true;
            break; //Out of for loop
        }
    }
    switch (_wPid) {
    case PID_ENGINE_SPEED:
    {
        fFinalVal = (fA*256.0 +fB)/4.0; //0|16383.75
    }
    break;
    case PID_VEHICLE_SPEED:
    {
        fFinalVal = fA; //0|255
    }
    break;
    case PID_ENGINE_LOAD:
    {
        fFinalVal = fA/255.0; //0|100
    }
    break;
    case PID_COOLANT_TEMP:
    {
        fFinalVal = fA - 40.0; //-40|215
    }
    break;
    case PID_INTAKE_MANIFOLD_ABS_PRESS:
    {
        fFinalVal = fA; //0|255
    }
    break;
    case PID_ABS_BAROMETRIC_PRESS:
    {
        fFinalVal = fA; //0|255
    }
    break;
    case PID_AMB_AIR_TEMP:
    {
        fFinalVal = fA - 40.0; //-40|215
    }
    break;
    case PID_ACCEL_POS_D:
    {
        fFinalVal = fA*(100.0/255.0); //0|100
    }
    break;
    case PID_ACCEL_POS_E:
    {
        fFinalVal = fA*(100.0/255.0); //0|100
    }
    break;
    case PID_THROTTLE_POS:
    {
        fFinalVal = fA*(100.0/255.0); //0|100
    }
    break;
    case PID_ENGINE_OIL_TEMP:
    {
        fFinalVal = fA - 40.0; //-40|210
    }
    break;
    case PID_ENGINE_RUNTIME:
    {
        fFinalVal = fA*256.0 + fB;
    }
    break;
    case PID_MAF_RATE:
    {
        fFinalVal = (fA*256.0 +fB)/100.0; //0|16383.75
    }
    break;
    case PID_FUEL_RAIL_GAUGE_PRESS:
    {
        fFinalVal = 10.0*(fA*256.0 +fB); //0|655350
    }
    break;
    case PID_CTL_MODULE_VOLT:
    {
        fFinalVal = (fA*256.0 +fB)/1000.0; //0|65.535
    }
    case PID_REL_THROTTLE_POS:
    {
        fFinalVal = (100.0/255.0)*fA; //0|100
    }
    break;
    case PID_REL_ACCEL_POS:
    {
        fFinalVal = (100.0/255.0)*fA; //0|100
    }
    break;
    default:
        break;
    }
    if(messageDecodable)
    {
        sOBDII_Signal_t OBDII_Sig = sOBDII_Signals[wSignalIdx];
        qDebug() << OBDII_Sig.cName << fFinalVal << OBDII_Sig.cUnits << "\n";
        //First append to bigger list till desired index
        while(qlLogValuesOBDII.length() <= wSignalIdx)
        {
            QList<double> emptyList;
            qlLogValuesOBDII.append(emptyList);
            QList<double> emptyList2;
            qlLogTimestampsOBDII.append(emptyList2);
        }
        //Rolling buffer
        if(qlLogValuesOBDII[wSignalIdx].length() > bLOG_LEN_MAX) //Rolling buffer, if bigger then remove value from start
        {
            qlLogValuesOBDII[wSignalIdx].takeFirst();//Remove element at start
            qlLogTimestampsOBDII[wSignalIdx].takeFirst();
        }
        qlLogValuesOBDII[wSignalIdx].append(fFinalVal);
        qlLogTimestampsOBDII[wSignalIdx].append(static_cast<double>(QDateTime::currentMSecsSinceEpoch())/1000.0);
        //Update the GUI with new data
        static_cast<QCheckBox*>(gridLive->itemAtPosition(wSignalIdx,0)->widget())->setText(OBDII_Sig.cName);
        static_cast<QLineEdit*>(gridLive->itemAtPosition(wSignalIdx,1)->widget())->setText(QString::number(fFinalVal));
        static_cast<QLabel*>(gridLive->itemAtPosition(wSignalIdx,2)->widget())->setText(OBDII_Sig.cUnits);
        ui->scrollAreaLiveWidget->update(); //To prevent strange screen tearing bug when scrolling
        if(qhGraphIdxMap.contains(wSignalIdx))
        {
           ui->widgetPlotLive->graph(qhGraphIdxMap.value(wSignalIdx))->setData(qlLogTimestampsOBDII[wSignalIdx],qlLogValuesOBDII[wSignalIdx]);
           ui->widgetPlotLive->yAxis->setLabel(OBDII_Sig.cUnits);
           ui->widgetPlotLive->xAxis->setRange(qlLogTimestampsOBDII[wSignalIdx].first(), qlLogTimestampsOBDII[wSignalIdx].last());
           ui->widgetPlotLive->yAxis->rescale(); //Basically an autoscale
           ui->widgetPlotLive->replot();
        }
    }
    else
    {
        qDebug() << QString::number( _wPid, 16 )  << "\n";
        qDebug() << "OBDII signal not decodable";
    }

}

void MainWindow::decodePropMsg(quint16 _wCanAddr, QByteArray _baRxMsg)
{
    qDebug() << "Proprietary message\n";
}
