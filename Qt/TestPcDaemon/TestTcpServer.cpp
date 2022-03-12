#include "TestTcpServer.h"
#include <QDebug>
#include <QDateTime>
#include <QHostInfo>
TestTcpServer::TestTcpServer(const quint16 &port, QObject *parent) : QObject(parent)
  ,mServer(nullptr)
  ,mSocket(nullptr)
  ,mPort(port)
  ,mRecvTextEdit(nullptr)
  ,mSendTextEdit(nullptr)
  ,mComboxClient(nullptr)
  ,mDevType("PX00")
  ,mSubCardType("P300")
  ,mChannel("1")
  ,mCard("1/2/3/4")
  ,mIsReadTimeout(false)
  ,mAutoReplay(QString(""))
  ,mIsAutoReply(true)
  ,mCurCardID(0)
{
    mServer = new QTcpServer();
    mSocketVector.clear();
    connect(mServer,&QTcpServer::newConnection,this,&TestTcpServer::serverNewConnect);
}

TestTcpServer::~TestTcpServer()
{
    mServer->close();
    mServer->deleteLater();
}

void TestTcpServer::setPort(const quint16 &port)
{
    mPort = port;
}

void TestTcpServer::setTextEdit(QTextEdit *recvEdit,QTextEdit *sendEdit)
{
    mRecvTextEdit = recvEdit;
    mSendTextEdit = sendEdit;
}

void TestTcpServer::setClientComBox(QComboBox *comBoxClient)
{
    mComboxClient = comBoxClient;
}

void TestTcpServer::listenClient(const bool &isListen)
{
    if(isListen)
    {
        if(mServer->isListening())
            return;
        //监听指定的端口
        if(!mServer->listen(QHostAddress::Any, mPort))
        {
            //若出错，则输出错误信息
            qDebug()<<mServer->errorString();
            return;
        }
        qDebug()<< "Listen succeessfully!";
        return;
    }
    if(mServer->isListening())
    {
        mServer->close();
    }
    if(nullptr != mSocket)
    {
        mSocket->abort();
    }
}

void TestTcpServer::sendToClient(const QByteArray &data)
{
    if(mComboxClient->count() < 1)
        return;

    /* 发送消息给当前combox选中的IP地址 */
    QStringList curItem = mComboxClient->currentText().split(':');
    if(2 != curItem.length())
        return;
    QTcpSocket *socket = nullptr;
    foreach (QTcpSocket *socketTmp, mSocketVector) {
        if(socketTmp->peerAddress().toString().contains(curItem.at(0))
                && QString(curItem.at(1)).toUInt() == socketTmp->peerPort())
        {
            socket = socketTmp;
            break;
        }
    }
    if(nullptr == socket)
        return;
    //获取文本框内容并以ASCII码形式发送
    QByteArray _data = data;
    if(!_data.contains('\n'))
    {
        _data.append('\n');
    }
    socket->write(_data);
    socket->flush();
#if 1
    if(nullptr != mSendTextEdit)
    {
        QString recvItem = QString("%1:%2").arg(socket->peerAddress().toString().remove(QString("::ffff:"))).arg(socket->peerPort());
        QString str = QString("%1<-[%2] %3")
                .arg(recvItem)
                .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm;ss.zzz"))
                .arg(QString(_data));
        mSendTextEdit->append(str);
    }
#endif
    qDebug() << "Send: " << _data;
}

void TestTcpServer::serverNewConnect()
{
    //获取客户端连接
    mSocket = mServer->nextPendingConnection();
    mSocketVector.append(mSocket);

    //连接QTcpSocket的信号槽，以读取新数据
    QObject::connect(mSocket, &QTcpSocket::readyRead, this, &TestTcpServer::recvFromClient);
    QObject::connect(mSocket, &QTcpSocket::disconnected, this, &TestTcpServer::socketDisconnected);
    qDebug() << "A Client connect!"<<mSocket->peerAddress().toString()<<(mSocket->peerPort());
    mComboxClient->addItem(QString("%1:%2").arg(mSocket->peerAddress().toString().remove(QString("::ffff:"))).arg(mSocket->peerPort()));
}

