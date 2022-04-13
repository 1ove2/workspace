/******************************************************************************
 *
 * 文件名  ： PssQtTools.h
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20220302
 * 版本号  ： v1.0 20220302
 * 文件描述： Pss 基于Qt的工具库 实现
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "PssRstDef.h"
#include "PssQtTools.h"
#include "PssVersion.h"

#include <QMutex>
#include <QList>
#include <QLabel>
#include <QTimer>
#include <QDebug>
#include <QDateTime>
#include <QNetworkInterface>
/*********************************** 命名空间 **********************************/

/*----------------------------------- 声明区 ----------------------------------*/

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/
static void OutPutMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);       /* qDebug重定向函数 */
static QHostAddress PssQtToolsGetIp(bool isBroadcast);                                                  /* 获取本机IP(或局域网广播地址) */

/********************************** 变量实现区 *********************************/
static QPlainTextEdit *sPtr2PlainTextEdit = nullptr;

/********************************** 函数实现区 *********************************/
QString PssQtToolsGetRepoTime(void)
{
    const char *str = PssGetGitTimeStr();
    QString qStr = QString(str);

    return qStr;
}

QString PssQtToolsGetRepoHash(void)
{
    const char *str = PssGetGitHashStr();
    QString qStr = QString(str);

    return qStr;
}

void PssQtToolsRedirect(QPlainTextEdit *plainTextEdit)
{
    /* 调试信息重定向 */
    sPtr2PlainTextEdit = plainTextEdit;
    qInstallMessageHandler(OutPutMessage);
}

static void OutPutMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context);

    QString allMsg;
    QString typeStr;
    static QMutex mutex;
    mutex.lock();

    switch (type) {
        case QtMsgType::QtInfoMsg:
        {
            typeStr += "INFO";
            break;
        }

        case QtMsgType::QtDebugMsg:
        {
            typeStr += "DEBG";
            break;
        }

        case QtMsgType::QtWarningMsg:
        {
            typeStr += "WARN";
            break;
        }

        case QtMsgType::QtCriticalMsg:
        {
            typeStr += "CRIT";
            break;
        }

        default:
        {
            typeStr += QString("000%1 ").arg(type);
            break;
        }
    }

    const int cMaxSize = 1024 * 1024; /* 1024 */
    int size = sPtr2PlainTextEdit->toPlainText().size();
    if(size > cMaxSize)
    {
        sPtr2PlainTextEdit->clear();
    }

    QString nowStr = QDateTime::currentDateTime().toString("hh:mm:ss.zzzzzzzzz");
    allMsg = QString("[%1 %2]:%3").arg(typeStr).arg(nowStr).arg(msg);
    sPtr2PlainTextEdit->appendPlainText(allMsg);

    mutex.unlock();
}

static QHostAddress PssQtToolsGetIp(bool isBroadcast)
{
    /* 获取本机IP */
    QList<QNetworkInterface> interfaceList = QNetworkInterface::allInterfaces();
    for(int i = 0; i < interfaceList.size(); i++)
    {
        QNetworkInterface qInterface = interfaceList[i];
        QList<QNetworkAddressEntry> entryList = qInterface.addressEntries();
        for(int j = 0; j < entryList.size(); j++)
        {
            QNetworkAddressEntry entry     = entryList[j];
            QHostAddress         ip        = entry.ip();
            QHostAddress         broadcast = entry.broadcast();
            QString broadcastStr = broadcast.toString();
            if(broadcastStr.size() > 0)
            {
                if(isBroadcast)
                {
                    return broadcast;
                }
                else
                {
                    return ip;
                }
            }
        }
    }

    return QHostAddress::Null;
}

QHostAddress PssQtToolsGetLocalIp(void)
{
    return PssQtToolsGetIp(false);
}

QHostAddress PssQtToolsGetLocalBroadcastIp(void)
{
    return PssQtToolsGetIp(true);
}

bool PssQtToolsIsValidIp(const QString &ipStr)
{
    QRegExp rxIp("^\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}\\.\\d{1,3}$");
    if (!rxIp.exactMatch(ipStr))
    {
        return false;
    }

    return true;
}

i32 PssQtToolsQString2CStr(char *cStr, i32 cStrSize, const QString &qString)
{
    QByteArray ba = qString.toUtf8();
    const char *ipStr = ba.data();
    i32 size = ba.size();

    if(cStrSize < size)
    {
        qCritical() << "QString2CStr" << qString << "over flow.";
        return PssRstOverFlow;
    }

    memmove(cStr, ipStr, size);
    return size;
}

i32 PssQtToolsCStr2QString(QString &qString, char *cStr, i32 cStrSize)
{
    QByteArray ba = QByteArray(cStr, cStrSize);
    qString.append(ba);

    return ba.size();
}
