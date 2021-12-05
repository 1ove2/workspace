#include <QThread>

#include "Sx00ScpiClient.h"
#include "QtTcpClient.h"
#include "QtCom.h"
#ifdef _PSS_GPIB_SUPPORT_
#include "GPIBIo.h"
#endif
using namespace Pss;

using namespace Pss::Qt;

const QString Sx00ScpiClient::mcClientIdStr = QString("Sx00ScpiClient");

Sx00ScpiClient::Sx00ScpiClient(IIo *iio, const QString &idStr):
    ScpiClient(iio, idStr),
    mDeleteIo(false)
{
}

Sx00ScpiClient::Sx00ScpiClient(const QHostAddress &ip, const QString &idStr):
    ScpiClient(new QtTcpClient(ip, 5025, mcClientIdStr), idStr),
    mDeleteIo(true)
{
}

Sx00ScpiClient::Sx00ScpiClient(const QString &commName, uint32_t commBaud, const QString &idStr):
    ScpiClient(new QtCom(commName, commBaud, mcClientIdStr), idStr),
    mDeleteIo(true)
{
}

#ifdef _PSS_GPIB_SUPPORT_
Sx00ScpiClient::Sx00ScpiClient(uint8_t cardAddr, uint8_t instrumentAddr, const QString &idStr):
    ScpiClient(new GPIBIo(instrumentAddr, mcClientIdStr, cardAddr), idStr),
    mDeleteIo(true)
{
}
#endif
Sx00ScpiClient::~Sx00ScpiClient(void)
{
    if(mDeleteIo)
    {
        delete mIIo;
        mIIo = nullptr;
    }
}

