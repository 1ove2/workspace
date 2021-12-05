/******************************************************************************
 *
 * 文件名  ： IOClient.cpp
 * 负责人  ： ryl
 * 创建日期： 20200720
 * 版本号  ： v1.1 20200720
 * 文件描述： SX00 IOClient线程
 * 其    他： 无
 * 修改日志：
 *
  *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/


/************************************ 头文件 ***********************************/
#include "QtIOClient.h"
#include "RstDef.h"




QtIOClient::QtIOClient(const QHostAddress &serverIp, uint16_t serverPort, const QString& clientName, int localPort, QObject *parent):
    QThread(parent),
    mTcpClient(nullptr),
    mClientName(clientName),
    mIsConnected(false),
    mServerIP(serverIp),
    mServerPort(serverPort),
    mClientPort(localPort),
    mLoopParseStoped(true),
    mLoopEventTimer(nullptr),
    mClient(nullptr)
{
    mRecvBuf.clear();
    mSendBuf.clear();
}

QtIOClient::~QtIOClient(void)
{
    slotStopThread();
}

bool QtIOClient::IsConnected(void)
{
    return mIsConnected;
}

int QtIOClient::Send2Server(const QByteArray &data)
{
    //qDebug() << "To be send data: " << data;
    if(QTcpSocket::ConnectedState != mClient.state())
    {
        return RstIOEmptyError;
    }
    int len = mClient.write(data);
    //qDebug() << "send data : " << data << ", len = " << len;
    return len;
}

bool QtIOClient::ConnectToServer(void)
{
    if(QTcpSocket::ConnectedState == mClient.state())
    {
        qDebug() << "Has Connected to Server: " << mServerIP << " : " << mServerPort;
        return false;
    }
    connect(&mClient, &QTcpSocket::connected, this, &QtIOClient::slotClientConnected);
    connect(&mClient, &QTcpSocket::disconnected, this, &QtIOClient::slotClientDisconnected);
    connect(&mClient, &QTcpSocket::readyRead, this, &QtIOClient::slotClientDataReady);
    connect(&mClient, &QTcpSocket::bytesWritten, this, &QtIOClient::slotClientBytesWritten);
    connect(&mClient, SIGNAL(error(QAbstractSocket::SocketError )), this, SLOT(slotClientErr(QAbstractSocket::SocketError)));

    mRecvBuf.clear();
    mSendBuf.clear();
    mClient.connectToHost(mServerIP, mServerPort);
    if(mClient.waitForConnected(mcConnectTimeoutMs))
    {
        mIsConnected = true;
    }
    else
    {
        qDebug() << "Connected to Server Timeout: " << mServerIP << " : " << mServerPort;
        mIsConnected = false;
    }
    return mIsConnected;

}
#if 1

void QtIOClient::slotClientConnected()
{

}

void QtIOClient::slotClientDisconnected()
{

}
void QtIOClient::slotClientDataReady()
{
#if 0
    while(mClient.bytesAvailable())
    {
        mRecvBuf += mClient.readLine();
    }
#else
    mRecvBuf = mClient.readAll();
#endif
    //qDebug()<<mRecvBuf;
}

void QtIOClient::slotClientBytesWritten(qint64 bytes)
{
    Q_UNUSED(bytes);
}

void QtIOClient::slotClientErr(QAbstractSocket::SocketError socketError)
{
    qDebug()<<"slotClientErr:"<<socketError;
    mClient.close();
}
#endif

bool QtIOClient::DisconnectFromServer(void)
{
    if(mTcpClient)
    {
        mTcpClient->Close();
    }

    mIsConnected = false;
    delete mTcpClient;
    mTcpClient = nullptr;
    return true;
}

int QtIOClient::RecvFromServer(QByteArray& data)
{
    data = mRecvBuf;
    mRecvBuf.clear();
    return RstSuccess;
}


void QtIOClient::slotStartThread(void)
{
    if(ConnectToServer())
    {
        qDebug() << "ProcessIO Client Started !";
        emit sigStartSuccess();
    }
    else
    {
        qDebug() << "ProcessIO Client Start Failed, Exited !";
        emit sigThreadAbort();
    }
}


void QtIOClient::slotStopThread(void)
{
    mLoopParseStoped = true;
    DisconnectFromServer();
    if(mLoopEventTimer)
    {
        mLoopEventTimer->stop();
    }
    delete mLoopEventTimer;
    mLoopEventTimer = nullptr;
    qDebug() << "ProcessIO Client exited !";
}

void QtIOClient::slotSend2Server(const QByteArray &data)
{
    mSendBuf.append(data);
}

void QtIOClient::slotLoopParse(void)
{
    /* 定时器槽中停止定时器是避免槽中操作耗时较长，
        超过了定时器超时时间导致定时器重复进入
    */
    mLoopEventTimer->stop();

    /* 接收服务端输入 */
    RecvFromServer(mRecvBuf);
    
    /* 解析 */
    Parse();
    
    /* 发送数据给服务端 */
    if(!mSendBuf.isEmpty())
    {
        int ret = Send2Server(mSendBuf);
        if(ret > 0)
        {
            mSendBuf.remove(0, ret);
        }
    }
    
    mLoopEventTimer->start(mcLoopParseTimeoutMs);
}

void QtIOClient::Parse(void)
{
    if(mRecvBuf.contains("\n"))
    {
        int pos = mRecvBuf.lastIndexOf('\n');

        /* 发送信号给IO server处理 */
        emit sigRecvDataFromServer(mRecvBuf.left(pos+1));
        mRecvBuf = mRecvBuf.mid(pos+1);
    }
}
