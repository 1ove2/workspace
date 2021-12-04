#ifndef QTUTIL_H
#define QTUTIL_H
//#include <qmath.h>
#include "Config.h"
class QtUtil
{
public:
    static void SetIniValue(const QString &node, const QString &key, const QVariant &value);  /*设置参数*/
    static QVariant GetIniValue(const QString &node,const QString &key);                /*读取参数*/
    static void RemoveIniValue(const QString &groupName, const QString &value);

    static bool IsFileExist(const QString &fileName);                                         /*判断文件是否存在*/
    static QString GetFormatTime(ulong ms);                                                   /*格式时间字符串*/
};

#endif // QTUTIL_H
