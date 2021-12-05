#include "CSx00Instrument.h"
#include "ITool.h"
#include "CSx00ScpiClient.h"
#include "PssRstDef.h"
#include "QtCom.h"
#include "QtTcpClient.h"
#ifdef _PSS_GPIB_SUPPORT_
#include "GPIBIo.h"
#endif
#include <QThread>
#include <QTime>

using namespace Pss::Qt;

const QString CSx00Instrument::mcName = "CSx00";

CSx00Instrument::CSx00Instrument(const QHostAddress &ip, QObject *parent) :
    QtIInstrument(InstrIOTypeNet, mcName, true, parent),
    mScpiClient(new CSx00ScpiClient(ip, mcName)),
    mChanID(0)
{
}

CSx00Instrument::CSx00Instrument(const QString comName, uint32_t comBaud, QObject *parent):
    QtIInstrument(InstrIOTypeComm, mcName, true, parent),
    mScpiClient(new CSx00ScpiClient(comName, comBaud, mcName)),
    mChanID(0)
{
}
#ifdef _PSS_GPIB_SUPPORT_
CSx00Instrument::CSx00Instrument(uint8_t cardAddr, uint8_t instrumentAddr, QObject *parent):
    QtIInstrument(InstrIOTypeGPIB, mcName, true, parent),
    mScpiClient(new CSx00ScpiClient(cardAddr, instrumentAddr, mcName)),
    mChanID(0)
{
}
#endif


CSx00Instrument::~CSx00Instrument()
{
    delete mScpiClient;
    mScpiClient = nullptr;
}

void CSx00Instrument::SetChannelID(uint8_t chanID)
{
    mChanID = chanID;
}

int CSx00Instrument::SetChannelID(uint8_t chanID, uint8_t childChanID)
{
    mChanID = chanID;

    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    int ret = mScpiClient->SetChanID(chanID, childChanID);
    if(PssRstSuccess != ret)
    {
        return PssRstSetSrcError;
    }
    
    return PssRstSuccess;
}

int CSx00Instrument::Set4W(uint8_t chanID, bool is4W)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    int ret = mScpiClient->Send4W(chanID, is4W);
    if(PssRstSuccess != ret)
    {
        return PssRstSetSrcError;
    }

    return PssRstSuccess;
}

int CSx00Instrument::SetVSrc(uint8_t chanID, bool isVSrc)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    int ret = mScpiClient->SendVSrc(chanID, isVSrc);
    if(PssRstSuccess != ret)
    {
        return PssRstSetSrcError;
    }

    return PssRstSuccess;
}

int CSx00Instrument::SetLmtRange(uint8_t chanID, bool isVSrc, double val)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    int ret = mScpiClient->SendLmtRange(chanID, isVSrc, val);
    if(PssRstSuccess != ret)
    {
        return PssRstSetSrcError;
    }

    return PssRstSuccess;
}

int CSx00Instrument::SetLmtVal(uint8_t chanID, bool isVSrc, double val)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    int ret = mScpiClient->SendLmtVal(chanID, isVSrc, val);
    if(PssRstSuccess != ret)
    {
        return PssRstSetSrcError;
    }

    return PssRstSuccess;
}

int CSx00Instrument::SetSrcRange(uint8_t chanID, bool isVSrc, double val)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    int ret = mScpiClient->SendSrcRange(chanID, isVSrc, val);
    if(PssRstSuccess != ret)
    {
        return PssRstSetSrcError;
    }

    return PssRstSuccess;
}

int CSx00Instrument::SetSrcVal(uint8_t chanID, bool isVSrc, double val)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    int ret = mScpiClient->SendSrcVal(chanID, isVSrc, val);
    if(PssRstSuccess != ret)
    {
        return PssRstSetSrcError;
    }

    return PssRstSuccess;
}

int CSx00Instrument::SetNPLC(uint8_t chanID, bool isV, double nplc)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    int ret = mScpiClient->SendNplc(chanID, isV, nplc);
    if(PssRstSuccess != ret)
    {
        return PssRstSetSrcError;
    }

    return PssRstSuccess;
}

