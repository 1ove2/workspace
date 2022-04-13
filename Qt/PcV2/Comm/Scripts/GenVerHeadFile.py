#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import os
import sys
import time

def GetGitContent(dirStr, contentStr):
    # 判断工作树是否clean
    cmd = "cd %s && git st" % dirStr
    p = os.popen(cmd)
    data = p.read()
    data = data.strip('\n')
    if len(data) > 0:
        return 'dirty work tree'

    # 获取 git hash
    cmdHead ="cd %s && " % dirStr
    cmdHead +="git log --pretty="
    cmdTail =" -1"
    cmdStr = cmdHead + contentStr + cmdTail

    #print(cmdStr)

    p = os.popen(cmdStr)
    data = p.read()
    data = data.strip('\n')
    return data

if __name__ == "__main__":
    print(sys.argv)
    fileName = sys.argv[0]
    rootDir  = sys.argv[1]
    commDir  = os.path.join(rootDir, 'Comm')
    destDir  = sys.argv[2]

    appHash  = GetGitContent(rootDir, '%H')
    appTime  = GetGitContent(rootDir, '%ci')
    commHash = GetGitContent(commDir, '%H')
    commTime = GetGitContent(commDir, '%ci')

    # 本机当前时间
    nowStr = time.strftime("%Y/%m/%d %H:%M:%S", time.localtime(time.time()))

    hFileFmt = r"""#ifdef __cplusplus
extern "C"
{
#endif

#ifndef _GIT_REPO_VERSION_H_
#define _GIT_REPO_VERSION_H_

#define _APP_REPO_HASH_         ("%s")
#define _APP_REPO_TIME_         ("%s")
#define _COMM_REPO_HASH_        ("%s")
#define _COMM_REPO_TIME_        ("%s")
#define _BLD_TIME_              ("%s")
#define _SPLITER_               (",")

const char *PssGetGitHashStr(void);
const char *PssGetGitTimeStr(void);
const char *PssGetGitAllStr(void);

#endif // #ifndef _GIT_REPO_VERSION_H_

#ifdef __cplusplus
}
#endif
"""

    print("APP  REPO HASH: %s" % appHash)
    print("APP  REPO TIME: %s" % appTime)
    print("COMM REPO HASH: %s" % commHash)
    print("COMM REPO TIME: %s" % commTime)
    print("BUILD TIME:     %s" % nowStr)

    content = (appHash, appTime, commHash, commTime, nowStr)

    hFileContent = hFileFmt % content

    # 写入文件
    fpDir = os.path.join(destDir, 'PssVersion.h')
    fp = open(fpDir, 'w')
    fp.write(hFileContent)
    fp.close()

    print("%s已生成" % fpDir)

