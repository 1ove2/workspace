/******************************************************************************
 *
 * 文件名  ： PssVersion.c
 * 负责人  ： 彭鹏(516190948@qq.com)
 * 创建日期： 20220228 
 * 版本号  ： v1.0
 * 文件描述： 版本相关功能
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/


/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "PssVersion.h"
#include <stdio.h>
#include <string.h>

#define  _S_GIT_STR_SIZE_                                       (128)

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/

/********************************** 变量实现区 *********************************/
static char sGitStr[_S_GIT_STR_SIZE_] = {0};

/********************************** 函数实现区 *********************************/
const char *PssGetGitHashStr(void)
{
    memset( sGitStr, 0, _S_GIT_STR_SIZE_);
    strncat(sGitStr, _APP_REPO_HASH_, 8);
    strncat(sGitStr, _SPLITER_, 2);
    strncat(sGitStr, _COMM_REPO_HASH_, 8);
    strncat(sGitStr, _SPLITER_, 2);
    strncat(sGitStr, _BLD_TIME_, 20);
    return sGitStr;
}

const char *PssGetGitTimeStr(void)
{
    memset( sGitStr, 0, _S_GIT_STR_SIZE_);
    strncat(sGitStr, _APP_REPO_TIME_, 20);
    strncat(sGitStr, _SPLITER_, 2);
    strncat(sGitStr, _COMM_REPO_TIME_, 20);
    strncat(sGitStr, _SPLITER_, 2);
    strncat(sGitStr, _BLD_TIME_, 20);
    return sGitStr;
}

const char *PssGetGitAllStr(void)
{
    memset( sGitStr, 0, _S_GIT_STR_SIZE_);
    strncat(sGitStr, _APP_REPO_HASH_, 8);
    strncat(sGitStr, _SPLITER_, 2);
    strncat(sGitStr, _APP_REPO_TIME_, 20);
    strncat(sGitStr, _SPLITER_, 2);
    strncat(sGitStr, _COMM_REPO_HASH_, 8);
    strncat(sGitStr, _SPLITER_, 2);
    strncat(sGitStr, _COMM_REPO_TIME_, 20);
    strncat(sGitStr, _SPLITER_, 2);
    strncat(sGitStr, _BLD_TIME_, 20);
    return sGitStr;
}