int CSx00Instrument::GetOutputStatus(uint8_t chanID, bool &on, uint32_t timeout)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    return mScpiClient->SendGetOutput(chanID, on, timeout);
}


int CSx00Instrument::GetSrcType(uint8_t chanID, bool &isVSrc, uint32_t timeout)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    return mScpiClient->SendGetSrcType(chanID, isVSrc, timeout);
}

int CSx00Instrument::AppendCustomSweepData(uint8_t chanID, bool isV, QVector<double> &data)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    return mScpiClient->SendAppendListVal(chanID, isV, data);
}

int CSx00Instrument::SetSrcAutoRange(uint8_t chanID, bool isVSrc, bool on)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    return mScpiClient->SendSrcAutoRange(chanID, isVSrc, on);
}

int CSx00Instrument::SetLmtAutoRange(uint8_t chanID, bool isVSrc, bool on)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    return mScpiClient->SendLmtAutoRange(chanID, isVSrc, on);
}

int CSx00Instrument::SrcAutoRangeQuery(uint8_t chanID, bool isVSrc, bool *on, uint32_t timeout)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }

    if(!on)
    {
        return PssRstParameterError;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    return mScpiClient->SendSrcAutoRangeQuery(chanID, isVSrc, on, timeout);
}

int CSx00Instrument::LmtAutoRangeQuery(uint8_t chanID, bool isVSrc, bool *on, uint32_t timeout)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }

    if(!on)
    {
        return PssRstParameterError;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    return mScpiClient->SendLmtAutoRangeQuery(chanID, isVSrc, on, timeout);
}

int CSx00Instrument::SrcRangeQuery(uint8_t chanID, bool isVSrc, char *range, uint32_t timeout)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }

    if(!range)
    {
        return PssRstParameterError;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    return mScpiClient->SendSrcRangeQuery(chanID, isVSrc, range, timeout);
}

int CSx00Instrument::LmtRangeQuery(uint8_t chanID, bool isVSrc, char *range, uint32_t timeout)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }

    if(!range)
    {
        return PssRstParameterError;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    return mScpiClient->SendLmtRangeQuery(chanID, isVSrc, range, timeout);
}

int CSx00Instrument::TraceSet(uint8_t chanID, bool on)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    return mScpiClient->SendTraceSet(chanID, on);
}

int CSx00Instrument::SetScanStartVal(uint8_t chanID, bool isVSrc, double val)
{
    if (!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    return mScpiClient->SendScanStartVal(chanID, isVSrc, val);
}

int CSx00Instrument::SetScanStopVal(uint8_t chanID, bool isVSrc, double val)
{
    if (!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    return mScpiClient->SendScanStopVal(chanID, isVSrc, val);
}

int CSx00Instrument::SetScanPointNum(uint8_t chanID, uint32_t val)
{
    if (!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    return mScpiClient->SendScanPointNum(chanID, val);
}

QString CSx00Instrument::ReqVal(uint32_t timeout)
{
    if (!IsConnected())
    {
        return QString("");
    }

    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return QString("");
    }
    
    return mScpiClient->GetReadValStr(timeout);
}

int CSx00Instrument::GetMeasureVal(uint8_t chanID, PssInstrumentMeasureType type, double &val, uint32_t timeout) const
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi client is null !";
        return PssRstParameterError;
    }
    QString recvValStr;
    int ret = mScpiClient->SendRead(chanID, recvValStr, timeout);
    if(ret < 0)
    {
        return ret;
    }
    QStringList valList = recvValStr.split(",");

    QString valStr;
    if(type == PssMeasureCurr)
    {
        valStr = valList[1];
    }
    else if(type == PssMeasureVolt)
    {
        valStr = valList[0];
    }
    else if(type == PssMeasureElec)
    {
        valStr = valList[0].toDouble() / valList[1].toDouble();
    }
    else
    {
        qCritical() << QString("type is null !");
    }
    
    val = valStr.toDouble();
    
    return PssRstSuccess;
}

