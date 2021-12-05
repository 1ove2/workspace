#include "Px00ScpiClient.h"
#include "QtTcpClient.h"
#include "QtCom.h"
#ifdef _PSS_GPIB_SUPPORT_
#include "GPIBIo.h"
#endif

using namespace Pss::Qt;

const QString Px00ScpiClient::mcClientIdStr = QString("Px00ScpiClient");

Px00ScpiClient::Px00ScpiClient(Pss::IIo *iio, const QString &idStr):
    ScpiClient(iio, idStr),
    mDeleteIo(false)
{

}

Px00ScpiClient::Px00ScpiClient(const QHostAddress &ip, const QString &idStr):
    ScpiClient(new QtTcpClient(ip, 5025, mcClientIdStr), idStr),
    mDeleteIo(true)
{
}

Px00ScpiClient::Px00ScpiClient(const QString &commName, uint32_t commBaud, const QString &idStr):
    ScpiClient(new QtCom(commName, commBaud, mcClientIdStr), idStr),
    mDeleteIo(true)
{
}

#ifdef _PSS_GPIB_SUPPORT_
Px00ScpiClient::Px00ScpiClient(uint8_t cardAddr, uint8_t instrumentAddr, const QString &idStr):
    ScpiClient(new GPIBIo(instrumentAddr, mcClientIdStr, cardAddr), idStr),
    mDeleteIo(true)
{
}
#endif

Px00ScpiClient::~Px00ScpiClient()
{
    if(mDeleteIo)
    {
        delete mIIo;
        mIIo = nullptr;
    }
}

