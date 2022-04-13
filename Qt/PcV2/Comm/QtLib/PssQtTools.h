/******************************************************************************
 *
 * 文件名  ： PssQtTools.h
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20220302
 * 版本号  ： v1.0 20220302
 * 文件描述： Pss 基于Qt的工具库 头文件
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _PSS_QT_TOOLS_H_
#define _PSS_QT_TOOLS_H_

/************************************ 头文件 ***********************************/
#include "PssTypeDef.h"

#include <QString>
#include <QHostAddress>
#include <QPlainTextEdit>

/*********************************** 命名空间 **********************************/

/*********************************** 注释定位 **********************************/
                                                                                                        /* 注释定位 */
/*********************************** 枚举定义 **********************************/

/********************************** 函数声明区 *********************************/
QString      PssQtToolsGetRepoTime(void);                                                                /* 获取git版本和编译时间 */
void         PssQtToolsRedirect(QPlainTextEdit *plainTextEdit);                                         /* qDebug等重定向 */
QHostAddress PssQtToolsGetLocalIp(void);                                                                /* 获取本机IP */
QHostAddress PssQtToolsGetLocalBroadcastIp(void);                                                       /* 获取本机广播地址 */
bool         PssQtToolsIsValidIp(const QString &ipStr);                                                 /* 字符串IP合法行检查 */
i32          PssQtToolsQString2CStr(char *cStr, i32 cStrSize, const QString &qString);                  /* QString转C字符串 */
i32          PssQtToolsCStr2QString(QString &qString, char *cStr, i32 cStrSize);                        /* C字符串转QString */

/************************************ 类定义 ***********************************/

#endif // _PSS_QT_TOOLS_H_

