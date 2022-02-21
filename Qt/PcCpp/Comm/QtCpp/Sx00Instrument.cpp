#include "Sx00Instrument.h"
#include "ITool.h"
#include "ScpiClient.h"
#include "RstDef.h"
#include <QThread>
#include <QTime>


const QString Sx00Instrument::mcName = QString("Sx00");

Sx00Instrument::Sx00Instrument(const QHostAddress &ip, const QString& instName, bool hasSrc, QObject *parent) :
    QtIInstrument(InstrIOTypeNet, instName, hasSrc, parent),
    mScpiClient(new Sx00ScpiClient(ip, instName))
{
}

/*
Sx00Instrument::Sx00Instrument(const QString strIP, const QString& instName, bool hasSrc, QObject *parent) :
    QtIInstrument(InstrIOTypeNet, instName, hasSrc, parent),
    mScpiClient(new Sx00ScpiClient(new Socket(strIP), instName))
{

}
*/
Sx00Instrument::Sx00Instrument(const QString comName, uint32_t comBaud, const QString& instName,bool hasSrc, QObject *parent):
    QtIInstrument(InstrIOTypeComm, instName, hasSrc, parent),
    mScpiClient(new Sx00ScpiClient(comName, comBaud, instName))
{
    InitSourceInfo();
}
#ifdef __GPIB_SUPPORT_
Sx00Instrument::Sx00Instrument(uint8_t cardAddr, uint8_t instrumentAddr, const QString& instName,bool hasSrc, QObject *parent):
    QtIInstrument(InstrIOTypeGPIB, instName, hasSrc, parent),
    mScpiClient(new Sx00ScpiClient(cardAddr, instrumentAddr, mcName))
{
    InitSourceInfo();
}
#endif

Sx00Instrument::~Sx00Instrument(void)
{
    delete mScpiClient;
    mScpiClient = nullptr;
}

QString Sx00Instrument::GetIdName(void)
{
    return mcName;
}

int Sx00Instrument::UpdateCfgToInstrument(void)
{
    if(!IsConnected())
    {
        return RstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return RstScpiClientNotFound;
    }

    bool isVSrc = IsVSrc();
    
    int ret = mScpiClient->SendVSrc(isVSrc);
    if(RstSuccess != ret)
    {
        return RstSetSrcError;
    }
    
    ret = mScpiClient->SendSrcRange(isVSrc, GetSrcVal());
    if(RstSuccess != ret)
    {
        return RstSetSrcRangeError;
    }
    
    ret = mScpiClient->SendSrcVal(isVSrc, GetSrcVal());
    if(RstSuccess != ret)
    {
        return RstSetSrcError;
    }
    
    ret = mScpiClient->SendLmtRange(isVSrc, GetLmtVal());
    if(RstSuccess != ret)
    {
        return RstSetLmtRangeError;
    }
    
    ret = mScpiClient->SendLmtVal(isVSrc,   GetLmtVal());
    if(RstSuccess != ret)
    {
        return RstSetLmtValError;
    }
    return RstSuccess;
}

int Sx00Instrument::Set4W(bool is4W)
{
    if(!IsConnected())
    {
        return RstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return RstScpiClientNotFound;
    }
    int ret = mScpiClient->Send4W(is4W);
    if(ret < 0)
    {
        return RstSetSrcError;
    }

    return RstSuccess;
}

int Sx00Instrument::SetF(bool isF)
{
    if(!IsConnected())
    {
        return RstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return RstScpiClientNotFound;
    }
    int ret = mScpiClient->SendF(isF);
    if(ret < 0)
    {
        return RstSetSrcError;
    }

    return RstSuccess;
}

int Sx00Instrument::SetVSrc(bool isVSrc)
{
    if(!IsConnected())
    {
        return RstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return RstScpiClientNotFound;
    }
    int ret = mScpiClient->SendVSrc(isVSrc);
    if(ret < 0)
    {
        return RstSetSrcError;
    }

    return RstSuccess;
}