void TestTcpServer::recvFromClient()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if(nullptr == socket)
        return;
    QString recvItem = QString("%1:%2").arg(socket->peerAddress().toString().remove(QString("::ffff:"))).arg(socket->peerPort());
    mComboxClient->setCurrentIndex(mComboxClient->findText(recvItem));
    QByteArray buffer;
    //读取缓冲区数据
    //buffer = socket->readAll();
    while(socket->bytesAvailable())
    {
        buffer += socket->readLine();
    }

    if(nullptr != mRecvTextEdit)
    {
        //static QString tmp = "";
        //if(tmp != QString(buffer))
        //{
        QString str = QString("%1->[%2] %3")
                .arg(recvItem)
                .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm;ss.zzz"))
                .arg(QString(buffer));
        mRecvTextEdit->append(str);
        //    tmp = QString(buffer);
        //}
    }
    qDebug() << "Recv: " << buffer;
    autoReplay(QString(buffer));
}

void TestTcpServer::socketDisconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if(nullptr != mComboxClient)
    {
        QString removeItem = QString("%1:%2").arg(socket->peerAddress().toString().remove(QString("::ffff:"))).arg(socket->peerPort());
        mComboxClient->removeItem(mComboxClient->findText(removeItem));
    }
    mSocketVector.removeOne(socket);
    mCurCardID = 0;
    mChannel   = "";
    qDebug() << "Disconnected!";
}

QStringList TestTcpServer::getLocalIp()
{
    QString localHostName=QHostInfo::localHostName();
    qDebug()<<"Local host name:"<<localHostName;
    QHostInfo info=QHostInfo::fromName(localHostName);//根据上边获得的主机名来获取本机的信息
    QString ip = QHostAddress(QHostAddress::LocalHost).toString();
    QStringList list;
    list.clear();
    foreach(QHostAddress address,info.addresses())//info.addresses()---QHostInfo的address函数获取本机ip地址
    {
        if(address.protocol()==QAbstractSocket::IPv4Protocol)//只取ipv4协议的地址
        {
            qDebug()<<"IPV4 addresses:"<<address.toString();
            ip = address.toString();
            list.append(QString("IPV4:")+ip);
        }
        if(address.protocol()==QAbstractSocket::IPv6Protocol)//只取ipv6协议的地址
        {
            qDebug()<<"IPV6 addresses:"<<address.toString();
            //ip = address.toString();
            //list.append(QString("IPV6: ")+ip);
        }
    }
    return list;
}

void TestTcpServer::setDevType(const QString type)
{
    mDevType = type.toUpper();
    qDebug()<<" dev:"<<mDevType;
}

void TestTcpServer::setSubCardType(const QString type)
{
    mSubCardType = type.toUpper();
}

void TestTcpServer::setCards(const QString cards)
{
    mCard = cards;
}

void TestTcpServer::setReadState(const bool &isReadTimeout)
{
    mIsReadTimeout = isReadTimeout;
}

void TestTcpServer::setAutoReplyStr(const bool autoReply)
{
    mIsAutoReply = autoReply;
}

