#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "signals_j1939.h"
#include "signals_obdii.h"

//Global variables
QList<QList<double>> logValuesLiveJ1939; //TODO back to float?
QList<QList<double>> logTimestampsLiveJ1939; //TODO back to qint64?
QList<QList<double>> logValuesLiveOBDII;
QList<QList<double>> logTimestampsLiveOBDII;
QList<QList<double>> logValuesLog;
QList<QList<double>> logTimestampsLog;
//Generics to assign to
QFile *fileInLive;
QFile *fileOut;
QTimer * timer;
QGridLayout *gridLive;
QGridLayout *gridLog;
QGridLayout *gridCmd;
quint16 wSignalCountLive = 0U;
quint16 wSignalCountLog = 0U;
QHash<quint16,quint16> graphIdxMapLive;
QHash<quint16,quint16> graphIdxMapLog;
QHash<quint16,quint16> sigIdxMapLive;
QHash<quint16,quint16> sigIdxMapLog;
//
quint16 wJ1939SignalCount = 0U;
quint16 wOBDIISignalCount = 0U;
quint16 wCANBSignalCount = 0U;
QFile *fileJ1939in;
QFile *fileOBDIIin;
QFile *fileCANBin;
bool bVirtualReadActive = false;
bool bLoggingActive = false;
eAppMode_t eMode = MODE_J1939;
bool bReadingSerialMsg = false;
quint8 bSerialMsgLength = 0;
quint8 bMsgTxCounter = 0U; //For commands like TSC1

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

    //    fileJ1939in = new QFile("../../../../TopDrywer/example_logs/j1939_log_example.csv");
    //    fileOBDIIin = new QFile("../../../../TopDrywer/example_logs/obdii_log_example.csv");
    //    fileCANBin = new QFile("../../../../TopDrywer/example_logs/canb_log_example.csv");

    fileInLive = new QFile("../../../../TopDrywer/example_logs/Volvo2_CAN.csv");
    QFileInfo fileInfoGlob(*fileInLive);
    QString fileNameShort(fileInfoGlob.fileName());
    ui->pbVirtualFile->setText(fileNameShort);

    //Set up Activate checkboxes
    ui->cbActivate->addItem("J1939");
    ui->cbActivate->addItem("OBDII");
    ui->cbActivate->addItem("CANB");

    ui->cbCmd->addItem("TSC1");

    //Set up graphs
    ui->widgetPlotLive->xAxis->setLabel("Timestamp");
    ui->widgetPlotLive->xAxis->setTickLabelRotation(90);
    ui->widgetPlotLive->xAxis2->setTickLabelRotation(90);
    QSharedPointer<QCPAxisTickerDateTime> dateTimeTicker2(new QCPAxisTickerDateTime);
    ui->widgetPlotLive->xAxis->setTicker(dateTimeTicker2);
    ui->widgetPlotLive->xAxis2->setTicker(dateTimeTicker2);
    dateTimeTicker2->setDateTimeFormat("h:m:s.z");
    ui->widgetPlotLive->yAxis->setTickPen(QPen(Qt::blue));
    ui->widgetPlotLive->yAxis2->setTickPen(QPen(Qt::red));
    ui->widgetPlotLive->setInteraction(QCP::iRangeDrag, true);
    ui->widgetPlotLive->setInteraction(QCP::iRangeZoom, true);


    ui->widgetPlotLog->xAxis->setLabel("Timestamp");
    ui->widgetPlotLog->xAxis->setTickLabelRotation(90);
    ui->widgetPlotLog->xAxis2->setTickLabelRotation(90);
    QSharedPointer<QCPAxisTickerDateTime> dateTimeTicker(new QCPAxisTickerDateTime);
    ui->widgetPlotLog->xAxis->setTicker(dateTimeTicker);
    ui->widgetPlotLog->xAxis2->setTicker(dateTimeTicker2);
    dateTimeTicker->setDateTimeFormat("h:m:s.z");
    ui->widgetPlotLog->yAxis->setTickPen(QPen(Qt::blue));
    ui->widgetPlotLog->yAxis2->setTickPen(QPen(Qt::red));
    ui->widgetPlotLog->setInteraction(QCP::iRangeDrag, true);
    ui->widgetPlotLog->setInteraction(QCP::iRangeZoom, true);


    gridLive = new QGridLayout;
    ui->scrollAreaLiveWidget->setLayout(gridLive);

    gridLog = new QGridLayout;
    ui->scrollAreaLogWidget->setLayout(gridLog);

    gridCmd = new QGridLayout;
    ui->groupBoxCmd->setLayout(gridCmd);

    handleActivate(); //Call the default which is J1939

    //Connect signals and slots
    connectSignalsAndSlots();
}

MainWindow::~MainWindow()
{
    closeSerialPort();
    //If files were still open
    if(fileInLive)
    {
        fileInLive->close();
    }
    delete ui;
}

