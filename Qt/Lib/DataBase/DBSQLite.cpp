#include "DBSQLite.h"

DBSQLite::DBSQLite()
{
    mdbDir = QDir::currentPath();
    m_pMutex = new QMutex;
}

DBSQLite::~DBSQLite()
{
    if(m_pMutex != NULL)
    {
        m_pMutex->unlock();
        m_pMutex = NULL;
    }
}

//1.设置数据库存放路径
bool DBSQLite::setDbDir(QString dirPath)
{
    QDir dir(dirPath);
    if(dir.exists())
    {
        mdbDir = dirPath;
        return true;
    }
    else
    {
        return false;
    }
}
//2.打开连接
bool DBSQLite::reOpenSql(QString dbName)
{
    QString fileName = (mdbDir + "/"+dbName + ".db");
    if(!QFile::exists(fileName))
    {
        return false;
    }
    QFileInfo file(fileName);
    if(file.suffix() != "db")
        return false;

    mdb = QSqlDatabase::database(dbName);
    if(!mdb.isValid())
    {
        mdb = QSqlDatabase::addDatabase("QSQLITE",dbName);
        mdb.setDatabaseName(fileName);
        if (!mdb.open())
        {
            return false;//打开失败
        }
    }
    mdbPath = fileName;
    mdbName = dbName;//数据库名字
    return true;
}
//3.关闭连接
bool DBSQLite::closeSql()
{
    if(mdbName.isEmpty())
        return true;
    if(!QFile::exists(mdbPath))
    {
        return false;
    }
    mdb = QSqlDatabase::database(mdbName);
    if(!mdb.isValid())
    {
        return true;
    }
    mdb.close();
    mdb = QSqlDatabase::database();
    QSqlDatabase::removeDatabase(mdbName);
    mdbName = "";
    mdbPath = "";
    return true;
}
//4.生成一个db文件
bool DBSQLite::creatDbFile(QString dbName)
{
    QString fileName = (mdbDir + "/"+dbName + ".db");
    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly))
    {
        file.close();
    }
    else
    {
        return false;
    }
    return true;
}
//5.错误打印
void DBSQLite::errorSql(QString sql)
{
    errorSqlText = sql;
    //QString("数据库执行错误：%1 ")+sql.toUtf8().constData();
}
//6.获取错误的数据库语句
QString DBSQLite::getErrorSql()
{
    if(mdbName.isEmpty())
    {
        return "db  not setting";//数据库未设置
    }
    return errorSqlText;
}
//7.执行sql语句，不获取结果
bool DBSQLite::queryExec(QString dbName,QString queryStr)
{
    if(mdbName.isEmpty())
    {
        if(!reOpenSql(dbName))
        {
            return false;
        }
    }
    QSqlQuery query(QSqlDatabase::database(dbName, true));
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
bool DBSQLite::queryExec(QString dbName,QString queryStr,QList<QHash<QString,QString>> &data)
{
    data.clear();
    if(mdbName.isEmpty())
    {
        if(!reOpenSql(dbName))
        {
            return false;
        }
    }
    QSqlQuery query(QSqlDatabase::database(dbName, true));
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
bool DBSQLite::getData(QString dbName,QString tableName,QHash<QString,QString> &data,QString sqlWhere)
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
bool DBSQLite::getData(QString dbName,QString tableName,QList<QHash<QString,QString>> &data,QString sqlWhere)
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
bool DBSQLite::getData(QString dbName,QString tableName,QHash<QString,QString> columndata,QList<QHash<QString,QString>> &data,QString sqlWhere)
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
bool DBSQLite::addData(QString dbName,QString tableName,QHash<QString,QString> data)
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
bool DBSQLite::delData(QString dbName, QString tableName, QString sqlWhere)
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
bool DBSQLite::updateData(QString dbName,QString tableName,QHash<QString,QString> data,QString sqlWhere)
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
bool DBSQLite::transaction()
{
    if(mdbName.isEmpty())
        return false;
    m_pMutex->lock();
    bool res = mdb.transaction();
    m_pMutex->unlock();
    return res;
}
bool DBSQLite::commit()
{
    if(mdbName.isEmpty())
        return false;
    m_pMutex->lock();
    bool res = mdb.commit();
    m_pMutex->unlock();
    return res;
}
//执行sql语句，不获取结果
bool DBSQLite::queryExec(QString sqlStr)
{
    if(mdbName.isEmpty())
        return false;
    return  queryExec(mdbName,sqlStr);
}
//执行sql语句，并获取结果
bool DBSQLite::queryExec(QString sqlStr,QList<QHash<QString,QString>> &data)
{
    if(mdbName.isEmpty())
        return false;
    return  queryExec(mdbName,sqlStr,data);
}
//获取数据
bool DBSQLite::getData(QString tableName,QHash<QString,QString> &data,QString sqlWhere)
{
    if(mdbName.isEmpty())
        return false;
    return  getData(mdbName,tableName,data,sqlWhere);
}
//获取数据
bool DBSQLite::getData(QString table,QList<QHash<QString,QString>> &data,QString sqlWhere)
{
    if(mdbName.isEmpty())
        return false;
    return  getData(mdbName,table,data,sqlWhere);
}
//获取数据
bool DBSQLite::getData(QString tableName,QHash<QString,QString> columndata,QList<QHash<QString,QString>> &data,QString sqlWhere)
{
    if(mdbName.isEmpty())
        return false;
    return  getData(mdbName,tableName,columndata,data,sqlWhere);
}
//增加
bool DBSQLite::addData(QString tableName,QHash<QString,QString> data)
{
    if(mdbName.isEmpty())
        return false;
    return  addData(mdbName,tableName,data);
}
//删除
bool DBSQLite::delData(QString tableName,QString sqlWhere)
{
    if(mdbName.isEmpty())
        return false;
    return  delData(mdbName,tableName,sqlWhere);
}
//修改
bool DBSQLite::updateData(QString tableName,QHash<QString,QString> data,QString sqlWhere)
{
    if(mdbName.isEmpty())
        return false;
    return  updateData(mdbName,tableName,data,sqlWhere);
}
//





