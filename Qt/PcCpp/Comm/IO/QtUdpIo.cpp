#include "QtUdpIo.h"
#include <QCoreApplication>
#include "ITool.h"


QtUdpIo::QtUdpIo(const QHostAddress &serverIp, uint16_t serverPort, const QHostAddress & localIp, uint16_t localPort, const QString &udpIdStr, QObject *parent):
    QtIo(udpIdStr, parent),
    mServerIp(serverIp),
    mServerPort(serverPort),
    mLocalIp(localIp),
    mLocalPort(localPort),
    mSocket(new QUdpSocket(this)),
    mIsConnectted(false)
{
    Open();
}

QtUdpIo::~QtUdpIo(void)
{
    Close();
}

bool QtUdpIo::IsOpen(void) const
{
    return mIsConnectted;
}

int QtUdpIo::Open(void)
{
    if(mSocket)
    {
        mSocket->bind(mLocalIp, mLocalPort, QUdpSocket::ShareAddress);
        mSocket->connectToHost(mServerIp, mServerPort);
        mIsConnectted = mSocket->waitForConnected(mcConnectWaitMs);
        if(mIsConnectted)
        {
            /* 使用信号槽 支撑 阻塞操作 */
            connect(mSocket, &QUdpSocket::bytesWritten, this, &QtUdpIo::BytesWritten);
            connect(mSocket, &QUdpSocket::readyRead,    this, &QtUdpIo::ReadyRead);

            qDebug() << mLocalIp.toString() << " connect to " << mServerIp.toString() << " : " << mServerPort << " whit udp success !";
            return 0;
        }
        else
        {
            qCritical() << mLocalIp.toString() << " connect to " << mServerIp.toString() << " : " << mServerPort << "  whit udp failed: " << mSocket->errorString();
    
        }
    }
    return -1;
}

int QtUdpIo::Close(void)
{
    if(mSocket)
    {
        /* 使用信号槽 支撑 阻塞操作 */
        disconnect(mSocket, &QUdpSocket::bytesWritten, this, &QtUdpIo::BytesWritten);
        disconnect(mSocket, &QUdpSocket::readyRead,    this, &QtUdpIo::ReadyRead);
        mSocket->disconnectFromHost();
        mSocket->close();
    }

    delete mSocket;
    mSocket = nullptr;
    return 0;
}

#if 0
int QtUdpIo::Send(const char *buf, size_t size, int32_t timeout)
{
    PSS_UNUSED(timeout);
    if(!mSocket)
    {
        qCritical() << "socket is null !";
        return PssRstIOEmptyError;
    }
    int ret = mSocket->writeDatagram(buf, size, mServerIp, mServerPort);
    qDebug() << "upd write data len =  " << ret;
    return ret;
}

int QtUdpIo::Recv(char *buf, size_t size, int32_t timeout)
{
    PSS_UNUSED(timeout);
    if(!mSocket)
    {
        qCritical() << "socket is null !";
        return PssRstIOEmptyError;
    }
    QHostAddress srcAddr;
    quint16 srcPort = 0;

    int ret = mSocket->readDatagram(buf, size, &srcAddr, &srcPort);
    if(ret > 0)
    {
        qDebug() << "udp recv data len = "  << ret << ", srcAddr = " << srcAddr.toString() << ", src port = " << srcPort;
    
    }
    return ret;
}
#endif

QIODevice *QtUdpIo::GetQIODevice(void) const
{
    return mSocket;
}
