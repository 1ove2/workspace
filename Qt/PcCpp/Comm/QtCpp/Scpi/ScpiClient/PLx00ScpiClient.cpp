#include <QThread>
#include "PLx00ScpiClient.h"
#include "QtTcpClient.h"
#include "QtCom.h"
#ifdef _PSS_GPIB_SUPPORT_
#include "GPIBIo.h"
#endif
using namespace Pss::Qt;

const QString PLx00ScpiClient::mcClientIdStr = QString("PLx00ScpiClient");

PLx00ScpiClient::PLx00ScpiClient(IIo *iio, const QString &idStr):
    ScpiClient(iio, idStr),
    mDeleteIo(false)
{
}

PLx00ScpiClient::PLx00ScpiClient(const QHostAddress &ip, const QString &idStr):
    ScpiClient(new QtTcpClient(ip, 5025, mcClientIdStr), idStr),
    mDeleteIo(true)
{
}

PLx00ScpiClient::PLx00ScpiClient(const QString &commName, uint32_t commBaud, const QString &idStr):
    ScpiClient(new QtCom(commName, commBaud, mcClientIdStr), idStr),
    mDeleteIo(true)
{
}

#ifdef _PSS_GPIB_SUPPORT_
PLx00ScpiClient::PLx00ScpiClient(uint8_t cardAddr, uint8_t instrumentAddr, const QString &idStr):
    ScpiClient(new GPIBIo(instrumentAddr, mcClientIdStr, cardAddr), idStr),
    mDeleteIo(true)
{
}
#endif

PLx00ScpiClient::~PLx00ScpiClient(void)
{
    if(mDeleteIo)
    {
        delete mIIo;
        mIIo = nullptr;
    }
}

int PLx00ScpiClient::SendVersionReq(QString &idnRstStr, uint32_t timeout)
{
    QString scpiStr = QString(":PSS:VERS?\n");
    int ret = Send(scpiStr,timeout);
    if(PssRstSuccess != ret)
    {
        return ret;
    }

    ret = Recv(idnRstStr, timeout);
    if(ret <= 0)/* Recv不会返回0 */
    {
        qCritical() << "recv idn failed: " << ret;
        return ret;
    }
    
    return PssRstSuccess;
}

int PLx00ScpiClient::SendRead(QString &readRstStr, uint32_t timeout)
{
    QString scpi = QString(":READ?");
    int ret = Send(scpi, timeout);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send READ? failed: " << ret;
        return ret;
    }
    ret = Recv(readRstStr, timeout);
    if(ret <= 0)/* Recv不会返回0 */
    {
        qCritical() << "recv read result failed: " << ret;
        return ret;
    }
    return PssRstSuccess;
}

int PLx00ScpiClient::SendOutputMode(const bool &isPulse, uint32_t timeout)
{
    QString scpi = QString(":SOUR:FUNC %1").arg(isPulse?QString("PULS"):QString("DC"));
    int ret = Send(scpi, timeout);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send READ? failed: " << ret;
        return ret;
    }

    return PssRstSuccess;
}

int PLx00ScpiClient::OutputModeQuery(bool &isPulse, uint32_t timeout)
{
    QString scpiStr = QString(":SOUR:FUNC?");
    int ret = Send(scpiStr);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send func? failed: " << ret;
        return ret;
    }
    QString status;

    ret = Recv(status, timeout);
    if(ret <= 0)/* Recv不会返回0 */
    {
        qCritical() << "recv func? failed: " << ret;
        return PssRstReadError;
    }
    
    if(status.contains("Pulse"))
    {
        isPulse = true;
    }
    else if(status.contains("DC"))
    {
        isPulse = false;
    }
    else
    {
        qCritical() << "invalid func? return : " << status;
        return PssRstReadError;
    }
    
    return PssRstSuccess;
}

int PLx00ScpiClient::SendPulseWidth(const uint32_t &pulseWidth, uint32_t timeout)
{
    QString scpiStr = QString(":SOUR:PULS:WIDT %1").arg(pulseWidth);
    int ret = Send(scpiStr, timeout);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send WIDT? failed: " << ret;
        return ret;
    }

    return PssRstSuccess;
}

