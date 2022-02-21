#include "DBMySql.h"
DBMySql::DBMySql(QString strSvrName,int nPort,QString strUserName,QString strUserPwd)
{
    this->m_SvrName = strSvrName;
    this->m_Port = nPort;
    this->m_UserName = strUserName;
    this->m_PassWord = strUserPwd;
    m_pMutex = new QMutex;
}
DBMySql::~DBMySql(void)
{
    if(m_pMutex != NULL)
    {
        m_pMutex->unlock();
        m_pMutex = NULL;
    }
}

//打开连接
bool DBMySql::reOpenSql(QString dbName)
{
    this->m_DbName = dbName;
    return initConnect(m_SvrName,m_Port,m_DbName,m_UserName,m_PassWord);
}

bool DBMySql::initConnect(QString strSvrName,int nPort,QString strdbName,QString strUserName,QString strUserPwd)
{
    m_StrConnName = "MySqlConn";
    mdb = QSqlDatabase::database(m_StrConnName,false);
    if(!mdb.isValid())
    {
        mdb = QSqlDatabase::addDatabase("QMYSQL",m_StrConnName);
        mdb.setHostName(strSvrName);
        mdb.setDatabaseName(strdbName);
        mdb.setPort(nPort);
        mdb.setUserName(strUserName);
        mdb.setPassword(strUserPwd);
        mdb.exec("SET NAMES 'utf8'");
        if(!mdb.open())
        {
            return false;
        }
    }
    return true;
}

bool DBMySql::closeSql()
{
    mdb.close();
    QSqlDatabase::removeDatabase(m_StrConnName);
    m_StrConnName = "";
    return true;
}

//错误打印
void DBMySql::errorSql(QString sql)
{
    errorSqlText = sql;
}

//获取错误的数据库语句
QString DBMySql::getErrorSql()
{
    if(m_StrConnName.isEmpty())
    {
        return "db  not setting";//数据库未设置
    }
    return errorSqlText;
}

//执行sql语句，不获取结果
bool DBMySql::queryExec(QString dbName,QString queryStr)
{
    if(m_StrConnName.isEmpty())
    {
        if(!reOpenSql(dbName))
        {
            return false;
        }
    }
    QSqlQuery query(QSqlDatabase::database(m_StrConnName, true));
    m_pMutex->lock();
    if(!query.exec(queryStr))
    {
        errorSql(queryStr);
        m_pMutex->unlock();
        return false;
    }
    m_pMutex->unlock();
    return  true;
}

//执行sql语句，并获取结果
bool DBMySql::queryExec(QString dbName,QString queryStr,QList<QHash<QString,QString>> &data)
{
    data.clear();
    if(m_StrConnName.isEmpty())
    {
        if(!reOpenSql(dbName))
        {
            return false;
        }
    }
    QSqlQuery query(QSqlDatabase::database(m_StrConnName, true));
    m_pMutex->lock();
    if(!query.exec(queryStr))
    {
        errorSql(queryStr);
        m_pMutex->unlock();
        return false;
    }
    m_pMutex->unlock();
    QSqlRecord rec = query.record();
    while(query.next())
    {
        QHash<QString,QString> rowData;
        for(int i =0;i<rec.count();i++)
        {
            QVariant::Type ty =  query.value(i).type();
            if( QVariant::Type::Date == ty)
            {
                QDate temp = query.value(i).toDate();
                rowData[rec.fieldName(i)]=temp.toString("yyyy-MM-dd");
            }
            else if( QVariant::Type::Time == ty)
            {
                QTime temp = query.value(i).toTime();
                rowData[rec.fieldName(i)]=temp.toString("hh:mm:ss");
            }
            else if( QVariant::Type::DateTime == ty)
            {
                QDateTime temp = query.value(i).toDateTime();
                rowData[rec.fieldName(i)]=temp.toString("yyyy-MM-dd hh:mm:ss");
            }
            else
                rowData[rec.fieldName(i)]=query.value(i).toString();
        }
        data.append(rowData);
    }
    return  true;
}

