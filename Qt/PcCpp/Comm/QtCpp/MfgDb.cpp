/******************************************************************************
 *
 * 文件名  ： MfgDb.cpp
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20200822
 * 版本号  ： v1.0 20200822
 * 文件描述： PSS Qt 版本MfgDb数据库
 * 其    他： 无
 * 修改日志： v1.0 20200822 初始版本
 *
  *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "MfgDb.h"
#include "PssRstDef.h"
#include "PssQtHal.h"
#include "ITool.h"

#include "PssBinAppCommMcuAtsha204aStatus.h"

#include <QSqlError>
#include <QSqlQuery>
#include <QSqlResult>


using namespace Pss::Qt;

/*----------------------------------- 声明区 ----------------------------------*/

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */

/********************************** 变量声明区 *********************************/


/********************************** 函数声明区 *********************************/

/********************************** 变量实现区 *********************************/
const QString MfgDb::mcType   = QString("QMYSQL");
const QString MfgDb::mcIp     = QString("192.168.12.101");
const int     MfgDb::mcPort   = 9001;
const QString MfgDb::mcName   = QString("mfg_test");
const QString MfgDb::mcUsr    = QString("pss");
const QString MfgDb::mcPasswd = QString("pss123");

const QString MfgDb::mcDefaultPasswdSuffix = QString("123456");

/********************************** 函数实现区 *********************************/
MfgDb::MfgDb(void) :
    QObject(),
    mDb(QSqlDatabase::addDatabase(mcType)),
    mBrdId(-1)
{
    mDb.setHostName(mcIp);
    mDb.setPort(mcPort);
    mDb.setUserName(mcUsr);
    mDb.setPassword(mcPasswd);
    bool ok = mDb.open();
    if(ok)
    {
        qDebug() << mcName << "Open OK";
    }
    else
    {
        QString msg = QString("%1 Open Failled: %2").arg(mcName).arg(mDb.lastError().text());
        ShowMsgBoxInfo(msg);
    }

    ChangeDb(mcName);

#if 0
    //TestDb();
    InitDb();
#endif
}

MfgDb::~MfgDb(void)
{
    if(mDb.isOpen())
    {
        mDb.close();
        qDebug() << mcName << "Close OK";
    }
}

void MfgDb::CreateNewUsr(const QString &usr, const QString &fullName)
{
    /* 添加用户: PengPeng(pp) */
    QString passwd = QString2Passwd(QString("%1%2").arg(usr).arg(mcDefaultPasswdSuffix));
    QString cmd = QString("INSERT INTO usr (usr, passwd, full_name) "
                          "VALUES ('%1', '%2', '%3');").arg(usr).arg(passwd).arg(fullName);
    QSqlQuery query;
    bool ok = query.exec(cmd);
    if(!ok)
    {
        QString msg = QString("CreateNewUsr Failled: %1").arg(query.lastError().text());
        ShowMsgBoxInfo(msg);
    }

    return;
}

int MfgDb::GetUsrId(const QString &usr, const QString &passwd)
{
    ChangeDb(mcName);

    /* step1: 获取账户对应密码 */
    QString cmd = QString("SELECT passwd, id FROM usr WHERE usr = '%1';").arg(usr);
    QSqlQuery query;
    //qDebug() << "\n" << cmd;
    bool ok = query.exec(cmd);
    if(!ok)
    {
        QString msg = QString("GetUsrId Failled: %1").arg(query.lastError().text());
        ShowMsgBoxInfo(msg);
        return PssRstMySqlCmdErr;
    }
    
    if(0 == query.size())
    {
        QString msg = QString(QStringLiteral("%1 用户不存在.")).arg(usr);
        ShowMsgBoxInfo(msg);
        return PssRstMySqlUsrErr;
    }
    
    /* step2: 所有同名账户密码 都被检查 */
    while (query.next())
    {
        QString passwdInDb = query.value(0).toString();
        QString passwdEncrypted = QString2Passwd(passwd);
        if(0 == passwdEncrypted.compare(passwdInDb)) /* 比对密码 */
        {
            int id = query.value(1).toInt();
            QString msg = QString("%1,%2(%3) check passed.").arg(usr).arg(passwd).arg(id);
            qDebug() << msg;
            return id;
        }
    }
    
    QString msg = QString(QStringLiteral("%1 密码错误.")).arg(usr);
    ShowMsgBoxInfo(msg);
    return PssRstMySqlPasswdErr;
}

