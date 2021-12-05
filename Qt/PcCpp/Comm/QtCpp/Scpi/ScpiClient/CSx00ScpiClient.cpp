#include "CSx00ScpiClient.h"
#include "QtTcpClient.h"
#include "QtCom.h"
#ifdef _GPIB_SUPPORT_
#include "GPIBIo.h"
#endif


const QString CSx00ScpiClient::mcClientIdStr = QString("CSx00ScpiClient");

CSx00ScpiClient::CSx00ScpiClient(Pss::IIo *iio, const QString &idStr):
    ScpiClient(iio, idStr),
    mDeleteIo(false)
{

}

CSx00ScpiClient::CSx00ScpiClient(const QHostAddress &ip, const QString &idStr):
    ScpiClient(new QtTcpClient(ip, 5025, mcClientIdStr), idStr),
    mDeleteIo(true)
{
}

CSx00ScpiClient::CSx00ScpiClient(const QString &commName, uint32_t commBaud, const QString &idStr):
    ScpiClient(new QtCom(commName, commBaud, mcClientIdStr), idStr),
    mDeleteIo(true)
{
}

#ifdef _PSS_GPIB_SUPPORT_
CSx00ScpiClient::CSx00ScpiClient(uint8_t cardAddr, uint8_t instrumentAddr, const QString &idStr):
    ScpiClient(new GPIBIo(instrumentAddr, mcClientIdStr, cardAddr), idStr),
    mDeleteIo(true)
{
}
#endif

CSx00ScpiClient::~CSx00ScpiClient()
{
    if(mDeleteIo)
    {
        delete mIIo;
        mIIo = nullptr;
    }
}

int CSx00ScpiClient::SetChanID(uint8_t chanID, uint8_t childChanID)
{
    QString scpiStr = QString(":SYST%1:GRO \"%2\"").arg(chanID).arg(childChanID);

    return Send(scpiStr);
}

