#include "PLx00Instrument.h"
#include "ITool.h"
#include "ScpiClient.h"
#include "PssRstDef.h"
#include <QThread>

using namespace Pss::Qt;

const QString PLx00Instrument::mcName = "PLx00";

PLx00Instrument::PLx00Instrument(const QHostAddress &ip, const QString& instName, bool hasSrc, QObject *parent) :
    QtIInstrument(InstrIOTypeNet, instName, hasSrc, parent),
    mScpiClient(new PLx00ScpiClient(ip, instName))
{
}

PLx00Instrument::PLx00Instrument(const QString comName, uint32_t comBaud, const QString& instName,bool hasSrc, QObject *parent):
    QtIInstrument(InstrIOTypeComm, instName, hasSrc, parent),
    mScpiClient(new PLx00ScpiClient(comName, comBaud, instName))
{
    InitSourceInfo();
}
#ifdef _PSS_GPIB_SUPPORT_
PLx00Instrument::PLx00Instrument(uint8_t cardAddr, uint8_t instrumentAddr, const QString& instName,bool hasSrc, QObject *parent):
    QtIInstrument(InstrIOTypeGPIB, instName, hasSrc, parent),
    mScpiClient(new PLx00ScpiClient(cardAddr, instrumentAddr, mcName))
{
    InitSourceInfo();
}
#endif

PLx00Instrument::~PLx00Instrument(void)
{
}

QString PLx00Instrument::GetIdnName(void)
{
    return mcName;
}

int PLx00Instrument::GetMeasureVal(PssInstrumentMeasureType type, double &val, uint32_t timeout)
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
    int ret = mScpiClient->SendRead(recvValStr, timeout);
    if(ret < 0)
    {
        return ret;
    }
    QStringList valList = recvValStr.split(",");
    if(valList.size() < 2)
    {
        return PssRstReadError;
    }
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

QString PLx00Instrument::GetVersion(uint32_t timeout)
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

int PLx00Instrument::SetOutputMode(const bool &isPulse, uint32_t timeout)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return PssRstScpiClientNotFound;
    }
    int ret = mScpiClient->SendOutputMode(isPulse, timeout);
    if(ret < 0)
    {
        return PssRstReadFromIOError;
    }
    return PssRstSuccess;
}

int PLx00Instrument::SetPulsePara(const uint32_t &pulseWidth, const uint32_t &pulsePeriod, uint32_t timeout)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return PssRstScpiClientNotFound;
    }
    int ret = mScpiClient->SendPulseWidth(pulseWidth, timeout);
    if(ret < 0)
    {
        return PssRstReadFromIOError;
    }

    ret = mScpiClient->SendPulsePeriod(pulsePeriod, timeout);
    if(ret < 0)
    {
        return PssRstReadFromIOError;
    }
    
    return PssRstSuccess;
}

int PLx00Instrument::GetPulsePara(int &pulseWidth, int &pulsePeriod, uint32_t timeout)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return PssRstScpiClientNotFound;
    }

    int ret = mScpiClient->PulseWidthQuery(pulseWidth, timeout);
    if (ret < 0)
    {
        return PssRstReadError;
    }
    
    ret = mScpiClient->PulsePeriodQuery(pulsePeriod, timeout);
    if (ret < 0)
    {
        return PssRstReadError;
    }
    return PssRstSuccess;
}

int PLx00Instrument::SetScanCurrVal(const double &startVal, const double &stepVal, const double &stopVal, uint32_t timeout)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return PssRstScpiClientNotFound;
    }
    int ret = mScpiClient->SendScanCurrStartVal(startVal, timeout);
    if (0 < ret)
    {
        return PssRstReadError;
    }
    ret = mScpiClient->SendScanCurrSteptVal(stepVal, timeout);
    if (0 < ret)
    {
        return PssRstReadError;
    }
    ret = mScpiClient->SendScanCurrStopVal(stopVal, timeout);
    if (0 < ret)
    {
        return PssRstReadError;
    }
    return PssRstSuccess;
}

