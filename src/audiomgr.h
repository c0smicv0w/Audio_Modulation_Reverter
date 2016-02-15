#ifndef AUDIOMGR_H
#define AUDIOMGR_H

#include <QByteArray>
#include <QAudioInput>
#include <QAudioOutput>
#include <QObject>

#include "audiomodulator.h"
#include "wavefile.h"

class AudioMgr : public QObject
{
    Q_OBJECT

private:
    static const int InputSize = 4096;
    static const int ProcessSize = 2048;

public:
    AudioMgr();
    ~AudioMgr() override;

    typedef enum
    {
        Closed, Active, Suspended
    } State;
    State state = Closed;

    AudioModulator am;

private:
    void initializeAudio();

public:
    void start();
    void suspend();
    void resume();
    void stop();

private:
    QAudioDeviceInfo m_Inputdevice;
    QAudioDeviceInfo m_Outputdevice;
    QAudioFormat m_format;
    QAudioInput *m_audioInput;
    QAudioOutput *m_audioOutput;
    QIODevice *m_input;
    QIODevice *m_output;
    QByteArray m_buffer;
    QByteArray m_totalBuffer;
    WavOutFile *pcmInFile;
    WavOutFile *pcmOutFile;

private slots:
    void processing();
    void setPitch(int);

signals:
    void dataAvail(AudioDataParam param);
};

#endif // AUDIOMGR_H