int Sx00ScpiClient::SendRead(QString &readRstStr, uint32_t timeout)
{
    QString scpiStr = QString(":READ?");
    int ret = Send(scpiStr, timeout);
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

int Sx00ScpiClient::SendResolution(bool isV, int digits) const
{
    QString funcStr = QString("VOLT");
    if(!isV)
    {
        funcStr = QString("CURR");
    }
    QString scpiStr = QString(":DISP:%1:DIG %2\n").arg(funcStr).arg(QString::number(digits));
    return Send(scpiStr);
}

int Sx00ScpiClient::SendOutputCtrl(bool on) const
{
    QString scpiStr = QString(":OUTP ");
    if(on)
    {
        scpiStr.append("ON\n");
    }
    else
    {
        scpiStr.append("OFF\n");
    }

    return Send(scpiStr);
}

int Sx00ScpiClient::SendVSrc(bool isVSrc) const
{
    QString srcName = ToScpiName(isVSrc);

    /* 源选择 */
    QString scpiStr = QString(":SOUR:FUNC %1").arg(srcName);
    return Send(scpiStr);
}

int Sx00ScpiClient::SendSrcRange(bool isVSrc, double val) const
{
    /* 设置源量程 */
    QString srcName = ToScpiName(isVSrc);
    QString scpiStr = QString(":SOUR:%1:RANG %2").arg(srcName).arg(val);

    return Send(scpiStr);
}

int Sx00ScpiClient::SendSrcVal(bool isVSrc, double val) const
{
    /* 设置源值 */
    QString srcName = ToScpiName(isVSrc);
    QString scpiStr = QString(":SOUR:%1:LEV %2").arg(srcName).arg(val);

    return Send(scpiStr);
}

int Sx00ScpiClient::SendLmtRange(bool isVSrc, double val) const
{
    /* 设置限量程 */
    QString lmtName = ToScpiName(!isVSrc);
    QString scpiStr = QString(":SENS:%1:RANG %2").arg(lmtName).arg(val);

    return Send(scpiStr);
}

int Sx00ScpiClient::SendLmtVal(bool isVSrc, double val) const
{
    /* 2450与2400不同 */
    /* 设置限值 */
    QString srcName = ToScpiName(isVSrc);
    QString lmtName;
    if(isVSrc)
    {
        lmtName = QString("ILIM");
    }
    else
    {
        lmtName = QString("VLIM");
    }

    QString scpiStr = QString(":SOUR:%1:%2 %3").arg(srcName).arg(lmtName).arg(val);
    
    return Send(scpiStr);
}

int Sx00ScpiClient::SendNplc(bool isVSens, double nplc) const
{
    /* 设置 NPLC */
    QString name = ToScpiName(isVSens);
    QString scpiStr = QString(":SENS:%1:NPLC %2").arg(name).arg(nplc);

    return Send(scpiStr);
}

int Sx00ScpiClient::Send4W(bool is4W) const
{
    /* 设置24线 */
    QString str;
    if(is4W)
    {
        str = QString("ON");
    }
    else
    {
        str = QString("OFF");
    }

    QString scpiStr = QString(":SYST:RSEN %1").arg(str);
    
    return Send(scpiStr);
}

int Sx00ScpiClient::SendF(bool isF) const
{
    /* 设置前后面板 */
    QString str;

    if(isF)
    {
        str = QString("FRON");
    }
    else
    {
        str = QString("REAR");
    }
    
    QString scpiStr = QString(":ROUT:TERM %1").arg(str);
    
    return Send(scpiStr);
}

int Sx00ScpiClient::SendScanStartVal(bool isVSrc, double val) const
{
    /* 设置扫描起点 */
    QString name = ToScpiName(isVSrc);

    QString scpiStr = QString(":SOUR:%1:STAR %2").arg(name).arg(val);
    
    return Send(scpiStr);
}

int Sx00ScpiClient::SendScanStopVal(bool isVSrc, double val) const
{
    /* 设置扫描终点 */
    QString name = ToScpiName(isVSrc);

    QString scpiStr = QString(":SOUR:%1:STOP %2").arg(name).arg(val);
    
    return Send(scpiStr);
}

int Sx00ScpiClient::SendScanPointNum(uint32_t val) const
{
    if(val > mcMaxSweepPoints)
    {
        qCritical() << "sweep points " << val << " more than " << mcMaxSweepPoints;
        return PssRstParameterError;
    }
    /* 设置扫描点数 */
    QString scpiStr = QString(":SOUR:SWE:POIN %1").arg(val);

    return Send(scpiStr);
}

int Sx00ScpiClient::SendScanType(bool isVSrc, Sx00ScpiSweepType type) const
{
    /* 设置扫描源 */
    QString name = ToScpiName(isVSrc);
    QString mode("SWE");
    switch(type)
    {
        case Sx00ScpiSweepTypeLiner:
        {
            mode = "SWE";
            break;
        }
        case Sx00ScpiSweepTypeLog:
        {
            mode = "LOG";
            break;
        }
        case Sx00ScpiSweepTypeCustom:
        {
            mode = "LIST";
            break;
        }
        case Sx00ScpiSweepTypeFix:
        {
            mode = "FIX";
            break;
        }
        default:
        {
            return PssRstParameterError;
        }
    }

    QString scpiStr = QString(":SOUR:%1:MODE %2").arg(name).arg(mode);
    
    return Send(scpiStr);
}

int Sx00ScpiClient::SendScanMaster(bool isMaster) const
{
    QString name;

    if(isMaster)
    {
        name = "SOUR";
    }
    else
    {
        name = "ACC";
    }
    
    QString scpiStr = QString(":TRIG:DIR %1").arg(name);
    
    return Send(scpiStr);
}

int Sx00ScpiClient::SendListVal(bool isVSrc, const QVector<double> &data) const
{
    /* 自定义参数每次最多设置100个点 */
    if(data.size() > mcPerListSweepValNumbers)
    {
        qCritical() << "list set numbers( " << data.size() << ") more than : " << mcPerListSweepValNumbers;
        return PssRstOverFlowError;
    }
    QString name = ToScpiName(isVSrc);
    QStringList list;
    for(int i = 0; i < data.size();i++)
    {
        list.append(QString::number(data[i], 'E', 4));
    }
    QString scpiStr = QString(":SOUR:LIST:%1 %2").arg(name).arg(list.join(","));
    int ret = Send(scpiStr);
    if(PssRstSuccess != ret)
    {
        qCritical() << "set list sweep val failed: " << ret;
    }
    return ret;
}

int Sx00ScpiClient::SendListNum(uint32_t num) const
{
    if(num > mcMaxSweepPoints)
    {
        qCritical() << "sweep points " << num << " more than " << mcMaxSweepPoints;
        return PssRstParameterError;
    }

    QString scpiStr = QString(":SOUR:SWE:POIN %1").arg(num);
    return Send(scpiStr);
}

/* FIXME: 研究自动量程是否会降低精度 */
int Sx00ScpiClient::SendConfigure(bool isV, double lmtVal)
{
    PSS_UNUSED(lmtVal);

    QString name = ToScpiName(isV);
    QString scpiStr = QString("CONF:%1:DC\n").arg(name);
    return Send(scpiStr);
}

int Sx00ScpiClient::SendFireCalParameter(void)
{
    QString scpi(":PSS:ANLG:CAL:FIRE\n");
    int ret = Send(scpi);
    if(PssRstSuccess != ret)
    {
        return ret;
    }
    QString str;
    ret = Recv(str);
    if(ret <= 0)
    {
        return PssRstRecvFromIOError;
    }
    return PssRstSuccess;
}

int Sx00ScpiClient::SendGetAdcValue(bool isV, double &adc, uint32_t timeout)
{

    QString type("VOLT");
    if(!isV)
    {
        type = QString("CURR");
    }
    QString scpi = QString(":PSS:ANLG:%1:ADC?\n").arg(type);
    mIIo->ClearIn();
    int ret = Send(scpi, timeout);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send get adc failed !";
        return ret;
    }
    
    QString res;
    ret = Recv(res, timeout);
    if(ret <= 0)/* Recv不会返回0 */
    {
        qCritical() << "recv adc return value failed: " << ret;
        return PssRstReadError;
    }
    adc = res.toDouble();
    return PssRstSuccess;
}

