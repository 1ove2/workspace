#include "Config.h"

QSettings mSetting(QString(CONFIGFILE_PATH)+QString(CONFIGFILE_NAME), QSettings::IniFormat);
Config::Config()
{
}

Config::~Config()
{
}

void Config::SetIniValue(const QString &node, const QString &key, const QVariant &value)
{
    mSetting.setValue(QString("/%1/%2").arg(node).arg(key), value);
    mSetting.sync();
}

QVariant Config::GetIniValue(const QString &node,const QString & key) const
{
    QVariant value = -1;
    value = mSetting.value(QString("/%1/%2").arg(node).arg(key));
    return value;
}

void Config::RemoveIniValue(const QString &groupName, const QString &value)
{
    mSetting.beginGroup(groupName);
    for (int i = 0; i < mSetting.allKeys().count(); ++i)
    {
        if (mSetting.value(mSetting.allKeys().value(i)) == value)
        {
            mSetting.remove(mSetting.allKeys().value(i));
        }
    }
    mSetting.endGroup();
}