int PLx00ScpiClient::SendPulsePeriod(const uint32_t &pulsePeriod, uint32_t timeout)
{
    QString scpiStr = QString(":SOUR:PULS:PERI %1").arg(pulsePeriod);
    int ret = Send(scpiStr, timeout);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send PERI? failed: " << ret;
        return ret;
    }

    return PssRstSuccess;
}

int Pss::Qt::PLx00ScpiClient::PulseWidthQuery(int &pulseWidth, uint32_t timeout)
{
    QString scpiStr = QString(":SOUR:PULS:WIDT?");
    int ret = Send(scpiStr);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send pulseWidth? failed: " << ret;
        return ret;
    }
    QString str;

    ret = Recv(str, timeout);
    if(ret <= 0)/* Recv不会返回0 */
    {
        qCritical() << "recv pulseWidth? failed: " << ret;
        return PssRstReadError;
    }
    
    pulseWidth = str.toInt();
    
    return PssRstSuccess;
}

int Pss::Qt::PLx00ScpiClient::PulsePeriodQuery(int &pulsePeriod, uint32_t timeout)
{
    QString scpiStr = QString(":SOUR:PULS:PERI?");
    int ret = Send(scpiStr);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send pulsePeriod? failed: " << ret;
        return ret;
    }

    QString str;
    ret = Recv(str, timeout);
    if(ret <= 0)/* Recv不会返回0 */
    {
        qCritical() << "recv pulsePeriod? failed: " << ret;
        return PssRstReadError;
    }
    
    pulsePeriod = str.toInt();
    
    return PssRstSuccess;
}

int Pss::Qt::PLx00ScpiClient::SendScanCurrStartVal(const double &val, uint32_t timeout)
{
    QString scpiStr = QString(":SOUR:CURR:STAR %1").arg(val);
    int ret = Send(scpiStr, timeout);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send STAR failed: " << ret;
        return ret;
    }

    return PssRstSuccess;
}

int PLx00ScpiClient::SendScanCurrSteptVal(const double &val, uint32_t timeout)
{
    QString scpiStr = QString(":SOUR:CURR:STEP %1").arg(val);
    int ret = Send(scpiStr, timeout);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send STEP failed: " << ret;
        return ret;
    }

    return PssRstSuccess;
}

int PLx00ScpiClient::SendScanCurrStopVal(const double &val, uint32_t timeout)
{
    QString scpiStr = QString(":SOUR:CURR:STOP %1").arg(val);
    int ret = Send(scpiStr, timeout);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send STOP failed: " << ret;
        return ret;
    }

    return PssRstSuccess;
}

int PLx00ScpiClient::ScanCurrStartValQuery(double &val, uint32_t timeout)
{
    QString scpiStr = QString(":SOUR:CURR:STAR?");
    int ret = Send(scpiStr);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send STAR? failed: " << ret;
        return ret;
    }

    QString str;
    ret = Recv(str, timeout);
    if(ret <= 0)/* Recv不会返回0 */
    {
        qCritical() << "recv STAR? failed: " << ret;
        return PssRstReadError;
    }
    
    val = str.toDouble();
    
    return PssRstSuccess;
}

int PLx00ScpiClient::ScanCurrStepValQuery(double &val, uint32_t timeout)
{
    QString scpiStr = QString(":SOUR:CURR:STEP?");
    int ret = Send(scpiStr);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send STEP? failed: " << ret;
        return ret;
    }

    QString str;
    ret = Recv(str, timeout);
    if(ret <= 0)/* Recv不会返回0 */
    {
        qCritical() << "recv STEP? failed: " << ret;
        return PssRstReadError;
    }
    
    val = str.toDouble();
    
    return PssRstSuccess;
}

int PLx00ScpiClient::ScanCurrStopValQuery(double &val, uint32_t timeout)
{
    QString scpiStr = QString(":SOUR:CURR:STOP?");
    int ret = Send(scpiStr);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send STOP? failed: " << ret;
        return ret;
    }

    QString str;
    ret = Recv(str, timeout);
    if(ret <= 0)/* Recv不会返回0 */
    {
        qCritical() << "recv STOP? failed: " << ret;
        return PssRstReadError;
    }
    
    val = str.toDouble();
    
    return PssRstSuccess;
}

