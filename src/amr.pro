#-------------------------------------------------
#
# Project created by QtCreator 2016-02-10T22:20:52
#
#-------------------------------------------------

QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = amr
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    audiomgr.cpp \
    audiomodulator.cpp \
    complex.cpp \
    freqwidget.cpp \
    parseparameter.cpp \
    pcmwidget.cpp \
    transform.cpp \
    wavefile.cpp

HEADERS  += mainwindow.h \
    audiomgr.h \
    audiomodulator.h \
    complex.h \
    freqwidget.h \
    parseparameter.h \
    pcmwidget.h \
    transform.h \
    wavefile.h

FORMS    += mainwindow.ui

CONFIG   += c++11
