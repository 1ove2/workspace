#ifndef TESTTCPSERVER_H
#define TESTTCPSERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QTextEdit>
#include <QComboBox>
class TestTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TestTcpServer(const quint16 &port, QObject *parent = 0);
    ~TestTcpServer(void);

    void setPort(const quint16 &port);

    void setTextEdit(QTextEdit *recvEdit, QTextEdit *sendEdit);

    void setClientComBox(QComboBox *comBoxClient);

    void listenClient(const bool &isListen);

    void sendToClient(const QByteArray &data);

    void serverNewConnect();

    void recvFromClient();

    void socketDisconnected();

    QStringList getLocalIp(void);

    void setDevType(const QString type);

    void setSubCardType(const QString type);

    void setCards(const QString cards);

    void setReadState(const bool &isReadTimeout);

    void setAutoReplyStr(const bool autoReply);
signals:
    void sgnOutputStatus(const bool &isOpen);
    void sgnPulseMode(const bool &isPulse);
    void sgnPulseInfiniteMode(void);
private slots:

private:
    QTcpServer* mServer;
    QTcpSocket *mSocket;
    quint16    mPort;
    QTextEdit  *mRecvTextEdit;
    QTextEdit  *mSendTextEdit;
    QComboBox  *mComboxClient;
    QVector<QTcpSocket *> mSocketVector;
    QString mDevType;
    QString mSubCardType;
    QString mChannel;
    QString mCard;
    bool mIsReadTimeout;
    QString mAutoReplay;
    bool    mIsAutoReply;
    int     mCurCardID;
    void autoReplay(const QString &data);    
};

#endif // TESTTCPSERVER_H
