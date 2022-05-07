!build_pass:system("python $$PWD/../Comm/Scripts/GenVerHeadFile.py $$PWD/../ $$PWD")

TARGET    = PssDllTester
TEMPLATE  = app
CONFIG   += console
CONFIG   += c++11
CONFIG   -= app_bundle
CONFIG   -= qt

DEFINES  += _PSS_WIN_
#DEFINES += QT_DEPRECATED_WARNINGS
#DEFINES += QT_NO_WARNING_OUTPUT
#DEFINES += QT_NO_DEBUG_OUTPUT
# winsock2
LIBS     += -lws2_32
LIBS     += -L$$PWD/../../CppDlls/Output/ -lPssDll

SOURCES += \
        ../../Comm/CLib/PssCLibLog.c \
        ../../Comm/CppLib/PssCppLog.cpp \
        main.cpp

HEADERS += \
    ../../Comm/CLib/PssCLibLog.h \
    ../../Comm/CppLib/PssCppLog.h \
    ../../CppDlls/Output/PssDll.h \
    ../../CppDlls/Output/PssRstDef.h \
    ../../CppDlls/Output/PssTypeDef.h

INCLUDEPATH += \
    ../../CppDlls/Output/ \
    ../../Comm/CLib/ \
    ../../Comm/CppLib/

DEPENDPATH += $$PWD/../../CppDlls/Output
