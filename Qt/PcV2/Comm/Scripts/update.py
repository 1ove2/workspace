#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import os
import sys
import time

gGitRepoPrefix  = 'mcu@192.168.12.101:/mnt/raid/local/'
gFPGAFsRepoPath = os.path.join(gGitRepoPrefix, 'FPGAFs.git')
gFPGAFsDirName  = 'FPGAFs'

def RunCmd(cmd):
    print(cmd)

    p = os.popen(cmd)
    data = p.read()
    data = data.strip('\n')

    print(data)
    return data

if __name__ == "__main__":
    print(sys.argv)

    # step1: McuV2
    cwd = os.getcwd()
    cmdStr ="cd %s && git pull" % cwd
    RunCmd(cmdStr)

    # step2: FPGAFs
    FPGAFsDir = os.path.join(cwd, 'FPGAFs')
    if os.path.exists(FPGAFsDir):
        cmdStr ="cd %s && git pull" % gFPGAFsDirName
    else:
        cmdStr ="cd %s && git clone %s %s" % (cwd, gFPGAFsRepoPath, gFPGAFsDirName)
    RunCmd(cmdStr)

