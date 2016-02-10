#ifndef AUDIOMGR_H
#define AUDIOMGR_H

#include <QObject>

#include <QByteArray>
#include <QBuffer>
#include <qaudioinput.h>
#include <qaudiooutput.h>

#include <QDebug>


class AudioMgr : public QObject
{
    Q_OBJECT
public:
    AudioMgr();
    ~AudioMgr();

//private:
    void initializeAudio();
    void createAudioInput();
    void createAudioOutput();
    void start();
    void suspend();
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

private slots:
    void processing();
};


#endif // AUDIOMGR_H
