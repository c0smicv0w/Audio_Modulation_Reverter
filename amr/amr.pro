TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    Transform.cpp \
    ParseParameter.cpp \
    WaveFile.cpp

HEADERS += \
    amr.h \
    Transform.h \
    ParseParameter.h \
    WaveFile.h