int Sx00Instrument::SetLmtRange(bool isVSrc, double val)
{
    if(!IsConnected())
    {
        return RstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return RstScpiClientNotFound;
    }
    int ret = mScpiClient->SendLmtRange(isVSrc, val);
    if(ret < 0)
    {
        return RstSetSrcError;
    }

    return RstSuccess;
}

int Sx00Instrument::SetLmtVal(bool isVSrc, double val)
{
    if(!IsConnected())
    {
        return RstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return RstScpiClientNotFound;
    }
    int ret = mScpiClient->SendLmtVal(isVSrc, val);
    if(ret < 0)
    {
        return RstSetSrcError;
    }

    return RstSuccess;
}


int Sx00Instrument::SetSrcRange(bool isVSrc, double val)
{
    if(!IsConnected())
    {
        return RstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return RstScpiClientNotFound;
    }
    int ret = mScpiClient->SendSrcRange(isVSrc, val);
    if(ret < 0)
    {
        return RstSetSrcError;
    }

    return RstSuccess;
}

int Sx00Instrument::SetSrcVal(bool isVSrc, double val)
{
    if(!IsConnected())
    {
        return RstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return RstScpiClientNotFound;
    }
    int ret = mScpiClient->SendSrcVal(isVSrc, val);
    if(ret < 0)
    {
        return RstSetSrcError;
    }

    return RstSuccess;
}

int Sx00Instrument::SetNPLC(bool isV, double nplc)
{
    if(!IsConnected())
    {
        return RstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return RstScpiClientNotFound;
    }
    int ret = mScpiClient->SendNplc(isV, nplc);
    if(ret < 0)
    {
        return RstSetSrcError;
    }

    return RstSuccess;
}

int Sx00Instrument::GetOutputStatus(bool& on, uint32_t timeout)
{
    if(!IsConnected())
    {
        return RstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return RstScpiClientNotFound;
    }
    return mScpiClient->SendGetOutput(on, timeout);
}


int Sx00Instrument::GetSrcType(bool &isVSrc, uint32_t timeout)
{
    if(!IsConnected())
    {
        return RstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return RstScpiClientNotFound;
    }
    return mScpiClient->SendGetSrcType(isVSrc, timeout);
}

int Sx00Instrument::MeasureQuery(bool isVSrc, double& val, uint32_t timeout)
{
    if(!IsConnected())
    {
        return RstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return RstScpiClientNotFound;
    }
    return mScpiClient->SendMeasureQuery(isVSrc, val, timeout);
}

int Sx00Instrument::AppendCustomSweepData(bool isV, const QVector<double> &data)
{
    if(!IsConnected())
    {
        return RstDeviceNotConnect;
    }

    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return RstScpiClientNotFound;
    }
    return mScpiClient->SendAppendListVal(isV, data);
}

int Sx00Instrument::SetSrcAutoRange(bool isVSrc, bool on)
{
    if(!IsConnected())
    {
        return RstDeviceNotConnect;
    }

    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return RstScpiClientNotFound;
    }
    
    return mScpiClient->SendSrcAutoRange(isVSrc, on);
}

int Sx00Instrument::SetLmtAutoRange(bool isVSrc, bool on)
{
    if(!IsConnected())
    {
        return RstDeviceNotConnect;
    }

    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return RstScpiClientNotFound;
    }
    
    return mScpiClient->SendLmtAutoRange(isVSrc, on);
}

int Sx00Instrument::SrcAutoRangeQuery(bool isVSrc, bool *on, uint32_t timeout)
{
    if(!IsConnected())
    {
        return RstDeviceNotConnect;
    }

    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return RstScpiClientNotFound;
    }
    
    if(!on)
    {
        return RstParameterError;
    }
    
    return mScpiClient->SendSrcAutoRangeQuery(isVSrc, on, timeout);
}

int Sx00Instrument::LmtAutoRangeQuery(bool isVSrc, bool *on, uint32_t timeout)
{
    if(!IsConnected())
    {
        return RstDeviceNotConnect;
    }

    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return RstScpiClientNotFound;
    }
    
    if(!on)
    {
        return RstParameterError;
    }
    
    return mScpiClient->SendLmtAutoRangeQuery(isVSrc, on, timeout);
}

