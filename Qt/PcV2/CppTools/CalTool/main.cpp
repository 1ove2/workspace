/******************************************************************************
 *
 * 文件名  ： main.cpp
 * 负责人  ： sls
 * 创建日期： 20220106
 * 版本号  ： v1.0 20220106
 * 文件描述： Pss 校准软件主程序,用于校准我司产品
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "PssCppLog.h"
#include "PssMainWnd.h"
#include "PssCalToolMainWgt.h"

#include <QApplication>

/*********************************** 命名空间 **********************************/

/*----------------------------------- 声明区 ----------------------------------*/

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */

/********************************** 变量声明区 *********************************/


/********************************** 函数声明区 *********************************/


/********************************** 变量实现区 *********************************/

/********************************** 函数实现区 *********************************/
int main(int argc, char *argv[])
{
    /* 调试打印 */
    PssCppLog::SetStdout(true);

    QApplication a(argc, argv);

    PssMainWnd        wnd;
    PssCalToolMainWgt wgt;
    wnd.setCentralWidget(&wgt);
    wnd.show();

    return a.exec();
}
