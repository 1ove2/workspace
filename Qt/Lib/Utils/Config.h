#ifndef CONFIG_H
#define CONFIG_H
#include "Singleton.h"
#include <QObject>
#include <QSettings>
#include <QtCore/QtCore>

#define CONFIGFILE_NAME "Config.ini"
#define CONFIGFILE_PATH ""
//单例 执行写入写出文件
class Config
{
    SINGLETON(Config)
public:
        void SetIniValue(const QString &node, const QString &key, const QVariant &value);  /*设置参数*/
        QVariant GetIniValue(const QString &node,const QString &key) const;                /*读取参数*/
        void RemoveIniValue(const QString &groupName, const QString &value);

};

#endif // CONFIG_H
