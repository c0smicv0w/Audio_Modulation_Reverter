TEMPLATE = app
CONFIG += console
TARGET = amr


QT += multimedia widgets

SOURCES += main.cpp \
    Transform.cpp \
    ParseParameter.cpp \
    WaveFile.cpp \
    complex.cpp \
    audioinput.cpp \
    audiomodulator.cpp

HEADERS += \
    Transform.h \
    ParseParameter.h \
    WaveFile.h \
    complex.h \
    audioinput.h \
    audiomodulator.h
