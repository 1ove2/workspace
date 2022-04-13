#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import os
import sys

def CopyFile(dst, src):
    print("%s ===> %s" % (src, dst))

    fw = open(dst, 'w', encoding = 'utf-8')
    fr = open(src, 'r', encoding = 'utf-8')
    content = fr.read()
    fw.write(content)
    fr.close()
    fw.close()

def GetAllHFileOfDir(hFileDir):
    files = os.listdir(hFileDir)
    hFiles = []
    for f in files:
        if f.endswith('.h'):
            hFiles.append(f)
    return hFiles

def GenCommHFile(outDir, commDir):
    # 获取comm顶级目录中所有.h文件
    hFiles = GetAllHFileOfDir(commDir)
    for f in hFiles:
        srcFullName = os.path.join(commDir, f)
        dstFullName = os.path.join(outDir, f)
        CopyFile(dstFullName, srcFullName)

def GenAllExportHFile(outDir, dllLibDir, prjHFile):
    # 获取公共头文件
    #allHFiles = GetAllHFileOfDir(dllLibDir)
    allHFiles = []
    allFullNameHFiles = []
    for f in allHFiles:
        fullName = os.path.join(dllLibDir, f)
        allFullNameHFiles.append(fullName)
    allFullNameHFiles.append(prjHFile)

    # 处理头文件
    for f in allFullNameHFiles:
        GenAExportHFile(outDir, f)

def GenAExportHFile(outDir, prjHFile):
    srcNameList = os.path.split(prjHFile)
    srcFileName = srcNameList[1]

    src = prjHFile
    dst = os.path.join(outDir, srcFileName)
    print("%s ===> %s" % (src, dst))

    fw = open(dst, 'w', encoding = 'utf-8')
    fr = open(src, 'r', encoding = 'utf-8')
    # TODO: 字符替换
    for oldLine in fr.readlines():
        newLine = oldLine.replace('__declspec(dllexport)', '__declspec(dllimport)')
        fw.write(newLine)
    fr.close()
    fw.close()

if __name__=='__main__':
    print(sys)
    cmd      = sys.argv[0]
    outDir   = sys.argv[1]
    commDir  = sys.argv[2]
    dllLibDir= sys.argv[3]
    prjHFile = sys.argv[4]

    GenCommHFile(outDir, commDir)
    GenAllExportHFile(outDir, dllLibDir, prjHFile)