void TestTcpServer::autoReplay(const QString &data)
{
    QString replaystr = "";
    if(data.contains("*IDN?",Qt::CaseInsensitive))
    {
        QString commStr = QString("46e60a-45fd94-91f258-c2a929");
        if(mDevType.contains("10") && mDevType.contains("c",Qt::CaseInsensitive))
        {
            commStr = QString("ebc581-%1-6e8653").arg(mCard);
        }
        replaystr = QString("Wuhan Precise Instrument,%1,q1f2314d23,%2").arg(mDevType.startsWith("10")?mDevType:mSubCardType).arg(commStr);
        sendToClient(replaystr.toLatin1());
        return;
    }
#if 0
    if(data.contains(QString(":SWE:POIN")))
    {
        QString sweepStr = QString(":SWE:POIN");
        QStringList list = data.split("\n");
        qDebug()<<list.length();
        int point = 0;
        for(int i = 0; i < list.length(); i++)
        {
            QString str = list.at(i);
            if(str.contains(sweepStr))
            {
                QStringList _list = str.split(' ');
                if(2 == _list.length())
                {
                    point = QString(_list.at(1)).toInt();
                    break;
                }
            }
        }

        if(point > 0)
        {
            replaystr.clear();
            for(int i = 0; i < point; i++)
            {
                replaystr.append(QString("%1,9.%2;").arg(i).arg(qrand()%30));
            }
            mAutoReplay = replaystr.left(replaystr.length() - 1);
        }
    }
#endif
    if(data.contains(":SYST",Qt::CaseInsensitive) && data.contains(":VERS",Qt::CaseInsensitive))
    {
        QStringList list = mSubCardType.split(',');
        int size = list.length();
        QString subType = mSubCardType;
        if(size > 0)
        {
            subType = list.at(qrand()%size);
        }
        replaystr = QString("%1,343030000000000000,a635450bc23eeeaf5663507ef7ec62b330a636e4,2021/09/27 16:11:59").arg(subType);
        sendToClient(replaystr.toLatin1());
        return;
    }
    if(data.contains(":READ",Qt::CaseInsensitive))
    {
        if(mIsReadTimeout)
        {
            replaystr = QString("-1,-1");
            sendToClient(replaystr.toLatin1());
            return;
        }
        if(!mAutoReplay.isEmpty())
        {
            /* 扫描 */
            sendToClient(mAutoReplay.toLatin1());
            return;
        }

        if(mCurCardID != 0)
        {
            replaystr = QString("[%1-%2:%1%2.%3,0.%4]").arg(mCurCardID).arg(mChannel).arg(qrand()%10).arg(qrand()%10);
        }
        else
        {
            if(mDevType.contains("1010C",Qt::CaseInsensitive))
            {
                QString cardID = data.mid(5,2);
                cardID.remove('?');
                if(cardID.isEmpty())
                {
                    cardID = "0";
                }
                replaystr = QString("[%1-%2:%1%2.%3,0.%4]").arg(cardID).arg(mChannel).arg(qrand()%10).arg(qrand()%10);
            }
            else if(mDevType.contains("PLX",Qt::CaseInsensitive))
            {
                replaystr = QString("%1,2.%2,%3,1.%4").arg(qrand()%10).arg(qrand()%10).arg(qrand()%10).arg(qrand()%10);
            }
            else
            {
                replaystr = QString("%1,0.%2").arg(qrand()%10).arg(qrand()%10);
            }
        }
        sendToClient(replaystr.toLatin1());
        return;
    }
    if(data.contains(":OUTP",Qt::CaseInsensitive) && !data.contains('?'))
    {
        bool isOpen = (data.contains("ON",Qt::CaseInsensitive));
        if(isOpen && (mDevType.startsWith("S") || mDevType.startsWith("E")))
        {
            replaystr = "ON";
            sendToClient(replaystr.toLatin1());
        }
#if 0
        if(isOpen)
        {
            QByteArray _data = ":PSS:ERROR 1234567";
            if(!_data.contains('\n'))
            {
                _data.append('\n');
            }
            qDebug() << "Send: " << _data;
            mSocket->write(_data);
            mSocket->flush();
            isOpen = false;
        }
#endif
        emit sgnOutputStatus(isOpen);
        return;
    }
    if(data.contains(":PSS:VERS?",Qt::CaseInsensitive))
    {
        replaystr = mDevType+",cdf3ee0812ad63f9bee7d0b0436b6e6054a1038a|2021/09/17 13:09:13,0d6a6333a0e08f3dc4d0c3537040772962143871|2021/09/23 08:14:51,a81e7ed00a2f6e4cff9cad8b0c4edd8f500312e8|2021/05/3110:36:31,1d6bae90f9d3e508ff22e420eae4beaae32066f6|2021/09/18 15:10:32";
        sendToClient(replaystr.toLatin1());
        return;
    }
    if(data.contains(":SOUR:FUNC:SHAP",Qt::CaseInsensitive))
    {
        emit sgnPulseMode(!data.contains("DC",Qt::CaseInsensitive));
        return;
    }
    if(data.contains(":SOUR:PULS:COUN",Qt::CaseInsensitive))
    {
        if(data.contains("9999",Qt::CaseInsensitive))
        {
            emit sgnPulseInfiniteMode();
        }
        return;
    }
    if(data.contains(QString(":SYST"),Qt::CaseInsensitive)
            && data.contains(QString(":GRO"),Qt::CaseInsensitive))
    {
        QStringList list = data.split(':');
        if(3 == list.length())
        {
            mCurCardID = QString(list.at(1)).remove("SYST").toInt();
            mChannel   = QString(list.at(2)).remove("GRO").simplified();
            mChannel.remove("\"");
            mChannel.remove("\"");
            qDebug()<<"data :"<<data<<mCurCardID<<mChannel;
        }
        return;
    }
#if 0
    if(data.contains(QString("SYST"),Qt::CaseInsensitive)
            && data.contains(QString("TEMP"),Qt::CaseInsensitive))
    {
        replaystr = QString(data);
        sendToClient(replaystr.toLatin1());
        return;
    }
#endif
    mAutoReplay.clear();
    mChannel = "1";
    if(!mIsAutoReply)
        return;
    sendToClient(QString("test auto reply").toLatin1());
}
