#include "TestPcDaemon.h"
#include "ui_TestPcDaemon.h"
//#include "ProcessShareMemory.h"

TestPcDaemon::TestPcDaemon(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TestPcDaemon)
  ,mServer(nullptr)
  ,mLEditMasterDevType(nullptr)
  ,mLEditDevType(nullptr)
{
    ui->setupUi(this);
    init();
}

TestPcDaemon::~TestPcDaemon()
{
#if 0
    mSharememory.detach();
#endif
    delete ui;
}


void TestPcDaemon::init()
{
    mServer = new TestTcpServer(9987,this);
    mServer->setTextEdit(ui->textEditRecv,ui->textEditSend);
    mServer->setClientComBox(ui->comboBoxClient);

    ui->pushButtonDisConnect->setEnabled(false);
    mLEditDevType = new QLineEdit("SX00",this);
    mLEditMasterDevType = new QLineEdit("S100",this);

    mLEditDevType->setPlaceholderText("型号,如:S100");
    mLEditMasterDevType->setPlaceholderText("系列号,如:SX00");

    ui->comboBoxDev->clear();
    ui->comboBox_subCard->clear();

    mLEditDevType->setClearButtonEnabled(true);
    ui->comboBox_subCard->setLineEdit(mLEditDevType);

    mLEditMasterDevType->setClearButtonEnabled(true);
    ui->comboBoxDev->setLineEdit(mLEditMasterDevType);

    //initShareMemory();

    /* 信号连接 */
    connect(ui->pushButtonConnect,&QPushButton::clicked,this,&TestPcDaemon::sltConnect);
    connect(ui->pushButtonDisConnect,&QPushButton::clicked,this,&TestPcDaemon::sltDisConnect);
    connect(ui->pushButtonSend,&QPushButton::clicked,this,&TestPcDaemon::sltSendData);
    connect(ui->pushButtonClear,&QPushButton::clicked,this,&TestPcDaemon::sltClearText);
    connect(ui->pushButtonUpdate,&QPushButton::clicked,this,&TestPcDaemon::sltUpdateShareMemory);
    connect(ui->pushButtonSwitchOutput,&QPushButton::clicked,this,&TestPcDaemon::sltSwitchOutput);
    connect(ui->pushButtonScan,&QPushButton::clicked,this,&TestPcDaemon::sltRemoteScan);
    connect(mServer,&TestTcpServer::sgnOutputStatus,this,&TestPcDaemon::sltChangeOutputStatus);
    connect(mServer,&TestTcpServer::sgnPulseMode,this,&TestPcDaemon::sltChangePulseMode);
    //connect(mServer,&TestTcpServer::sgnPulseInfiniteMode,this,&TestPcDaemon::sltPulseInfiniteMode);

    connect(ui->comboBoxDev,&QComboBox::currentTextChanged,mServer,&TestTcpServer::setDevType);
    connect(ui->comboBox_subCard,&QComboBox::currentTextChanged,mServer,&TestTcpServer::setSubCardType);
    connect(ui->lineEdit_card,&QLineEdit::editingFinished,this,&TestPcDaemon::sltEditCard);
    connect(ui->checkBox_readTimeout,&QCheckBox::clicked,mServer,&TestTcpServer::setReadState);
    connect(ui->checkBox_defReply,&QCheckBox::clicked,this,&TestPcDaemon::sltCheckAutoReply);

    ui->textEditRecv->document()->setMaximumBlockCount(1000);
    ui->textEditSend->document()->setMaximumBlockCount(1000);
}

void TestPcDaemon::initShareMemory()
{    
#if 0
    mSharememory.setKey(mcProcessShareMemoryKeyName);
    if(mSharememory.isAttached())
    {
        mSharememory.detach();
    }
    bool ret = mSharememory.create(mcProcessShareMemorySize);
    if(!ret)
    {
        qDebug()<<mSharememory.errorString();
        return;
    }

    /* 初始化ShareMemory */
    mSharememory.lock();
    char* share = (char*)mSharememory.data();
    int index = ui->comboBoxDev->currentIndex();
    switch(index)
    {
    case 0://px00
    {
        Px00AnlgStatusInfo* anlgStatus = (Px00AnlgStatusInfo*)(share+ mcAnlgStatusInfoShareMemoryOffset);
        anlgStatus->voltRange = 0x05;
        anlgStatus->currRange = 0x86;
        anlgStatus->isPulseShape = false;
        anlgStatus->pulseWidth = 220;
        anlgStatus->pulsePeriodBackup = 5000;
        anlgStatus->pulseDelayNs = 0;
        anlgStatus->pulseCount =2;
        anlgStatus->pulseSamplePoints = 1;
        anlgStatus->pulseLowSamplePoints = 0;
        anlgStatus->nplcValue = 0.01;
        anlgStatus->currRangeType = 0x81;
        anlgStatus->voltRangeType = 0x01;
        anlgStatus->isVSrc = true;
        anlgStatus->is4W = false;
        anlgStatus->voltValue = 0;
        anlgStatus->currValue = 0;
        break;
    }
    case 1://sx00
    {
        ProcessAnlgStatusShareMemory* anlgStatus = (ProcessAnlgStatusShareMemory*)(share+ mcAnlgStatusInfoShareMemoryOffset);
        anlgStatus->mIsSet = true;
        anlgStatus->mIsVSrc = true;
        anlgStatus->mIsOutput = false;
        anlgStatus->mIs4W = false;
        anlgStatus->mIsF = false;
        //anlgStatus->mVRangeType;
        //anlgStatus->mIRangeType;
        //anlgStatus->mCurrentVRange;
        //anlgStatus->mCurrentIRange;
        //anlgStatus->mSetVVal;
        //anlgStatus->mSetIVal;
        //anlgStatus->mSetVRange;
        //anlgStatus->mSetIRange;
        //anlgStatus->mNPLCValue;
        //anlgStatus->mTrigInputOn;
        //anlgStatus->mTrigInputLine;
        anlgStatus->mReadVVal = 0.1;
        anlgStatus->mReadIVal = 0.2;
        //anlgStatus->mTracTrig;
    }
    case 2://plx00
    {
        break;
    }
    default:break;
    }

    mSharememory.unlock();
#endif
}

