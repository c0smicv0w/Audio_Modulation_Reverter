#ifndef AUDIOMODULATOR
#define AUDIOMODULATOR

#include <QAudioFormat>
#include <QByteArray>
#include <QList>
#include "complex.h"

typedef struct {
    QByteArray *pcmIn;
    QList<complex> *freqIn;
    QByteArray *pcmOut;
    QList<complex> *freqOut;
}AudioDataParam;

class AudioModulator
{
private:
    QAudioFormat audioFormat;
    double pitch;

public:
    AudioModulator() {  pitch = 0;  }
    void pitchShift(AudioDataParam param);

    double getPitch() const;
    void setPitch(double value);

    QAudioFormat getAudioFormat() const;
    void setAudioFormat(const QAudioFormat &value);
};

#endif // AUDIOMODULATOR

