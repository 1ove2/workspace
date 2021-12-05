#include "QtIInstrument.h"
#include "ScpiClient.h"
#include "PssRstDef.h"

#include <QThread>

using namespace Pss::Qt;

QtIInstrument::QtIInstrument(QtIInstrument::InstrIOType ioType, const QString &name, bool hasSrc, QObject *parent):
    QObject(parent),
    mName(name),
    mConnected(false),
    mHasSrc(hasSrc),
    mIOType(ioType)
{
    InitSourceInfo();
}

QtIInstrument::~QtIInstrument(void)
{
}

int QtIInstrument::GetScpiTcpServerPort(void)
{
    return mcScpiTcpServerPort;
}

bool QtIInstrument::Connect(uint32_t timeout)
{
    mConnected = false;
    ScpiClient* scpiClient = GetScpiClient();
    if(!scpiClient)
    {
        qCritical() << "scpi client is null !";
        return false;
    }
    scpiClient->ClearIoRecv();
    QString idnStr;
    int ret = scpiClient->SendIDNReq(idnStr, timeout);
    if(PssRstSuccess != ret)
    {
        return false;
    }

    QStringList strList = idnStr.split(",");
    if(strList.size() > 1)
    {
        QString name = strList[1];
        mName = name;
    }
    mIdnStr = idnStr;
    mConnected = true;
    return true;
}

int QtIInstrument::Reset(void)
{
    ScpiClient* scpiClient = GetScpiClient();
    if(!scpiClient)
    {
        qCritical() << "scpi client is null !";
        return PssRstParameterError;
    }
    scpiClient->ClearIoRecv();

    return scpiClient->SendReset();
}

QString QtIInstrument::GetIdn(void) const
{
    /* 移除首尾空白 */
    return mIdnStr.simplified();
}

QString QtIInstrument::GetName(void) const
{
    return mName;
}


bool QtIInstrument::IsConnected(void) const
{
    return mConnected;
}

bool QtIInstrument::HasSrc(void) const
{
    return mHasSrc;
}

int QtIInstrument::SetSrcInfo(const PssInstrumentCfgInfo *info)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }

    memmove(&mCfgInfo, info, sizeof(PssInstrumentCfgInfo));
    int ret = UpdateCfgToInstrument();
    return ret;
}

int QtIInstrument::GetSrcInfo(PssInstrumentCfgInfo *info) const
{
    memmove(info, &mCfgInfo, sizeof(PssInstrumentCfgInfo));
    return PssRstSuccess;
}


int QtIInstrument::ClearScpiError(void)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    ScpiClient* scpiClient = GetScpiClient();
    if(!scpiClient)
    {
        qCritical() << "scpi client is null !";
        return PssRstParameterError;
    }
    return scpiClient->SendClearScpiError();
}

int QtIInstrument::GetScpiError(int &errCode, uint32_t timeout)
{
    if(!IsConnected())
    {
        return PssRstDeviceNotConnect;
    }
    ScpiClient* scpiClient = GetScpiClient();
    if(!scpiClient)
    {
        qCritical() << "scpi client is null !";
        return PssRstParameterError;
    }

    return scpiClient->SendGetScpiError(errCode, timeout);
}

bool QtIInstrument::IsVSrc(void) const
{
    return mCfgInfo.isVSrc;
}

double QtIInstrument::GetSrcVal(void) const
{
    return mCfgInfo.srcVal;
}

double QtIInstrument::GetLmtVal(void) const
{
    return mCfgInfo.lmtVal;
}

double QtIInstrument::GetNplc(void) const
{
    return mCfgInfo.nplc;
}

void QtIInstrument::InitSourceInfo(void)
{
    /* 电压源,30V输出,0.1uA限值,0.1 NPLC */
    mCfgInfo.isVSrc = true;
    mCfgInfo.srcVal = 0.03;
    mCfgInfo.lmtVal = 0.0000001;
    mCfgInfo.nplc = 0.1;
}