//获取数据
bool DBMySql::getData(QString dbName,QString tableName,QHash<QString,QString> &data,QString sqlWhere)
{
    data.clear();
    QList<QHash<QString,QString>> dataList;
    if(!getData(dbName,tableName,dataList,sqlWhere))
    {
        return false;
    }
    if(dataList.count() > 0)
    {
        data = dataList[0];
    }
    return true;
}
//获取数据
bool DBMySql::getData(QString dbName,QString tableName,QList<QHash<QString,QString>> &data,QString sqlWhere)
{
    QString queryStr="select * from "+tableName;
    if(!sqlWhere.isEmpty())
        queryStr+=" "+sqlWhere;

    if(!queryExec(dbName,queryStr,data))
    {

        return  false;
    }

    return  true;
}
//获取数据
bool DBMySql::getData(QString dbName,QString tableName,QHash<QString,QString> columndata,QList<QHash<QString,QString>> &data,QString sqlWhere)
{
    QString colunmStr;
    if(columndata.count() == 0)
        colunmStr = "*";
    else
    {
        QStringList keys = columndata.keys();
        for(auto key : keys)
        {
            QString column = QString("%1 AS `%2`").arg(key).arg(columndata[key]);
            if(!colunmStr.isEmpty())
                colunmStr += ",";
            colunmStr += column;
        }
    }
    QString queryStr = QString("SELECT %1 FROM %2 %3").arg(colunmStr).arg(tableName).arg( sqlWhere);

    if(!queryExec(dbName,queryStr,data))
    {

        return  false;
    }
    return  true;
}
//增加
bool DBMySql::addData(QString dbName,QString tableName,QHash<QString,QString> data)
{
    if(data.isEmpty())
        return false;
    QString queryStr="insert into "+tableName+" ";
    QString fieldStr="(",valueStr="values(";
    QHash<QString,QString>::iterator it;
    for ( it = data.begin(); it != data.end(); ++it )
    {
        fieldStr+=it.key()+",";
        valueStr+="'"+it.value()+"',";
    }
    fieldStr=fieldStr.left(fieldStr.length()-1);
    valueStr=valueStr.left(valueStr.length()-1);
    fieldStr+=")";
    valueStr+=")";
    queryStr+=fieldStr+" "+valueStr;
    if(!queryExec(dbName,queryStr))
    {

        return false;
    }
    return true;
}

//删除
bool DBMySql::delData(QString dbName, QString tableName, QString sqlWhere)
{
    QString queryStr="delete from "+tableName;
    if(!sqlWhere.isEmpty())
        queryStr+=" "+sqlWhere;

    if(!queryExec(dbName,queryStr))
    {
        m_pMutex->unlock();
        return false;
    }

    return true;
}

//修改
bool DBMySql::updateData(QString dbName,QString tableName,QHash<QString,QString> data,QString sqlWhere)
{
    QString queryStr="update "+tableName+" ";
    QHash<QString,QString>::iterator it;
    QString setStr="set ";
    for ( it = data.begin(); it != data.end(); ++it )
    {
        setStr+=it.key()+"='"+it.value()+"'";
        setStr+=",";
    }
    setStr=setStr.left(setStr.length()-1);
    queryStr+=setStr;
    if(!sqlWhere.isEmpty())
        queryStr+=" "+sqlWhere;

    if(!queryExec(dbName,queryStr))
    {
        m_pMutex->unlock();
        return false;
    }

    return true;
}

bool DBMySql::transaction()
{
    if(m_StrConnName.isEmpty())
        return false;
    m_pMutex->lock();
    bool res = mdb.transaction();
    m_pMutex->unlock();
    return res;
}

bool DBMySql::commit()
{
    if(m_StrConnName.isEmpty())
        return false;
    m_pMutex->lock();
    bool res = mdb.commit();
    m_pMutex->unlock();
    return res;
}

//执行sql语句，不获取结果
bool DBMySql::queryExec(QString sqlStr)
{
    if(m_StrConnName.isEmpty())
        return false;
    return  queryExec(m_DbName,sqlStr);
}

//执行sql语句，并获取结果
bool DBMySql::queryExec(QString sqlStr,QList<QHash<QString,QString>> &data)
{
    if(m_StrConnName.isEmpty())
        return false;
    return  queryExec(m_DbName,sqlStr,data);
}

//获取数据
bool DBMySql::getData(QString tableName,QHash<QString,QString> &data,QString sqlWhere)
{
    if(m_StrConnName.isEmpty())
        return false;
    return  getData(m_DbName,tableName,data,sqlWhere);
}

//获取数据
bool DBMySql::getData(QString table,QList<QHash<QString,QString>> &data,QString sqlWhere)
{
    if(m_StrConnName.isEmpty())
        return false;
    return  getData(m_DbName,table,data,sqlWhere);
}

//获取数据
bool DBMySql::getData(QString tableName,QHash<QString,QString> columndata,QList<QHash<QString,QString>> &data,QString sqlWhere)
{
    if(m_StrConnName.isEmpty())
        return false;
    return  getData(m_DbName,tableName,columndata,data,sqlWhere);
}

//增加
bool DBMySql::addData(QString tableName,QHash<QString,QString> data)
{
    if(m_StrConnName.isEmpty())
        return false;
    return  addData(m_DbName,tableName,data);
}
//删除
bool DBMySql::delData(QString tableName,QString sqlWhere)
{
    if(m_StrConnName.isEmpty())
        return false;
    return  delData(m_DbName,tableName,sqlWhere);
}
//修改
bool DBMySql::updateData(QString tableName,QHash<QString,QString> data,QString sqlWhere)
{
    if(m_StrConnName.isEmpty())
        return false;
    return  updateData(m_DbName,tableName,data,sqlWhere);
}
