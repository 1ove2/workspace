#-------------------------------------------------
#
# Project created by QtCreator 2021-11-06T11:11:18
#
#-------------------------------------------------

QT       += core gui  sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DataBaseDemo
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS




SOURCES += \
        main.cpp \
        MainWindow.cpp \
    DataBase/IDataBase.cpp \
    DataBase/DBSQLite.cpp \
    DataBase/DBMySql.cpp \
    demowgt/LoginDlg.cpp

HEADERS += \
        MainWindow.h \
    DataBase/IDataBase.h \
    DataBase/DBSQLite.h \
    DataBase/DBMySql.h \
    demowgt/LoginDlg.h

FORMS += \
        MainWindow.ui \
    demowgt/LoginDlg.ui
