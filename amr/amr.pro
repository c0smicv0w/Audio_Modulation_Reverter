TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    Transform.cpp \
    ParseParameter.cpp \
    WaveFile.cpp \
    amr.cpp \
    complex.cpp

HEADERS += \
    amr.h \
    Transform.h \
    ParseParameter.h \
    WaveFile.h \
    FIFOSamplePipe.h \
    complex.h
