#include "ScpiClient.h"
#include "ITool.h"
#include "PssQtHal.h"
#include <QThread>
#include <QTime>
#include <QDebug>
#include <QCoreApplication>


ScpiClient::ScpiClient(IIo *iio, const QString &idStr) :
    QObject(0),
    mIIo(iio),
    mIdStr(idStr)
{
}

ScpiClient::~ScpiClient(void)
{
}

int ScpiClient::SendReset(void) const
{
    QString scpiStr = QString("*RST\n");
    return Send(scpiStr);
}

int ScpiClient::SendTrigDir(int dir) const
{
    QString scpiStr = QString(":TRIG:ILIN %1").arg(dir);

    return Send(scpiStr);
}

int ScpiClient::SendTrigInput(bool on)
{
    QString input("OFF");
    if(on)
    {
        input = QString("ON");
    }
    QString scpiStr = QString(":TRIG:INP %1\n").arg(input);

    return Send(scpiStr);
}


int ScpiClient::SendClearScpiError(void)
{
    QString scpiStr(":SYST:CLE\n");
    return Send(scpiStr);
}

int ScpiClient::SendGetScpiError(int &errCode, uint32_t timeout)
{
    QString scpiStr(":SYST:ERR:CODE?\n");
    int ret = Send(scpiStr);
    if(Success != ret)
    {
        qCritical() << "send get scpi error code failed: " << ret;
        return ret;
    }
    QString errStr;

    ret = Recv(errStr, timeout);
    if(ret <= 0)/* Recv不会返回0 */
    {
        qCritical() << "recv get scpi error code failed: " << ret;
        return ret;
    }
    errCode = errStr.toInt();
    return Success;
}

void ScpiClient::ClearIoRecv(void)
{
    if(mIIo)
    {
        mIIo->ClearIn();
    }
}

QString ScpiClient::GetReadValStr(uint32_t timeout, bool check)
{
    QString valStr;

    int ret = Recv(valStr, timeout, check);
    if(ret <= 0)/* Recv不会返回0 */
    {
        qCritical() << "Recv Read result failed: " << ret;
        return QString("");
    }
    
    return valStr;
}

int ScpiClient::SendRawScpi(const QString &scpi, uint32_t timeout)
{
    return Send(scpi, timeout);
}

int ScpiClient::SendIDNReq(QString &idnRstStr, uint32_t timeout)
{
    QString scpiStr = QString("*IDN?\n");

    int ret = Send(scpiStr, timeout);
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


int ScpiClient::Send(const QString &str, uint32_t delayMs) const
{
    const int bufSize = 1024;
    char buf[bufSize] = {0};

    /* 避免误换行帧尾 */
    QString cmd = str;
    if(!str.endsWith('\n'))
    {
        cmd += '\n';
    }
    size_t size = QString2Utf8(buf, bufSize, cmd);
    
    size_t len = 0;
    int ret = 0;
    do{
        ret = mIIo->Send(buf + len, size - len, delayMs);
        if(ret < 0)
        {
            return ret;
        }
        len += ret;
    }while(len < size);
    //qDebug() << "scpi client send: " << str;
    mIIo->FlushOut();
    return RstSuccess;
}

/*
 *   此接口为接收SCPI数据返回接口，每次接收应该保证数据全部接收（以\n结束），
 *   否则接口应该返回接收失败错误，因为返回不完整的数据没有意义.
 *   check参数是为了兼容之前的功能，让Recv接口作为接收数据接口
 */
int ScpiClient::Recv(QString &str, uint32_t delayMs, bool check) const
{
    const int bufSize = 1024;
    char buf[bufSize] = {0};
    QTime startTime = QTime::currentTime();
    int recvLen = 0;
    startTime.start();
    do
    {
        int recvBytesNums = mIIo->Recv(buf + recvLen, bufSize - recvLen, delayMs);
        if(recvBytesNums < 0)
        {
            qCritical() << "scpi client recv failed: " << recvBytesNums;
            str.clear();
            return recvBytesNums;
        }
        recvLen += recvBytesNums;
        if((recvLen) && ('\n' == buf[recvLen-1]))
        {
            break;
        }
        if(startTime.elapsed() > delayMs)
        {
            if(check)
            {
                return RstOptTimeoutError;
            }
            if(recvLen > 0)
            {
                break;
            }
            qCritical() << "scpi recv timeout for " << delayMs << " ms !";
            return RstOptTimeoutError;
        }
        if((!check) && (recvLen > 0))
        {
            break;
        }
    }while(1);

    Utf82QString(str, buf, recvLen);
    //qDebug() << "ScpiClient Recv:" + str;
    return recvLen;
}

const QString ScpiClient::ToScpiName(bool isV) const
{
    QString srcName;
    if(isV)
    {
        srcName = "VOLT";
    }
    else
    {
        srcName = "CURR";
    }

    return srcName;

}
