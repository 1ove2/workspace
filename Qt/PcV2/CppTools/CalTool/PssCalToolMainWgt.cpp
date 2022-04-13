/******************************************************************************
 *
 * 文件名  ： PssCalToolMainWgt.cpp
 * 负责人  ： sls
 * 创建日期： 20220317
 * 版本号  ： v1.0 20220317
 * 文件描述： Pss 校准工具 主控件 实现
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/
#include <QFile>
#include <QTime>
#include <QThread>
#include <QDebug>
#include <QJsonArray>
#include <QFileDialog>
#include <QJsonObject>
#include <QMessageBox>
#include <QJsonDocument>

#include "PssCalToolMainWgt.h"
#include "ui_PssCalToolMainWgt.h"
#include "PssQtTools.h"
#include "PssDeviceFactory.h"

/*********************************** 命名空间 **********************************/

/*----------------------------------- 声明区 ----------------------------------*/

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */
/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/


/********************************** 变量实现区 *********************************/

/********************************** 函数实现区 *********************************/
PssCalToolMainWgt::PssCalToolMainWgt(QWidget *parent) :
    QWidget(parent),
    mUi(new Ui::PssCalToolMainWgt),
    mBaseCfgPath("")
{
    mUi->setupUi(this);

    /* 调试信息重定向 */
    PssQtToolsRedirect(mUi->txtLog);

    /* 配置文件存储基路径 */
    QString mBaseCfgPath = QCoreApplication::applicationDirPath() + "/Json";
}

PssCalToolMainWgt::~PssCalToolMainWgt(void)
{
    delete mUi;
}