int CSx00Instrument::SetScanType(uint8_t chanID, bool isV, CSx00ScpiClient::CSx00ScpiSweepType isSwe)
{
    if (!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    return mScpiClient->SendScanType(chanID, isV, isSwe);
}

int CSx00Instrument::SetMasterMode(uint8_t chanID, bool isMaster)
{
    if (!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    return mScpiClient->SendScanMaster(chanID, isMaster);
}

int CSx00Instrument::SetTrigInput(bool on)
{
    if (!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    return mScpiClient->SendTrigInput(on);
}

int CSx00Instrument::SetCustomScanData(uint8_t chanID, bool isV, const QVector<double> &data)
{
    if (!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    return mScpiClient->SendListVal(chanID, isV, data);
}

int CSx00Instrument::SetLmtAbort(uint8_t chanID, bool on)
{
    if (!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    return mScpiClient->SendLmtAbort(chanID, on);
}

int CSx00Instrument::SetTrigDelay(uint8_t chanID, uint32_t delayUs)
{
    if (!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    return mScpiClient->SendTrigDelay(chanID, delayUs);
}

int CSx00Instrument::SetOutputDelay(uint8_t chanID, uint32_t delayUs)
{
    if (!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    return mScpiClient->SendOutputDelay(chanID, delayUs);
}

int CSx00Instrument::SetIPConfig(bool isAuto, const QString &ip, const QString &netMask, const QString &gateWay)
{
    if (!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }

    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    return mScpiClient->SendIPConfig(isAuto, ip, netMask, gateWay);
}

int CSx00Instrument::IPConfigQuery(bool &isAuto, QString &ip, QString &netMask, QString &gateWay, uint32_t timeout)
{
    if (!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    return mScpiClient->SendIPConfigQuery(isAuto, ip, netMask, gateWay, timeout);
}

int CSx00Instrument::SetUpdateIPConfig()
{
    if (!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    return mScpiClient->SendUpdateIPConfig();
}

int CSx00Instrument::SetGPIBConfig(uint8_t devAddr)
{
    if (!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(devAddr > 30)
    {
        return PssRstParameterError;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    return mScpiClient->SendGPIBConfig(devAddr);
}

int CSx00Instrument::GPIBConfigQuery(uint8_t &devAddr, uint32_t timeout)
{
    if (!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    return mScpiClient->SendGPIBConfigQuery(devAddr, timeout);
}

int CSx00Instrument::SetUARTConfig(uint32_t baud)
{
    if (!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    return mScpiClient->SendUARTConfig(baud);
}

int CSx00Instrument::UARTConfigQuery(uint32_t &baud, uint32_t timeout)
{
    if (!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    return mScpiClient->SendUARTConfigQuery(baud, timeout);
}

int CSx00Instrument::SetStartOutputEvent(uint8_t chanID, int inputLine, int outputLine, CSx00ScpiClient::CSx00ScpiTrigEdgeType trigType)
{
    if (!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    return mScpiClient->SendStartOutputEvent(chanID, inputLine, outputLine, trigType);
}

int CSx00Instrument::SetFinishOutputEvent(uint8_t chanID, int inputLine, int outputLine, CSx00ScpiClient::CSx00ScpiTrigEdgeType trigType)
{
    if (!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    return mScpiClient->SendFinishOutputEvent(chanID, inputLine, outputLine, trigType);
}

int CSx00Instrument::SetStartSamplingEvent(uint8_t chanID, int inputLine, int outputLine, CSx00ScpiClient::CSx00ScpiTrigEdgeType trigType)
{
    if (!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    return mScpiClient->SendStartSamplingEvent(chanID, inputLine, outputLine, trigType);
}

int CSx00Instrument::SetFinishSamplingEvent(uint8_t chanID, int inputLine, int outputLine, CSx00ScpiClient::CSx00ScpiTrigEdgeType trigType)
{
    if (!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    return mScpiClient->SendFinishSamplingEvent(chanID, inputLine, outputLine, trigType);
}

int CSx00Instrument::SetStartSweEvent(uint8_t chanID, int inputLine, int outputLine, CSx00ScpiClient::CSx00ScpiTrigEdgeType trigType)
{
    if (!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    return mScpiClient->SendStartSweEvent(chanID, inputLine, outputLine, trigType);
}

int CSx00Instrument::ClearEvent(uint8_t chanID)
{
    if (!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    return mScpiClient->SendClearEvent(chanID);
}

int CSx00Instrument::SetTrigCount(uint8_t chanID, int count)
{
    if (!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    return mScpiClient->SendTrigCount(chanID, count);
}

int CSx00Instrument::MeasureQuery(uint8_t chanID, bool isVSrc, double &val, uint32_t timeout)
{
    if (!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    return mScpiClient->SendMeasureQuery(chanID, isVSrc, val, timeout);
}

int CSx00Instrument::SetOutput(uint8_t chanID, bool on)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }

    if(!HasSrc())
    {
        return PssRstSuccess;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    int ret = mScpiClient->SendOutputCtrl(chanID, on);
    if(PssRstSuccess != ret)
    {
        return PssRstOutputFail;
    }
    
    return PssRstSuccess;
}

int CSx00Instrument::ReadAllOutput(QVector<double> &vecV, QVector<double> &vecI, uint32_t timeout)
{
    if (!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }

    QString val;
    int ret = mScpiClient->SendReadAll(val);
    if(ret < 0)
    {
        return ret;
    }
    
    double vVal,iVal;
    // 返回格式，[3-1:1.01459E+00,1.01459E+00]\r[3-1:1.01459E+00,1.01459E+00]
    val.remove('\n');
    QStringList strValList = val.split('\r');
    int nSize = strValList.size();
    for(int i = 0; i < nSize; i++)
    {
        QString strTmpVal = strValList[i];
        if(strTmpVal.contains(":"))
        {
            int nChannelID = strTmpVal.mid(1, 1).toInt();
            // 去掉通道号信息
            int nStartPos = strTmpVal.indexOf(':') + 1;
            int nEndPos = strTmpVal.lastIndexOf(']') - 1;
            strTmpVal = strTmpVal.mid(nStartPos, nEndPos - nStartPos + 1);
    
            QStringList list;
            list = strTmpVal.split(',');
            if(list.isEmpty() || list.size() < 2)
            {
                return PssRstReadFromIOError;
            }
    
            vVal = list[0].toDouble();
            iVal = list[1].toDouble();
    
            vecV.push_back(vVal);
            vecI.push_back(iVal);
        }
    }
    
    return PssRstSuccess;
}

int CSx00Instrument::ReadOutput(uint8_t chanID, double &vVal, double &iVal, uint32_t timeout)
{
    if (!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    QString val;

    int ret = mScpiClient->SendRead(chanID, val, timeout);
    if(ret < 0)
    {
        return ret;
    }
    
    // 判断是否是单卡4通道,临时处理方法（单卡四通道含有通道号信息，从字符串中去掉该信息）
    // 返回格式，[3-1:1.01459E+00,1.01459E+00]
    if(val.contains(":"))
    {
        // 去掉通道号信息
        int nStartPos = val.indexOf(':') + 1;
        int nEndPos = val.lastIndexOf(']') - 1;
        val = val.mid(nStartPos, nEndPos - nStartPos + 1);
    }
    
    if(!val.contains(","))
    {
        return PssRstReadFromIOError;
    }
    
    QStringList list;
    list = val.split(',');
    if(list.isEmpty() || list.size() < 2)
    {
        return PssRstReadFromIOError;
    }
    
    vVal = list[0].toDouble();
    iVal = list[1].toDouble();
    
    return PssRstSuccess;
}

int CSx00Instrument::SetTrigLine(int inputLine)
{
    if (!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }

    return mScpiClient->SendTrigDir(inputLine);
}

int CSx00Instrument::VersionQuery(uint8_t chanID, QString &ver, uint32_t timeout)
{
    if (!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << "scpi cleint is null !";
        return PssRstParameterError;
    }
    return mScpiClient->SendVersionQuery(chanID, ver, timeout);
}

int CSx00Instrument::GetSweepResult(uint8_t chanID, uint32_t &points, QVector<double>& vResList, QVector<double>& iResList, uint32_t timeout)
{
    vResList.clear();
    iResList.clear();

    if(points <= 0)
    {
        return PssRstSuccess;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return PssRstParameterError;
    }
    QString scpiStr = QString(":READ%1?\n").arg(chanID);
    int ret = mScpiClient->SendRawScpi(scpiStr);
    if(PssRstSuccess != ret)
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
            return PssRstOptTimeoutError;
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
    return PssRstSuccess;
}

QString CSx00Instrument::GetVersion(uint32_t timeout)
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
    if(PssRstSuccess != ret)
    {
        qCritical() << "send version req failed !";
        return QString("");
    }
    return ver;
}

int CSx00Instrument::GetVoltAdcVal(uint8_t chanID, double &vAdc, uint32_t timeout)
{
    if(!IsConnected())
    {
        qCritical() << "device not connect !";
        return PssRstDeviceNotConnect;
    }

    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return PssRstParameterError;
    }
    
    return mScpiClient->SendGetAdcValue(chanID, true, vAdc, timeout);
}

int CSx00Instrument::GetCurrAdcVal(uint8_t chanID, double& iAdc, uint32_t timeout)
{
    if(!IsConnected())
    {
        qCritical() << "device not connect !";
        return PssRstDeviceNotConnect;
    }


    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return PssRstParameterError;
    }
    
    return mScpiClient->SendGetAdcValue(chanID, false, iAdc, timeout);
}

int CSx00Instrument::GetVoltDacVal(uint8_t chanID, double &vDac, uint32_t timeout)
{
    if(!IsConnected())
    {
        qCritical() << "device not connect !";
        return PssRstDeviceNotConnect;
    }


    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return PssRstParameterError;
    }
    
    return mScpiClient->SendGetDacVaule(chanID, true, vDac, timeout);
}

int CSx00Instrument::GetCurrDacVal(uint8_t chanID, double& iDac, uint32_t timeout)
{
    if(!IsConnected())
    {
        qCritical() << "device not connect !";
        return PssRstDeviceNotConnect;
    }


    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return PssRstParameterError;
    }
    
    return mScpiClient->SendGetDacVaule(chanID, false, iDac, timeout);
}

int CSx00Instrument::SetCalParameter(uint8_t chanID, int index, double dac0, double dac1, double adc0, double adc1)
{
    if(!IsConnected())
    {
        qCritical() << "device not connect !";
        return PssRstDeviceNotConnect;
    }


    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return PssRstParameterError;
    }
    
    return mScpiClient->SendSetCalParameter(chanID, index, dac0, dac1, adc0, adc1);
}

int CSx00Instrument::FireCalParameter(uint8_t chanID)
{
    if(!IsConnected())
    {
        qCritical() << "device not connect !";
        return PssRstDeviceNotConnect;
    }


    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return PssRstParameterError;
    }
    
    int ret = mScpiClient->SendFireCalParameter(chanID);
    return ret;
}


ScpiClient *CSx00Instrument::GetScpiClient(void) const
{
    return mScpiClient;
}

int CSx00Instrument::UpdateCfgToInstrument(void)
{
#if 1
    bool conn = false;
    conn = IsConnected();
    if(!conn)
    {
        return PssRstDeviceNotConnect;
    }
    if(mChanID < 1)
    {
        return PssRstDeviceNotConnect;
    }
    bool isVSrc = IsVSrc();

    int ret = mScpiClient->SendVSrc(mChanID,isVSrc);
    if(PssRstSuccess != ret)
    {
        return PssRstSetSrcError;
    }
    
    ret = mScpiClient->SendSrcRange(mChanID,isVSrc, GetSrcVal());
    if(PssRstSuccess != ret)
    {
        return PssRstSetSrcRangeError;
    }
    
    ret = mScpiClient->SendSrcVal(mChanID,isVSrc, GetSrcVal());
    if(PssRstSuccess != ret)
    {
        return PssRstSetSrcError;
    }
    
    ret = mScpiClient->SendLmtRange(mChanID,isVSrc, GetLmtVal());
    if(PssRstSuccess != ret)
    {
        return PssRstSetLmtRangeError;
    }
    
    ret = mScpiClient->SendLmtVal(mChanID,isVSrc,  GetLmtVal());
    if(PssRstSuccess != ret)
    {
        return PssRstSetLmtValError;
    }
#endif
    return PssRstSuccess;
}