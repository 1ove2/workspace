#include <QThread>

#include "StandardScpiClient.h"
#include "QtTcpClient.h"
#include "QtCom.h"
#ifdef _PSS_GPIB_SUPPORT_
#include "GPIBIo.h"
#endif
using namespace Pss::Qt;

const QString StandardScpiClient::mcClientIdStr = QString("StandardScpiClient");

StandardScpiClient::StandardScpiClient(IIo *iio, const QString &idStr):
    ScpiClient(iio, idStr),
    mDeleteIo(false)
{
}

StandardScpiClient::StandardScpiClient(const QHostAddress &ip, const QString &idStr):
    ScpiClient(new QtTcpClient(ip, 5025, mcClientIdStr), idStr),
    mDeleteIo(true)
{
}

StandardScpiClient::StandardScpiClient(const QString &commName, uint32_t commBaud, const QString &idStr):
    ScpiClient(new QtCom(commName, commBaud, mcClientIdStr), idStr),
    mDeleteIo(true)
{
}

#ifdef _PSS_GPIB_SUPPORT_
StandardScpiClient::StandardScpiClient(uint8_t cardAddr, uint8_t instrumentAddr, const QString &idStr):
    ScpiClient(new GPIBIo(instrumentAddr, mcClientIdStr, cardAddr), idStr),
    mDeleteIo(true)
{
}
#endif
StandardScpiClient::~StandardScpiClient(void)
{
    if(mDeleteIo)
    {
        delete mIIo;
        mIIo = nullptr;
    }
}

int StandardScpiClient::SendRead(QString &readRstStr, uint32_t timeout)
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

int StandardScpiClient::SendOutputCtrl(bool on) const
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

int StandardScpiClient::SendVSrc(bool isVSrc) const
{
    QString srcName = ToScpiSrcName(isVSrc);

    /* 源选择 */
    QString scpiStr = QString(":SOUR:FUNC %1").arg(srcName);
    return Send(scpiStr);
}

int StandardScpiClient::SendSrcRange(bool isVSrc, double val) const
{
    /* 设置源量程 */
    QString srcName = ToScpiSrcName(isVSrc);
    QString scpiStr = QString(":SOUR:%1:RANG %2").arg(srcName).arg(val);

    return Send(scpiStr);
}

int StandardScpiClient::SendSrcVal(bool isVSrc, double val) const
{
    /* 设置源值 */
    QString srcName = ToScpiSrcName(isVSrc);
    QString scpiStr = QString(":SOUR:%1:LEV %2").arg(srcName).arg(val);

    return Send(scpiStr);
}

int StandardScpiClient::SendLmtRange(bool isVSrc, double val) const
{
    /* 设置限量程 */
    QString lmtName = ToScpiLmtName(isVSrc);
    QString scpiStr = QString(":SENS:%1:RANG %2").arg(lmtName).arg(val);

    return Send(scpiStr);
}

int StandardScpiClient::SendLmtVal(bool isVSrc, double val) const
{
    /* 2450与2400不同 */
    /* 设置限值 */
    QString srcName = ToScpiSrcName(isVSrc);
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

int StandardScpiClient::SendNplc(bool isVSens, double nplc) const
{
    /* 设置 NPLC */
    QString name = ToScpiName(isVSens);
    QString scpiStr = QString(":SENS:%1:NPLC %2").arg(name).arg(nplc);

    return Send(scpiStr);
}

int StandardScpiClient::SendSensFunc(bool isVSrc) const
{
    QString lmtName = ToScpiLmtName(isVSrc);
    /* 源选择 */
    QString scpiStr = QString(":SENS:FUNC \"%1\"").arg(lmtName);
    return Send(scpiStr);
}

const QString StandardScpiClient::ToScpiSrcName(bool isVSrc) const
{
    QString srcName;
    if(isVSrc)
    {
        srcName = "VOLT";
    }
    else
    {
        srcName = "CURR";
    }

    return srcName;
}

const QString StandardScpiClient::ToScpiLmtName(bool isVSrc) const
{
    QString lmtName;
    if(isVSrc)
    {
        lmtName = "CURR";
    }
    else
    {
        lmtName = "VOLT";
    }

    return lmtName;

}