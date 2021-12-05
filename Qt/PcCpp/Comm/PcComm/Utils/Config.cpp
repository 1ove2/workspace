#include "Config.h"
#include <QSettings>
#include <QDir>
Config::Config()
{

}

Config::~Config()
{

}

QStringList Config::GetQssFiles(const QString &dirPath) const
{
    /* 获取路径 */
    QDir dir(dirPath);
    /* 过滤类型为文件 区分大小写 */
    QFileInfoList subFileList = dir.entryInfoList(QDir::Files | QDir::CaseSensitive);
    QStringList qssFiles;
    /* 遍历得到后缀为qss的文件 */
    for (int i = 0; i < subFileList.size(); ++i)
    {
        QString suffix = subFileList[i].suffix();  /* 获取后缀名 */
        if (0 == suffix.compare("qss"))
        {
            qssFiles << subFileList[i].filePath();
        }
    }
    return qssFiles;
}

bool Config::SetIniValue(const QString &fileName, const QString &groupName, const QStringList &keys, const QStringList &values)
{
	QSettings ini(fileName, QSettings::IniFormat);
	ini.beginGroup(groupName);
	ini.remove("");
	if (keys.count() != values.count())
	{
		return false;
	}
    for (int i = 0; i < keys.count(); ++i)
	{
		ini.setValue(keys.value(i), values.value(i));
	}
	ini.endGroup();
	return true;
}
QStringList Config::GetIniValue(const QString &fileName, const QString &groupName) const
{
	QStringList values;
	QSettings ini(fileName, QSettings::IniFormat);
    ini.beginGroup(groupName);
    for (int i = 0; i < ini.allKeys().count(); ++i)
    {
        QString value = ini.value(ini.allKeys().value(i)).toString();
        values.append(value);
    }
    ini.endGroup();
	return values;
}

void Config::RemoveIniValue(const QString &fileName, const QString &groupName, const QString &value)
{
	QSettings ini(fileName, QSettings::IniFormat);
    ini.beginGroup(groupName);
    for (int i = 0; i < ini.allKeys().count(); ++i)
    {
        if (ini.value(ini.allKeys().value(i)) == value)
        {
            ini.remove(ini.allKeys().value(i));
        }
    }
    ini.endGroup();
}
