#-------------------------------------------------
#
# Project created by QtCreator 2021-11-06T11:11:18
#
#-------------------------------------------------

QT       += core gui  sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DataBaseDemo
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS




SOURCES += \
        main.cpp \
        MainWindow.cpp \
        demowgt/LoginDlg.cpp \
        ../../Lib/DataBase/DBMySql.cpp \
        ../../Lib/DataBase/DBSQLite.cpp \
        ../../Lib/DataBase/IDataBase.cpp \
        ../../Lib/Utils/Config.cpp \
        ../../Lib/Utils/QtUtil.cpp \
    demowgt/UserRegisterWgt.cpp \
    demowgt/ConfigWgt.cpp \
    demowgt/ConnectDlg.cpp \
    ../../Lib/CustomControl/EditableComboBox/EditableComboBox.cpp \
    ../../Lib/CustomControl/EditableComboBox/CustomItemWgt.cpp \
    demowgt/DataQueryWgt.cpp \
    demowgt/DataAnalysis.cpp \
    ../../3Party/QCustomPlot/Curve.cpp \
    ../../3Party/QCustomPlot/qcustomplot.cpp \
    demowgt/ChangePwdWgt.cpp \
    demowgt/ChanCfgDlg.cpp \
    demowgt/ChanWgt.cpp \
    demowgt/ConsCfgWgt.cpp \
    demowgt/DBCfgWgt.cpp \
    demowgt/DevCfgWgt.cpp \
    demowgt/RealTimeDataWgt.cpp \
    demowgt/UserManageWgt.cpp \
    demowgt/TestCfgWgt.cpp

HEADERS += \
        MainWindow.h \
        demowgt/LoginDlg.h \
        ../../Lib/DataBase/DBMySql.h \
        ../../Lib/DataBase/DBSQLite.h \
        ../../Lib/DataBase/IDataBase.h \
        ../../Lib/Utils/Config.h \
        ../../Lib/Utils/QtUtil.h \
        ../../Lib/Utils/singleton.h \
    demowgt/UserRegisterWgt.h \
    demowgt/ConfigWgt.h \
    demowgt/ConnectDlg.h \
    ../../Lib/CustomControl/EditableComboBox/EditableComboBox.h \
    ../../Lib/CustomControl/EditableComboBox/CustomItemWgt.h \
    demowgt/DataQueryWgt.h \
    demowgt/DataAnalysis.h \
    ../../3Party/QCustomPlot/Curve.h \
    ../../3Party/QCustomPlot/qcustomplot.h \
    demowgt/ChangePwdWgt.h \
    demowgt/ChanCfgDlg.h \
    demowgt/ChanWgt.h \
    demowgt/ConsCfgWgt.h \
    demowgt/DBCfgWgt.h \
    demowgt/DevCfgWgt.h \
    demowgt/RealTimeDataWgt.h \
    demowgt/UserManageWgt.h \
    demowgt/TestCfgWgt.h

FORMS += \
        MainWindow.ui \
        demowgt/LoginDlg.ui \
    demowgt/UserRegisterWgt.ui \
    demowgt/ConfigWgt.ui \
    demowgt/ConnectDlg.ui \
    demowgt/DataQueryWgt.ui \
    demowgt/DataAnalysis.ui \
    demowgt/ChangePwdWgt.ui \
    demowgt/ChanCfgDlg.ui \
    demowgt/ChanWgt.ui \
    demowgt/ConsCfgWgt.ui \
    demowgt/DBCfgWgt.ui \
    demowgt/DevCfgWgt.ui \
    demowgt/RealTimeDataWgt.ui \
    demowgt/UserManageWgt.ui \
    demowgt/TestCfgWgt.ui

INCLUDEPATH += demowgt \
            += ../../Lib/Utils \
            += ../../Lib/DataBase \
            += ../../Lib/CustomControl/EditableComboBox \
            += ../../3Party/QCustomPlot \
