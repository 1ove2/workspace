#include "IOServer.h"
#include "ITool.h"
#include <QCoreApplication>

IOServer::IOServer(const uint serverPort, const uint clientPort, const QString& serverName, QObject *parent):
    QThread(parent),
    mServerPort(serverPort),
    mClientPort(clientPort),
    mServerName(serverName),
    mLoopParseTimer(nullptr),
    mLoopParseStoped(true)
{
    mRecvBuf.clear();
    mSendBuf.clear();
}

IOServer::~IOServer()
{
    slotStopThrad();
}

bool IOServer::StartServer(void)
{
    /*1. 创建socket */
    mServerSocket = new QtTcpServer(mServerPort, mServerName);

    return true;
}

void IOServer::StopServer(void)
{

    if(mServerSocket)
    {
        mServerSocket->Close();
    }
    
    delete mServerSocket;
    mServerSocket = nullptr;
}

void IOServer::RecvFromClient(QByteArray &data)
{
    if(!mServerSocket)
    {
        return ;
    }
    char buf[1024] = {0};
    int ret = mServerSocket->Recv(buf, sizeof(buf), mcRecvWaitMs);
    if(ret <= 0)
    {
        return ;
    }
    data.append(buf, ret);
    //qDebug() << "\n\nio server recv data: " << data << "\n\n";
}

int IOServer::Send2Client(const QByteArray &data)
{
    if(!mServerSocket)
    {
        return 0;
    }

    int len = 0, ret = 0;
    int size = data.size();
    if(size <= 0)
    {
        return 0;
    }
    
    while(len < size)
    {
        ret = mServerSocket->Send(data.constData() + len, size - len, mcSendWaitMs);
        if(ret <= 0)
        {
            if(RstOptTimeoutError == ret)
            {
                continue;
            }
            qCritical() << mServerSocket->GetIdStr()  <<  " Write to Process Client: " << data << " failed : " << QString::number(ret);
            return ret;
        }
        len += ret;
    }
    //qDebug() << "send to client: " << data.mid(0, len);
    return len;
}

void IOServer::slotStopThrad(void)
{
    mLoopParseStoped = true;
    if(mLoopParseTimer)
    {
        mLoopParseTimer->stop();
    }

    StopServer();
    
    mRecvBuf.clear();
    mSendBuf.clear();
    delete mLoopParseTimer;
    mLoopParseTimer = nullptr;
    
    qDebug() << "ProcessIO Server Thread Exited !";
}

void IOServer::slotStartThread(void)
{
    mRecvBuf.clear();

    /*1. 创建socket */
    if(!StartServer())
    {
        qCritical() << "UI Process Server Start failed !";
        emit sigThreadAbort();
        return ;
    }
    
    /* 此处选用定时器间隔驱动而不使用死循环是因为
        ARM只有双核，死循环情况下在多线程切换间时间消耗较大
        使用定时器能让线程间歇休眠，提高CPU实际使用效率
        降低线程间切换时延，不在死循环中休眠是利用定时器机制
        间接调用processEvent来驱动当前线程事件循环
    */
    mLoopParseStoped = false;
    mLoopParseTimer = new QTimer;
    connect(mLoopParseTimer, &QTimer::timeout, this, &IOServer::slotLoopParse);
    mLoopParseTimer->start(mcLoopParseTimeoutMs);
    qDebug() << "ProcessIO Server" <<  mServerSocket->GetIdStr() << " Started , thread ID = " << QThread::currentThreadId();
}

void IOServer::slotSend2Client(const QByteArray &data)
{
    mSendBuf.append(data);
}

void IOServer::slotLoopParse(void)
{

    /* 此处选用定时器间隔驱动而不使用死循环是因为
        ARM只有双核，死循环情况下在多线程切换间时间消耗较大
        使用定时器能让线程间歇休眠，提高CPU实际使用效率
        降低线程间切换时延，不在死循环中休眠是利用定时器机制
        间接调用processEvent来驱动当前线程事件循环
    */
    
    /* 定时器槽中停止定时器是避免槽中操作耗时较长，
        超过了定时器超时时间导致定时器重复进入
    */
    mLoopParseTimer->stop();
    
    /* 接收输入 */
    RecvFromClient(mRecvBuf);
    
    /* 解析 */
    Parse();
    
    /* 输出给客户端 */
    if(!mSendBuf.isEmpty())
    {
#if 0
        int ret = Send2Client(mSendBuf);
        if(ret > 0)
        {
            mSendBuf.remove(0, ret);
        }
#else
        Send2Client(mSendBuf);
        mSendBuf.clear();
#endif

    }
    
    mLoopParseTimer->start(mcLoopParseTimeoutMs);
}

void IOServer::Parse(void)
{
    if(!mRecvBuf.contains("\n"))
    {
        return ;
    }
    int pos = mRecvBuf.lastIndexOf('\n');
    //qDebug() << "io server recv complete scpi: " << mRecvBuf.left(pos+1);
    emit sigRecvFromClient(mRecvBuf.left(pos+1));

    mRecvBuf.remove(0, pos + 1);

}