int Sx00ScpiClient::SendGetDacVaule(bool isV, double &dac, uint32_t timeout)
{
    QString type("VOLT");
    if(!isV)
    {
        type = QString("CURR");
    }
    QString scpi = QString(":PSS:ANLG:%1:DAC?\n").arg(type);

    mIIo->ClearIn();
    int ret = Send(scpi, timeout);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send get adc failed !";
        return ret;
    }
    
    QString res;
    
    ret = Recv(res, timeout);
    if(ret <= 0)/* Recv不会返回0 */
    {
        qCritical() << "recv adc return value failed !";
        return PssRstReadError;
    }
    
    dac = res.toDouble();
    return PssRstSuccess;
}

int Sx00ScpiClient::SendSetCalParameter(int index, double dac0, double dac1, double adc0, double adc1)
{
    QString scpi(QString(":PSS:ANLG:CAL:PARA \"%1,%2,%3,%4,%5\"\n")
                 .arg(index)
                 .arg(dac0)
                 .arg(dac1)
                 .arg(adc0)
                 .arg(adc1));

    return Send(scpi);
}


int Sx00ScpiClient::SendGetOutput(bool& on, uint32_t timeout)
{
    QString scpiStr = QString(":OUTP?\n");
    int ret = Send(scpiStr, timeout);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send output? failed: " << ret;
        return ret;
    }
    QString status;

    ret = Recv(status, timeout);
    if(ret <= 0)/* Recv不会返回0 */
    {
        qCritical() << "recv output? failed: " << ret;
        return PssRstReadError;
    }
    
    if(status.contains("ON"))
    {
        on = true;
    }
    else if(status.contains("OFF"))
    {
        on = false;
    }
    else
    {
        qCritical() << "invalid output? return : " << status;
        return PssRstReadError;
    }
    
    return PssRstSuccess;
}

int Sx00ScpiClient::SendLmtAbort(bool on)
{
    QString scpiStr;
    if(on)
    {
        scpiStr = QString(":SOUR:SWE:CAB ON\n");
    }
    else
    {
        scpiStr = QString(":SOUR:SWE:CAB OFF\n");
    }
    return Send(scpiStr);
}