QString MfgDb::GetUsrFullName(int usrId)
{
    ChangeDb(mcName);

    /* step1: 获取账户对应密码 */
    QString cmd = QString("SELECT full_name FROM usr WHERE id = '%1';").arg(usrId);
    QSqlQuery query;
    //qDebug() << "\n" << cmd;
    bool ok = query.exec(cmd);
    if(!ok)
    {
        QString msg = query.lastError().text();
        ShowMsgBoxInfo(msg);
        return PssRstMySqlCmdErr;
    }
    
    int size = query.size();
    if(1 != size)
    {
        QString msg = QString(QStringLiteral("%1 用户异常:%d.")).arg(usrId).arg(size);
        ShowMsgBoxInfo(msg);
        return PssRstMySqlUsrErr;
    }
    
    query.next();
    QString fullName = query.value(0).toString();
    return fullName;
}

void MfgDb::UpdateBrd(const Pss::PssBinAppCommMcuAtsha204aStatus &status, bool isNewBrd)
{
    const int utf8BufSize = 32;
    char utf8Buf[utf8BufSize];

    /* atsha204a id号 */
    QString atsha204aIdStr = Atsha204aIdBuf2QString(status);
    qDebug() << __FILE__ << __LINE__ << isNewBrd << atsha204aIdStr;
    
    QString companyStr;
    QString productStr;
    QString brdTypeStr;
    QString cfgStr;
    QString bomStr;
    QString hwverStr;
    
    memset(utf8Buf, 0, utf8BufSize);status.GetCompany(utf8Buf, utf8BufSize); Utf82QString(companyStr, utf8Buf, utf8BufSize);
    memset(utf8Buf, 0, utf8BufSize);status.GetProduct(utf8Buf, utf8BufSize); Utf82QString(productStr, utf8Buf, utf8BufSize);
    memset(utf8Buf, 0, utf8BufSize);status.GetBrdType(utf8Buf, utf8BufSize); Utf82QString(brdTypeStr, utf8Buf, utf8BufSize);
    memset(utf8Buf, 0, utf8BufSize);status.GetCfg(utf8Buf, utf8BufSize);     Utf82QString(cfgStr, utf8Buf, utf8BufSize);
    memset(utf8Buf, 0, utf8BufSize);status.GetBom(utf8Buf, utf8BufSize);     Utf82QString(bomStr, utf8Buf, utf8BufSize);
    memset(utf8Buf, 0, utf8BufSize);status.GetHwVer(utf8Buf, utf8BufSize);   Utf82QString(hwverStr, utf8Buf, utf8BufSize);
    
    ChangeDb(mcName);
    
    /* 获取子板ID */
    mBrdId = 1;
    int brdId = mBrdId;
    if(!isNewBrd) /* 需要查找 brd id */
    {
        brdId = GetBrdIdFromAtsha204aId(atsha204aIdStr);
    }
    
    /* 更新子板表 */
    QString cmd = QString("UPDATE brd "
                          "SET atsha204a_id = '%1',"
                          "    company      = '%2',"
                          "    product      = '%3',"
                          "    brd_type     = '%4',"
                          "    cfg          = '%5',"
                          "    bom          = '%6',"
                          "    hwver        = '%7',"
                          "    modify_time  = now() "
                          "WHERE id = %8;")
            .arg(atsha204aIdStr).arg(companyStr).arg(productStr).arg(brdTypeStr)
            .arg(cfgStr).arg(bomStr).arg(hwverStr)
            .arg(brdId);
    QSqlQuery query;
    bool ok = query.exec(cmd);
    qDebug() << "";
    qDebug() << cmd;
    if(!ok)
    {
        QString msg = query.lastError().text();
        ShowMsgBoxInfo(msg);
        return;
    }
}

void MfgDb::InsertFireInfo(int usrId, const QString imagesHash)
{
    qDebug() << __FILE__ << __LINE__ << usrId << imagesHash;

    ChangeDb(mcName);
    
    /* 插入子板表 */
    QString cmd = QString("INSERT INTO brd (fire_time, modify_time) "
                          "VALUES (now(), now());");
    QSqlQuery query;
    bool ok = query.exec(cmd);
    if(!ok)
    {
        QString msg = query.lastError().text();
        ShowMsgBoxInfo(msg);
        return;
    }
    
    /* 获取子板ID */
    mBrdId = query.lastInsertId().toInt();
    
    /* 插入烧录信息表 */
    cmd = QString("INSERT INTO fire (brd, usr, time, image_hash) "
                  "VALUES (%1, '%2', now(), '%3');").arg(mBrdId).arg(usrId).arg(imagesHash);
    ok = query.exec(cmd);
    if(!ok)
    {
        QString msg = query.lastError().text();
        ShowMsgBoxInfo(msg);
        return;
    }
    
    return;
}

