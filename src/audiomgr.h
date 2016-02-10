#ifndef AUDIOMGR_H
#define AUDIOMGR_H

#include <QObject>

#include <QByteArray>
#include <QBuffer>
#include <qaudioinput.h>
#include <qaudiooutput.h>

#include <QDebug>
#include "audiomodulator.h"


class AudioMgr : public QObject
{
    Q_OBJECT
public:
    AudioMgr();
    ~AudioMgr();

    typedef enum
    {
        Closed, Active, Suspended
    }State;
    State state = Closed;

    AudioModulator am;

//private:
    void initializeAudio();
    void createAudioInput();
    void createAudioOutput();
    void start();
    void suspend();
    void resume();
    void stop();

//private:
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
};


#endif // AUDIOMGR_H
