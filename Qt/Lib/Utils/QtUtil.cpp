#include "QtUtil.h"
#include <QFile>
#include <QFileInfo>

void QtUtil::SetIniValue(const QString &node, const QString &key, const QVariant &value)
{
    Singleton<Config>::getInstance().SetIniValue(node, key, value);
}

QVariant QtUtil::GetIniValue(const QString &node, const QString &key)
{
    return Singleton<Config>::getInstance().GetIniValue(node, key);
}

void QtUtil::RemoveIniValue(const QString &groupName, const QString &value)
{
    Singleton<Config>::getInstance().RemoveIniValue(groupName, value);
}

bool QtUtil::IsFileExist(const QString &fileName)
{
    QFileInfo fileInfo(fileName);
    if (fileInfo.isFile())
    {
        return true;
    }
    return false;
}

QString QtUtil::GetFormatTime(ulong ms)
{
    int ss = 1000;
    int mi = ss * 60;
    int hh = mi * 60;
    int dd = hh * 24;

    ulong day = ms / dd;
    ulong hour = (ms - day * dd) / hh;
    ulong minute = (ms - day * dd - hour * hh) / mi;
    ulong second = (ms - day * dd - hour * hh - minute * mi) / ss;
    ulong milliSecond = ms - day * dd - hour * hh - minute * mi - second * ss;

    QString da = QString::number(day);
    QString hou = QString::number(hour);
    QString min = QString::number(minute);
    QString sec = QString::number(second);
    QString msec = QString::number(milliSecond);

    return da + "d" + hou + ":" + min + ":" + sec + "." + msec;
}
