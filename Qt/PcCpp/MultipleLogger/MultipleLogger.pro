#-------------------------------------------------
#
# Project created by QtCreator 2021-04-25T13:18:45
#
#-------------------------------------------------
!build_pass:system("python $$PWD/../../Comm/Scripts/pss/genVersion.py $$PWD/.. $$PWD")

QT       += core gui printsupport network serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include(../Comm/PcComm/Pc3party/QtXlsxWriter/src/xlsx/qtxlsx.pri)

TARGET = MultipleLogger
TEMPLATE = app

###RC_ICONS += res/Wave.ico

CONFIG += c++11

SOURCES += main.cpp \
        src/LegendWgt.cpp \
        src/ConfigDlg.cpp \
        src/InstManager.cpp \
        src/CustomLogPlot.cpp \
        src/CustomLogTable.cpp \
        src/ParaWgt.cpp \
        src/ScpiWorker.cpp \
        src/MainWindow.cpp \
        src/ConnectDlg.cpp \
    CSx00ReaderWorker.cpp \
    Sx00ReaderWorker.cpp \
    UpgradeDlg.cpp

HEADERS  += \
        inc/LegendWgt.h \
        inc/ConfigDlg.h \
        inc/InstManager.h \
        inc/CustomLogPlot.h \
        inc/CustomLogTable.h \
        inc/ParaWgt.h \
        inc/ScpiWorker.h \
        inc/MainWindow.h \
        inc/ConnectDlg.h \
    ParaDef.h \
    CSx00ReaderWorker.h \
    Sx00ReaderWorker.h \
    UpgradeDlg.h

FORMS    += \
        ui/LegendWgt.ui \
        ui/ConfigDlg.ui \
        ui/ParaWgt.ui \
        ui/MainWindow.ui \
        ui/ConnectDlg.ui \
    UpgradeDlg.ui

INCLUDEPATH += ui/ \
               inc/ \
               src/
#lib
SOURCES += $$PWD/../Comm/PcComm/PssCustomControl/PssCustomPlot/QCustomPlot.cpp \
           $$PWD/../PcComm/Utils/PssConfig.cpp \
           $$PWD/../PcComm/Utils/PssUtil.cpp \
           $$PWD/../PcComm/PssCustomControl/PssEditableComboBox/PssCustomItemWidget.cpp \
           $$PWD/../PcComm/PssCustomControl/PssEditableComboBox/PssEditableComboBox.cpp \
           $$PWD/../Comm/QtCpp/GPIBIo.cpp \
           $$PWD/../Comm/QtCpp/QtCom.cpp \
           $$PWD/../Comm/QtCpp/QtIo.cpp \
           $$PWD/../Comm/QtCpp/QtTcpClient.cpp \
           $$PWD/../Comm/QtCpp/QtIInstrument.cpp \
           $$PWD/../Comm/QtCpp/Sx00Instrument.cpp \
           $$PWD/../Comm/QtCpp/CSx00Instrument.cpp \
           $$PWD/../Comm/QtCpp/PssQtHal.cpp \
           $$PWD/../Comm/QtCpp/Scpi/ScpiClient/ScpiClient.cpp \
           $$PWD/../Comm/QtCpp/Scpi/ScpiClient/Sx00/Sx00ScpiClient.cpp \
           $$PWD/../Comm/QtCpp/Scpi/ScpiClient/CSx00/CSx00ScpiClient.cpp \
           $$PWD/../Comm/IIo.cpp \
           $$PWD/../Comm/Bytes.cpp \
           $$PWD/../Comm/ITool.cpp \
           $$PWD/../Comm/CLib/PssCLibCrc32.c \
           $$PWD/../Comm/CLib/PssCLibEndian.c \
           $$PWD/../Comm/CLib/PssCLibLeastSquare.c \
           $$PWD/../Comm/CLib/PssCLibSha256.c

HEADERS += $$PWD/../PcComm/PssCustomControl/PssCustomPlot/QCustomPlot.h \
           $$PWD/../PcComm/Utils/PssConfig.h \
           $$PWD/../PcComm/Utils/PssSingleton.h \
           $$PWD/../PcComm/Utils/PssUtil.h \
           $$PWD/../Comm/PcComm/Lib/ScanPara.h \
           $$PWD/../Comm/PcComm/Lib/PssErrorDef.h \
           $$PWD/../Comm/PcComm/PssCustomControl/PssEditableComboBox/PssCustomItemWidget.h \
           $$PWD/../Comm/PcComm/PssCustomControl/PssEditableComboBox/PssEditableComboBox.h \
           $$PWD/../Comm/QtCpp/GPIBIo.h \
           $$PWD/../Comm/QtCpp//QtCom.h \
           $$PWD/../Comm/QtCpp/QtIo.h \
           $$PWD/../Comm/QtCpp/QtTcpClient.h \
           $$PWD/../Comm/QtCpp/QtIInstrument.h \
           $$PWD/../Comm/QtCpp/Sx00Instrument.h \
           $$PWD/../Comm/QtCpp/CSx00Instrument.h \
           $$PWD/../Comm/QtCpp/PssQtHal.h \
           $$PWD/../Comm/QtCpp/Scpi/ScpiClient/ScpiClient.h \
           $$PWD/../Comm/QtCpp/Scpi/ScpiClient/Sx00/Sx00ScpiClient.h \
           $$PWD/../Comm/QtCpp/Scpi/ScpiClient/CSx00/CSx00ScpiClient.h \
           $$PWD/../Comm/IIo.h \
           $$PWD/../Comm/Bytes.h \
           $$PWD/../Comm/ITool.h \

           $$PWD/../Comm/CLib/PssCLibEndian.h \
           $$PWD/../Comm/CLib/PssCLibLeastSquare.h \
           $$PWD/../Comm/CLib/PssCLibSha256.h \
           $$PWD/../Comm/RstDef.h \
           $$PWD/../Comm/TypeDef.h

INCLUDEPATH += $$PWD/../Comm/PcComm/PssCustomControl/PssCustomPlot/ \
               $$PWD/../Comm/PcComm/Utils/ \
               $$PWD/../Comm/PcComm/Lib/ \
               $$PWD/../Comm/PcComm/PssCustomControl/PssEditableComboBox/ \
               $$PWD/../Comm/ \
               $$PWD/../Comm/CLib/ \
               $$PWD/../Comm/QtCpp/ \
               $$PWD/../Comm/QtCpp/Scpi/ScpiClient/ \
               $$PWD/../Comm/QtCpp/Scpi/ScpiClient/Sx00/ \
               $$PWD/../Comm/QtCpp/Scpi/ScpiClient/CSx00/

RESOURCES += \
    res/res.qrc