int Pss::Qt::PLx00ScpiClient::SendWaveLength(const uint32_t &len, uint32_t timeout)
{
    QString scpiStr = QString(":SOUR:WAVE:LEN %1").arg(len);
    int ret = Send(scpiStr, timeout);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send LEN failed: " << ret;
        return ret;
    }

    return PssRstSuccess;
}

int PLx00ScpiClient::WaveLengthQuery(int &len, uint32_t timeout)
{
    QString scpiStr = QString(":SOUR:WAVE:LEN?");
    int ret = Send(scpiStr);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send LEN? failed: " << ret;
        return ret;
    }

    QString str;
    ret = Recv(str, timeout);
    if(ret <= 0)/* Recv不会返回0 */
    {
        qCritical() << "recv LEN? failed: " << ret;
        return PssRstReadError;
    }
    
    len = str.toInt();
    
    return PssRstSuccess;
}

int Pss::Qt::PLx00ScpiClient::SendStartScan(uint32_t timeout)
{
    QString scpiStr = QString(":SOUR:SWE:STAR ON");
    int ret = Send(scpiStr, timeout);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send LEN STAR: " << ret;
        return ret;
    }

    return PssRstSuccess;
}

int PLx00ScpiClient::ScanStatusQuery(bool &isFree, uint32_t timeout)
{
    QString scpiStr = QString(":SOUR:SWE:STAT?");
    int ret = Send(scpiStr);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send status? failed: " << ret;
        return ret;
    }

    QString str;
    ret = Recv(str, timeout);
    if(ret <= 0)/* Recv不会返回0 */
    {
        qCritical() << "recv status? failed: " << ret;
        return PssRstReadError;
    }
    
    if (str.contains("Free"))
    {
        isFree = true;
    }
    else if (str.contains("Busy"))
    {
        isFree = false;
    }
    else
    {
        qCritical() << "invalid status? return : " << str;
        return PssRstReadError;
    }
    
    return PssRstSuccess;
}

int PLx00ScpiClient::SendDCScanPoint(const int &point, uint32_t timeout)
{
    QString scpi = QString(":SOUR:SWE:POIN %1").arg(point);
    int ret = Send(scpi, timeout);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send POIN? failed: " << ret;
        return ret;
    }
    return PssRstSuccess;
}

int PLx00ScpiClient::SendSampPoint(const int &sampPoint, uint32_t timeout)
{
    QString scpi = QString(":SOUR:PULS:POIN %1").arg(sampPoint);
    int ret = Send(scpi, timeout);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send POIN failed: " << ret;
        return ret;
    }
    return PssRstSuccess;
}

int PLx00ScpiClient::SendSampDelay(const int &sampDelay, uint32_t timeout)
{
    QString scpi = QString(":SOUR:DEL %1").arg(sampDelay);
    int ret = Send(scpi, timeout);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send DEL failed: " << ret;
        return ret;
    }
    return PssRstSuccess;
}

int Pss::Qt::PLx00ScpiClient::SendOutputDelay(uint32_t delayUs, uint32_t timeout)
{
    QString scpi = QString(":TRIG:SOUR:DEL %1").arg(delayUs);
    int ret = Send(scpi, timeout);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send DEL failed: " << ret;
        return ret;
    }
    return PssRstSuccess;
}

int PLx00ScpiClient::SendTrigDelay(uint32_t delayUs, uint32_t timeout)
{
    QString scpi = QString(":TRIG:OUT:DEL %1").arg(delayUs);
    int ret = Send(scpi, timeout);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send DEL failed: " << ret;
        return ret;
    }
    return PssRstSuccess;
}

int Pss::Qt::PLx00ScpiClient::SendTrigPulseWidth(uint32_t pulseWidth, uint32_t timeout)
{
    QString scpi = QString(":TRIG:OUT:PULS %1").arg(pulseWidth);
    int ret = Send(scpi, timeout);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send DEL failed: " << ret;
        return ret;
    }
    return PssRstSuccess;
}

int PLx00ScpiClient::SendInitDataCtrl(bool isOn, uint32_t timeout)
{
    QString str;
    if (isOn)
    {
        str = QString("ON");
    }
    else
    {
        str = QString("OFF");
    }
    QString scpiStr = QString(":SOUR:PULS:IDAT %1").arg(str);
    int ret = Send(scpiStr, timeout);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send initdata failed: " << ret;
        return ret;
    }
    return PssRstSuccess;
}

