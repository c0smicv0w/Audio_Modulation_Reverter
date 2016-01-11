TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    Transform.cpp \
    ParseParameter.cpp

HEADERS += \
    amr.h \
    Transform.h \
    ParseParameter.h \
    wavefile.h

