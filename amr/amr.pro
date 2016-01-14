TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    Transform.cpp \
    ParseParameter.cpp \
    WaveFile.cpp \
    complex.cpp

HEADERS += \
    Transform.h \
    ParseParameter.h \
    WaveFile.h \
    complex.h
