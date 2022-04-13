/******************************************************************************
 *
 * 文件名  ： PssMainWnd.cpp
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20220106
 * 版本号  ： v1.0 20220106
 * 文件描述： Pss 主窗口 实现
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/
#include "PssMainWnd.h"
#include "ui_PssMainWnd.h"
#include "PssQtTools.h"

#include <QTimer>
#include <QLabel>
#include <QDateTime>

/*********************************** 命名空间 **********************************/

/*----------------------------------- 声明区 ----------------------------------*/

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */
/********************************** 变量声明区 *********************************/


/********************************** 函数声明区 *********************************/


/********************************** 变量实现区 *********************************/

/********************************** 函数实现区 *********************************/
PssMainWnd::PssMainWnd(QMainWindow *parent) :
    QMainWindow(parent),
    mUi(new Ui::PssMainWnd),
    mVerLabel(new QLabel),
    mTmrLabel(new QLabel),
    mTmr(new QTimer)
{
    mUi->setupUi(this);

    /* 状态栏 */
    QStatusBar *statusBar = this->statusBar();
    statusBar->addWidget(mVerLabel);
    statusBar->addPermanentWidget(mTmrLabel);

    /* 版本信息 */
    QString verStr = PssQtToolsGetRepoTime();
    mVerLabel->setText(verStr);

    SltTmrUpdate();
    connect(mTmr, &QTimer::timeout, this, &PssMainWnd::SltTmrUpdate);
    mTmr->start(1000); /* 1s */
}

PssMainWnd::~PssMainWnd(void)
{
    disconnect(mTmr, &QTimer::timeout, this, &PssMainWnd::SltTmrUpdate);
    delete mTmr;
    delete mTmrLabel;
    delete mVerLabel;
    delete mUi;
}

void PssMainWnd::SltTmrUpdate(void)
{
    QDateTime now = QDateTime::currentDateTime();
    QString nowStr = now.toString("yyyy-MM-dd hh:mm:ss");
    mTmrLabel->setText(nowStr);

    update();
}

