#include "ITool.h"
#include "PssRstDef.h"
#include "Qt2450.h"
#include "ScpiClient.h"
#include "QtTcpClient.h"

#include <QThread>

using namespace Pss::Qt;


const char *Qt2450::mcName = "MODEL 2450";

Qt2450::Qt2450(const QHostAddress &ip, QObject *parent) :
    Sx00Instrument(ip, QString(mcName), true, parent),
    mIsMeasI(false)
{
}

Qt2450::~Qt2450(void)
{
}

QString Qt2450::GetIdName(void)
{
    return QString(mcName);
}

int Qt2450::GetMeasureVal(PssInstrumentMeasureType type, double &val, uint32_t timeout)
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

    val = recvValStr.toDouble();
    
    return PssRstSuccess;
}

int Qt2450::UpdateCfgToInstrument(void)
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
    
    /*
     * 待测为电压,设置为电流源,输出0;
     * 待测为电流,设置为电压源,输出0;
     *
     * */
    bool isVSrc = IsVSrc();
    bool isMeasV = !isVSrc;
    
    int ret = mScpiClient->SendVSrc(isVSrc);
    if(PssRstSuccess != ret)
    {
        return PssRstSetSrcError;
    }
    
    ret = mScpiClient->SendSensFunc(isVSrc);
    if(PssRstSuccess != ret)
    {
        return PssRstSetLmtValError;
    }
    
    ret = mScpiClient->SendSrcRange(isVSrc, GetSrcVal());
    if(PssRstSuccess != ret)
    {
        return PssRstSetSrcRangeError;
    }
    
    ret = mScpiClient->SendSrcVal(isVSrc,   GetSrcVal());
    if(PssRstSuccess != ret)
    {
        return PssRstSetSrcValError;
    }
    
    ret = mScpiClient->SendLmtRange(isVSrc, GetLmtVal());
    if(PssRstSuccess != ret)
    {
        return PssRstSetLmtRangeError;
    }
    
    ret = mScpiClient->SendLmtVal(isVSrc,   GetLmtVal());
    if(PssRstSuccess != ret)
    {
        return PssRstSetLmtValError;
    }
    
    /* 调大NPLC 增加测试精度 */
    ret = mScpiClient->SendNplc(isMeasV, GetNplc());
    if(PssRstSuccess != ret)
    {
        return PssRstSetLmtValError;
    }
    
    /* 标记测电流需要反向 */
    mIsMeasI = isVSrc;
    
    return PssRstSuccess;
}