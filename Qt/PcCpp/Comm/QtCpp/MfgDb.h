/******************************************************************************
 *
 * 文件名  ： MfgDb.h
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20200822
 * 版本号  ： v1.0 20200822
 * 文件描述： PSS Qt 版本MfgDb数据库
 * 其    他： 无
 * 修改日志： v1.0 20200822 初始版本
 *
  *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _PSS_QT_MFG_DB_H_
#define _PSS_QT_MFG_DB_H_

/************************************ 头文件 ***********************************/
#include <QObject>
#include <QString>
#include <QSqlDatabase>

/*********************************** 命名空间 **********************************/
namespace Pss {
class PssBinAppCommMcuAtsha204aStatus;

namespace Qt {

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */

/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/
class MfgDb : public QObject
{
    Q_OBJECT

    public:
        MfgDb(void);
        ~MfgDb(void);
    
        static QString Atsha204aIdBuf2QString(const PssBinAppCommMcuAtsha204aStatus &status);           /* 获取atsha204a ID字符串 */


        void CreateNewUsr(const QString &usr, const QString &fullName);                                 /* 新建用户 */
        int GetUsrId(const QString &usr, const QString &passwd);                                        /* 返回合法用户的ID */
        QString GetUsrFullName(int usrId);                                                              /* 返回用户的全名 */
    
        void UpdateBrd(const PssBinAppCommMcuAtsha204aStatus &status, bool isNewBrd = false);           /* 更新子板 */
        void InsertFireInfo(int usrId, const QString imagesHash);                                       /* 更新子板烧录信息 */
        void InsertCalInfo(int brdId, int usrId, const QString &inst,
                           int cost, bool isFired, bool isPassed, const QString &log,
                           const QString &idn, const QString calHash );                                 /* 更新子板校准信息 */
    
    private:
        int  GetBrdIdFromAtsha204aId(const QString &atsha204aIdStr);                                    /* 从atsha20a的id获取子板号 */
        void ChangeDb(const QString &name) const;                                                       /* 选库 */
        void InitDb(void);                                                                              /* 建库 */
        void TestDb(void);                                                                              /* 测库 */
    
        const static QString mcType;                                                                    /* 数据库 类型 */
        const static QString mcIp;                                                                      /* 数据库 IP */
        const static int     mcPort;                                                                    /* 数据库 端口 */
        const static QString mcName;                                                                    /* 数据库 名字 */
        const static QString mcUsr;                                                                     /* 数据库 账户 */
        const static QString mcPasswd;                                                                  /* 数据库 密码 */
    
        const static QString mcDefaultPasswdSuffix;                                                     /* 用户默认密码:usr+suffix */
    
        QSqlDatabase         mDb;                                                                       /* 数据库 */
        int                  mBrdId;                                                                    /* 子板id */
};
/*********************************** 工具函数 **********************************/

}
}

#endif // _PSS_QT_MFG_DB_H_