int Px00ScpiClient::SendRead(uint8_t chanID, QString &readRstStr, uint32_t timeout)
{
    /* 发送READ并获取返回值 */
    QString scpiStr = QString(":READ%1?").arg(chanID);
    int ret = Send(scpiStr);
    if(PssRstSuccess != ret)
    {
        qDebug() << "send READ? failed: " << ret;
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

int Px00ScpiClient::SendOutputCtrl(uint8_t chanID, bool on) const
{
    /* 设置输出状态 */
    QString str = on? "ON" : "OFF";
    QString scpiStr = QString(":OUTP%1 %2").arg(chanID).arg(str);

    return Send(scpiStr);
}

int Px00ScpiClient::SendVSrc(uint8_t chanID, bool isVSrc) const
{
    /* 源选择 */
    QString srcName = ToScpiName(isVSrc);
    QString scpiStr = QString(":SOUR%1:FUNC %2").arg(chanID).arg(srcName);

    return Send(scpiStr);
}

int Px00ScpiClient::SendSrcRange(uint8_t chanID, bool isVSrc, double val) const
{
    /* 设置源量程 */
    QString srcName = ToScpiName(isVSrc);
    QString scpiStr = QString(":SOUR%1:%2:RANG %3").arg(chanID).arg(srcName).arg(val);

    return Send(scpiStr);
}

int Px00ScpiClient::SendSrcVal(uint8_t chanID, bool isVSrc, double val) const
{
    /* 设置源值 */
    QString srcName = ToScpiName(isVSrc);
    QString scpiStr = QString(":SOUR%1:%2:LEV %3").arg(chanID).arg(srcName).arg(val);

    return Send(scpiStr);
}

int Px00ScpiClient::SendLmtRange(uint8_t chanID, bool isVSrc, double val) const
{
    /* 设置限量程 */
    QString lmtName = ToScpiName(!isVSrc);
    QString scpiStr = QString(":SENS%1:%2:RANG %3").arg(chanID).arg(lmtName).arg(val);

    return Send(scpiStr);
}

int Px00ScpiClient::SendLmtVal(uint8_t chanID, bool isVSrc, double val) const
{
    /* 设置限值 */
    QString srcName = ToScpiName(isVSrc);
    QString lmtName = isVSrc? "ILIM" : "VLIM";
    QString scpiStr = QString(":SOUR%1:%2:%3 %4").arg(chanID).arg(srcName).arg(lmtName).arg(val);

    return Send(scpiStr);
}

int Px00ScpiClient::Send4W(uint8_t chanID, bool is4W) const
{
    /* 设置24线 */
    QString str = is4W? "ON" : "OFF";
    QString scpiStr = QString(":SYST%1:RSEN %2").arg(chanID).arg(str);

    return Send(scpiStr);
}

int Px00ScpiClient::SendF(uint8_t chanID, bool isF) const
{
    Q_UNUSED(chanID);
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

int Px00ScpiClient::SendScanStartVal(uint8_t chanID, bool isVSrc, double val) const
{
    /* 设置扫描起点 */
    QString name = ToScpiName(isVSrc);
    QString scpiStr = QString(":SOUR%1:%2:STAR %3").arg(chanID).arg(name).arg(val);

    return Send(scpiStr);
}

int Px00ScpiClient::SendScanStopVal(uint8_t chanID, bool isVSrc, double val) const
{
    /* 设置扫描终点 */
    QString name = ToScpiName(isVSrc);
    QString scpiStr = QString(":SOUR%1:%2:STOP %3").arg(chanID).arg(name).arg(val);

    return Send(scpiStr);
}

int Px00ScpiClient::SendScanStepVal(uint8_t chanID, bool isVSrc, double val) const
{
    /* 设置扫描步进 */
    QString name = ToScpiName(isVSrc);
    QString scpiStr = QString(":SOUR%1:%2:STEP %3").arg(chanID).arg(name).arg(val);

    return Send(scpiStr);
}

int Px00ScpiClient::SendScanPointNum(uint8_t chanID, uint32_t points) const
{
    if(mcMaxSweepPoints < points)
    {
        return PssRstOverFlowError;
    }
    /* 设置扫描点数 */
    QString scpiStr = QString(":SOUR%1:SWE:POIN %2").arg(chanID).arg(points);

    return Send(scpiStr);
}

int Px00ScpiClient::SendScanType(uint8_t chanID, bool isVSrc, Px00ScpiSweepType type) const
{
    /* 设置扫描类型 */
    QString name = ToScpiName(isVSrc);
    QString mode("SWE");
    switch(type)
    {
        case Px00ScpiSweepTypeLiner:
        {
            mode = "SWE";
            break;
        }
        case Px00ScpiSweepTypeLog:
        {
            mode = "LOG";
            break;
        }
        case Px00ScpiSweepTypeCustom:
        {
            mode = "LIST";
            break;
        }
        case Px00ScpiSweepTypeFix:
        {
            mode = "FIX";
            break;
        }
        default:
        {
            return PssRstParameterError;
        }
    }
    QString scpiStr = QString(":SOUR%1:%2:MODE %3").arg(chanID).arg(name).arg(mode);

    return Send(scpiStr);
}

int Px00ScpiClient::SendScanMaster(uint8_t chanID, bool isMaster) const
{
    /* 设置主从设备 */
    QString str = isMaster? "SOUR" : "ACC";
    QString scpiStr = QString(":TRIG%1:DIR %2").arg(chanID).arg(str);

    return Send(scpiStr);
}

int Px00ScpiClient::SendTrigInput(bool on)
{
    /* 设置trig开关 */
    QString str = on? "ON" : "OFF";
    QString scpiStr = QString(":TRIG:INP %1").arg(str);

    return Send(scpiStr);
}

int Px00ScpiClient::SendListVal(uint8_t chanID, bool isVSrc, const QVector<double> &data) const
{
    /* 自定义参数每次最多设置50个点 */
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
    QString scpiStr = QString(":SOUR%1:LIST:%2 %3").arg(chanID).arg(name).arg(list.join(","));
    return Send(scpiStr);
}

int Px00ScpiClient::SendAppendListVal(uint8_t chanID, bool isV, const QVector<double> &data)
{
    /* 追加设置自定义扫描参数 */
    if(data.size() > mcPerListSweepValNumbers)
    {
        qCritical() << "list append numbers( " << data.size() << ") more than : " << mcPerListSweepValNumbers;
        return PssRstOverFlowError;
    }
    QString name = ToScpiName(isV);
    QStringList list;
    for(int i = 0;i < data.size();i++)
    {
        list.append(QString::number(data[i], 'E', 4));
    }
    QString scpiStr = QString(":SOUR%1:LIST:%2:APP %3").arg(chanID).arg(name).arg(list.join(","));
    return Send(scpiStr);
}

int Px00ScpiClient::SendLmtAbort(uint8_t chanID, bool on)
{
    QString str = on? "ON" : "OFF";
    QString scpiStr = QString(":SOUR%1:SWE:CAB %2").arg(chanID).arg(str);

    return Send(scpiStr);
}

int Px00ScpiClient::SendNplc(uint8_t chanID, bool isVSens, double nplc) const
{
    /* 设置 NPLC */
    QString name = ToScpiName(isVSens);
    QString scpiStr = QString(":SENS%1:%2:NPLC %3").arg(chanID).arg(name).arg(nplc);

    return Send(scpiStr);
}

int Px00ScpiClient::SendGetOutput(uint8_t chanID, bool &on, uint32_t timeout)
{
    QString scpiStr = QString(":OUTP%1?").arg(chanID);
    int ret = Send(scpiStr);
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

int Px00ScpiClient::SendGetSrcType(uint8_t chanID, bool &isVSrc, uint32_t timeout)
{
    QString scpiStr = QString(":SOUR%1:FUNC?").arg(chanID);
    int ret = Send(scpiStr);
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

int Px00ScpiClient::SendSrcAutoRange(uint8_t chanID, bool isVSrc, bool on)
{
    QString srcName = ToScpiName(isVSrc);
    QString str = on? "ON" : "OFF";

    QString scpiStr = QString(":SOUR%1:%2:RANG:AUTO %3").arg(chanID).arg(srcName).arg(str);
    return Send(scpiStr);
}

int Px00ScpiClient::SendLmtAutoRange(uint8_t chanID, bool isVSrc, bool on)
{
    QString lmtName = ToScpiName(!isVSrc);
    QString str = on? "ON" : "OFF";

    QString scpiStr = QString(":SENS%1:%2:RANG:AUTO %3").arg(chanID).arg(lmtName).arg(str);
    return Send(scpiStr);
}

int Px00ScpiClient::SendSrcAutoRangeQuery(uint8_t chanID, bool isVSrc, bool *on, uint32_t timeout)
{
    if(!on)
    {
        return PssRstParameterError;
    }
    QString srcName = ToScpiName(isVSrc);

    QString scpiStr = QString(":SOUR%1:%2:RANG:AUTO?\n").arg(chanID).arg(srcName);
    int ret = Send(scpiStr);
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

int Px00ScpiClient::SendLmtAutoRangeQuery(uint8_t chanID, bool isVSrc, bool *on, uint32_t timeout)
{
    if(!on)
    {
        return PssRstParameterError;
    }

    QString lmtName = ToScpiName(!isVSrc);
    QString scpiStr = QString(":SENS%1:%2:RANG:AUTO?\n").arg(chanID).arg(lmtName);
    int ret = Send(scpiStr);
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

int Px00ScpiClient::SendSrcRangeQuery(uint8_t chanID, bool isVSrc, char *range, uint32_t timeout)
{
    if(!range)
    {
        return PssRstParameterError;
    }
    QString srcName = ToScpiName(isVSrc);
    QString scpiStr = QString(":SOUR%1:%2:RANG?\n").arg(chanID).arg(srcName);
    int ret = Send(scpiStr);
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

int Px00ScpiClient::SendLmtRangeQuery(uint8_t chanID, bool isVSrc, char *range, uint32_t timeout)
{
    if(!range)
    {
        return PssRstParameterError;
    }
    QString lmtName = ToScpiName(!isVSrc);
    QString scpiStr = QString(":SENS%1:%2:RANG?").arg(chanID).arg(lmtName);
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

int Px00ScpiClient::SendOutputMode(uint8_t chanID, bool isPulse)
{
    QString str = isPulse? "PULS" : "DC";
    QString scpiStr = QString(":SOUR%1:FUNC:SHAP %2").arg(chanID).arg(str);

    return Send(scpiStr);
}

int Px00ScpiClient::SendOutputModeQuery(uint8_t chanID, bool &isPulse, uint32_t timeout)
{
    QString scpiStr = QString(":SOUR%1:FUNC:SHAP?").arg(chanID);
    int ret = Send(scpiStr);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send SHAP? failed: " << ret;
        return ret;
    }
    QString status;

    ret = Recv(status, timeout);
    if(ret <= 0)/* Recv不会返回0 */
    {
        qCritical() << "recv SHAP? failed: " << ret;
        return PssRstReadError;
    }
    
    if(status.contains("PULS"))
    {
        isPulse = true;
    }
    else if(status.contains("DC"))
    {
        isPulse = false;
    }
    else
    {
        qCritical() << "invalid SHAP? return : " << status;
        return PssRstReadError;
    }
    
    return PssRstSuccess;
}

int Px00ScpiClient::SendPulseDelay(uint8_t chanID, uint32_t delayNs)
{
    QString scpiStr = QString(":SOUR%1:PULS:DEL %2").arg(chanID).arg(delayNs);

    return Send(scpiStr);
}

int Px00ScpiClient::SendPulseDelayQuery(uint8_t chanID, uint32_t &delayNs, uint32_t timeout)
{
    QString scpiStr = QString(":SOUR%1:PULS:DEL?").arg(chanID);

    int ret = Send(scpiStr);
    if (PssRstSuccess != ret)
    {
        qCritical() << "send pulse delay? failed: " << ret;
        return ret;
    }
    QString delayStr;
    
    ret = Recv(delayStr, timeout);
    if(ret <= 0)/* Recv不会返回0 */
    {
        qCritical() << "recv pulse delay? failed: " << ret;
        return PssRstReadError;
    }
    
    delayNs = delayStr.toUInt();
    return PssRstSuccess;
}

int Px00ScpiClient::SendPulseWidth(uint8_t chanID, uint32_t pulseWidth)
{
    QString scpiStr = QString(":SOUR%1:PULS:WIDT %2").arg(chanID).arg(pulseWidth);

    return Send(scpiStr);
}

int Px00ScpiClient::SendPulseWidthQuery(uint8_t chanID, uint32_t &pulseWidth, uint32_t timeout)
{
    QString scpiStr = QString(":SOUR%1:PULS:WIDT?").arg(chanID);

    int ret = Send(scpiStr);
    if (PssRstSuccess != ret)
    {
        qCritical() << "send pulse delay? failed: " << ret;
        return ret;
    }
    QString pulseWidthStr;
    
    ret = Recv(pulseWidthStr, timeout);
    if(ret <= 0)/* Recv不会返回0 */
    {
        qCritical() << "recv pulse delay? failed: " << ret;
        return PssRstReadError;
    }
    
    pulseWidth = pulseWidthStr.toUInt();
    return PssRstSuccess;
}

int Px00ScpiClient::SendPulsePeriod(uint8_t chanID, uint32_t pulsePeriod)
{
    QString scpiStr = QString(":SOUR%1:PULS:PERI %2").arg(chanID).arg(pulsePeriod);

    return Send(scpiStr);
}

int Px00ScpiClient::SendPulsePeriodQuery(uint8_t chanID, uint32_t &pulsePeriod, uint32_t timeout)
{
    QString scpiStr = QString(":SOUR%1:PULS:PERI?").arg(chanID);

    int ret = Send(scpiStr);
    if (PssRstSuccess != ret)
    {
        qCritical() << "send pulse delay? failed: " << ret;
        return ret;
    }
    QString pulsePeriodStr;
    
    ret = Recv(pulsePeriodStr, timeout);
    if(ret <= 0)/* Recv不会返回0 */
    {
        qCritical() << "recv pulse delay? failed: " << ret;
        return PssRstReadError;
    }
    
    pulsePeriod = pulsePeriodStr.toUInt();
    return PssRstSuccess;
}

int Px00ScpiClient::SendTraceSet(uint8_t chanID, bool on)
{
    QString str = on? "TRIG" : "CLE";
    QString scpiStr = QString(":TRAC%1:%2").arg(chanID).arg(str);

    return Send(scpiStr);
}

int Px00ScpiClient::SendTrigDelay(uint8_t chanID, uint32_t delayUs)
{
    QString scpiStr = QString(":TRIG%1:DEL %2").arg(chanID).arg(delayUs);

    return Send(scpiStr);
}

int Px00ScpiClient::SendInitTrigState(uint8_t chanID, Px00ScpiClient::Px00ScpiTrigType trigType)
{
    QString type = "ALL";
    switch(trigType)
    {
        case Px00ScpiTrigTypeSOUR:
        {
            type = "SOUR";
            break;
        }
        case Px00ScpiTrigTypeMEAS:
        {
            type = "MEAS";
            break;
        }
        case Px00ScpiTrigTypeALL:
        {
            type = "ALL";
            break;
        }
        default:
        {
            return PssRstParameterError;
        }
    }

    QString scpiStr = QString(":TRIG%1:INIT:%2").arg(chanID).arg(type);
    
    return Send(scpiStr);
}

int Px00ScpiClient::SendTrigStateQuery(uint8_t chanID, Px00ScpiClient::Px00ScpiTrigType trigType, int &state)
{
    QString type = "ALL";
    switch(trigType)
    {
        case Px00ScpiTrigTypeSOUR:
        {
            type = "SOUR";
            break;
        }
        case Px00ScpiTrigTypeMEAS:
        {
            type = "MEAS";
            break;
        }
        case Px00ScpiTrigTypeALL:
        {
            type = "ALL";
            break;
        }
        default:
        {
            return PssRstParameterError;
        }
    }
    QString scpiStr = QString(":TRIG%1:IDLE:%2?\n").arg(chanID).arg(type);
    int ret = Send(scpiStr);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send get trig state failed: " << ret;
        return ret;
    }
    QString stateStr;

    ret = Recv(stateStr, mcWait4RecvTimeout);
    if(ret <= 0)/* Recv不会返回0 */
    {
        qCritical() << "recv get trig state failed: " << ret;
        return ret;
    }
    state = stateStr.toInt();
    return PssRstSuccess;
}

int Px00ScpiClient::SendIPConfig(bool isAuto, const QString &ip, const QString &netMask, const QString &gateWay)
{
    QString type = isAuto? "AUTO" : "MAN";

    QString scpiStr = QString(":SYST:COMM:LAN:CONF \"%1,%2,%3,%4\"").arg(type).arg(ip).arg(netMask).arg(gateWay);
    return Send(scpiStr);
}

int Px00ScpiClient::SendIPConfigQuery(bool &isAuto, QString &ip, QString &netMask, QString &gateWay, uint32_t timeout)
{
    QString scpiStr = QString(":SYST:COMM:LAN:CONF?");
    int ret = Send(scpiStr);
    if (PssRstSuccess != ret)
    {
        qCritical() << "send get ip config failed: " << ret;
        return ret;
    }
    QString recvStr;
    ret = Recv(recvStr, timeout);
    if (ret <= 0)
    {
        qCritical() << "recv get ip config failed: " << ret;
        return ret;
    }
    if(!recvStr.contains(","))
    {
        return PssRstReadFromIOError;
    }
    recvStr.remove(" ");
    QStringList  recvList = recvStr.split(",");
    if (recvList.size() < 4)
    {
        qCritical() << "recv get ip config incomplete";
        return recvList.size();
    }
    QString type = recvList.value(0);
    if (type.contains("AUTO"))
    {
        isAuto = true;
    }
    else if (type.contains("MAN"))
    {
        isAuto = false;
    }
    else
    {
        qCritical() << "invalid return for ip type: " << type;
        return PssRstReadFromIOError;
    }
    ip = recvList.value(1);
    netMask = recvList.value(2);
    gateWay = recvList.value(3);
    gateWay.remove("\n");
    return PssRstSuccess;
}

int Px00ScpiClient::SendUpdateIPConfig()
{
    QString scpiStr = QString(":SYST:COMM:LAN:UPD");

    return Send(scpiStr);
}

int Px00ScpiClient::SendGPIBConfig(uint8_t devAddr)
{
    QString scpiStr = QString(":SYST:COMM:GPIB:ADDR %1").arg(devAddr);

    return Send(scpiStr);
}

int Px00ScpiClient::SendGPIBConfigQuery(bool& on, uint8_t &devAddr, uint32_t timeout)
{
    QString scpiStr = QString(":SYST:COMM:GPIB:ADDR?");
    int ret = Send(scpiStr);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send get gpib config failed: " << ret;
        return ret;
    }
    QString devStr;

    ret = Recv(devStr, timeout);
    if(ret <= 0)/* Recv不会返回0 */
    {
        qCritical() << "recv get gpib config failed: " << ret;
        return ret;
    }
    
    QStringList list = devStr.split(",");
    if(list.size() != 2)
    {
        qCritical() << "invalid GPIB addr str: " << devStr;
        return PssRstReadError;
    }
    if(list[0] == "ON")
    {
        on = true;
    }
    else
    {
        on = false;
    }
    devAddr = list[1].toUShort();
    return PssRstSuccess;
}

int Px00ScpiClient::SendUARTConfig(uint32_t baud)
{
    QString scpiStr = QString(":SYST:COMM:UART:BAUD %1").arg(baud);

    return Send(scpiStr);
}

int Px00ScpiClient::SendUARTConfigQuery(bool& on, uint32_t &baud, uint32_t timeout)
{
    QString scpiStr = QString(":SYST:COMM:UART:BAUD?");
    int ret = Send(scpiStr);
    if(PssRstSuccess != ret)
    {
        qCritical() << "send get uart config failed: " << ret;
        return ret;
    }
    QString baudStr;

    ret = Recv(baudStr, timeout);
    if(ret <= 0)/* Recv不会返回0 */
    {
        qCritical() << "recv get uart config failed: " << ret;
        return ret;
    }
    QStringList list = baudStr.split(",");
    if(list.size() != 2)
    {
        qCritical() << "invalid uart baud str: " << baudStr;
        return PssRstReadError;
    }
    if(list[0] == "ON")
    {
        on = true;
    }
    else
    {
        on = false;
    }
    baud = list[1].toUInt();
    return PssRstSuccess;
}

int Px00ScpiClient::SendMeasureQuery(uint8_t chanID, bool isVSrc, double &val, uint32_t timeout)
{
    QString scpiStr = QString(":MEAS%1:").arg(chanID);
    if(isVSrc)
    {
        scpiStr.append("VOLT?\n");
    }
    else
    {
        scpiStr.append("CURR?\n");
    }
    int ret = Send(scpiStr);
    if(PssRstSuccess != ret)
    {
        return ret;
    }
    QString valStr;
    ret = Recv(valStr, timeout);
    if(ret <= 0)
    {
        return ret;
    }
    val = valStr.toDouble();
    return PssRstSuccess;
}


int Px00ScpiClient::SendFireCalParameter(uint8_t chanID)
{
    QString scpi = QString(":PSS:ANLG%1:CAL:FIRE\n").arg(chanID);
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

int Px00ScpiClient::SendGetAdcValue(uint8_t chanID, bool isV, double &adc, uint32_t timeout)
{
    QString scpi = QString(":PSS:ANLG%1:GET:ADC:").arg(chanID);
    if(isV)
    {
        scpi += QString("VOLT");
    }
    else
    {
        scpi += QString("CURR");
    }
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
    int pos = res.lastIndexOf(":");
    if(pos < 0)
    {
        qCritical() << "recv error adc return: " << res;
        return PssRstReadError;
    }
    adc = res.mid(pos+1).toDouble();
    return PssRstSuccess;
}

int Px00ScpiClient::SendGetDacVaule(uint8_t chanID, bool isV, double &dac, uint32_t timeout)
{
    QString scpi = QString(":PSS:ANLG%1:GET:DAC:").arg(chanID);
    if(isV)
    {
        scpi += QString("VOLT");
    }
    else
    {
        scpi += QString("CURR");
    }
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
    
    int pos = res.lastIndexOf(":");
    if(pos < 0)
    {
        qCritical() << "recv error dac return: " << res;
        return PssRstReadError;
    }
    dac = res.mid(pos+1).toDouble();
    return PssRstSuccess;
}

int Px00ScpiClient::SendSetCalParameter(uint8_t chanID, int index, double dac0, double dac1, double adc0, double adc1)
{
    QString scpi(QString(":PSS:ANLG%1:SET:CALPARA:%2:%3:%4:%5:%6\n")
                 .arg(chanID)
                 .arg(index)
                 .arg(dac0)
                 .arg(dac1)
                 .arg(adc0)
                 .arg(adc1));

    return Send(scpi);
}

int Px00ScpiClient::SendVersionReq(QString &idnRstStr, uint32_t timeout)
{
    QString scpiStr = QString(":PSS:GET:VER\n");
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

int Px00ScpiClient::SendVersionQuery(uint8_t chanID, QString &verStr, uint32_t timeout)
{
    QString scpiStr = QString(":SYST%1:VERS?\n").arg(chanID);
    int ret = Send(scpiStr);
    if(PssRstSuccess != ret)
    {
        return ret;
    }

    ret = Recv(verStr, timeout);
    if(ret <= 0)/* Recv不会返回0 */
    {
        qCritical() << "recv idn failed: " << ret;
        return ret;
    }
    
    return PssRstSuccess;
}

int Px00ScpiClient::SetPulseCount(uint8_t chanID, uint32_t count)
{
    QString scpiStr = QString(":SOUR%1:PULS:COUN %2").arg(chanID).arg(count);

    return Send(scpiStr);
}

int Px00ScpiClient::PulseCountQuery(uint8_t chanID, uint32_t &count, uint32_t timeout)
{
    QString scpiStr = QString(":SOUR%1:PULS:COUN?\n").arg(chanID);
    int ret = Send(scpiStr);
    if(PssRstSuccess != ret)
    {
        return ret;
    }
    QString str;
    ret = Recv(str, timeout);
    if(ret <= 0)/* Recv不会返回0 */
    {
        qCritical() << "recv pulse count failed: " << ret;
        return ret;
    }
    count = str.toUInt();
    return PssRstSuccess;
}

int Px00ScpiClient::SetPulseSamplePoint(uint8_t chanID, uint32_t point)
{
    QString scpiStr = QString(":SOUR%1:PULS:POIN %2").arg(chanID).arg(point);

    return Send(scpiStr);
}

int Px00ScpiClient::PulseSamplePointQuery(uint8_t chanID, uint32_t& point, uint32_t timeout)
{
    QString scpiStr = QString(":SOUR%1:PULS:POIN?\n").arg(chanID);
    int ret = Send(scpiStr);
    if(PssRstSuccess != ret)
    {
        return ret;
    }
    QString str;
    ret = Recv(str, timeout);
    if(ret <= 0)/* Recv不会返回0 */
    {
        qCritical() << "recv pulse sample point failed: " << ret;
        return ret;
    }
    point = str.toUInt();
    return PssRstSuccess;
}

int Px00ScpiClient::TraceDataQuery(uint8_t chanID, const char *traceName, char *data, uint32_t &dataSize, uint32_t timeout)
{
    QString scpiStr = QString(":TRAC%1:DATA? \"%2\"\n").arg(chanID).arg(QString(traceName));
    int ret = Send(scpiStr);
    if(PssRstSuccess != ret)
    {
        return ret;
    }
    if(!data)
    {
        return PssRstParameterError;
    }
    QString str;
    ret = Recv(str, timeout);
    if(ret <= 0)/* Recv不会返回0 */
    {
        qCritical() << "recv pulse sample point failed: " << ret;
        return ret;
    }
    if(str.length() < dataSize)
    {
        dataSize = str.length();
    }
    memcpy(data, str.toUtf8().constData(), dataSize);
    return PssRstSuccess;
}

int Px00ScpiClient::SendFireCalParameter(void)
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

int Px00ScpiClient::SendGetAdcValue(bool isV, double &adc, uint32_t timeout)
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

int Px00ScpiClient::SendGetDacVaule(bool isV, double &dac, uint32_t timeout)
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

int Px00ScpiClient::SendSetCalParameter(int index, double dac0, double dac1, double adc0, double adc1)
{
    QString scpi(QString(":PSS:ANLG:CAL:PARA \"%1,%2,%3,%4,%5\"\n")
                 .arg(index)
                 .arg(dac0)
                 .arg(dac1)
                 .arg(adc0)
                 .arg(adc1));

    return Send(scpi);

}