int PLx00ScpiClient::SendDutvCtlType(const double &dut, uint32_t timeout)
{
    QString scpi = QString(":SYST:DUT %1").arg(dut);
    int ret = Send(scpi, timeout);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send READ? failed: " << ret;
        return ret;
    }
    return PssRstSuccess;
}

int PLx00ScpiClient::SendDCCurrent(const double &val, uint32_t timeout)
{
    QString scpi = QString(":SOUR:CURR:LEV %1").arg(val);
    int ret = Send(scpi, timeout);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send READ? failed: " << ret;
        return ret;
    }
    return PssRstSuccess;
}

int PLx00ScpiClient::ReqAdc(const bool &isVol, double &adcVal, uint32_t timeout)
{
    QString scpi = QString(":PSS:ANLG:%1:ADC?").arg(isVol?QString("VOLT"):QString("CURR"));
    mIIo->ClearIn();
    int ret = Send(scpi, timeout);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send get adc failed !";
        return ret;
    }
    QString retStr;
    ret = Recv(retStr, timeout);
    if(ret <= 0)/* Recv不会返回0 */
    {
        qCritical() << "recv adc return value failed: " << ret;
        return PssRstReadError;
    }
    adcVal = retStr.toFloat();
    return PssRstSuccess;
}

int PLx00ScpiClient::ReqDac(const bool &isVol, double &adcVal, uint32_t timeout)
{
    QString scpi = QString(":PSS:ANLG:%1:DAC?").arg(isVol?QString("VOLT"):QString("CURR"));
    mIIo->ClearIn();
    int ret = Send(scpi, timeout);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send get adc failed !";
        return ret;
    }
    QString retStr;
    ret = Recv(retStr, timeout);
    if(ret <= 0)/* Recv不会返回0 */
    {
        qCritical() << "recv adc return value failed: " << ret;
        return PssRstReadError;
    }
    adcVal = retStr.toFloat();
    return PssRstSuccess;
}

int PLx00ScpiClient::SendOverVolProtect(const double &val, uint32_t timeout)
{
    QString scpi = QString(":SENS:VOLT:PROT %1").arg(val);
    int ret = Send(scpi, timeout);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send READ? failed: " << ret;
        return ret;
    }
    return PssRstSuccess;
}

int PLx00ScpiClient::SendMaxPower(const double &val, uint32_t timeout)
{
    QString scpi = QString(":SYST:MAXP %1").arg(val);
    int ret = Send(scpi, timeout);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send READ? failed: " << ret;
        return ret;
    }
    return PssRstSuccess;
}

int PLx00ScpiClient::SendOffsetVoltage(const double &val, uint32_t timeout)
{
    QString scpi = QString(":SYST:VBB %1").arg(val);
    int ret = Send(scpi, timeout);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send READ? failed: " << ret;
        return ret;
    }
    return PssRstSuccess;
}

int PLx00ScpiClient::SendImPower(const float &val, uint32_t timeout)
{
    QString scpi = QString(":SOUR:BACK:RANG %1").arg(val);
    int ret = Send(scpi, timeout);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send READ? failed: " << ret;
        return ret;
    }
    return PssRstSuccess;
}

int PLx00ScpiClient::SendOpticalPort(const int &val, uint32_t timeout)
{
    QString scpi = QString(":SYST:PORT %1").arg(val);
    int ret = Send(scpi, timeout);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send READ? failed: " << ret;
        return ret;
    }
    return PssRstSuccess;
}

int PLx00ScpiClient::SendCalFire(uint32_t timeout)
{
    QString scpi = QString(":PSS:ANLG:CAL:FIRE");
    int ret = Send(scpi, timeout);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send READ? failed: " << ret;
        return ret;
    }
    return PssRstSuccess;
}

int PLx00ScpiClient::SendCalVal(const int &index, const double &dac0, const double &dac1, const double &adc0, const double &adc1, uint32_t timeout)
{
    QString scpi = QString(":PSS:ANLG:CAL:PARA %1,%2,%3,%4,%5").arg(index)
            .arg(dac0)
            .arg(dac1)
            .arg(adc0)
            .arg(adc1);
    int ret = Send(scpi, timeout);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send READ? failed: " << ret;
        return ret;
    }
    return PssRstSuccess;
}