void MainWindow::selectCommand()
{
    //Clear the layout first
    while(gridCmd->count())
    {
        QWidget* widget = gridCmd->itemAt(0)->widget();
        if(widget) {
            gridCmd->removeWidget(widget);
            delete widget;
        }
    }
    //Now repopulate
    switch(ui->cbCmd->currentIndex()){
    case 0: //TSC1
    {


        QLabel * lb0 = new QLabel("EngOverrideCtrlMode", ui->groupBoxCmd);
        QComboBox *cb0 = new QComboBox();
        cb0->addItem("Override disabled", 0b00);
        cb0->addItem("Speed control", 0b01);
        cb0->addItem("Torque control", 0b10);
        cb0->addItem("Speed/torque limit", 0b11);
        cb0->setCurrentIndex(3); //Set limit default

        QLabel * lb1 = new QLabel("EngRqedSpeedCtrlConditions", ui->groupBoxCmd);
        QComboBox *cb1 = new QComboBox();
        cb1->addItem("Trans", 0b00);
        cb1->addItem("Stab nolock", 0b01);
        cb1->addItem("Stab lock1", 0b10);
        cb1->addItem("Stab lock2", 0b11);
        cb1->setCurrentIndex(2); //Set on stab lock1 default

        QLabel * lb2 = new QLabel("OverrideCtrlModePriority", ui->groupBoxCmd);
        QComboBox *cb2 = new QComboBox();
        cb2->addItem("Highest", 0b00);
        cb2->addItem("High", 0b01);
        cb2->addItem("Medium", 0b10);
        cb2->addItem("Low", 0b11);
        cb2->setCurrentIndex(2); //Set on medium default

        QLabel * lb3 = new QLabel("EngRqedSpeed_SpeedLimit", ui->groupBoxCmd);
        QLabel * lb3_1 = new QLabel("rpm", ui->groupBoxCmd);
        QDoubleSpinBox *dsb3 = new QDoubleSpinBox();
        dsb3->setMinimum(0.0);
        dsb3->setMaximum(8031.875);
        dsb3->setValue(8031.875);

        QLabel * lb4 = new QLabel("EngRqedTorque_TorqueLimit", ui->groupBoxCmd);
        QLabel * lb4_1 = new QLabel("%", ui->groupBoxCmd);
        QSpinBox *sb4 = new QSpinBox();
        sb4->setMinimum(0);
        sb4->setMaximum(125);
        sb4->setValue(80);

        QLabel * lb5 = new QLabel("TransmissionRate", ui->groupBoxCmd);
        QComboBox *cb5 = new QComboBox();
        cb5->addItem("10ms",0b111);
        cb5->addItem("20ms", 0b110);
        cb5->addItem("50ms", 0b101);
        cb5->addItem("100ms", 0b100);
        cb5->setCurrentIndex(0); //Default 10ms

        QLabel * lb6 = new QLabel("ControlPurpose", ui->groupBoxCmd);
        QComboBox *cb6 = new QComboBox();
        cb6->addItem("Accel pedal", 0b00000);
        cb6->addItem("Cruise ctl", 0b00001);
        cb6->addItem("PTO gov", 0b00010);
        cb6->addItem("Road Speed gov", 0b00011);
        cb6->addItem("Eng protection", 0b00100);
        cb6->addItem("Temp", 0b11111);
        cb6->setCurrentIndex(5); //Set default to temporary power train control

        QLabel * lb00 = new QLabel("SA", ui->groupBoxCmd);
        QComboBox *cb00 = new QComboBox();
        cb00->addItem("00 - VECU", 0x00);
        cb00->addItem("03 - TECU", 0x03);
        cb00->addItem("11 - OEM contr.", 0x11);
        cb00->addItem("E6 - BBM", 0xE6);
        cb00->addItem("7F", 0x7F);
        cb00->setCurrentIndex(3);

        QLabel * lb01 = new QLabel("DA", ui->groupBoxCmd);
        QComboBox *cb01 = new QComboBox();
        cb01->addItem("00 - VECU", 0x00);
        cb01->addItem("E6 - BBM", 0xE6);
        cb01->setCurrentIndex(0); //Set limit default

        gridCmd->addWidget(lb0, 0, 0);
        gridCmd->addWidget(lb1, 1, 0);
        gridCmd->addWidget(lb2, 2, 0);
        gridCmd->addWidget(lb3, 3, 0);
        gridCmd->addWidget(lb3_1, 3, 2);
        gridCmd->addWidget(lb4, 4, 0);
        gridCmd->addWidget(lb4_1, 4, 2);
        gridCmd->addWidget(lb5, 5, 0);
        gridCmd->addWidget(lb6, 6, 0);
        gridCmd->addWidget(lb00, 7, 0);
        gridCmd->addWidget(lb01, 7, 2);

        gridCmd->addWidget(cb0, 0, 1);
        gridCmd->addWidget(cb1, 1, 1);
        gridCmd->addWidget(cb2, 2, 1);
        gridCmd->addWidget(dsb3, 3, 1);
        gridCmd->addWidget(sb4, 4, 1);
        gridCmd->addWidget(cb5, 5, 1);
        gridCmd->addWidget(cb6, 6, 1);
        gridCmd->addWidget(cb00, 7, 1);
        gridCmd->addWidget(cb01, 7, 3);

        break;
    }
    default:
        break;
    }
}

