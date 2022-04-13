/******************************************************************************
 *
 * 文件名  ： PssAuthKey.cpp
 * 负责人  ： yuanchaofeng(yuancf@whprecise.com)
 * 创建日期： 20220105
 * 版本号  ： v1.0 20220105
 * 文件描述： 密钥库 实现
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/
#include "PssAuthKey.h"
#include "PssVersion.h"

#include <stdlib.h>
#include <QFile>
#include <QDebug>
#include <QDateTime>
#include <QMessageBox>
#include <QApplication>

/*********************************** 命名空间 **********************************/

/*----------------------------------- 声明区 ----------------------------------*/

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */
/********************************** 变量声明区 *********************************/


/********************************** 函数声明区 *********************************/


/********************************** 变量实现区 *********************************/
const static QString scDateStrFmt = QString("yyyy.MM.dd");

/********************************** 函数实现区 *********************************/
PssAuthKey* PssAuthKey::self = new PssAuthKey();

PssAuthKey *PssAuthKey::GetAuthKey(void)
{
    return self;
}

PssAuthKey::PssAuthKey(void)
{
    /* 版本信息 */
    const char *cVerStr = PssVersionGetStr();
    QString mVerStr = QString(cVerStr);
}

PssAuthKey::~PssAuthKey(void)
{
}

QString PssAuthKey::Encrypt(const QString &verStr, const PssAuthData &data)
{
    /* 构造密钥明文 */
    QString keyStr = data.ToQString();

    /* 加密 */
    QByteArray buf = keyStr.toUtf8();
    int size = buf.size();
    for (int i = 0; i < size; ++i)
    {
        buf[i] = ~buf[i];            /* 取反 */
        buf[i] = buf[i] ^ mKey;      /* 异或 */
    }
    QString key = verStr + "-" + QString(buf);

    qDebug() << keyStr << " => " << key;
    return key;
}

bool PssAuthKey::Decrypt(QString &keyStr)
{
    QFile rFile(QApplication::applicationDirPath() + "/key.pss");
    bool ok = rFile.open(QFile::ReadOnly | QIODevice::Text);
    if(!ok)
    {
        QMessageBox::critical(0, "错误", "密钥key.pss无法打开(读)");
        return false;
    }
    QByteArray buf = rFile.readAll();
    rFile.close();

    /* step1: 解析版本 */
    QString dataStr = QString(buf);
    QStringList list = dataStr.split("-");
    QString verStr = list[0];



    /* step2: 解析密钥 */
    /* step2.1: 获取密钥密文字符串 */
    QString secretStr;
    int size = list.size();
    for(int i = 1; i < list.size(); i++)
    {
        secretStr += list[i];
    }
    qDebug() << buf << " => " << verStr << ":" << secretStr;
    /* step2.2: 解密 */
    QByteArray secretBuf = secretStr.toUtf8();
    size = secretBuf.size();
    for (int i = 0; i < size; ++i)
    {
        secretBuf[i] = secretBuf[i] ^ mKey;      /* 异或 */
        secretBuf[i] = ~secretBuf[i];            /* 取反 */
    }
    keyStr = QString(secretBuf);
    qDebug() << secretStr << " => " << keyStr;

    /* step2.3: 合法性判断 */
    PssAuthData data(keyStr);
    if(!data.IsDateValid())
    {
        QMessageBox::critical(0, "错误", "有效日期已到。");
        return false;
    }
    if(!data.IsCountValid())
    {
        QMessageBox::critical(0, "错误", "有效次数已到.");
        return false;
    }

    /* step2.4: 处理次数限制 */
    data.DecCount();
    secretStr = Encrypt(mVerStr, data);
    QFile wFile(QApplication::applicationDirPath() + "/key.pss");
    ok = wFile.open(QFile::WriteOnly | QIODevice::Text);
    if(!ok)
    {
        QMessageBox::critical(0, "错误", "密钥key.pss无法打开(写)");
        return false;
    }
    wFile.write(secretStr.toUtf8());
    wFile.close();
    return true;
}

PssAuthData::PssAuthData(bool useDate, const QDate &date, bool useCount, int count) :
    mUseDate(useDate),
    mDate(date),
    mUseCount(useCount),
    mCount(count)
{
}

PssAuthData::PssAuthData(const QString &keyStr)
{
    QStringList list = keyStr.split("|");
    mUseDate = bool(list[0].toInt());
    mDate = QDate::fromString(list[1], scDateStrFmt);
    mUseCount = bool(list[2].toInt());
    mCount    = list[3].toInt();

    qDebug() << "PssAuthData build from str:" << keyStr << ":\n"
             << mUseDate << mDate << mUseCount << mCount;
}

PssAuthData::~PssAuthData(void)
{
}

bool PssAuthData::IsCountValid(void) const
{
    if(mCount > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool PssAuthData::IsDateValid(void) const
{
    QDate today = QDate::currentDate();
    if(mDate > today)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool PssAuthData::DecCount(void)
{
    if(mCount > 0)
    {
        mCount--;
        return true;
    }
    else
    {
        return false;
    }
}

bool PssAuthData::IsNoLimit(void) const
{
    if((!mUseDate) && (!mUseCount))
    {
        return true;
    }
    else
    {
        return false;
    }
}

QString PssAuthData::ToQString(void) const
{
    QString dateStr = mDate.toString(scDateStrFmt);
    QString str = QString("%1|%2|%3|%4").arg(mUseDate).arg(dateStr).arg(mUseCount).arg(mCount);
    return str;
}