int Sx00Instrument::SrcRangeQuery(bool isVSrc, char *range, uint32_t timeout)
{
    if(!IsConnected())
    {
        return RstDeviceNotConnect;
    }

    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return RstScpiClientNotFound;
    }
    
    if(!range)
    {
        return RstParameterError;
    }
    return mScpiClient->SendSrcRangeQuery(isVSrc, range, timeout);
}

int Sx00Instrument::LmtRangeQuery(bool isVSrc, char *range, uint32_t timeout)
{
    if(!IsConnected())
    {
        return RstDeviceNotConnect;
    }

    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return RstScpiClientNotFound;
    }
    
    if(!range)
    {
        return RstParameterError;
    }
    return mScpiClient->SendLmtRangeQuery(isVSrc, range, timeout);
}

int Sx00Instrument::TraceSet(bool on)
{
    if(!IsConnected())
    {
        return RstDeviceNotConnect;
    }

    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return RstScpiClientNotFound;
    }
    return mScpiClient->SendTraceSet(on);
}

RstT Sx00Instrument::SetResolution(bool isV, int digits)
{
    if(!IsConnected())
    {
        return RstDeviceNotConnect;
    }

    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return RstScpiClientNotFound;
    }
    
    int ret = mScpiClient->SendResolution(isV, digits);
    if(RstSuccess != ret)
    {
        return RstSetResolutinError;
    }
    return RstSuccess;
}

int Sx00Instrument::ReadOutput(double &voltVal, double &currVal, uint32_t timeout)
{
    if(!IsConnected())
    {
        return RstDeviceNotConnect;
    }

    QString val;
    
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return RstScpiClientNotFound;
    }
    
    int ret = mScpiClient->SendRead(val, timeout);
    if(ret < 0)
    {
        return ret;
    }
    if(!val.contains(","))
    {
        return RstReadFromIOError;
    }
    
    QStringList list;
    list = val.split(',');
    if(list.isEmpty())
    {
        return RstReadFromIOError;
    }
    
    val = list[0];
    voltVal = val.toDouble();
    val = list[1];
    currVal = val.toDouble();
    
    return RstSuccess;
}

int Sx00Instrument::SetOutput(bool on,  bool isRead)
{
    if(!IsConnected())
    {
        return RstDeviceNotConnect;
    }

    if(!HasSrc())
    {
        return RstSuccess;
    }
    
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return RstScpiClientNotFound;
    }
    
    int ret = mScpiClient->SendOutputCtrl(on);
    if(ret < 0)
    {
        return RstOutputFail;
    }
    
    /* ON需要发送"Read?"指令 用于优化性能 */
    if(on && isRead)
    {
        QString readStr;
        ret = mScpiClient->SendRead(readStr);
        if(ret < 0)
        {
            return RstReadFromIOError;
        }
    }
    return RstSuccess;
}

int Sx00Instrument::SetScanStartVal(bool isVSrc, double val)
{
    if(!IsConnected())
    {
        return RstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return RstScpiClientNotFound;
    }

    int ret = mScpiClient->SendScanStartVal(isVSrc, val);
    if(RstSuccess != ret)
    {
        return RstSetSrcError;
    }
    
    return RstSuccess;
}

int Sx00Instrument::SetScanStopVal(bool isVSrc, double val)
{
    if(!IsConnected())
    {
        return RstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return RstScpiClientNotFound;
    }

    int ret = mScpiClient->SendScanStopVal(isVSrc, val);
    if(RstSuccess != ret)
    {
        return RstSetSrcError;
    }
    
    return RstSuccess;
}

int Sx00Instrument::SetScanPointNum(int val)
{
    if(!IsConnected())
    {
        return RstDeviceNotConnect;
    }

    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return RstScpiClientNotFound;
    }
    
    int ret = mScpiClient->SendScanPointNum(val);
    if(RstSuccess != ret)
    {
        return RstSetSrcError;
    }
    
    return RstSuccess;
}