void MfgDb::InsertCalInfo(int brdId, int usrId, const QString &inst,
                          int cost, bool isFired, bool isPassed,
                          const QString &log, const QString &idn, const QString calHash)
{
    qDebug() << __FILE__ << __LINE__ << brdId << usrId << inst << cost << isFired << isPassed
             << log << idn << calHash << "not impl.";
}

int MfgDb::GetBrdIdFromAtsha204aId(const QString &atsha204aIdStr)
{
    ChangeDb(mcName);

    /* step1: 获取账户对应密码 */
    QString cmd = QString("SELECT id FROM brd WHERE atsha204a_id = '%1';").arg(atsha204aIdStr);
    QSqlQuery query;
    bool ok = query.exec(cmd);
    if(!ok)
    {
        QString msg = query.lastError().text();
        ShowMsgBoxInfo(msg);
        return PssRstMySqlCmdErr;
    }
    
    int size = query.size();
    if(1 != size)
    {
        QString msg = QString(QStringLiteral("%1 子板异常:%d.")).arg(atsha204aIdStr).arg(size);
        ShowMsgBoxInfo(msg);
        return PssRstMySqlUsrErr;
    }
    
    query.next();
    int brdId = query.value(0).toInt();
    return brdId;
}

void MfgDb::ChangeDb(const QString &name) const
{
    /* 选库 */
    QString cmd = QString("USE %1").arg(name);
    QSqlQuery query;
    bool ok = query.exec(cmd);
    if(!ok)
    {
        QString msg = QString("ChangeDb Failled: %1").arg(query.lastError().text());
        ShowMsgBoxInfo(msg);
    }
}

