#include "Px00Instrument.h"
#include "ITool.h"
#include "Px00ScpiClient.h"
#include "PssRstDef.h"
#include "QtCom.h"
#include "QtTcpClient.h"
#ifdef _PSS_GPIB_SUPPORT_
#include "GPIBIo.h"
#endif
#include <QThread>
#include <QTime>

using namespace Pss::Qt;

const QString Px00Instrument::mcName = "Px00";

Px00Instrument::Px00Instrument(const QHostAddress &ip, QObject *parent) :
    QtIInstrument(InstrIOTypeNet, mcName, true, parent),
    mScpiClient(new Px00ScpiClient(ip, mcName)),
    mChanID(0)
{
}

Px00Instrument::Px00Instrument(const QString comName, uint32_t comBaud, QObject *parent):
    QtIInstrument(InstrIOTypeComm, mcName, true, parent),
    mScpiClient(new Px00ScpiClient(comName, comBaud, mcName)),
    mChanID(0)
{
}
#ifdef _PSS_GPIB_SUPPORT_
Px00Instrument::Px00Instrument(uint8_t cardAddr, uint8_t instrumentAddr, QObject *parent):
    QtIInstrument(InstrIOTypeGPIB, mcName, true, parent),
    mScpiClient(new Px00ScpiClient(cardAddr, instrumentAddr, mcName)),
    mChanID(0)
{
}
#endif


Px00Instrument::~Px00Instrument()
{
    delete mScpiClient;
    mScpiClient = nullptr;
}

void Px00Instrument::SetChannelID(uint8_t chanID)
{
    mChanID = chanID;
}

int Px00Instrument::Set4W(uint8_t chanID, bool is4W)
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

int Px00Instrument::SetF(uint8_t chanID, bool isF)
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
    int ret = mScpiClient->SendF(chanID, isF);
    if(PssRstSuccess != ret)
    {
        return PssRstSetSrcError;
    }

    return PssRstSuccess;
}

int Px00Instrument::SetVSrc(uint8_t chanID, bool isVSrc)
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

int Px00Instrument::SetLmtRange(uint8_t chanID, bool isVSrc, double val)
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

int Px00Instrument::SetLmtVal(uint8_t chanID, bool isVSrc, double val)
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

int Px00Instrument::SetSrcRange(uint8_t chanID, bool isVSrc, double val)
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

int Px00Instrument::SetSrcVal(uint8_t chanID, bool isVSrc, double val)
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

int Px00Instrument::SetNPLC(uint8_t chanID, bool isV, double nplc)
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

int Px00Instrument::GetOutputStatus(uint8_t chanID, bool &on, uint32_t timeout)
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


int Px00Instrument::GetSrcType(uint8_t chanID, bool &isVSrc, uint32_t timeout)
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

int Px00Instrument::AppendCustomSweepData(uint8_t chanID, bool isV, QVector<double> &data)
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

int Px00Instrument::SetSrcAutoRange(uint8_t chanID, bool isVSrc, bool on)
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

int Px00Instrument::SetLmtAutoRange(uint8_t chanID, bool isVSrc, bool on)
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

int Px00Instrument::SrcAutoRangeQuery(uint8_t chanID, bool isVSrc, bool *on, uint32_t timeout)
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

int Px00Instrument::LmtAutoRangeQuery(uint8_t chanID, bool isVSrc, bool *on, uint32_t timeout)
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

int Px00Instrument::SrcRangeQuery(uint8_t chanID, bool isVSrc, char *range, uint32_t timeout)
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

int Px00Instrument::LmtRangeQuery(uint8_t chanID, bool isVSrc, char *range, uint32_t timeout)
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

int Px00Instrument::TraceSet(uint8_t chanID, bool on)
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

int Px00Instrument::SetScanStartVal(uint8_t chanID, bool isVSrc, double val)
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

int Px00Instrument::SetScanStopVal(uint8_t chanID, bool isVSrc, double val)
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

int Px00Instrument::SetScanStepVal(uint8_t chanID, bool isVSrc, double val)
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
    return mScpiClient->SendScanStepVal(chanID, isVSrc, val);
}

int Px00Instrument::SetScanPointNum(uint8_t chanID, uint32_t val)
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

QString Px00Instrument::ReqVal(uint32_t timeout)
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

int Px00Instrument::GetMeasureVal(uint8_t chanID, PssInstrumentMeasureType type, double &val, uint32_t timeout) const
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

int Px00Instrument::SetScanType(uint8_t chanID, bool isV, Px00ScpiClient::Px00ScpiSweepType isSwe)
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

int Px00Instrument::SetMasterMode(uint8_t chanID, bool isMaster)
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

int Px00Instrument::SetTrigInput(bool on)
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

int Px00Instrument::SetCustomScanData(uint8_t chanID, bool isV, const QVector<double> &data)
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

int Px00Instrument::SetLmtAbort(uint8_t chanID, bool on)
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

int Px00Instrument::SetTrigDelay(uint8_t chanID, uint32_t delayUs)
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

int Px00Instrument::SetOutputMode(uint8_t chanID, bool isPulse)
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
    return mScpiClient->SendOutputMode(chanID, isPulse);
}