int Sx00ScpiClient::SendGetSrcType(bool &isVSrc, uint32_t timeout)
{
    QString scpiStr(":SOUR:FUNC?\n");
    int ret = Send(scpiStr, timeout);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send get src type failed: " << ret;
        return ret;
    }
    QString src;
    ret = Recv(src, timeout);
    if(ret <= 0)/* Recv不会返回0 */
    {
        qCritical() << "receive src type failed: " << ret;
        return PssRstReadFromIOError;
    }

    if(src.contains("VOLT"))
    {
        isVSrc = true;
    }
    else if(src.contains("CURR"))
    {
        isVSrc = false;
    }
    else
    {
        qCritical() << "invalid return for src type: " << src;
        return PssRstReadFromIOError;
    }
    return PssRstSuccess;
}

int Sx00ScpiClient::SendMeasureQuery(bool isVSrc, double& val, uint32_t timeout)
{
    QString scpiStr(":MEAS:");
    if(isVSrc)
    {
        scpiStr.append("VOLT?\n");
    }
    else
    {
        scpiStr.append("CURR?\n");
    }
    int ret = Send(scpiStr, timeout);
    if(PssRstSuccess == ret)
    {
        /* 延时保证设备测量界面初始化完成 */
        QThread::msleep(1000);
    }
    QString valStr;
    ret = Recv(valStr, timeout);
    if(ret <= 0)/* Recv不会返回0 */
    {
        qCritical() << "receive src type failed: " << ret;
        return PssRstReadFromIOError;
    }
    val = valStr.toDouble();
    return ret;
}

int Sx00ScpiClient::SendAppendListVal(bool isV, const QVector<double>& data)
{
    if(data.size() > mcPerListSweepValNumbers)
    {
        qCritical() << "list append numbers( " << data.size() << ") more than : " << mcPerListSweepValNumbers;
        return PssRstOverFlowError;
    }
    QStringList list;
    for(int i = 0;i < data.size();i++)
    {
        list.append(QString::number(data[i], 'E', 4));
    }
    QString name = ToScpiName(isV);
    QString scpiStr = QString(":SOUR:LIST:%1:APP %2").arg(name).arg(list.join(","));
    int ret = Send(scpiStr);
    if(PssRstSuccess != ret)
    {
        qCritical() << "set list sweep val failed: " << ret;
    }
    return ret;
}

int Sx00ScpiClient::SendSrcAutoRange(bool isVSrc, bool on)
{
    QString srcName = ToScpiName(isVSrc);
    QString onStr = "ON";
    if(!on)
    {
        onStr = "OFF";
    }
    QString scpiStr = QString(":SOUR:%1:RANG:AUTO %2\n").arg(srcName).arg(onStr);
    return Send(scpiStr);
}

int Sx00ScpiClient::SendLmtAutoRange(bool isVSrc, bool on)
{
    QString lmtName = ToScpiName(!isVSrc);
    QString onStr = "ON";
    if(!on)
    {
        onStr = "OFF";
    }
    QString scpiStr = QString(":SENS:%1:RANG:AUTO %2\n").arg(lmtName).arg(onStr);
    return Send(scpiStr);
}

int Sx00ScpiClient::SendSrcAutoRangeQuery(bool isVSrc, bool *on, uint32_t timeout)
{
    if(!on)
    {
        return PssRstParameterError;
    }
    QString srcName = ToScpiName(isVSrc);

    QString scpiStr = QString(":SOUR:%1:RANG:AUTO?\n").arg(srcName);
    int ret = Send(scpiStr, timeout);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send SendSrcAutoRangeQuery failed: " << ret;
        return ret;
    }
    QString str;
    ret = Recv(str, timeout);
    if(ret <= 0)
    {
        qCritical() << "recv SrcAutoRangeQuery failed: " << ret;
        return ret;
    }
    
    if(str.contains("ON"))
    {
        *on = true;
    }
    else if(str.contains("OFF"))
    {
        *on = false;
    }
    else
    {
        qCritical() << "invalid return value: " << str;
        return PssRstReadError;
    }
    
    return PssRstSuccess;
}

