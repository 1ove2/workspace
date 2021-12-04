#ifndef DBMYSQL_H
#define DBMYSQL_H
#include <QDir>
#include <QDate>
#include <QDateTime>
#include <QFileInfo>
#include <QTime>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QVariant>
#include <QMutex>
#include "IDataBase.h"
class DBMySql: public IDataBase
{
public:
    DBMySql(QString strSvrName,int nPort,QString strUserName,QString strUserPwd);
    ~DBMySql(void);
public:
    bool reOpenSql(QString dbName);
    bool initConnect(QString strSvrName,int nPort,QString strDbName,QString strUserName,QString strUserPwd);//初始化连接
    bool closeSql(void);
    bool queryExec(QString dbName,QString sqlStr);//执行sql语句，不获取结果
    bool queryExec(QString dbName,QString sqlStr,QList<QHash<QString,QString>> &data);//执行sql语句，并获取结果
    bool getData(QString dbName,QString tableName,QHash<QString,QString> &data,QString sqlWhere=""); //获取数据
    bool getData(QString dbName,QString table,QList<QHash<QString,QString>> &data,QString sqlWhere=""); //获取数据
    bool getData(QString dbName,QString tableName,QHash<QString,QString> columndata,QList<QHash<QString,QString>> &data,QString sqlWhere=""); //获取数据
    bool addData(QString dbName,QString tableName,QHash<QString,QString> data);//增加
    bool delData(QString dbName,QString tableName,QString sqlWhere);//删除
    bool updateData(QString dbName,QString tableName,QHash<QString,QString> data,QString sqlWhere="");//修改

    bool queryExec(QString sqlStr);//执行sql语句，不获取结果
    bool queryExec(QString sqlStr,QList<QHash<QString,QString>> &data);//执行sql语句，并获取结果
    bool getData(QString tableName,QHash<QString,QString> &data,QString sqlWhere=""); //获取数据
    bool getData(QString table,QList<QHash<QString,QString>> &data,QString sqlWhere=""); //获取数据
    bool getData(QString tableName,QHash<QString,QString> columndata,QList<QHash<QString,QString>> &data,QString sqlWhere=""); //获取数据
    bool addData(QString tableName,QHash<QString,QString> data);//增加
    bool delData(QString tableName,QString sqlWhere);//删除
    bool updateData(QString tableName,QHash<QString,QString> data,QString sqlWhere="");//修改
    bool transaction();//事务缓存
    bool commit();//事务提交
    QString getErrorSql();//获取错误的数据库语句
private:
    QSqlDatabase mdb;
    QString m_SvrName;//主机地址
    QString m_UserName; //用户名
    QString m_PassWord;//密码
    QString m_DbName;//数据库名
    int m_Port;//端口号
    QString errorSqlText;//错误语句
    QString m_StrConnName;
private:
    void errorSql(QString sql);//错误打印
    QMutex* m_pMutex;
};

#endif // DBMYSQL_H
