#include "ScpiClient.h"
#include "Qt34470A.h"
#include "ITool.h"
#include "PssRstDef.h"

using namespace Pss::Qt;


const char *Qt34470A::mcName = "34470A";

Qt34470A::Qt34470A(const QHostAddress &ip, QObject *parent) :
    Sx00Instrument(ip, QString(mcName), false, parent)
{
}

Qt34470A::~Qt34470A(void)
{
}

QString Qt34470A::GetIdName(void)
{
    return mcName;
}

int Qt34470A::UpdateCfgToInstrument(void)
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
    
    /* 电压源测试电流,电流源测试电压 */
    bool isMeasV = !IsVSrc();
    double lmtVal = GetLmtVal();
    int ret = mScpiClient->SendConfigure(isMeasV, lmtVal);
    if(PssRstSuccess != ret)
    {
        return PssRstSetMeasureSrcError;
    }
    
    return PssRstSuccess;
}