int Sx00Instrument::SetMasterMode(bool isMaster)
{
    if(!IsConnected())
    {
        return RstDeviceNotConnect;
    }

    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return RstScpiClientNotFound;
    }
    
    return mScpiClient->SendScanMaster(isMaster);
}

int Sx00Instrument::SetTrigInput(bool on)
{
    if(!IsConnected())
    {
        return RstDeviceNotConnect;
    }

    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return RstScpiClientNotFound;
    }
    
    return mScpiClient->SendTrigInput(on);
}

int Sx00Instrument::SetCustomScanData(bool isV, const QVector<double>& data)
{
    if(!IsConnected())
    {
        return RstDeviceNotConnect;
    }

    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return RstScpiClientNotFound;
    }
    
    return mScpiClient->SendListVal(isV, data);
}

int Sx00Instrument::SetLmtAbort(bool on)
{
    if(!IsConnected())
    {
        return RstDeviceNotConnect;
    }

    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return RstScpiClientNotFound;
    }
    
    return mScpiClient->SendLmtAbort(on);
}

int Sx00Instrument::SetScanType(bool isV, Sx00ScpiClient::Sx00ScpiSweepType type)
{
    if(!IsConnected())
    {
        return RstDeviceNotConnect;
    }

    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return RstScpiClientNotFound;
    }
    
    int ret = mScpiClient->SendScanType(isV, type);
    if(RstSuccess != ret)
    {
        return RstSetSrcError;
    }
    
    return RstSuccess;
}

int Sx00Instrument::SetTrigLine(int inputLine)
{
    if(!IsConnected())
    {
        return RstDeviceNotConnect;
    }

    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return RstScpiClientNotFound;
    }
    
    return mScpiClient->SendTrigDir(inputLine);
}

int Sx00Instrument::SetNetUpgradePort(uint32_t port)
{
    if(!IsConnected())
    {
        return RstDeviceNotConnect;
    }

    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return RstScpiClientNotFound;
    }
    
    return mScpiClient->SendNetUpgradePort(port);
}

int Sx00Instrument::GetMeasureVal(InstrumentMeasureType type, double &val, uint32_t timeout)
{
    if(!IsConnected())
    {
        return RstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi client is null !";
        return RstParameterError;
    }
    QString recvValStr;
    int ret = mScpiClient->SendRead(recvValStr, timeout);
    if(ret < 0)
    {
        return ret;
    }
    QStringList valList = recvValStr.split(",");
    if(valList.size() < 2)
    {
        return RstReadError;
    }
    QString valStr;
    if(type == MeasureCurr)
    {
        valStr = valList[1];
    }
    else if(type == MeasureVolt)
    {
        valStr = valList[0];
    }
    else if(type == MeasureElec)
    {
        valStr = valList[0].toDouble() / valList[1].toDouble();
    }
    else
    {
        qCritical() << QString("type is null !");
    }

    val = valStr.toDouble();
    
    return RstSuccess;
}

QString Sx00Instrument::ReqVal(uint32_t timeout)
{
    if(!IsConnected())
    {
        return QString("");
    }

    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return QString("");
    }
    
    return mScpiClient->GetReadValStr(timeout);
}

