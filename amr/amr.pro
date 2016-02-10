TEMPLATE = app
CONFIG += console
TARGET = amr


QT += multimedia widgets core

SOURCES += \
    ../src/audioinput.cpp \
    ../src/audiomodulator.cpp \
    ../src/complex.cpp \
    ../src/main.cpp \
    ../src/ParseParameter.cpp \
    ../src/Transform.cpp \
    ../src/WaveFile.cpp

HEADERS += \
    ../src/audioinput.h \
    ../src/audiomodulator.h \
    ../src/complex.h \
    ../src/ParseParameter.h \
    ../src/Transform.h \
    ../src/WaveFile.h
