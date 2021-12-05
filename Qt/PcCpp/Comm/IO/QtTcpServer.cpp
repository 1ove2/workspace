/******************************************************************************
 *
 * 文件名  ： QtTcp.cpp
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20200103
 * 版本号  ： v1.0 20200103
 * 文件描述：  Qt版本TCP类,实现IIo接口
 * 其    他： 无
 * 修改日志： 无
 *
  *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "QtTcpServer.h"
#include "ITool.h"
#include "RstDef.h"
#include <QCoreApplication>


/*----------------------------------- 声明区 ----------------------------------*/

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */

/********************************** 变量声明区 *********************************/


/********************************** 函数声明区 *********************************/


/********************************** 变量实现区 *********************************/


/********************************** 函数实现区 *********************************/
QtTcpServer::QtTcpServer(uint16_t listenPort, const QString& tcpIdStr, QObject *parent) :
    QtIo(tcpIdStr, parent),
    mListenPort(listenPort),
    mTcpServer(nullptr),
    mSocket4Client(nullptr)
{
    Open();
}

QtTcpServer::~QtTcpServer(void)
{
    Close();
}

bool QtTcpServer::IsOpen(void) const
{
    if(nullptr == mSocket4Client)
    {
        return false;
    }

    return true;
}

int QtTcpServer::Open(void)
{
    mTcpServer = new QTcpServer(this);
    bool success = mTcpServer->listen(QHostAddress::Any, mListenPort);
    if(!success)
    {
        qDebug() << QString("Tcp Listenning: Any Ip with %1 port for %2 Failled: %3")
                    .arg(mListenPort).arg(GetIdStr()).arg(mTcpServer->errorString());
        return RstDevCreateError;
    }

    connect(mTcpServer, &QTcpServer::newConnection, this, &QtTcpServer::slotAcceptClientSocket);
    qDebug() << QString("Tcp Listenning: Any Ip with %1 port for %2 Success")
                .arg(mListenPort).arg(GetIdStr());
    
    return RstSuccess;
}

int QtTcpServer::Close(void)
{
    ClearSocket4Client();
    if(mTcpServer)
    {
        delete mTcpServer;
        mTcpServer = nullptr;
    }

    qDebug() << QString("close QtTcpServer %1 of %2").arg(mListenPort).arg(GetIdStr());
    
    return RstSuccess;
}

QIODevice *QtTcpServer::GetQIODevice(void) const
{
    return mSocket4Client;
}

void QtTcpServer::slotAcceptClientSocket(void)
{

    QTcpSocket *socket4Client = mTcpServer->nextPendingConnection();
    /* 踢掉之前的连接 使当前连接生效 */
    ClearSocket4Client();
    
    mSocket4Client = socket4Client;
    
    /* 使用信号槽 支撑 阻塞操作 */
    connect(socket4Client, &QTcpSocket::bytesWritten, this, &QtTcpServer::BytesWritten);
    connect(socket4Client, &QTcpSocket::readyRead,    this, &QtTcpServer::ReadyRead);
    qDebug() << QString("new connection request arrived: %1:%2,%3")
                .arg(socket4Client->peerName())
                .arg(socket4Client->peerAddress().toString())
                .arg(socket4Client->peerPort());
    qDebug() << "mSocket4Client = " << mSocket4Client;

}

void QtTcpServer::ClearSocket4Client(void)
{
    if(nullptr != mSocket4Client)
    {
        qDebug() << QString("Clear Old Socket For Client: %1:%2,%3")
                    .arg(mSocket4Client->peerName())
                    .arg(mSocket4Client->peerAddress().toString())
                    .arg(mSocket4Client->peerPort());

        /* 使用信号槽 支撑 阻塞操作 */
        disconnect(mSocket4Client, &QTcpSocket::bytesWritten, this, &QtTcpServer::BytesWritten);
        disconnect(mSocket4Client, &QTcpSocket::readyRead,    this, &QtTcpServer::ReadyRead);
    
        mSocket4Client->close();
        delete mSocket4Client;
        mSocket4Client = nullptr;
    }
}
