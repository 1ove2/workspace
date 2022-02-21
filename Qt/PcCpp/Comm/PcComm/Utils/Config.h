#ifndef _CONFIG_H_
#define _CONFIG_H_
#include "Singleton.h"
#include <QString>


//单例 执行写入写出文件
class Config
{
    SINGLETON(PssConfig)
public:
    QStringList GetQssFiles(const QString &dirPath) const; /* 读取qss样式表文件 */
	
	bool SetIniValue(const QString &fileName, const QString &groupName, const QStringList &keys, const QStringList &values);
	QStringList GetIniValue(const QString &fileName, const QString &groupName) const;
	void RemoveIniValue(const QString &fileName, const QString &groupName, const QString &value);

private:
    //const QString dirPath = ":/qss";
};

#endif // _CONFIG_H_
