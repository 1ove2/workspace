/******************************************************************************
 *
 * 文件名  ： PssCalToolMainWgt.h
 * 负责人  ： sls
 * 创建日期： 20220317
 * 版本号  ： v1.0 20220317
 * 文件描述： Pss 校准工具 主控件 头文件
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _PSS_CAL_TOOL_MAIN_WGT_H_
#define _PSS_CAL_TOOL_MAIN_WGT_H_

/************************************ 头文件 ***********************************/
#include <QWidget>

/*********************************** 命名空间 **********************************/
namespace Ui {
class PssCalToolMainWgt;
}
/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */
/*********************************** 枚举定义 **********************************/

/************************************ 类定义 ***********************************/
class PssStdDevice;
class PssPssDevice;
class PssCalToolMainWgt : public QWidget
{
    Q_OBJECT

public:
    explicit PssCalToolMainWgt(QWidget *parent = 0);                                                    /* 构造 */
    ~PssCalToolMainWgt(void);                                                                           /* 析构 */

private slots:

private:

private:
    Ui::PssCalToolMainWgt   *mUi;                                                                       /* UI */
    PssPssDevice            *mPssDevice;                                                                /* Pss设备 */
    PssStdDevice            *mStdDevice;                                                                /* 标准设备 */
    QString                  mBaseCfgPath;                                                              /* 配置文件基路径 */
};
/*********************************** 工具函数 **********************************/

#endif // _PSS_CAL_TOOL_MAIN_WGT_H_
