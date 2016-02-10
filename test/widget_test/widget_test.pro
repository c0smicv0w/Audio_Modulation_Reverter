#-------------------------------------------------
#
# Project created by QtCreator 2016-02-10T23:12:17
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = widget_test
TEMPLATE = app
CONFIG += c++11


SOURCES += main.cpp\
        mainwindow.cpp \
    ../../src/pcmwidget.cpp \
    ../../src/freqwidget.cpp

HEADERS  += mainwindow.h \
    ../../src/pcmwidget.h \
    ../../src/freqwidget.h

FORMS    += mainwindow.ui