int PLx00Instrument::GetScanCurrVal(double &startVal, double &stepVal, double &stopVal, uint32_t timeout)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return PssRstScpiClientNotFound;
    }
    int ret = mScpiClient->ScanCurrStartValQuery(startVal, timeout);
    if (0 != ret)
    {
        return PssRstReadError;
    }
    ret = mScpiClient->ScanCurrStepValQuery(stepVal, timeout);
    if (0 != ret)
    {
        return PssRstReadError;
    }
    ret = mScpiClient->ScanCurrStopValQuery(stopVal, timeout);
    if (0 != ret)
    {
        return PssRstReadError;
    }
    return PssRstSuccess;
}

int PLx00Instrument::SetWaveLength(const uint32_t &len, uint32_t timeout)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return PssRstScpiClientNotFound;
    }
    return mScpiClient->SendWaveLength(len, timeout);
}

int PLx00Instrument::GetWaveLength(int &len, uint32_t timeout)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return PssRstScpiClientNotFound;
    }
    return mScpiClient->WaveLengthQuery(len, timeout);
}

int PLx00Instrument::SetSampPoint(const int &sampPoint, uint32_t timeout)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return PssRstScpiClientNotFound;
    }

    return mScpiClient->SendSampPoint(sampPoint, timeout);
}

int PLx00Instrument::SetSampDelay(const int &sampDelay, uint32_t timeout)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return PssRstScpiClientNotFound;
    }

    return mScpiClient->SendSampDelay(sampDelay, timeout);
}

int PLx00Instrument::SetDutvCtlType(const double &dut, uint32_t timeout)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return PssRstScpiClientNotFound;
    }

    return mScpiClient->SendDutvCtlType(dut, timeout);
}

int PLx00Instrument::SetDCCurrent(const double &val, uint32_t timeout)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return PssRstScpiClientNotFound;
    }

    return mScpiClient->SendDCCurrent(val, timeout);
}

int PLx00Instrument::GetTestDC(double &iVal, double &vVal, double &pVal, double &imVal, uint32_t timeout)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return PssRstScpiClientNotFound;
    }
    QString str;
    int ret = mScpiClient->SendRead(str, timeout);

    if (ret < 0)
    {
        return PssRstReadError;
    }
    
    QStringList list = str.split(' ');
    if(4 != list.length())
    {
        return PssRstSetSrcError;
    }
    
    iVal = QString(list.at(0)).toDouble();
    vVal = QString(list.at(1)).toDouble();
    pVal = QString(list.at(2)).toDouble();
    imVal = QString(list.at(3)).toDouble();
    return PssRstSuccess;
}

int PLx00Instrument::SetStartScan(uint32_t timeout)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return PssRstScpiClientNotFound;
    }

    return mScpiClient->SendStartScan(timeout);
}

int PLx00Instrument::SetDCScanPoint(const uint32_t &point, uint32_t timeout)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return PssRstScpiClientNotFound;
    }
    int ret = mScpiClient->SendDCScanPoint(point,timeout);
    if(ret < 0)
    {
        return PssRstReadFromIOError;
    }
    return PssRstSuccess;
}

int PLx00Instrument::SetInitDataCtrl(const bool &isOn, uint32_t timeout)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return PssRstScpiClientNotFound;
    }

    return mScpiClient->SendInitDataCtrl(isOn, timeout);
}


int PLx00Instrument::SetOutputDelay(const uint32_t &delayUs, uint32_t timeout)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return PssRstScpiClientNotFound;
    }

    return mScpiClient->SendOutputDelay(delayUs, timeout);
}

int PLx00Instrument::SetTrigDelay(const uint32_t &delayUs, uint32_t timeout)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return PssRstScpiClientNotFound;
    }

    return mScpiClient->SendTrigDelay(delayUs, timeout);
}

int PLx00Instrument::SetTrigPulseWidth(const uint32_t &pulseWidth, uint32_t timeout)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return PssRstScpiClientNotFound;
    }

    return mScpiClient->SendTrigPulseWidth(pulseWidth, timeout);
}

int PLx00Instrument::GetSweepStatus(bool &isFree, uint32_t timeout)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return PssRstScpiClientNotFound;
    }

    return mScpiClient->ScanStatusQuery(isFree, timeout);
}

