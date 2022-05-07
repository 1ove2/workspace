!build_pass:system("python $$PWD/../Comm/Scripts/GenVerHeadFile.py $$PWD/../ $$PWD")
!build_pass:system("python $$PWD/../Comm/Scripts/GenDllHFile.py $$PWD/Output/ $$PWD/../Comm/ $$PWD/../Lib/ $$PWD/PssDll.h")

CONFIG   -= app_bundle
CONFIG   -= qt

TARGET    = PssDll

TEMPLATE  = lib
CONFIG   += c++11
DEFINES  += _PSS_WIN_
#DEFINES += QT_DEPRECATED_WARNINGS
#DEFINES += QT_NO_WARNING_OUTPUT
#DEFINES += QT_NO_DEBUG_OUTPUT

# winsock2
LIBS    += -lws2_32

DESTDIR += $$PWD/Output/

SOURCES += \
    ../Comm/CLib/PssCLibLog.c \
    ../Comm/CLib/PssVersion.c \
    ../Comm/CppLib/PssCppLog.cpp \
    ../Comm/CppLib/PssIIo.cpp \
    ../Comm/CppLib/PssWinSerialPort.cpp \
    ../Comm/CppLib/PssWinTcpClient.cpp \
    ../CppLibs/Pss2400.cpp \
    ../CppLibs/Pss2611B.cpp \
    ../CppLibs/PssCx00.cpp \
    ../CppLibs/PssDeviceFactory.cpp \
    ../CppLibs/PssIDevice.cpp \
    ../CppLibs/PssPssDevice.cpp \
    ../CppLibs/PssPx00.cpp \
    ../CppLibs/PssStdDevice.cpp \
    ../CppLibs/PssSx00.cpp \
    PssDll.cpp \

HEADERS += \
    ../Comm/CLib/PssCLibLog.h \
    ../Comm/CppLib/PssCppLog.h \
    ../Comm/CppLib/PssIIo.h \
    ../Comm/CppLib/PssWinSerialPort.h \
    ../Comm/CppLib/PssWinTcpClient.h \
    ../Comm/PssRstDef.h \
    ../Comm/PssTypeDef.h \
    ../CppLibs/Pss2400.h \
    ../CppLibs/Pss2611B.h \
    ../CppLibs/PssCx00.h \
    ../CppLibs/PssDeviceFactory.h \
    ../CppLibs/PssIDevice.h \
    ../CppLibs/PssPssDevice.h \
    ../CppLibs/PssPx00.h \
    ../CppLibs/PssStdDevice.h \
    ../CppLibs/PssSx00.h \
    PssDll.h

INCLUDEPATH += \
    ../Comm/ \
    ../Comm/CLib/ \
    ../Comm/CppLib/ \
    ../CppLibs/
