TEMPLATE = app
CONFIG += console
TARGET = amr


QT += multimedia widgets core

SOURCES += \
    ../src/audioinput.cpp \
    ../src/audiomodulator.cpp \
    ../src/complex.cpp \
    ../src/main.cpp \
    ../test/pitchshifttest.cpp \
    ../src/parseparameter.cpp \
    ../src/transform.cpp \
    ../src/wavefile.cpp

HEADERS += \
    ../src/audioinput.h \
    ../src/audiomodulator.h \
    ../src/complex.h \
    ../src/parseparameter.h \
    ../src/transform.h \
    ../src/wavefile.h
