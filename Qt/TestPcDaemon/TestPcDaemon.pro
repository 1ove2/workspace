#-------------------------------------------------
#
# Project created by QtCreator 2021-09-23T15:16:52
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestPcDaemon
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
        TestPcDaemon.cpp \
    TestTcpServer.cpp

HEADERS  += TestPcDaemon.h \
    TestTcpServer.h \
#    ../../local/PssNew/LinuxUi/Ui/Common/Cfg/ProcessShareMemory.h \

#INCLUDEPATH += ../../local/PssNew/LinuxUi/Ui/Common/Cfg/ \

FORMS    += TestPcDaemon.ui