void MfgDb::InitDb(void)
{
    bool ok = false;
    QSqlQuery query;
#if 1
    /* 删库 */
    QString cmd = QString("DROP DATABASE %1;").arg(mcName);
    ok = query.exec(cmd);
    if(!ok)
    {
        QString msg = QString("%1 Failled: %2").arg(cmd).arg(query.lastError().text());
        ShowMsgBoxInfo(msg);
        return;
    }
#endif

    /* 建库 */
    cmd = QString("CREATE DATABASE %1;").arg(mcName);
    ok = query.exec(cmd);
    if(!ok)
    {
        QString msg = QString("%1 Failled: %2").arg(cmd).arg(query.lastError().text());
        ShowMsgBoxInfo(msg);
        return;
    }
    
    /* 选库 */
    ChangeDb(mcName);
    
    /* 建表: 权限表 */
    cmd = QString("CREATE TABLE permission ("
                  "id         INT UNSIGNED NOT NULL AUTO_INCREMENT,"
                  "name       VARCHAR(32),"
                  "permission INT UNSIGNED NOT NULL UNIQUE,"
                  "PRIMARY KEY(id)"
                  ")ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;");
    ok = query.exec(cmd);
    if(!ok)
    {
        QString msg = QString("%1 Failled: %2").arg(cmd).arg(query.lastError().text());
        ShowMsgBoxInfo(msg);
        return;
    }
    
    /* 建表: 用户表 */
    cmd = QString("CREATE TABLE usr ("
                  "id         INT UNSIGNED NOT NULL AUTO_INCREMENT,"
                  "permission INT UNSIGNED NOT NULL DEFAULT 1,"
                  "usr        VARCHAR(32) NOT NULL UNIQUE,"
                  "passwd     BLOB(32) NOT NULL,"
                  "full_name  VARCHAR(64) NOT NULL,"
                  "PRIMARY KEY(id),"
                  "CONSTRAINT permission FOREIGN KEY (permission) REFERENCES permission (id)"
                  ")ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;");
    ok = query.exec(cmd);
    if(!ok)
    {
        QString msg = QString("%1 Failled: %2").arg(cmd).arg(query.lastError().text());
        ShowMsgBoxInfo(msg);
        return;
    }
    
    /* 建表: 子板表 */
    cmd = QString("CREATE TABLE brd ("
                  "id           INT UNSIGNED NOT NULL AUTO_INCREMENT,"
                  "atsha204a_id TEXT(18),"
                  "company      BLOB(32),"
                  "product      BLOB(32),"
                  "brd_type     BLOB(8),"
                  "fire_time    DATETIME NOT NULL,"
                  "cfg          BLOB(32),"
                  "bom          BLOB(32),"
                  "hwver        BLOB(32),"
                  "modify_time  DATETIME NOT NULL,"
                  "PRIMARY KEY(id)"
                  ")ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;");
    ok = query.exec(cmd);
    if(!ok)
    {
        QString msg = QString("%1 Failled: %2").arg(cmd).arg(query.lastError().text());
        ShowMsgBoxInfo(msg);
        return;
    }
    
    /* 建表: 生产烧录表 */
    cmd = QString("CREATE TABLE fire ("
                  "id         INT UNSIGNED NOT NULL AUTO_INCREMENT,"
                  "brd        INT UNSIGNED NOT NULL,"
                  "usr        INT UNSIGNED NOT NULL,"
                  "time       DATETIME NOT NULL,"
                  "image_hash BLOB(32) NOT NULL,"
                  "PRIMARY KEY(id),"
                  "CONSTRAINT fire_brd FOREIGN KEY (brd) REFERENCES brd (id),"
                  "CONSTRAINT fire_usr FOREIGN KEY (usr) REFERENCES usr (id)"
                  ")ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;");
    ok = query.exec(cmd);
    if(!ok)
    {
        QString msg = QString("%1 Failled: %2").arg(cmd).arg(query.lastError().text());
        ShowMsgBoxInfo(msg);
        return;
    }
    
    /* 建表: 校准表 */
    cmd = QString("CREATE TABLE cal ("
                  "id         INT UNSIGNED NOT NULL AUTO_INCREMENT,"
                  "brd        INT UNSIGNED NOT NULL,"
                  "usr        INT UNSIGNED NOT NULL,"
                  "inst       TINYTEXT NOT NULL,"
                  "time       DATETIME NOT NULL,"
                  "cost       INT UNSIGNED NOT NULL,"
                  "fired      TINYINT UNSIGNED NOT NULL,"
                  "ok         TINYINT UNSIGNED NOT NULL,"
                  "log        TEXT NOT NULL,"
                  "idn        MEDIUMTEXT NOT NULL,"
                  "cal_hash   BLOB(32) NOT NULL,"
                  "PRIMARY KEY(id),"
                  "CONSTRAINT cal_brd FOREIGN KEY (brd) REFERENCES brd (id),"
                  "CONSTRAINT cal_usr FOREIGN KEY (usr) REFERENCES usr (id)"
                  ")ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;");
    ok = query.exec(cmd);
    if(!ok)
    {
        QString msg = QString("%1 Failled: %2").arg(cmd).arg(query.lastError().text());
        ShowMsgBoxInfo(msg);
        return;
    }
    
    /* 添加数据: 添加一个权限项目,root */
    cmd = QString("INSERT INTO permission (name, permission) "
                  "VALUES ('root', 0xffffffff);");
    ok = query.exec(cmd);
    if(!ok)
    {
        QString msg = QString("%1 Failled: %2").arg(cmd).arg(query.lastError().text());
        ShowMsgBoxInfo(msg);
        return;
    }
    
    /* 添加用户 */
    CreateNewUsr(QString("pp"),  QString("PengPeng"));
    CreateNewUsr(QString("xcy"), QString("XiongCaiYun"));
    CreateNewUsr(QString("ryl"), QString("RuanYuLong"));
    CreateNewUsr(QString("ll"),  QString("LiuLi"));
    CreateNewUsr(QString("db"),  QString("DaiBin"));
    CreateNewUsr(QString("lp"),  QString("LiPin"));
    
    qDebug() << "InitDb done.";
}

void MfgDb::TestDb(void)
{
    /* 测试 */
    QSqlQuery query;
    QString cmd = QString("USE mysql;");
    bool ok = query.exec(cmd);
    if(!ok)
    {
        QString msg = QString("%1 Failled: %2").arg(cmd).arg(query.lastError().text());
        ShowMsgBoxInfo(msg);
        return;
    }

    cmd = QString("SELECT user, host FROM user;");
    ok = query.exec(cmd);
    if(!ok)
    {
        QString msg = QString("%1 Failled: %2").arg(cmd).arg(query.lastError().text());
        ShowMsgBoxInfo(msg);
        return;
    }
    
    QString result;
    while (query.next())
    {
        QString name = query.value(0).toString();
        QString host = query.value(1).toString();
        result.append(QString("%1,%2\n").arg(name).arg(host));
    }
    ShowMsgBoxInfo(result);
}


QString MfgDb::Atsha204aIdBuf2QString(const Pss::PssBinAppCommMcuAtsha204aStatus &status)
{
    char buf[9] = {0};
    status.GetId(buf, 9);

    QString valStr;
    char strBuf[19] = {0};
    sprintf(strBuf, "%02X%02X%02X%02X%02X%02X%02X%02X%02X",
            (u8)buf[0], (u8)buf[1], (u8)buf[2],
            (u8)buf[3], (u8)buf[4], (u8)buf[5],
            (u8)buf[6], (u8)buf[7], (u8)buf[8]);
    valStr = QString(strBuf);
    return valStr;
}