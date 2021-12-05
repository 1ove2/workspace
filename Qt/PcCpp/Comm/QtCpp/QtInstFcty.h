/******************************************************************************
 *
 * 文件名  ： QtInstFcty.h
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20200417
 * 版本号  ： v1.0 20200417
 * 文件描述： 仪表工厂类 用于创建实际仪表对象
 * 其    他： 无
 * 修改日志： 无
 *
  *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _INST_FCTY_H_
#define _INST_FCTY_H_

/************************************ 头文件 ***********************************/
#include "Sx00Instrument.h"
#include <QObject>
#include <QHostAddress>

/*********************************** 命名空间 **********************************/


/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */

/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/
/* 用于连接后判断实际的仪表类型 */
class QtEmpytInst : public Sx00Instrument
{
    Q_OBJECT
    public:
        /* 使用网口 */
        QtEmpytInst(const QHostAddress &ip, const QString &name, QObject *parent = 0);
        ~QtEmpytInst(void);

    private:
        virtual int UpdateCfgToInstrument(void) override;                                               /* 配置信息 设置 到仪表 */
        const static QString mcName;
        //const char *mcName;
        //const char *mcName = "EmpytInst";                                                             /* 占位 */
        Sx00ScpiClient* mScpiClient;

};

class QtInstFcty : public QObject
{
    Q_OBJECT
    public:
        QtInstFcty(QObject *parent = 0);
        ~QtInstFcty(void);

        QtIInstrument *CreateInstrument(const QHostAddress &ip);                                        /* 创建当前连接的仪表 */
    
    private:
};

/*********************************** 工具函数 **********************************/


#endif // _INST_FCTY_H_
