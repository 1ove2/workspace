#ifndef IDATABASE_H
#define IDATABASE_H
#include <QString>

class IDataBase
{
public:
    virtual ~IDataBase(void);

    virtual bool reOpenSql(QString dbName) = 0;                                                      /* 打开 */
    virtual bool closeSql(void) = 0;                                                                 /* 关闭 */
    virtual bool addData(QString tableName,QHash<QString,QString> data) = 0;                         /* 增加 */
    virtual bool delData(QString tableName,QString sqlWhere) = 0;                                    /* 删除 */
    virtual bool updateData(QString tableName,QHash<QString,QString> data,QString sqlWhere="") = 0;  /* 修改 */

protected:
     IDataBase(void);                                                                            /* 构造函数,抽象类,不可实例化 */

private:
};


#endif // IDATABASE_H