void MainWindow::sendCommand()
{
    //Build message from selected fields
    QByteArray qbCmd;
    qbCmd.append(0x77); //Indicate to Arduino that we are going to send a message for the CAN bus
    switch(ui->cbCmd->currentIndex()){
    case 0: //TSC1
    {
        bMsgTxCounter++;
        if(bMsgTxCounter > 7)
        {
            bMsgTxCounter = 0U;
        }
        //Sending as from the BBM (Body Builder Module) Source Address 0xE6
        //Append ID reverse order (little endian) (The Russians use 0x0C 0x00 0xE6 0x7F, which I don't understand where they get that)+
        qbCmd.append(static_cast<QComboBox*>(gridCmd->itemAtPosition(7,1)->widget())->currentData().toUInt());
        qbCmd.append(static_cast<QComboBox*>(gridCmd->itemAtPosition(7,3)->widget())->currentData().toUInt());
        qbCmd.append(quint8(0x00));
        qbCmd.append(0x0C);
        //Retrieve data from GUI
        quint8 bEngOverrideCtrlMode = static_cast<QComboBox*>(gridCmd->itemAtPosition(0,1)->widget())->currentData().toUInt();
        quint8 bEngRqedSpeedCtrlConditions = static_cast<QComboBox*>(gridCmd->itemAtPosition(1,1)->widget())->currentData().toUInt();
        quint8 bOverrideCtrlModePriority = static_cast<QComboBox*>(gridCmd->itemAtPosition(2,1)->widget())->currentData().toUInt();
        float fEngRqedSpeed_SpeedLimit = static_cast<QDoubleSpinBox*>(gridCmd->itemAtPosition(3,1)->widget())->value();
        quint8 bEngRqedTorque_TorqueLimit = static_cast<QSpinBox*>(gridCmd->itemAtPosition(4,1)->widget())->value();
        quint8 bTransmissionRate = static_cast<QComboBox*>(gridCmd->itemAtPosition(5,1)->widget())->currentData().toUInt();
        quint8 bControlPurpose = static_cast<QComboBox*>(gridCmd->itemAtPosition(6,1)->widget())->currentData().toUInt();
        //Append data
        quint8 byte0 = bEngOverrideCtrlMode | (bEngRqedSpeedCtrlConditions << 2) | (bOverrideCtrlModePriority << 4);
        quint16 speedLimit = (fEngRqedSpeed_SpeedLimit/0.125);
        quint8 byte1 = speedLimit & 0xFF; //LSB
        quint8 byte2 = (speedLimit >> 8) & 0xFF;
        quint8 byte3 = bEngRqedTorque_TorqueLimit;
        quint8 byte4 = bTransmissionRate | (bControlPurpose << 5);
        qbCmd.append(byte0);
        qbCmd.append(byte1);
        qbCmd.append(byte2);
        qbCmd.append(byte3);
        qbCmd.append(byte4);
        qbCmd.append(0xFF); //EngineRequestedTorqueHiRes
        qbCmd.append(0xFF); //NA
        qbCmd.append(0xFF); //Ignore message counter and checksum for now
        break;
    }
    default:
        break;
    }

    //Send command
    ui->pteTerminal->appendPlainText(qbCmd.toHex(' ').toUpper());
    sendSerial(qbCmd);
}

void MainWindow::clearLiveTable()
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
}

void MainWindow::clearLogTable()
{
    //Clear the grid
    while(gridLog->count())
    {
        QWidget* widget = gridLog->itemAt(0)->widget();
        if(widget) {
            gridLog->removeWidget(widget);
            delete widget;
        }
    }
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
    connect(ui->pbVirtualFile, &QPushButton::clicked, this, &MainWindow::selectVirtualFile);
    connect(ui->cbCmd, &QComboBox::textActivated, this, &MainWindow::selectCommand);
    connect(ui->tabWidgetLiveLogs, &QTabWidget::currentChanged, this, &MainWindow::tabLiveLogCmdChanged);
    connect(ui->pbCmd, &QPushButton::clicked, this, &MainWindow::sendCommand);
}

void MainWindow::tabLiveLogCmdChanged()
{
    if(ui->tabWidgetLiveLogs->currentIndex() == 0) //Live
    {

    }
    else if(ui->tabWidgetLiveLogs->currentIndex() == 1) //Logs
    {

    }
    else //Command
    {
        selectCommand(); //Select the default command
    }
}

void MainWindow::sendSerial(QByteArray _qbCmd)
{
    if(serial->isOpen())
    {
        if(serial->write(_qbCmd) < 1)
        {
            qDebug() << "Failed to write serial command\n";
        }
        else
        {
            qDebug() << "Sent serial command\n";
        }
    }
}

