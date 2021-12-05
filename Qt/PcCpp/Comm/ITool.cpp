/******************************************************************************
 *
 * 文件名  ： ITool.cpp
 * 创建日期： 20200223
 * 版本号  ： v1.0 20200223
 * 文件描述： C++实现的工具库
 * 其    他： 无
 * 修改日志： 无
 *
  *******************************************************************************/
 /*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "ITool.h"
#include <QDateTime>
#include <QDate>
#include <QTime>
#include <QDebug>
#include <QtDebug>
#include <QThread>
#include <QDateTime>
#include <QMutex>
#include <QFile>
#include <QTextStream>

#include <stdlib.h>


/*----------------------------------- 声明区 ----------------------------------*/

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/

/********************************** 变量实现区 *********************************/

/********************************** 函数实现区 *********************************/
IQStringAble::IQStringAble(void)
{
}

IQStringAble::~IQStringAble(void)
{
}

void DelayMs(int32_t ms)
{
    QThread::msleep(ms);
}

int32_t GetNowMs(void)
{
    int32_t ms = 0;
    ms = static_cast<int32_t>(QDateTime::currentMSecsSinceEpoch());
    return ms;
}

int GetRandomInRange(int begin, int end)
{
    if(begin == end)
    {
        return begin;
    }

    /* [0, end - begin) */
    int random = rand() % (end - begin);
    
    /* [begin, end) */
    return random + begin;
}

void DelayRadomMs(int min, int max)
{
    int delayMs = GetRandomInRange(min, max);
    DelayMs(delayMs);
}