void TestPcDaemon::sltConnect()
{
    mServer->setPort(quint16(ui->lineEditPort->text().toUInt()));
    mServer->listenClient(true);
    ui->pushButtonConnect->setEnabled(false);
    ui->pushButtonDisConnect->setEnabled(true);
    ui->labelConnectState->setText(QString("ConnectSuccess"));
    ui->comboBoxIP_List->clear();
    ui->comboBoxIP_List->addItems(mServer->getLocalIp());
}

void TestPcDaemon::sltDisConnect()
{
    mServer->listenClient(false);
    ui->pushButtonConnect->setEnabled(true);
    ui->pushButtonDisConnect->setEnabled(false);
    ui->labelConnectState->setText(QString("NotConnect"));
}

void TestPcDaemon::sltSendData()
{
    QByteArray data;
    data = ui->textEditSend->toPlainText().toLatin1();
    mServer->sendToClient(data);
}

void TestPcDaemon::sltClearText()
{
    ui->textEditSend->clear();
    ui->textEditRecv->clear();
}

void TestPcDaemon::sltUpdateShareMemory()
{
#if 0
    if(!mSharememory.isAttached())
        return;
    mSharememory.lock();
    char* share = (char*)mSharememory.data();
    Px00AnlgStatusInfo* anlgStatus = (Px00AnlgStatusInfo*)(share+ mcAnlgStatusInfoShareMemoryOffset);
    mSharememory.unlock();
    QString statusStr = QString("Output:%1\nPulseShape:%2\nPulsePeriodBack:%3\n")
            .arg(anlgStatus->isOutput)
            .arg(anlgStatus->isPulseShape)
            .arg(anlgStatus->pulsePeriodBackup);
    ui->textEditShareMemory->setText(statusStr);
#endif
}

void TestPcDaemon::sltChangeOutputStatus(const bool &isopen)
{
#if 0
    if(!mSharememory.isAttached())
        return;
    mSharememory.lock();
    char* share = (char*)mSharememory.data();
    Px00AnlgStatusInfo* anlgStatus = (Px00AnlgStatusInfo*)(share+ mcAnlgStatusInfoShareMemoryOffset);
    anlgStatus->isOutput = isopen;
    mSharememory.unlock();
#endif
}

void TestPcDaemon::sltChangePulseMode(const bool &isPulse)
{
#if 0
    if(!mSharememory.isAttached())
        return;
    mSharememory.lock();
    char* share = (char*)mSharememory.data();
    Px00AnlgStatusInfo* anlgStatus = (Px00AnlgStatusInfo*)(share+ mcAnlgStatusInfoShareMemoryOffset);
    anlgStatus->isPulseShape = isPulse;
    mSharememory.unlock();
#endif
}

//void TestPcDaemon::sltPulseInfiniteMode(void)
//{
//    mSharememory.lock();
//    char* share = (char*)mSharememory.data();
//    Px00AnlgStatusInfo* anlgStatus = (Px00AnlgStatusInfo*)(share+ mcAnlgStatusInfoShareMemoryOffset);
//    anlgStatus->isOutput = true;
//    mSharememory.unlock();
//}

void TestPcDaemon::sltSwitchOutput()
{
#if 0
    if(!mSharememory.isAttached())
        return;
    mSharememory.lock();
    char* share = (char*)mSharememory.data();
    Px00AnlgStatusInfo* anlgStatus = (Px00AnlgStatusInfo*)(share+ mcAnlgStatusInfoShareMemoryOffset);
    anlgStatus->isOutput = !anlgStatus->isOutput;
    mSharememory.unlock();
#endif
}

void TestPcDaemon::sltRemoteScan()
{
#if 1
    QByteArray data = ":PSS:FNT:SWEEP:PARA \"v,2,F,0.1,1,100,0.1,0,0.1\"";
#endif
    mServer->sendToClient(data);
}

void TestPcDaemon::sltEditCard()
{
    if(nullptr == mServer)
        return;
    mServer->setCards(ui->lineEdit_card->text());
}

void TestPcDaemon::sltCheckAutoReply(const bool &isCheck)
{
    if(nullptr == mServer)
        return;
    mServer->setAutoReplyStr(isCheck);
}
