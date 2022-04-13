/******************************************************************************
 *
 * 文件名  ： PssAuthKey.h
 * 负责人  ： yuanchaofeng(yuancf@whprecise.com)
 * 创建日期： 20220105
 * 版本号  ： v1.0 20220105
 * 文件描述： 密钥库 接口
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _PSS_AUTH_KEY_H_
#define _PSS_AUTH_KEY_H_

/************************************ 头文件 ***********************************/
#include <QDate>
#include <QString>

/*********************************** 命名空间 **********************************/

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */
/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/
class PssAuthData
{
public:
    explicit PssAuthData(bool useDate, const QDate &datetime, bool useCount, int count);                /* 构造 */
    explicit PssAuthData(const QString &keyStr);                                                        /* 构造 */
    ~PssAuthData(void);                                                                                 /* 析构 */

    bool    IsCountValid(void) const;                                                                   /* 次数限制已到 */
    bool    IsDateValid(void) const;                                                                    /* 日期限制已到 */
    bool    DecCount(void);                                                                             /* 启动次数减1 */
    bool    IsNoLimit(void) const;                                                                      /* 无限制 */
    QString ToQString(void) const;                                                                      /* 转换为字符串 */

private:
    bool  mUseDate;                                                                                     /* 是否启用日期限制 */
    QDate mDate;                                                                                        /* 截至日期 */
    bool  mUseCount;                                                                                    /* 是否启用次数限制 */
    int   mCount;                                                                                       /* 可用次数 */
};

class PssAuthKey
{
public:
    static PssAuthKey *GetAuthKey(void);                                                                /* 获取单例 */
    static QString Encrypt(const QString &verStr, const PssAuthData &data);                             /* 加密 */
           bool    Decrypt(QString &keyStr);                                                            /* 解密 */

private:
    explicit PssAuthKey(void);                                                                          /* 构造 */
    ~PssAuthKey(void);                                                                                  /* 析构 */
    static PssAuthKey *self;                                                                            /* 类指针 */

    QString mVerStr;                                                                                    /* 版本信息 */
    const static char mKey = 0xA5;                                                                      /* 密钥 */
};

#endif // _PSS_AUTH_KEY_H_