int Px00Instrument::OutputModeQuery(uint8_t chanID, bool &isPulse, uint32_t timeout)
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
    return mScpiClient->SendOutputModeQuery(chanID, isPulse, timeout);
}

int Px00Instrument::SetPulseDelay(uint8_t chanID, uint32_t delayNs)
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
    return mScpiClient->SendPulseDelay(chanID, delayNs);
}

int Px00Instrument::PulseDelayQuery(uint8_t chanID, uint32_t &delayUs, uint32_t timeout)
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
    return mScpiClient->SendPulseDelayQuery(chanID, delayUs, timeout);
}

int Px00Instrument::SetPulseWidth(uint8_t chanID, uint32_t pulseWidth)
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
    return mScpiClient->SendPulseWidth(chanID, pulseWidth);
}

int Px00Instrument::PulseWidthQuery(uint8_t chanID, uint32_t &pulseWidth, uint32_t timeout)
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
    return mScpiClient->SendPulseWidthQuery(chanID, pulseWidth, timeout);
}

int Px00Instrument::SetPulsePeriod(uint8_t chanID, uint32_t pulsePeriod)
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
    return mScpiClient->SendPulsePeriod(chanID, pulsePeriod);
}

int Px00Instrument::PulsePeriodQuery(uint8_t chanID, uint32_t &pulsePeriod, uint32_t timeout)
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
    return mScpiClient->SendPulsePeriodQuery(chanID, pulsePeriod, timeout);
}

int Px00Instrument::SetInitTrigState(uint8_t chanID, Px00ScpiClient::Px00ScpiTrigType trigType)
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
    return mScpiClient->SendInitTrigState(chanID, trigType);
}

int Px00Instrument::SetTrigStateQuery(uint8_t chanID, Px00ScpiClient::Px00ScpiTrigType trigType, int &state)
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
    return mScpiClient->SendTrigStateQuery(chanID, trigType, state);
}

int Px00Instrument::SetIPConfig(bool isAuto, const QString &ip, const QString &netMask, const QString &gateWay)
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

int Px00Instrument::IPConfigQuery(bool &isAuto, QString &ip, QString &netMask, QString &gateWay, uint32_t timeout)
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

int Px00Instrument::SetUpdateIPConfig()
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

int Px00Instrument::SetGPIBConfig(uint8_t devAddr)
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

int Px00Instrument::GPIBConfigQuery(bool& on, uint8_t &devAddr, uint32_t timeout)
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
    return mScpiClient->SendGPIBConfigQuery(on, devAddr, timeout);
}

int Px00Instrument::SetUARTConfig(uint32_t baud)
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

int Px00Instrument::UARTConfigQuery(bool& on, uint32_t &baud, uint32_t timeout)
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
    return mScpiClient->SendUARTConfigQuery(on, baud, timeout);
}

int Px00Instrument::MeasureQuery(uint8_t chanID, bool isVSrc, double &val, uint32_t timeout)
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

int Px00Instrument::SetOutput(uint8_t chanID, bool on)
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

int Px00Instrument::ReadOutput(uint8_t chanID, double &vVal, double &iVal, uint32_t timeout)
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

int Px00Instrument::SetTrigLine(int inputLine)
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

int Px00Instrument::VersionQuery(uint8_t chanID, QString &ver, uint32_t timeout)
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

int Px00Instrument::SetPulseCount(uint8_t chanID, uint32_t count)
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
    return mScpiClient->SetPulseCount(chanID, count);
}

int Px00Instrument::PulseCountQuery(uint8_t chanID, uint32_t &count, uint32_t timeout)
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
    return mScpiClient->PulseCountQuery(chanID, count, timeout);
}

int Px00Instrument::SetPulseSamplePoint(uint8_t chanID, uint32_t point)
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
    return mScpiClient->SetPulseSamplePoint(chanID, point);
}

int Px00Instrument::PulseSamplePointQuery(uint8_t chanID, uint32_t& point, uint32_t timeout)
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
    return mScpiClient->PulseSamplePointQuery(chanID, point, timeout);
}

int Px00Instrument::TraceDataQuery(uint8_t chanID, const char *traceName, char *data, uint32_t &dataSize, uint32_t timeout)
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

    return mScpiClient->TraceDataQuery(chanID, traceName, data, dataSize, timeout);
}

int Px00Instrument::GetSweepResult(uint8_t chanID, uint32_t &points, QVector<double>& vResList, QVector<double>& iResList, uint32_t timeout)
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

QString Px00Instrument::GetVersion(uint32_t timeout)
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

int Px00Instrument::GetVoltAdcVal(uint8_t chanID, double &vAdc, uint32_t timeout)
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

int Px00Instrument::GetCurrAdcVal(uint8_t chanID, double& iAdc, uint32_t timeout)
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

int Px00Instrument::GetVoltDacVal(uint8_t chanID, double &vDac, uint32_t timeout)
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

int Px00Instrument::GetCurrDacVal(uint8_t chanID, double& iDac, uint32_t timeout)
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

int Px00Instrument::SetCalParameter(uint8_t chanID, int index, double dac0, double dac1, double adc0, double adc1)
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

int Px00Instrument::FireCalParameter(uint8_t chanID)
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


ScpiClient *Px00Instrument::GetScpiClient(void) const
{
    return mScpiClient;
}

int Px00Instrument::UpdateCfgToInstrument(void)
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