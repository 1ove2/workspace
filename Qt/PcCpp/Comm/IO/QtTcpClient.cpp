/******************************************************************************
 *
 * 文件名  ： QtTcpClient.cpp
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20200119
 * 版本号  ： v1.0 20200119
 * 文件描述：  Qt版本TCP 客户类,实现IIo接口
 * 其    他： 无
 * 修改日志： 无
 *
  *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "ITool.h"
#include "QtTcpClient.h"
#include "RstDef.h"
#include <QTime>
#include <QDebug>
#include <QTcpSocket>
#include <QHostAddress>


/*----------------------------------- 声明区 ----------------------------------*/

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */

/********************************** 变量声明区 *********************************/


/********************************** 函数声明区 *********************************/


/********************************** 变量实现区 *********************************/


/********************************** 函数实现区 *********************************/
QtTcpClient::QtTcpClient(const QHostAddress &serverIp, uint16_t serverPort, const QString& tcpIdStr, QObject *parent) :
    QtIo(tcpIdStr, parent),

    mServerIp(serverIp),
    mServerPort(serverPort),
    
    mSocket(new QTcpSocket()),
    mIsConnectted(false)
{
    Open();
}

QtTcpClient::~QtTcpClient(void)
{
    Close();
    delete mSocket;
    mSocket = nullptr;
}

bool QtTcpClient::IsOpen(void) const
{
    return mIsConnectted;
}

int QtTcpClient::Open(void)
{
    if(mIsConnectted)
    {
        return RstSuccess;
    }

    mSocket->connectToHost(mServerIp, mServerPort);
    mIsConnectted = mSocket->waitForConnected(mcConnectWaitMs);
    
    QString msg = QString("%1 Connect To %2:%3")
            .arg(GetIdStr())
            .arg(mServerIp.toString())
            .arg(mServerPort);
    if(!mIsConnectted)
    {
        msg.append(QString(" error %1.").arg(mSocket->errorString()));
        qCritical() << msg;
        return RstSocketConnectError;
    }
    
    /* 使用信号槽 支撑 阻塞操作 */
    connect(mSocket, &QTcpSocket::bytesWritten, this, &QtTcpClient::BytesWritten);
    connect(mSocket, &QTcpSocket::readyRead,    this, &QtTcpClient::ReadyRead);
    
    msg.append(" success.");
    qDebug().noquote() << msg;
    qDebug() << "mSocket = " << mSocket;
    return RstSuccess;
}

int QtTcpClient::Close(void)
{
    if(IsOpen())
    {
        mSocket->close();
    }

    disconnect(mSocket, &QTcpSocket::bytesWritten, this, &QtTcpClient::BytesWritten);
    disconnect(mSocket, &QTcpSocket::readyRead,    this, &QtTcpClient::ReadyRead);
    
    QString msg = QString("%1 Disconnect From %2:%3 success.")
            .arg(GetIdStr())
            .arg(mServerIp.toString())
            .arg(mServerPort);
    qDebug().noquote() << msg;
    return RstSuccess;
}

QIODevice *QtTcpClient::GetQIODevice(void) const
{
    return mSocket;
}
