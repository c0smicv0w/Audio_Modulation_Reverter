#ifndef AUDIOMGR_H
#define AUDIOMGR_H

#include <QByteArray>
#include <QAudioInput>
#include <QAudioOutput>
#include <QObject>

#include "audiomodulator.h"

class AudioMgr : public QObject
{
    Q_OBJECT

private:
    static const int BufferSize = 4096;
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

public:
    void initializeAudio();
    void createAudioInput();
    void createAudioOutput();
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

private slots:
    void processing();

signals:
    void dataAvail(AudioDataParam param);
    void stopDraw();
};

#endif // AUDIOMGR_H