int Sx00Instrument::GetSweepResult(int &points, QVector<double>& vResList, QVector<double>& iResList, uint32_t timeout)
{
    vResList.clear();
    iResList.clear();

    if(points <= 0)
    {
        return RstSuccess;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return RstParameterError;
    }
    QString scpiStr = ":READ?\n";
    int ret = mScpiClient->SendRawScpi(scpiStr);
    if(RstSuccess != ret)
    {
        qCritical() << "send get sweep result scpi failed: " << ret;
        return ret;
    }
    int p = 0;/* 接收的点数 */
    QString lastLeftStr;
    QTime startTime = QTime::currentTime();
    startTime.start();
    do
    {
        if(startTime.elapsed() > timeout)
        {
            qCritical() << "recv sweep result timeout !";
            points = p;
            return RstOptTimeoutError;
        }
        QString res;
        res = mScpiClient->GetReadValStr(0, false);
        if(res.isEmpty())
        {
            continue;
        }
        lastLeftStr.append(res);
        QStringList list = lastLeftStr.split("\n");
        /* 接收数据有残缺 */
        if(!lastLeftStr.endsWith("\n"))
        {
            lastLeftStr = list.takeLast();
        }
        else
        {
            lastLeftStr.clear();
        }
        for(int i = 0;i < list.size();i++)
        {
            QString viVal = list.at(i);
            QStringList valList = viVal.split(",");
            if(valList.size() == 2)
            {
                vResList.append(valList[0].toDouble());
                iResList.append(valList[1].toDouble());
                p++;
            }
        }
    }while(p < points);
    points = p;
    return RstSuccess;
}

QString Sx00Instrument::GetVersion(uint32_t timeout)
{
    if(!IsConnected())
    {
        return QString("");
    }

    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return QString("");
    }
    
    QString ver;
    int ret = mScpiClient->SendVersionReq(ver, timeout);
    if(RstSuccess != ret)
    {
        qCritical() << "send version req failed !";
        return QString("");
    }
    return ver;
}

int Sx00Instrument::GetVoltAdcVal(double &vAdc, uint32_t timeout)
{
    if(!IsConnected())
    {
        qCritical() << "device not connect !";
        return RstDeviceNotConnect;
    }

    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return RstParameterError;
    }
    
    return mScpiClient->SendGetAdcValue(true, vAdc, timeout);
}

int Sx00Instrument::GetCurrAdcVal(double& iAdc, uint32_t timeout)
{
    if(!IsConnected())
    {
        qCritical() << "device not connect !";
        return RstDeviceNotConnect;
    }


    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return RstParameterError;
    }
    
    return mScpiClient->SendGetAdcValue(false, iAdc, timeout);
}

int Sx00Instrument::GetVoltDacVal(double &vDac, uint32_t timeout)
{
    if(!IsConnected())
    {
        qCritical() << "device not connect !";
        return RstDeviceNotConnect;
    }


    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return RstParameterError;
    }
    
    return mScpiClient->SendGetDacVaule(true, vDac, timeout);
}

int Sx00Instrument::GetCurrDacVal(double& iDac, uint32_t timeout)
{
    if(!IsConnected())
    {
        qCritical() << "device not connect !";
        return RstDeviceNotConnect;
    }


    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return RstParameterError;
    }
    
    return mScpiClient->SendGetDacVaule(false, iDac, timeout);
}

int Sx00Instrument::GetAnlgState(QString &retStr, uint32_t timeout)
{
    if(!IsConnected())
    {
        qCritical() << "device not connect !";
        return RstDeviceNotConnect;
    }


    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return RstParameterError;
    }
    
    return mScpiClient->SendAnlgStateReq(retStr, timeout);
}

int Sx00Instrument::SetCalParameter(int index, double dac0, double dac1, double adc0, double adc1)
{
    if(!IsConnected())
    {
        qCritical() << "device not connect !";
        return RstDeviceNotConnect;
    }


    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return RstParameterError;
    }
    
    return mScpiClient->SendSetCalParameter(index, dac0, dac1, adc0, adc1);
}

int Sx00Instrument::FireCalParameter(void)
{
    if(!IsConnected())
    {
        qCritical() << "device not connect !";
        return RstDeviceNotConnect;
    }

    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return RstParameterError;
    }
    
    int ret = mScpiClient->SendFireCalParameter();
    return ret;
}

ScpiClient *Sx00Instrument::GetScpiClient(void) const
{
    return mScpiClient;
}

void Sx00Instrument::InitSourceInfo(void)
{
    /* 电压源,30V输出,0.1uA限值,0.1 NPLC */
    mCfgInfo.isVSrc = true;
    mCfgInfo.srcVal = 0.03;
    mCfgInfo.lmtVal = 0.0000001;
    mCfgInfo.nplc = 0.1;
}
