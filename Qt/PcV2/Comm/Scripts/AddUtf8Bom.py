#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys
import chardet

def ConvertFile(f):
    rf = open(f, 'rb')
    data = rf.read()
    rf.close()

    fileType = chardet.detect(data)['encoding']
    if 'UTF-8-SIG' != fileType:  #不是 带BOM的UTF8 转为 带BOM的UTF8
        """
        转换为带BOM的UTF8字节流
        """
        print("%s => utf8-sig:%s" % (fileType, f))
        #print(fileType)
        strData = data.decode(encoding = fileType, errors = 'strict')
        newData = bytes(strData, encoding = 'UTF-8-SIG')
        wf = open(f, 'wb')
        wf.write(newData)
        wf.close()

def WalkDir(dirStr):
    srcFileList = []

    # 获取 cpp/c/h 文件
    for root, dirs, files in os.walk(rootDir):
        for path in files:

            fullFileName = os.path.join(root, path)

            dirName, baseName = os.path.split(fullFileName)
            name, ext = os.path.splitext(baseName)

            """
            滤出c/cpp/h文件
            """
            if '.c' == ext or '.h' == ext or '.cpp' == ext:
                srcFileList.append(fullFileName)
                #print(fullFileName)

    # 处理源文件
    for f in srcFileList:
        ConvertFile(f)

    return None

if __name__ == "__main__":
    #print(sys.argv)
    rootDir  = sys.argv[1]
    rootDir  = os.path.abspath(rootDir)
    #print("根目录:    %s" % rootDir)

    #ConvertFile(rootDir)

    WalkDir(rootDir)

