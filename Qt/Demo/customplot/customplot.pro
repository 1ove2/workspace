#-------------------------------------------------
#
# Project created by QtCreator 2021-10-25T16:39:36
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = customplot
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS



SOURCES += \
        main.cpp \
        Customplot.cpp \
        ../../3Party/QCustomPlot/qcustomplot.cpp \
        Curve.cpp

HEADERS += \
        Customplot.h \
        ../../3Party/QCustomPlot/qcustomplot.h \
        Curve.h

INCLUDEPATH += ../../3Party/QCustomPlot/
FORMS += \
        Customplot.ui
