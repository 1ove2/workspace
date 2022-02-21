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
#include "IOClient.h"
#include "RstDef.h"



IOClient::IOClient(const QHostAddress &serverIp, uint16_t serverPort, const QString& clientName, int localPort, QObject *parent):
    QThread(parent),
    mTcpClient(nullptr),
    mClientName(clientName),
    mIsConnected(false),
    mServerIP(serverIp),
    mServerPort(serverPort),
    mClientPort(localPort),
    mLoopParseStoped(true),
    mLoopEventTimer(nullptr)
{
    mRecvBuf.clear();
    mSendBuf.clear();
}

IOClient::~IOClient(void)
{
    slotStopThread();
}

bool IOClient::IsConnected(void)
{
    return mIsConnected;
}

int IOClient::Send2Server(const QByteArray &data)
{
    //qDebug() << "To be send data: " << data;
    if(NULL == mTcpClient)
    {
        return RstIOEmptyError;
    }

    int len = 0, size = data.size();
    if(size <= 0)
    {
        return 0;
    }
    do
    {
       int ret = mTcpClient->Send(data.constData() + len, size - len, mcSendTimeoutMs);
       if(ret > 0)
       {
           len += ret;
       }
       else
       {
           qCritical() << "Tcp Client send data failed: " << ret << ", had write len: " << len;
           return ret;
       }
    }while(len < size);
    
    //qDebug() << "send data : " << data << ", size = " << size << ", len = " << len;
    return len;
}

bool IOClient::ConnectToServer(void)
{
    if(mTcpClient)
    {
        qDebug() << "Has Connected to Server: " << mServerIP << " : " << mServerPort;
        return false;
    }
    mTcpClient = new QtTcpClient(mServerIP, mServerPort, mClientName);
    if(!mTcpClient->IsOpen())
    {
        delete mTcpClient;
        mTcpClient = nullptr;
        qCritical() << "Client Connect to " << mServerIP << ":" <<  mServerPort << " failed !";
        return false;
    }
    mIsConnected = mTcpClient->IsOpen();
    mRecvBuf.clear();
    mSendBuf.clear();
    return mIsConnected;

}

bool IOClient::DisconnectFromServer(void)
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

int IOClient::RecvFromServer(QByteArray& data)
{

    if(NULL == mTcpClient)
    {
        return RstIOEmptyError;
    }
    
    char buf[1024] = {0};
    
    int ret = mTcpClient->Recv(buf, sizeof(buf), mcRecvTimeoutMs);
    if(ret > 0)
    {
        data.append(buf, ret);
        return RstSuccess;
    }
    return ret;
}


void IOClient::slotStartThread(void)
{
    if(ConnectToServer())
    {
        qDebug() << "ProcessIO Client Started !";

        mLoopEventTimer = new QTimer;
        disconnect(mLoopEventTimer, &QTimer::timeout, this, &IOClient::slotLoopParse);
        connect(mLoopEventTimer, &QTimer::timeout, this, &IOClient::slotLoopParse);
        //mLoopEventTimer->setSingleShot(true);
        mLoopParseStoped = false;
        mLoopEventTimer->start(mcLoopParseTimeoutMs);
        emit sigStartSuccess();
    }
    else
    {
        qDebug() << "ProcessIO Client Start Failed, Exited !";
        emit sigThreadAbort();
    }
}


void IOClient::slotStopThread(void)
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

void IOClient::slotSend2Server(const QByteArray &data)
{
    mSendBuf.append(data);
}

void IOClient::slotLoopParse(void)
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

void IOClient::Parse(void)
{
    if(mRecvBuf.contains("\n"))
    {
        int pos = mRecvBuf.lastIndexOf('\n');

        /* 发送信号给IO server处理 */
        emit sigRecvDataFromServer(mRecvBuf.left(pos+1));
        mRecvBuf = mRecvBuf.mid(pos+1);
    }
}
