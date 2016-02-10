#ifndef AUDIOMODULATOR
#define AUDIOMODULATOR

#include <QAudioFormat>
#include <QByteArray>
#include <QList>
#include "complex.h"

class AudioModulator
{
private:
    QAudioFormat audioFormat;
    double pitch;

public:
    void pitchShift(const QByteArray &pcmIn, QList<complex> &freqIn, QByteArray &pcmOut, QList<complex> &freqOut);

};

#endif // AUDIOMODULATOR

