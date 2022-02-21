#include "StandardInstrument.h"
#include "ITool.h"
#include "ScpiClient.h"
#include "PssRstDef.h"
#include <QThread>
#include <QTime>


StandardInstrument::StandardInstrument(const QHostAddress &ip, const QString& instName, bool hasSrc, QObject *parent) :
    QtIInstrument(InstrIOTypeNet, instName, hasSrc, parent),
    mScpiClient(new StandardScpiClient(ip, instName)),
    mIsSourSameSens(false)
{
}

StandardInstrument::StandardInstrument(const QString comName, uint32_t comBaud, const QString& instName,bool hasSrc, QObject *parent):
    QtIInstrument(InstrIOTypeComm, instName, hasSrc, parent),
    mScpiClient(new StandardScpiClient(comName, comBaud, instName)),
    mIsSourSameSens(false)
{
    InitSourceInfo();
}
#ifdef _GPIB_SUPPORT_
StandardInstrument::StandardInstrument(uint8_t cardAddr, uint8_t instrumentAddr, const QString& instName,bool hasSrc, QObject *parent):
    QtIInstrument(InstrIOTypeGPIB, instName, hasSrc, parent),
    mScpiClient(new Sx00ScpiClient(cardAddr, instrumentAddr, mcName)),
    mIsSourSameSens(false)
{
    InitSourceInfo();
}
#endif

StandardInstrument::~StandardInstrument(void)
{
    delete mScpiClient;
    mScpiClient = nullptr;
}

int StandardInstrument::GetMeasureVal(PssInstrumentMeasureType type, double &val, uint32_t timeout)
{
    PSS_UNUSED(type);
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
    
    if(recvValStr.isEmpty())
    {
        return PssRstReadFromIOError;
    }
    val = recvValStr.toDouble();
    
    return PssRstSuccess;
}

int StandardInstrument::SetOutput(bool on, bool isRead)
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
        qCritical() << QString("scpi client is null !");
        return PssRstScpiClientNotFound;
    }
    
    int ret = mScpiClient->SendOutputCtrl(on);
    if(ret < 0)
    {
        return PssRstOutputFail;
    }
    
    /* ON需要发送"Read?"指令 用于优化性能 */
    if(on && isRead)
    {
        QString readStr;
        ret = mScpiClient->SendRead(readStr);
        if(ret < 0)
        {
            return PssRstReadFromIOError;
        }
    }
    return PssRstSuccess;
}

void StandardInstrument::SetSourSameSens(const bool &isSrc)
{
    mIsSourSameSens = isSrc;
}

ScpiClient *StandardInstrument::GetScpiClient() const
{
    return mScpiClient;
}

void StandardInstrument::InitSourceInfo()
{

}

int StandardInstrument::UpdateCfgToInstrument()
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
    
    if(mIsSourSameSens)
    {
        ret = mScpiClient->SendSensFunc(!isVSrc);
    }
    else
    {
        ret = mScpiClient->SendSensFunc(isVSrc);
    }
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
    return PssRstSuccess;
}