int CSx00ScpiClient::SendReadAll(QString& readRstStr, uint32_t timeout)
{
    QString scpiStr = ":READ:ARR? \"1,2,3\"";

    int ret = Send(scpiStr);
    if(PssRstSuccess != ret)
    {
        qDebug() << "send READ:ARR? failed: " << ret;
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

int CSx00ScpiClient::SendRead(uint8_t chanID, QString &readRstStr, uint32_t timeout)
{
    /* 发送READ并获取返回值 */
    QString scpiStr = QString(":READ%1?").arg(chanID);
    int ret = Send(scpiStr);
    if(RstSuccess != ret)
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
    return RstSuccess;
}

int CSx00ScpiClient::SendOutputCtrl(uint8_t chanID, bool on) const
{
    /* 设置输出状态 */
    QString str = on? "ON" : "OFF";
    QString scpiStr = QString(":OUTP%1 %2").arg(chanID).arg(str);

    return Send(scpiStr);
}

int CSx00ScpiClient::SendVSrc(uint8_t chanID, bool isVSrc) const
{
    /* 源选择 */
    QString srcName = ToScpiName(isVSrc);
    QString scpiStr = QString(":SOUR%1:FUNC %2").arg(chanID).arg(srcName);

    return Send(scpiStr);
}

int CSx00ScpiClient::SendSrcRange(uint8_t chanID, bool isVSrc, double val) const
{
    /* 设置源量程 */
    QString srcName = ToScpiName(isVSrc);
    QString scpiStr = QString(":SOUR%1:%2:RANG %3").arg(chanID).arg(srcName).arg(val);

    return Send(scpiStr);
}

int CSx00ScpiClient::SendSrcVal(uint8_t chanID, bool isVSrc, double val) const
{
    /* 设置源值 */
    QString srcName = ToScpiName(isVSrc);
    QString scpiStr = QString(":SOUR%1:%2:LEV %3").arg(chanID).arg(srcName).arg(val);

    return Send(scpiStr);
}

int CSx00ScpiClient::SendLmtRange(uint8_t chanID, bool isVSrc, double val) const
{
    /* 设置限量程 */
    QString lmtName = ToScpiName(!isVSrc);
    QString scpiStr = QString(":SENS%1:%2:RANG %3").arg(chanID).arg(lmtName).arg(val);

    return Send(scpiStr);
}

int CSx00ScpiClient::SendLmtVal(uint8_t chanID, bool isVSrc, double val) const
{
    /* 设置限值 */
    QString srcName = ToScpiName(isVSrc);
    QString lmtName = isVSrc? "ILIM" : "VLIM";
    QString scpiStr = QString(":SOUR%1:%2:%3 %4").arg(chanID).arg(srcName).arg(lmtName).arg(val);

    return Send(scpiStr);
}

int CSx00ScpiClient::Send4W(uint8_t chanID, bool is4W) const
{
    /* 设置24线 */
    QString str = is4W? "ON" : "OFF";
    QString scpiStr = QString(":SYST%1:RSEN %2").arg(chanID).arg(str);

    return Send(scpiStr);
}

int CSx00ScpiClient::SendScanStartVal(uint8_t chanID, bool isVSrc, double val) const
{
    /* 设置扫描起点 */
    QString name = ToScpiName(isVSrc);
    QString scpiStr = QString(":SOUR%1:%2:STAR %3").arg(chanID).arg(name).arg(val);

    return Send(scpiStr);
}

int CSx00ScpiClient::SendScanStopVal(uint8_t chanID, bool isVSrc, double val) const
{
    /* 设置扫描终点 */
    QString name = ToScpiName(isVSrc);
    QString scpiStr = QString(":SOUR%1:%2:STOP %3").arg(chanID).arg(name).arg(val);

    return Send(scpiStr);
}

int CSx00ScpiClient::SendScanPointNum(uint8_t chanID, uint32_t points) const
{
    if(mcMaxSweepPoints < points)
    {
        return RstOverFlowError;
    }
    /* 设置扫描点数 */
    QString scpiStr = QString(":SOUR%1:SWE:POIN %2").arg(chanID).arg(points);

    return Send(scpiStr);
}

int CSx00ScpiClient::SendScanType(uint8_t chanID, bool isVSrc, CSx00ScpiSweepType type) const
{
    /* 设置扫描类型 */
    QString name = ToScpiName(isVSrc);
    QString mode("SWE");
    switch(type)
    {
        case CSx00ScpiSweepTypeLiner:
        {
            mode = "SWE";
            break;
        }
        case CSx00ScpiSweepTypeLog:
        {
            mode = "LOG";
            break;
        }
        case CSx00ScpiSweepTypeCustom:
        {
            mode = "LIST";
            break;
        }
        case CSx00ScpiSweepTypeFix:
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

int CSx00ScpiClient::SendScanMaster(uint8_t chanID, bool isMaster) const
{
    /* 设置主从设备 */
    QString str = isMaster? "SOUR" : "ACC";
    QString scpiStr = QString(":TRIG%1:DIR %2").arg(chanID).arg(str);

    return Send(scpiStr);
}

int CSx00ScpiClient::SendTrigInput(bool on)
{
    /* 设置trig开关 */
    QString str = on? "ON" : "OFF";
    QString scpiStr = QString(":TRIG:INP %1").arg(str);

    return Send(scpiStr);
}

int CSx00ScpiClient::SendListVal(uint8_t chanID, bool isVSrc, const QVector<double> &data) const
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

int CSx00ScpiClient::SendAppendListVal(uint8_t chanID, bool isV, const QVector<double> &data)
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

int CSx00ScpiClient::SendLmtAbort(uint8_t chanID, bool on)
{
    QString str = on? "ON" : "OFF";
    QString scpiStr = QString(":SOUR%1:SWE:CAB %2").arg(chanID).arg(str);

    return Send(scpiStr);
}

int CSx00ScpiClient::SendNplc(uint8_t chanID, bool isVSens, double nplc) const
{
    /* 设置 NPLC */
    QString name = ToScpiName(isVSens);
    QString scpiStr = QString(":SENS%1:%2:NPLC %3").arg(chanID).arg(name).arg(nplc);

    return Send(scpiStr);
}

int CSx00ScpiClient::SendGetOutput(uint8_t chanID, bool &on, uint32_t timeout)
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

int CSx00ScpiClient::SendGetSrcType(uint8_t chanID, bool &isVSrc, uint32_t timeout)
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

int CSx00ScpiClient::SendSrcAutoRange(uint8_t chanID, bool isVSrc, bool on)
{
    QString srcName = ToScpiName(isVSrc);
    QString str = on? "ON" : "OFF";

    QString scpiStr = QString(":SOUR%1:%2:RANG:AUTO %3").arg(chanID).arg(srcName).arg(str);
    return Send(scpiStr);
}

int CSx00ScpiClient::SendLmtAutoRange(uint8_t chanID, bool isVSrc, bool on)
{
    QString lmtName = ToScpiName(!isVSrc);
    QString str = on? "ON" : "OFF";

    QString scpiStr = QString(":SENS%1:%2:RANG:AUTO %3").arg(chanID).arg(lmtName).arg(str);
    return Send(scpiStr);
}

int CSx00ScpiClient::SendSrcAutoRangeQuery(uint8_t chanID, bool isVSrc, bool *on, uint32_t timeout)
{
    if(!on)
    {
        return RstParameterError;
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
        return RstReadError;
    }
    
    return RstSuccess;
}

int CSx00ScpiClient::SendLmtAutoRangeQuery(uint8_t chanID, bool isVSrc, bool *on, uint32_t timeout)
{
    if(!on)
    {
        return RstParameterError;
    }

    QString lmtName = ToScpiName(!isVSrc);
    QString scpiStr = QString(":SENS%1:%2:RANG:AUTO?\n").arg(chanID).arg(lmtName);
    int ret = Send(scpiStr);
    if(RstSuccess != ret)
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

int CSx00ScpiClient::SendSrcRangeQuery(uint8_t chanID, bool isVSrc, char *range, uint32_t timeout)
{
    if(!range)
    {
        return RstParameterError;
    }
    QString srcName = ToScpiName(isVSrc);
    QString scpiStr = QString(":SOUR%1:%2:RANG?\n").arg(chanID).arg(srcName);
    int ret = Send(scpiStr);
    if(RstSuccess != ret)
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

int CSx00ScpiClient::SendLmtRangeQuery(uint8_t chanID, bool isVSrc, char *range, uint32_t timeout)
{
    if(!range)
    {
        return RstParameterError;
    }
    QString lmtName = ToScpiName(!isVSrc);
    QString scpiStr = QString(":SENS%1:%2:RANG?").arg(chanID).arg(lmtName);
    int ret = Send(scpiStr);
    if(RstSuccess != ret)
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
    
    return RstSuccess;
}

int CSx00ScpiClient::SendTraceSet(uint8_t chanID, bool on)
{
    QString str = on? "TRIG" : "CLE";
    QString scpiStr = QString(":TRAC%1:%2").arg(chanID).arg(str);

    return Send(scpiStr);
}

int CSx00ScpiClient::SendTrigDelay(uint8_t chanID, uint32_t delayUs)
{
    QString scpiStr = QString(":TRIG%1:DEL %2").arg(chanID).arg(delayUs);

    return Send(scpiStr);
}

int CSx00ScpiClient::SendOutputDelay(uint8_t chanID, uint32_t delayUs)
{
    QString scpiStr = QString(":SOUR%1:DEL %2").arg(chanID).arg(delayUs);

    return Send(scpiStr);
}

int CSx00ScpiClient::SendIPConfig(bool isAuto, const QString &ip, const QString &netMask, const QString &gateWay)
{
    QString type = isAuto? "AUTO" : "MAN";

    QString scpiStr = QString(":SYST:COMM:LAN:CONF \"%1,%2,%3,%4\"").arg(type).arg(ip).arg(netMask).arg(gateWay);
    return Send(scpiStr);
}

int CSx00ScpiClient::SendIPConfigQuery(bool &isAuto, QString &ip, QString &netMask, QString &gateWay, uint32_t timeout)
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
        return RstReadFromIOError;
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
        return RstReadFromIOError;
    }
    ip = recvList.value(1);
    netMask = recvList.value(2);
    gateWay = recvList.value(3);
    gateWay.remove("\n");
    return RstSuccess;
}

int CSx00ScpiClient::SendUpdateIPConfig()
{
    QString scpiStr = QString(":SYST:COMM:LAN:UPD");

    return Send(scpiStr);
}

int CSx00ScpiClient::SendGPIBConfig(uint8_t devAddr)
{
    QString scpiStr = QString(":SYST:COMM:GPIB:ADDR %1").arg(devAddr);

    return Send(scpiStr);
}

int CSx00ScpiClient::SendGPIBConfigQuery(uint8_t &devAddr, uint32_t timeout)
{
    QString scpiStr = QString(":SYST:COMM:GPIB:ADDR?");
    int ret = Send(scpiStr);
    if(RstSuccess != ret)
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
    devAddr = devStr.toInt();
    return RstSuccess;
}

int CSx00ScpiClient::SendUARTConfig(uint32_t baud)
{
    QString scpiStr = QString(":SYST:COMM:UART:BAUD %1").arg(baud);

    return Send(scpiStr);
}

int CSx00ScpiClient::SendUARTConfigQuery(uint32_t &baud, uint32_t timeout)
{
    QString scpiStr = QString(":SYST:COMM:UART:BAUD?");
    int ret = Send(scpiStr);
    if(RstSuccess != ret)
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
    baud = baudStr.toUInt();
    return RstSuccess;
}

int CSx00ScpiClient::SendStartOutputEvent(uint8_t chanID, int inputLine, int outputLine, CSx00ScpiTrigEdgeType trigType)
{
    QString type = "RIS";
    switch(trigType)
    {
        case PssCSx00TrigEdgeTypeFall:
        {
            type = "FALL";
            break;
        }
        case PssCSx00TrigEdgeTypeRise:
        {
            type = "RIS";
            break;
        }
        case PssCSx00TrigEdgeTypeEither:
        {
            type = "EITH";
            break;
        }
        default:
        {
            return RstParameterError;
        }
    }

    QString scpiStr = QString(":TRIG%1:LOAD:EVEN:STOUT \"%2,%3,%4\"").arg(chanID).arg(inputLine).arg(outputLine).arg(type);
    
    return Send(scpiStr);
}

int CSx00ScpiClient::SendFinishOutputEvent(uint8_t chanID, int inputLine, int outputLine, CSx00ScpiTrigEdgeType trigType)
{
    QString type = "RIS";
    switch(trigType)
    {
        case PssCSx00TrigEdgeTypeFall:
        {
            type = "FALL";
            break;
        }
        case PssCSx00TrigEdgeTypeRise:
        {
            type = "RIS";
            break;
        }
        case PssCSx00TrigEdgeTypeEither:
        {
            type = "EITH";
            break;
        }
        default:
        {
            return RstParameterError;
        }
    }

    QString scpiStr = QString(":TRIG%1:LOAD:EVEN:FIOUT \"%2,%3,%4\"").arg(chanID).arg(inputLine).arg(outputLine).arg(type);
    
    return Send(scpiStr);
}

int CSx00ScpiClient::SendStartSamplingEvent(uint8_t chanID, int inputLine, int outputLine, CSx00ScpiTrigEdgeType trigType)
{
    QString type = "RIS";
    switch(trigType)
    {
        case PssCSx00TrigEdgeTypeFall:
        {
            type = "FALL";
            break;
        }
        case PssCSx00TrigEdgeTypeRise:
        {
            type = "RIS";
            break;
        }
        case PssCSx00TrigEdgeTypeEither:
        {
            type = "EITH";
            break;
        }
        default:
        {
            return PssRstParameterError;
        }
    }

    QString scpiStr = QString(":TRIG%1:LOAD:EVEN:STSAM \"%2,%3,%4\"").arg(chanID).arg(inputLine).arg(outputLine).arg(type);
    
    return Send(scpiStr);
}

int CSx00ScpiClient::SendFinishSamplingEvent(uint8_t chanID, int inputLine, int outputLine, CSx00ScpiTrigEdgeType trigType)
{
    QString type = "RIS";
    switch(trigType)
    {
        case PssCSx00TrigEdgeTypeFall:
        {
            type = "FALL";
            break;
        }
        case PssCSx00TrigEdgeTypeRise:
        {
            type = "RIS";
            break;
        }
        case PssCSx00TrigEdgeTypeEither:
        {
            type = "EITH";
            break;
        }
        default:
        {
            return PssRstParameterError;
        }
    }

    QString scpiStr = QString(":TRIG%1:LOAD:EVEN:FISAM \"%2,%3,%4\"").arg(chanID).arg(inputLine).arg(outputLine).arg(type);
    
    return Send(scpiStr);
}

int CSx00ScpiClient::SendStartSweEvent(uint8_t chanID, int inputLine, int outputLine, CSx00ScpiTrigEdgeType trigType)
{
    QString type = "RIS";
    switch(trigType)
    {
        case PssCSx00TrigEdgeTypeFall:
        {
            type = "FALL";
            break;
        }
        case PssCSx00TrigEdgeTypeRise:
        {
            type = "RIS";
            break;
        }
        case PssCSx00TrigEdgeTypeEither:
        {
            type = "EITH";
            break;
        }
        default:
        {
            return PssRstParameterError;
        }
    }

    QString scpiStr = QString(":TRIG%1:LOAD:EVEN:STSWE \"%2,%3,%4\"").arg(chanID).arg(inputLine).arg(outputLine).arg(type);
    
    return Send(scpiStr);
}

int CSx00ScpiClient::SendClearEvent(uint8_t chanID)
{
    QString scpiStr = QString(":TRIG%1:LOAD:EVEN:CLE").arg(chanID);

    return Send(scpiStr);
}

int CSx00ScpiClient::SendTrigCount(uint8_t chanID, int count)
{
    QString scpiStr = QString(":TRIG%1:COUN %2").arg(chanID).arg(count);

    return Send(scpiStr);
}

int CSx00ScpiClient::SendMeasureQuery(uint8_t chanID, bool isVSrc, double &val, uint32_t timeout)
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
    return RstSuccess;
}


int CSx00ScpiClient::SendFireCalParameter(uint8_t chanID)
{
    QString scpi = QString(":PSS:ANLG%1:CAL:FIRE\n").arg(chanID);
    int ret = Send(scpi);
    if(RstSuccess != ret)
    {
        return ret;
    }
    QString str;
    ret = Recv(str);
    if(ret <= 0)
    {
        return RstRecvFromIOError;
    }
    return RstSuccess;
}

int CSx00ScpiClient::SendGetAdcValue(uint8_t chanID, bool isV, double &adc, uint32_t timeout)
{
    QString type("VOLT");
    if(!isV)
    {
        type = QString("CURR");
    }
    QString scpi = QString(":PSS:ANLG%1:%2:ADC?\n").arg(chanID).arg(type);
    mIIo->ClearIn();
    int ret = Send(scpi, timeout);
    if(RstSuccess != ret)
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
    return RstSuccess;
}

int CSx00ScpiClient::SendGetDacVaule(uint8_t chanID, bool isV, double &dac, uint32_t timeout)
{
    QString type("VOLT");
    if(!isV)
    {
        type = QString("CURR");
    }
    QString scpi = QString(":PSS:ANLG%1:%2:DAC?\n").arg(chanID).arg(type);

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
        return RstReadError;
    }
    
    dac = res.toDouble();
    return RstSuccess;
}

int CSx00ScpiClient::SendSetCalParameter(uint8_t chanID, int index, double dac0, double dac1, double adc0, double adc1)
{
    QString scpi(QString(":PSS:ANLG%1:CAL:PARA \"%2,%3,%4,%5,%6\"\n")
                 .arg(chanID)
                 .arg(index)
                 .arg(dac0)
                 .arg(dac1)
                 .arg(adc0)
                 .arg(adc1));

    return Send(scpi);
}

int CSx00ScpiClient::SendVersionReq(QString &idnRstStr, uint32_t timeout)
{
    QString scpiStr = QString(":PSS:VERS?\n");
    int ret = Send(scpiStr);
    if(RstSuccess != ret)
    {
        return ret;
    }

    ret = Recv(idnRstStr, timeout);
    if(ret <= 0)/* Recv不会返回0 */
    {
        qCritical() << "recv idn failed: " << ret;
        return ret;
    }
    
    return RstSuccess;
}

int CSx00ScpiClient::SendVersionQuery(uint8_t chanID, QString &verStr, uint32_t timeout)
{
    QString scpiStr = QString(":SYST%1:VERS?\n").arg(chanID);
    int ret = Send(scpiStr);
    if(RstSuccess != ret)
    {
        return ret;
    }

    ret = Recv(verStr, timeout);
    if(ret <= 0)/* Recv不会返回0 */
    {
        qCritical() << "recv idn failed: " << ret;
        return ret;
    }
    
    return RstSuccess;
}
