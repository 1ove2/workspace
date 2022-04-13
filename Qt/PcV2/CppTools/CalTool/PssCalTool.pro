!build_pass:system("python $$PWD/../../Comm/Scripts/GenVerHeadFile.py $$PWD/../../ $$PWD")
QT      += core gui widgets uitools network serialport

TARGET = PssCalTool
TEMPLATE = app

CONFIG  += c++11
DEFINES += _PSS_WIN_
DEFINES += _PSS_QT_
#DEFINES += QT_DEPRECATED_WARNINGS
#DEFINES += QT_NO_WARNING_OUTPUT
#DEFINES += QT_NO_DEBUG_OUTPUT

# winsock2
LIBS    += -lws2_32

SOURCES += \
    ../../Comm/CLib/PssCLibEndian.c \
    ../../Comm/CLib/PssCLibLog.c \
    ../../Comm/CLib/PssVersion.c \
    ../../Comm/CppLib/PssCppLog.cpp \
    ../../Comm/CppLib/PssIIo.cpp \
    ../../Comm/CppLib/PssWinTcpClient.cpp \
    ../../Comm/CppLib/PssWinUdp.cpp \
    ../../Comm/QtLib/PssQtTools.cpp \
    ../../CppLibs/Pss2400.cpp \
    ../../CppLibs/Pss2611B.cpp \
    ../../CppLibs/PssCx00.cpp \
    ../../CppLibs/PssDeviceFactory.cpp \
    ../../CppLibs/PssIDevice.cpp \
    ../../CppLibs/PssPssDevice.cpp \
    ../../CppLibs/PssPx00.cpp \
    ../../CppLibs/PssStdDevice.cpp \
    ../../CppLibs/PssSx00.cpp \
    ../Lib/PssMainWnd.cpp \
    main.cpp \
    PssCalToolMainWgt.cpp

HEADERS += \
    ../../Comm/CLib/PssCLibEndian.h \
    ../../Comm/CLib/PssCLibLog.h \
    ../../Comm/CppLib/PssCppLog.h \
    ../../Comm/CppLib/PssIIo.h \
    ../../Comm/CppLib/PssWinTcpClient.h \
    ../../Comm/CppLib/PssWinUdp.h \
    ../../Comm/PssTypeDef.h \
    ../../Comm/QtLib/PssQtTools.h \
    ../../CppLibs/Pss2400.h \
    ../../CppLibs/Pss2611B.h \
    ../../CppLibs/PssCx00.h \
    ../../CppLibs/PssDeviceFactory.h \
    ../../CppLibs/PssIDevice.h \
    ../../CppLibs/PssPssDevice.h \
    ../../CppLibs/PssPx00.h \
    ../../CppLibs/PssStdDevice.h \
    ../../CppLibs/PssSx00.h \
    ../Lib/PssMainWnd.h \
    PssCalToolMainWgt.h

INCLUDEPATH += \
    ../Lib/ \
    ../../CppLibs/ \
    ../../Comm/ \
    ../../Comm/QtLib/ \
    ../../Comm/CLib/ \
    ../../Comm/CppLib/ \
    ../../Comm/QtCpp/

FORMS += \
    ../Lib/PssMainWnd.ui \
    PssCalToolMainWgt.ui

RESOURCES += \
    image.qrc