int Sx00ScpiClient::SendLmtAutoRangeQuery(bool isVSrc, bool *on, uint32_t timeout)
{
    if(!on)
    {
        return PssRstParameterError;
    }

    QString lmtName = ToScpiName(!isVSrc);
    QString scpiStr = QString(":SENS:%1:RANG:AUTO?\n").arg(lmtName);
    int ret = Send(scpiStr, timeout);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send LmtAutoRangeQuery failed: " << ret;
        return ret;
    }
    QString str;
    ret = Recv(str, timeout);
    if(ret <= 0)
    {
        qCritical() << "recv LmtAutoRangeQuery failed: " << ret;
        return ret;
    }
    
    if(str.contains("ON"))
    {
        *on = true;
    }
    else if(str.contains("OFF"))
    {
        *on = false;
    }
    else
    {
        qCritical() << "invalid return value: " << str;
        return PssRstReadError;
    }
    
    return PssRstSuccess;
}

int Sx00ScpiClient::SendSrcRangeQuery(bool isVSrc, char *range, uint32_t timeout)
{
    if(!range)
    {
        return PssRstParameterError;
    }
    QString srcName = ToScpiName(isVSrc);
    QString scpiStr = QString(":SOUR:%1:RANG?\n").arg(srcName);
    int ret = Send(scpiStr, timeout);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send SrcRangeQuery failed: " << ret;
        return ret;
    }
    QString str;
    ret = Recv(str, timeout);
    if(ret <= 0)
    {
        qCritical() << "recv SrcRangeQuery failed: " << ret;
        return ret;
    }

    memcpy(range, str.toStdString().c_str(), str.length());
    
    return PssRstSuccess;
}

int Sx00ScpiClient::SendLmtRangeQuery(bool isVSrc, char *range, uint32_t timeout)
{
    if(!range)
    {
        return PssRstParameterError;
    }
    QString lmtName = ToScpiName(!isVSrc);
    QString scpiStr = QString(":SENS:%1:RANG?\n").arg(lmtName);
    int ret = Send(scpiStr);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send LmtRangeQuery failed: " << ret;
        return ret;
    }
    QString str;
    ret = Recv(str, timeout);
    if(ret <= 0)
    {
        qCritical() << "recv LmtRangeQuery failed: " << ret;
        return ret;
    }

    memcpy(range, str.toStdString().c_str(), str.length());
    
    return PssRstSuccess;
}

int Sx00ScpiClient::SendTraceSet(bool on)
{
    QString scpiStr;
    if(on)
    {
        scpiStr = QString(":TRAC:TRIG\n");
    }
    else
    {
        scpiStr = QString(":TRAC:CLE\n");
    }
    return Send(scpiStr);
}


int Sx00ScpiClient::SendVersionReq(QString &idnRstStr, uint32_t timeout)
{
    QString scpiStr = QString(":PSS:VERS?\n");
    int ret = Send(scpiStr);
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

int Sx00ScpiClient::SendNetUpgradePort(uint32_t port)
{
    QString scpiStr = QString(":PSS:NETUPGRADE %1\n").arg(port);

    return Send(scpiStr);
}

int Sx00ScpiClient::SendAnlgStateReq(QString &retStr, uint32_t timeout)
{
    QString scpiStr = QString(":PSS:ANLG:STAT?\n");
    int ret = Send(scpiStr);
    if(PssRstSuccess != ret)
    {
        return ret;
    }

    ret = Recv(retStr, timeout);
    if(ret <= 0)/* Recv不会返回0 */
    {
        qCritical() << "recv idn failed: " << ret;
        return ret;
    }
    
    return PssRstSuccess;
}

int Sx00ScpiClient::SendSensFunc(bool isVSrc) const
{
    QString lmtName = ToScpiName(!isVSrc);
    /* 源选择 */
    QString scpiStr = QString(":SENS:FUNC \"%1\"").arg(lmtName);
    return Send(scpiStr);
}