void::MainWindow::handleLog()
{
    bLoggingActive = !bLoggingActive;
    if(bLoggingActive)
    {
        ui->pbLog->setText("Stop");
        QString filename = "../../../../TopDrywer/logs_out/";
        filename.append((eMode == MODE_J1939) ? "j1939" : ((eMode == MODE_OBDII) ? "obdii" : "canb"));
        filename.append("_log_out_" + QString::number(QDateTime::currentSecsSinceEpoch()) +"_v" + QString(APP_VERSION).replace(".","_") + ".csv");
        delete fileOut; // Delete old pointers
        fileOut = new QFile(filename);
        if(fileOut->open(QFile::WriteOnly | QFile::Truncate))
        {
            QTextStream stream(fileOut);
            stream << "Timestamp," << "CanId," << "IDE," << "DataBytes"; //IDE stands for ID EXTENDED
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
    bVirtualReadActive = false;
    ui->pbVirtualRead->setText("Start");
    timer->stop();
    if(fileInLive)
    {
        fileInLive->close();
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
        //        fileInLive = fileOBDIIin;
        timer->setInterval(40); //TODO change when using own log file, then 10ms or whatever we set in the serialCANrelay
        wSignalCountLive = wOBDIISignalCount;
    }
    else if(ui->cbActivate->currentText() == "CANB")
    {
        ui->tabWidgetMode->setTabText(0, "CANB");
        qbCmd = "C"; //This is letter C (0x43) for CANB not zero
        eMode = MODE_CANB;
        //        fileInLive = fileCANBin;
        timer->setInterval(40);
        wSignalCountLive = wCANBSignalCount;
    }
    else //J1939
    {
        ui->tabWidgetMode->setTabText(0, "J1939");
        qbCmd = "J"; //This is letter J (0x4A) for J1939
        eMode = MODE_J1939;
        //        fileInLive = fileJ1939in;
        timer->setInterval(5);
        wSignalCountLive = wJ1939SignalCount;
    }
    sendSerial(qbCmd);
    //Set up the table
    clearLiveTable();
    //Clear the graphs
    ui->widgetPlotLive->clearGraphs();
    graphIdxMapLive.clear();
    sigIdxMapLive.clear();
}

void MainWindow::selectVirtualFile()
{
    //Stop the virtual read
    bVirtualReadActive = false;
    ui->pbVirtualRead->setText("Start");
    timer->stop();
    if(fileInLive)
    {
        fileInLive->close();
    }

    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Log file"), "/Users/nelissimo/Library/CloudStorage/OneDrive-SharedLibraries-CreoX/CreoX - 029_TOPDRYWER/Code/TopDrywer/TopDrywer/example_logs/", tr("CSV Files (*.csv)"));
    delete fileInLive; //Delete old pointer
    fileInLive = new QFile(fileName);

    //Set button text
    QFileInfo fileInfo(*fileInLive);
    QString fileNameShort(fileInfo.fileName());
    ui->pbVirtualFile->setText(fileNameShort);

    //Set up the table
    clearLiveTable();
    //Clear the graphs
    ui->widgetPlotLive->clearGraphs();
    graphIdxMapLive.clear();
    sigIdxMapLive.clear();
    timer->start();
}

void MainWindow::toggleVirtualRead()
{
    bVirtualReadActive = !bVirtualReadActive;
    if(bVirtualReadActive)
    {
        clearLiveTable();
        //Clear the graphs
        ui->widgetPlotLive->clearGraphs();
        graphIdxMapLive.clear();
        sigIdxMapLive.clear();
        ui->pbVirtualRead->setText("Stop");
        if (!fileInLive->open(QIODevice::ReadOnly)) {
            qDebug() << fileInLive->errorString();
        }
        else
        {
            fileInLive->readLine(); //Read the header
        }
        timer->start();
    }
    else
    {
        ui->pbVirtualRead->setText("Start");
        timer->stop();
        fileInLive->close();
    }
}

void MainWindow::selectLogFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open Log file"), "/Users/nelissimo/Library/CloudStorage/OneDrive-SharedLibraries-CreoX/CreoX - 029_TOPDRYWER/Code/TopDrywer/TopDrywer/logs_out/", tr("CSV Files (*.csv)"));
    QFile *file = new QFile(fileName);
    if (!file->open(QIODevice::ReadOnly)) {
        qDebug() << file->errorString();
    }
    else
    {
        ui->pbLogFile->hide();
        QProgressBar *progressBar = new QProgressBar();
        ui->groupBoxFile->layout()->addWidget(progressBar);
        QList<QByteArray> completeLogs = file->readAll().split('\n'); //Read the header too
        quint32 lineCount = completeLogs.count();
        QByteArray qbCanId;
        QByteArray qbData;
        double dTimestamp;
        bool bExt;
        quint32 lCanId;
        quint16 wCanId;
        clearLogTable();
        for(quint32 i = 1U;i < lineCount;i++) //NOTE Assuming less than 65535 signals in a csv file we logged, starting at 1 to skip the header
        {
            QList<QByteArray> line = completeLogs[i].split(',');
            dTimestamp = line[0].toDouble();
            qbCanId = QByteArray::fromHex(line[1]);
            bExt = line[2].toUShort();
            qbData = QByteArray::fromHex(line[3]);
            if(bExt)
            {
                lCanId = qFromBigEndian<quint32>(qbCanId);
                parseLogExtFrame(dTimestamp, lCanId, qbData);
            }
            else
            {
                wCanId = qFromBigEndian<quint16>(qbCanId);
                parseLogStdFrame(dTimestamp, wCanId, qbData);
            }
            progressBar->setValue((i+1)/completeLogs.count());
        }
        //Update the GUI with new data
        quint16 wSignalCountLocal = (eMode == MODE_J1939) ? wJ1939SignalCount : wOBDIISignalCount;
        for(quint32 i = 0U;i < wSignalCountLocal;i++) //NOTE Assuming less than 65535 signals in a csv file we logged, starting at 1 to skip the header
        {
            if(sigIdxMapLog.contains(i))
            {
                QCheckBox *sigCheck = new QCheckBox((eMode == MODE_J1939) ? (sJ1939_Signals[i].cName) : (sOBDII_Signals[i].cName), ui->scrollAreaLogWidget);
                gridLog->addWidget(sigCheck,sigIdxMapLog[i],0);
                connect(sigCheck,&QCheckBox::stateChanged,this, [=](){
                    checkboxClickedLog(sigIdxMapLog[i]);
                });
            }
        }
        //
        ui->groupBoxFile->layout()->removeWidget(progressBar);
        delete progressBar;
        ui->pbLogFile->show();
        QFileInfo fileInfo(*file);
        QString filename(fileInfo.fileName());
        ui->pbLogFile->setText(filename);
        file->close();
        delete file;
    }
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

void MainWindow::checkboxClickedLive(quint16 _wSigIdx)
{
    Qt::CheckState cbState = static_cast<QCheckBox*>(gridLive->itemAtPosition(_wSigIdx,0)->widget())->checkState();
    if(cbState == Qt::Unchecked)
    {
        //Update all the other indexes of still plotted graphs
        quint16 wRemovedGraphIdx = graphIdxMapLive.value(_wSigIdx);
        foreach(auto key, graphIdxMapLive.keys())
        {
            if(graphIdxMapLive[key] > wRemovedGraphIdx)
            {
                graphIdxMapLive[key] = graphIdxMapLive.value(key) - 1;
            }
        }
        ui->widgetPlotLive->removeGraph(graphIdxMapLive.value(_wSigIdx));
        graphIdxMapLive.remove(_wSigIdx);
        if(ui->widgetPlotLive->graphCount() < 2)
        {
            ui->widgetPlotLive->xAxis2->setVisible(false);
            ui->widgetPlotLive->yAxis2->setVisible(false);
        }
        ui->widgetPlotLive->replot();
    }
    else
    {
        graphIdxMapLive.insert(_wSigIdx, ui->widgetPlotLive->graphCount()); //Index before we increment with addGraph
        if(ui->widgetPlotLive->graphCount() == 0)
        {
            ui->widgetPlotLive->addGraph(ui->widgetPlotLive->xAxis, ui->widgetPlotLive->yAxis); //Now add the graph at this index
            ui->widgetPlotLive->graph(ui->widgetPlotLive->graphCount() - 1)->setPen(QPen(Qt::blue));
        }
        else //Add to second axis
        {
            ui->widgetPlotLive->addGraph(ui->widgetPlotLive->xAxis2, ui->widgetPlotLive->yAxis2); //Now add the graph at this index
            ui->widgetPlotLive->graph(ui->widgetPlotLive->graphCount() - 1)->setPen(QPen(Qt::red));
            ui->widgetPlotLive->xAxis2->setVisible(true);
            ui->widgetPlotLive->yAxis2->setVisible(true);
        }
    }
}

void MainWindow::checkboxClickedLog(quint16 _wSigIdx)
{
    Qt::CheckState cbState = static_cast<QCheckBox*>(gridLog->itemAtPosition(_wSigIdx,0)->widget())->checkState();
    if(cbState == Qt::Unchecked)
    {
        //Update all the other indexes of still plotted graphs
        quint16 wRemovedGraphIdx = graphIdxMapLog.value(_wSigIdx);
        foreach(auto key, graphIdxMapLog.keys())
        {
            if(graphIdxMapLog[key] > wRemovedGraphIdx)
            {
                graphIdxMapLog[key] = graphIdxMapLog.value(key) - 1;
            }
        }
        ui->widgetPlotLog->removeGraph(graphIdxMapLog.value(_wSigIdx));
        graphIdxMapLog.remove(_wSigIdx);
        if(ui->widgetPlotLog->graphCount() < 2)
        {
            ui->widgetPlotLog->xAxis2->setVisible(false);
            ui->widgetPlotLog->yAxis2->setVisible(false);
        }
        ui->widgetPlotLog->replot();
    }
    else
    {
        graphIdxMapLog.insert(_wSigIdx, ui->widgetPlotLog->graphCount()); //Index before we increment with addGraph
        if(ui->widgetPlotLog->graphCount() == 0)
        {
            ui->widgetPlotLog->addGraph(ui->widgetPlotLog->xAxis, ui->widgetPlotLog->yAxis); //Now add the graph at this index
            ui->widgetPlotLog->graph(ui->widgetPlotLog->graphCount() - 1)->setPen(QPen(Qt::blue));
        }
        else
        {
            ui->widgetPlotLog->addGraph(ui->widgetPlotLog->xAxis2, ui->widgetPlotLog->yAxis2); //Now add the graph at this index
            ui->widgetPlotLog->graph(ui->widgetPlotLog->graphCount() - 1)->setPen(QPen(Qt::red));
            ui->widgetPlotLog->xAxis2->setVisible(true);
            ui->widgetPlotLog->yAxis2->setVisible(true);
        }
        //Update graph
        ui->widgetPlotLog->graph(graphIdxMapLog.value(_wSigIdx))->setData(logTimestampsLog[_wSigIdx],logValuesLog[_wSigIdx]);
        ui->widgetPlotLog->graph(graphIdxMapLog.value(_wSigIdx))->setName(eMode == MODE_J1939 ? sJ1939_Signals[sigIdxMapLog.key(_wSigIdx)].cName : sOBDII_Signals[sigIdxMapLog.key(_wSigIdx)].cName);
        QString cYAxisName = "";
        cYAxisName.append((eMode == MODE_J1939) ? sJ1939_Signals[sigIdxMapLog.key(_wSigIdx)].cName : sOBDII_Signals[sigIdxMapLog.key(_wSigIdx)].cName);
        cYAxisName.append(" ");
        cYAxisName.append((eMode == MODE_J1939) ? sJ1939_Signals[sigIdxMapLog.key(_wSigIdx)].cUnits : sOBDII_Signals[sigIdxMapLog.key(_wSigIdx)].cUnits);
        if(graphIdxMapLog.value(_wSigIdx) == 0) //If first graph
        {
            ui->widgetPlotLog->yAxis->setLabel(cYAxisName);
            ui->widgetPlotLog->xAxis->setRange(logTimestampsLog[_wSigIdx].first(),logTimestampsLog[_wSigIdx].last());
            ui->widgetPlotLog->yAxis->rescale(); //Basically an autoscale
        }
        else
        {
            ui->widgetPlotLog->yAxis2->setLabel(cYAxisName);
            ui->widgetPlotLog->xAxis2->setRange(logTimestampsLog[_wSigIdx].first(),logTimestampsLog[_wSigIdx].last());
            ui->widgetPlotLog->yAxis2->rescale(); //Basically an autoscale
        }

        ui->widgetPlotLog->replot();
    }
}

void MainWindow::handleReadyRead()
{
    quint8 bytesAvailable = serial->bytesAvailable();
    if(!bReadingSerialMsg && bytesAvailable)
    {
        QByteArray qbFirstByte = serial->read(1U);
        bSerialMsgLength = qbFirstByte[0]; //Assume perfect comms, first byte always length of bytes to follow
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
    if(!fileInLive->atEnd())
    {
        QList<QByteArray> line = fileInLive->readLine().split(';');
        QByteArray qbCanId = QByteArray::fromHex(line[2]);
        bool bExt = line[3].toUShort();
        QByteArray qbData = QByteArray::fromHex(line[9]);
        quint32 lCanId;
        quint16 wCanId;
        if(bExt)
        {
            lCanId = qFromBigEndian<quint32>(qbCanId);
            parseExtFrame(lCanId, qbData);
        }
        else
        {
            wCanId = qFromBigEndian<quint16>(qbCanId);
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
    stream << "\n" << QString::number(timeNow, 'f', 6) << "," << QString::number(_lCanId,16).toUpper() << "," << (quint8)bExt << ","  << _baRxMsg.toHex(0).toUpper();
    //If we reach a certain size, start a new file
    if(fileOut->size() >= lMAX_LOG_FILE_SIZE)
    {
        //Call handle log twice to stop and start a new files
        handleLog(); //To stop
        handleLog(); //To start
    }
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
    quint16 wSigIndex;
    for(quint8 j=0;j<wJ1939SignalCount;j++)
    {
        sJ1939_Signal_t J1939_Sig = sJ1939_Signals[j];
        if((J1939_Sig.wPgn == wPgn) && (J1939_Sig.bSourceAddressFilter?(J1939_Sig.bSourceAddress == bSourceAddress):true))
        {
            messageDecodable = true;
            wSigIndex = j;
            //Take only the part of the message that contains the bits for this signal
            QBitArray btaBitMask(J1939_Sig.bLengthInBits,true);
            quint32 lRawVal = (dlRawVal >> J1939_Sig.bStartBit) & btaBitMask.toUInt32(QSysInfo::LittleEndian); //No single signal is bigger than 32 bits
            float fFinalVal = lRawVal*J1939_Sig.fScale + J1939_Sig.fOffset;
            //                qDebug() << J1939_Sig.cName << ":" << fFinalVal << J1939_Sig.cUnits << "\n";

            bool bNewMsgType = false;
            if(!sigIdxMapLive.contains(wSigIndex))
            {
                bNewMsgType = true;
                QList<double> emptyList;
                logValuesLiveJ1939.append(emptyList);
                QList<double> emptyList2;
                logTimestampsLiveJ1939.append(emptyList2);
                sigIdxMapLive.insert(wSigIndex, logValuesLiveJ1939.count() - 1); //Could have used values or timestamps count
            }
            //Keep display buff length to size
            if(logValuesLiveJ1939[sigIdxMapLive[wSigIndex]].length() > bLOG_LEN_MAX) //Rolling buffer, if bigger then remove value from start
            {
                logValuesLiveJ1939[sigIdxMapLive[wSigIndex]].takeFirst();//Remove element at start
                logTimestampsLiveJ1939[sigIdxMapLive[wSigIndex]].takeFirst();
            }
            //Now add
            logValuesLiveJ1939[sigIdxMapLive[wSigIndex]].append(fFinalVal);
            logTimestampsLiveJ1939[sigIdxMapLive[wSigIndex]].append(static_cast<double>(QDateTime::currentMSecsSinceEpoch())/1000.0);

            //Update the GUI with new data
            if(bNewMsgType){
                QCheckBox *sigCheck = new QCheckBox(J1939_Sig.cName, ui->scrollAreaLiveWidget);
                QLineEdit *sigVal = new QLineEdit(QString::number(fFinalVal),ui->scrollAreaLiveWidget);
                sigVal->setMinimumWidth(75);
                QLabel *sigUnits = new QLabel(J1939_Sig.cUnits,ui->scrollAreaLiveWidget);
                gridLive->addWidget(sigCheck,sigIdxMapLive[wSigIndex],0);
                gridLive->addWidget(sigVal,sigIdxMapLive[wSigIndex],1);
                gridLive->addWidget(sigUnits,sigIdxMapLive[wSigIndex],2);
                //Connect all the signals' checkbox events
                connect(sigCheck,&QCheckBox::stateChanged,this, [=](){
                    checkboxClickedLive(sigIdxMapLive[wSigIndex]);
                });
            }
            else //Just update the value
            {
                static_cast<QLineEdit*>(gridLive->itemAtPosition(sigIdxMapLive[wSigIndex],1)->widget())->setText(QString::number(fFinalVal));
            }

            ui->scrollAreaLiveWidget->update(); //To prevent strange screen tearing bug when scrolling
            if(graphIdxMapLive.contains(sigIdxMapLive[wSigIndex]))
            {
                ui->widgetPlotLive->graph(graphIdxMapLive.value(sigIdxMapLive[wSigIndex]))->setData(logTimestampsLiveJ1939[sigIdxMapLive[wSigIndex]],logValuesLiveJ1939[sigIdxMapLive[wSigIndex]]);
                ui->widgetPlotLive->graph(graphIdxMapLive.value(wSigIndex))->setName(J1939_Sig.cName);

                QString cYAxisName = "";
                cYAxisName.append(J1939_Sig.cName);
                cYAxisName.append(" ");
                cYAxisName.append(J1939_Sig.cUnits);
                if(graphIdxMapLive.value(sigIdxMapLive[wSigIndex]) == 0) //If first graph
                {
                    ui->widgetPlotLive->yAxis->setLabel(cYAxisName);
                    ui->widgetPlotLive->xAxis->setRange(logTimestampsLiveJ1939[sigIdxMapLive[wSigIndex]].first(), logTimestampsLiveJ1939[sigIdxMapLive[wSigIndex]].last());
                    ui->widgetPlotLive->yAxis->rescale(); //Basically an autoscale
                }
                else
                {
                    ui->widgetPlotLive->yAxis2->setLabel(cYAxisName);
                    ui->widgetPlotLive->xAxis2->setRange(logTimestampsLiveJ1939[sigIdxMapLive[wSigIndex]].first(), logTimestampsLiveJ1939[sigIdxMapLive[wSigIndex]].last());
                    ui->widgetPlotLive->yAxis2->rescale(); //Basically an autoscale
                }
                ui->widgetPlotLive->replot();
            }
        }
    }
    if(!messageDecodable)
    {
        QString hexCanId = QString::number( _lCanId, 16 );
        qDebug() << "Unknown ext msg" << "CAN ID:" << hexCanId <<"\n";
    }
}

void MainWindow::parseLogExtFrame(double _dTimestamp, quint32 _lCanId, QByteArray _baRxMsg)
{
    //TODO determine first if we the vehicle is on the J1939 standard, the CANB mode can also send its extended proprietary data here
    bool messageDecodable = false;
    quint16 wPgn = (_lCanId >> 8) & 0xFFFF;
    quint8 bSourceAddress = _lCanId & 0xFF;
    quint64 dlRawVal = qFromLittleEndian<quint64>(_baRxMsg); //Whole message is 64 bits or 8 bytes
    quint16 wSigIndex;
    sJ1939_Signal_t J1939_Sig;
    for(quint8 j=0;j<wJ1939SignalCount;j++) //Assume for now that all extended is J1939, TODO expand, the CANB messages that are extended will also come here
    {
        J1939_Sig = sJ1939_Signals[j];
        if((J1939_Sig.wPgn == wPgn) && (J1939_Sig.bSourceAddressFilter?(J1939_Sig.bSourceAddress == bSourceAddress):true))
        {
            messageDecodable = true;
            wSigIndex = j;
            //Take only the part of the message that contains the bits for this signal
            QBitArray btaBitMask(J1939_Sig.bLengthInBits,true);
            quint32 lRawVal = (dlRawVal >> J1939_Sig.bStartBit) & btaBitMask.toUInt32(QSysInfo::LittleEndian); //No single signal is bigger than 32 bits
            float fFinalVal = lRawVal*J1939_Sig.fScale + J1939_Sig.fOffset;
            qDebug() << J1939_Sig.cName << ":" << fFinalVal << J1939_Sig.cUnits << "\n";
            if(!sigIdxMapLog.contains(wSigIndex))
            {
                QList<double> emptyList;
                logValuesLog.append(emptyList);
                QList<double> emptyList2;
                logTimestampsLog.append(emptyList2);
                sigIdxMapLog.insert(wSigIndex, logValuesLog.count() - 1); //Could have used values or timestamps count
            }
            logValuesLog[sigIdxMapLog[wSigIndex]].append(fFinalVal);
            logTimestampsLog[sigIdxMapLog[wSigIndex]].append(_dTimestamp); //From CSV file
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
            decodeOBDIIMsg(wPid, _baRxMsg.sliced(3,dataBytes));
        }
        else if(0x62) //Proprietary extended mode 0x22 + 0x40 (see Wikipedia OBDII)
        {
            quint8 dataBytes = _baRxMsg[0] - 3; //Minus service code and 2 byte pid code
            quint16 wPid = qFromLittleEndian<quint16>(_baRxMsg.sliced(2,2));
            decodeOBDIIMsg(wPid, _baRxMsg.sliced(4,dataBytes));
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


void MainWindow::parseLogStdFrame(double _dTimestamp, quint16 _wCanAddr, QByteArray _baRxMsg)
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
            decodeOBDIILogMsg(_dTimestamp, wPid, _baRxMsg.sliced(3,dataBytes));
        }
        else if(0x62) //Proprietary extended mode 0x22 + 0x40 (see Wikipedia OBDII)
        {
            quint8 dataBytes = _baRxMsg[0] - 3; //Minus service code and 2 byte pid code
            quint16 wPid = qFromLittleEndian<quint16>(_baRxMsg.sliced(2,2));
            decodeOBDIILogMsg(_dTimestamp, wPid, _baRxMsg.sliced(4,dataBytes));
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

void MainWindow::decodeOBDIIMsg(quint16 _wPid, QByteArray _baRxMsg)
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
    quint16 wSigIndex;
    for(quint16 i = 0U; i < wOBDIISignalCount;i++)
    {
        if(sOBDII_Signals[i].wPid == _wPid)
        {
            wSigIndex = i;
            messageDecodable = true;
            break; //Out of for loop
        }
    }
    if(messageDecodable)
    {
        fFinalVal = mapOBDIIToVal(_wPid, fA, fB);

        sOBDII_Signal_t OBDII_Sig = sOBDII_Signals[wSigIndex];
        qDebug() << OBDII_Sig.cName << fFinalVal << OBDII_Sig.cUnits << "\n";

        bool bNewMsgType = false;
        if(!sigIdxMapLive.contains(wSigIndex))
        {
            bNewMsgType = true;
            QList<double> emptyList;
            logValuesLiveOBDII.append(emptyList);
            QList<double> emptyList2;
            logTimestampsLiveOBDII.append(emptyList2);
            sigIdxMapLive.insert(wSigIndex, logValuesLiveOBDII.count() - 1); //Could have used values or timestamps count
        }
        //Keep display buff length to size
        if(logValuesLiveOBDII[sigIdxMapLive[wSigIndex]].length() > bLOG_LEN_MAX) //Rolling buffer, if bigger then remove value from start
        {
            logValuesLiveOBDII[sigIdxMapLive[wSigIndex]].takeFirst();//Remove element at start
            logTimestampsLiveOBDII[sigIdxMapLive[wSigIndex]].takeFirst();
        }
        //Now add
        logValuesLiveOBDII[sigIdxMapLive[wSigIndex]].append(fFinalVal);
        logTimestampsLiveOBDII[sigIdxMapLive[wSigIndex]].append(static_cast<double>(QDateTime::currentMSecsSinceEpoch())/1000.0);

        //Update the GUI with new data
        if(bNewMsgType){
            QCheckBox *sigCheck = new QCheckBox(OBDII_Sig.cName, ui->scrollAreaLiveWidget);
            QLineEdit *sigVal = new QLineEdit(QString::number(fFinalVal),ui->scrollAreaLiveWidget);
            sigVal->setMinimumWidth(75);
            QLabel *sigUnits = new QLabel(OBDII_Sig.cUnits,ui->scrollAreaLiveWidget);
            gridLive->addWidget(sigCheck,sigIdxMapLive[wSigIndex],0);
            gridLive->addWidget(sigVal,sigIdxMapLive[wSigIndex],1);
            gridLive->addWidget(sigUnits,sigIdxMapLive[wSigIndex],2);
            //Connect all the signals' checkbox events
            connect(sigCheck,&QCheckBox::stateChanged,this, [=](){
                checkboxClickedLive(sigIdxMapLive[wSigIndex]);
            });
        }
        else //Just update the value
        {
            static_cast<QLineEdit*>(gridLive->itemAtPosition(sigIdxMapLive[wSigIndex],1)->widget())->setText(QString::number(fFinalVal));
        }


        ui->scrollAreaLiveWidget->update(); //To prevent strange screen tearing bug when scrolling
        if(graphIdxMapLive.contains(sigIdxMapLive[wSigIndex]))
        {
            ui->widgetPlotLive->graph(graphIdxMapLive.value(sigIdxMapLive[wSigIndex]))->setData(logTimestampsLiveOBDII[sigIdxMapLive[wSigIndex]],logValuesLiveOBDII[sigIdxMapLive[wSigIndex]]);
            ui->widgetPlotLive->graph(graphIdxMapLive.value(wSigIndex))->setName(OBDII_Sig.cName);

            QString cYAxisName = "";
            cYAxisName.append(OBDII_Sig.cName);
            cYAxisName.append(" ");
            cYAxisName.append(OBDII_Sig.cUnits);
            if(graphIdxMapLive.value(sigIdxMapLive[wSigIndex]) == 0) //If first graph
            {
                ui->widgetPlotLive->yAxis->setLabel(cYAxisName);
                ui->widgetPlotLive->xAxis->setRange(logTimestampsLiveOBDII[sigIdxMapLive[wSigIndex]].first(), logTimestampsLiveOBDII[sigIdxMapLive[wSigIndex]].last());
                ui->widgetPlotLive->yAxis->rescale(); //Basically an autoscale
            }
            else
            {
                ui->widgetPlotLive->yAxis2->setLabel(cYAxisName);
                ui->widgetPlotLive->xAxis2->setRange(logTimestampsLiveOBDII[sigIdxMapLive[wSigIndex]].first(), logTimestampsLiveOBDII[sigIdxMapLive[wSigIndex]].last());
                ui->widgetPlotLive->yAxis2->rescale(); //Basically an autoscale
            }
            ui->widgetPlotLive->replot();
        }
    }
    else
    {
        qDebug() << QString::number( _wPid, 16 )  << "\n";
        qDebug() << "OBDII signal not decodable";
    }
}

void MainWindow::decodeOBDIILogMsg(double _dTimestamp, quint16 _wPid, QByteArray _baRxMsg)
{
    bool messageDecodable = false;
    float fA = (quint8)_baRxMsg[0]; //Force unsigned, otherwise it can intepret as int
    float fB = 0.0;
    if(_baRxMsg.count() > 1U)
    {
        fB = (quint8)_baRxMsg[1];  //Force unsigned, otherwise it can intepret as int
    }
    float fFinalVal = 0;
    quint16 wSigIndex;
    for(quint16 i = 0U; i < wOBDIISignalCount;i++)
    {
        if(sOBDII_Signals[i].wPid == _wPid)
        {
            wSigIndex = i;
            messageDecodable = true;
            break; //Out of for loop
        }
    }
    if(messageDecodable)
    {
        fFinalVal = mapOBDIIToVal(_wPid, fA, fB);

        sOBDII_Signal_t OBDII_Sig = sOBDII_Signals[wSigIndex];
        qDebug() << OBDII_Sig.cName << fFinalVal << OBDII_Sig.cUnits << "\n";
        if(!sigIdxMapLog.contains(wSigIndex))
        {
            QList<double> emptyList;
            logValuesLog.append(emptyList);
            QList<double> emptyList2;
            logTimestampsLog.append(emptyList2);
            sigIdxMapLog.insert(wSigIndex, logValuesLog.count() - 1); //Could have used values or timestamps count
        }
        logValuesLog[sigIdxMapLog[wSigIndex]].append(fFinalVal);
        logTimestampsLog[sigIdxMapLog[wSigIndex]].append(_dTimestamp); //From CSV file
    }
}

void MainWindow::decodePropMsg(quint16 _wCanAddr, QByteArray _baRxMsg)
{
    qDebug() << "Proprietary message\n";
}

float MainWindow::mapOBDIIToVal(quint16 _wPid, float _fA, float _fB)
{
    float fFinalVal = 0;
    switch (_wPid) {
    case PID_ENGINE_SPEED:
    {
        fFinalVal = (_fA*256.0 +_fB)/4.0; //0|16383.75
    }
    break;
    case PID_VEHICLE_SPEED:
    {
        fFinalVal = _fA; //0|255
    }
    break;
    case PID_ENGINE_LOAD:
    {
        fFinalVal = _fA/255.0; //0|100
    }
    break;
    case PID_COOLANT_TEMP:
    {
        fFinalVal = _fA - 40.0; //-40|215
    }
    break;
    case PID_INTAKE_MANIFOLD_ABS_PRESS:
    {
        fFinalVal = _fA; //0|255
    }
    break;
    case PID_ABS_BAROMETRIC_PRESS:
    {
        fFinalVal = _fA; //0|255
    }
    break;
    case PID_AMB_AIR_TEMP:
    {
        fFinalVal = _fA - 40.0; //-40|215
    }
    break;
    case PID_ACCEL_POS_D:
    {
        fFinalVal = _fA*(100.0/255.0); //0|100
    }
    break;
    case PID_ACCEL_POS_E:
    {
        fFinalVal = _fA*(100.0/255.0); //0|100
    }
    break;
    case PID_THROTTLE_POS:
    {
        fFinalVal = _fA*(100.0/255.0); //0|100
    }
    break;
    case PID_ENGINE_OIL_TEMP:
    {
        fFinalVal = _fA - 40.0; //-40|210
    }
    break;
    case PID_ENGINE_RUNTIME:
    {
        fFinalVal = _fA*256.0 + _fB;
    }
    break;
    case PID_MAF_RATE:
    {
        fFinalVal = (_fA*256.0 +_fB)/100.0; //0|16383.75
    }
    break;
    case PID_FUEL_RAIL_GAUGE_PRESS:
    {
        fFinalVal = 10.0*(_fA*256.0 +_fB); //0|655350
    }
    break;
    case PID_CTL_MODULE_VOLT:
    {
        fFinalVal = (_fA*256.0 +_fB)/1000.0; //0|65.535
    }
    case PID_REL_THROTTLE_POS:
    {
        fFinalVal = (100.0/255.0)*_fA; //0|100
    }
    break;
    case PID_REL_ACCEL_POS:
    {
        fFinalVal = (100.0/255.0)*_fA; //0|100
    }
    break;
    default:
        break;
    }
    return fFinalVal;
}