int PLx00Instrument::GetSweepResult(QVector<double> &vecI, QVector<double> &vecV, QVector<double> &vecPower, QVector<double> &vecBackI, uint32_t timeout)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return PssRstScpiClientNotFound;
    }
    QString scpiStr(":READ?");
    int ret = mScpiClient->SendRawScpi(scpiStr);
    if (0 < ret)
    {
        return PssRstReadFromIOError;
    }
    QString data = mScpiClient->GetReadValStr(timeout, false);
    if (data.contains("\r\n"))
    {
        data.remove("\r\n");
    }
    QStringList list = data.split(' ');
    int len = list.length();
    if((len > 4) && (len % 4 == 1))
    {
        for(int i = 1; i < len; i++)
        {
            if((i % 4) == 1)
            {
                vecI.append(list.value(i).toDouble());
            }
            else if((i % 4) == 2)
            {
                vecV.append(list.value(i).toDouble());
            }
            else if((i % 4) == 3)
            {
                vecPower.append(list.value(i).toDouble());
            }else if((i % 4) == 0)
            {
                vecBackI.append(list.value(i).toDouble());
            }
        }
    }
    else
    {
        return PssRstReadError;
    }
    return PssRstSuccess;
}

int PLx00Instrument::SetImPower(const float &val)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return PssRstScpiClientNotFound;
    }
    int ret = mScpiClient->SendImPower(val,mcWait4RecvTimeout);
    if(PssRstSuccess != ret)
    {
        return PssRstReadFromIOError;
    }
    return PssRstSuccess;
}

int PLx00Instrument::SetOpticalPort(const int &port)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return PssRstScpiClientNotFound;
    }
    int ret = mScpiClient->SendOpticalPort(port,mcWait4RecvTimeout);
    if(PssRstSuccess != ret)
    {
        return PssRstReadFromIOError;
    }
    return PssRstSuccess;
}

int PLx00Instrument::ReqAdc(const bool &isVol, double &adcVal)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return PssRstScpiClientNotFound;
    }

    return mScpiClient->ReqAdc(isVol,adcVal,mcWait4RecvTimeout);
}

int PLx00Instrument::ReqDac(const bool &isVol, double &adcVal)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return PssRstScpiClientNotFound;
    }

    return mScpiClient->ReqDac(isVol,adcVal,mcWait4RecvTimeout);
}

int PLx00Instrument::SetOverVolProtect(const double &val, uint32_t timeout)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return PssRstScpiClientNotFound;
    }

    return mScpiClient->SendOverVolProtect(val,timeout);
}

int PLx00Instrument::SetMaxPower(const double &val, uint32_t timeout)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return PssRstScpiClientNotFound;
    }

    return mScpiClient->SendMaxPower(val, timeout);
}

int PLx00Instrument::SetOffsetVol(const double &val, uint32_t timeout)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return PssRstScpiClientNotFound;
    }

    return mScpiClient->SendOffsetVoltage(val, timeout);
}

int PLx00Instrument::SetCalFire()
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return PssRstScpiClientNotFound;
    }
    int ret = mScpiClient->SendCalFire(mcWait4RecvTimeout);
    if(PssRstSuccess != ret)
    {
        return PssRstReadFromIOError;
    }
    return PssRstSuccess;
}

int PLx00Instrument::SetCalVal(const int &index, const double &dac0, const double &dac1, const double &adc0, const double &adc1)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    if(!mScpiClient)
    {
        qCritical() << QString("scpi client is null !");
        return PssRstScpiClientNotFound;
    }
    int ret = mScpiClient->SendCalVal(index,dac0,dac1,adc0,adc1,mcWait4RecvTimeout);
    if(PssRstSuccess != ret)
    {
        return PssRstReadFromIOError;
    }
    return PssRstSuccess;
}

ScpiClient *PLx00Instrument::GetScpiClient() const
{
    return mScpiClient;
}

void PLx00Instrument::InitSourceInfo()
{

}

int PLx00Instrument::UpdateCfgToInstrument(void)
{
    return PssRstSuccess;
}