#include "audiomgr.h"
#include <QDebug>
#include <QDate>

AudioMgr::AudioMgr():
    m_Inputdevice(QAudioDeviceInfo::defaultInputDevice()),
    m_Outputdevice(QAudioDeviceInfo::defaultOutputDevice()),
    m_audioInput(0),
    m_audioOutput(0),
    m_input(0),
    m_output(0),
    m_buffer(BufferSize, 0),
    pcmInFile(0),
    pcmOutFile(0)
{
}

AudioMgr::~AudioMgr()
{
}

//Initialize audio format
void AudioMgr::initializeAudio()
{
    m_format.setSampleRate(44100); //set SampleRate
    m_format.setChannelCount(1); //set channelCount to mono
    m_format.setSampleSize(16); //set sample size to 16 bit
    m_format.setSampleType(QAudioFormat::SignedInt ); //Sample type as signed integer sample
    m_format.setByteOrder(QAudioFormat::LittleEndian); //Byte order
    m_format.setCodec("audio/pcm"); //set codec as simple audio/pcm

    QAudioDeviceInfo infoIn(QAudioDeviceInfo::defaultInputDevice());
    if (!infoIn.isFormatSupported(m_format))
    {
        //Default format not supported - trying to use nearest
        m_format = infoIn.nearestFormat(m_format);
    }

    qDebug() << infoIn.deviceName();

    QAudioDeviceInfo infoOut(QAudioDeviceInfo::defaultOutputDevice());
    if (!infoOut.isFormatSupported(m_format))
    {
       //Default format not supported - trying to use nearest
        m_format = infoOut.nearestFormat(m_format);
    }


    qDebug() << infoOut.deviceName();


    createAudioInput();
    createAudioOutput();
}

void AudioMgr::createAudioOutput()
{
    m_audioOutput = new QAudioOutput(m_Outputdevice, m_format, this);
}

void AudioMgr::createAudioInput()
{
    if (m_input != 0) {
        disconnect(m_input, 0, this, 0);
        m_input = 0;
    }
    m_audioInput = new QAudioInput(m_Inputdevice, m_format, this);

}

void AudioMgr::processing()
{
    //Return if audio input is null
    if(!m_audioInput)
        return;

    //Check the number of samples in input buffer
    qint64 len = m_audioInput->bytesReady();

    //Limit sample size
    if(len > BufferSize)
        len = BufferSize;

    //Read sound samples from input device to buffer
    qint64 l = m_input->read(m_buffer.data(), len);

    // Return if suspend is triggered
    if(state == Suspended)
        return;

    if (l <= 0)
        return;

    m_totalBuffer.append(m_buffer.constData(), l);

    while(true)
    {
        qint64 size = m_totalBuffer.size();
        if(  size < ProcessSize )
            break;

        const short* begin = reinterpret_cast<short*>(m_totalBuffer.data());
        const short* end = begin + ProcessSize / sizeof(short);

        std::vector<short> pcmIn(begin, end);
        std::vector<complex> freqIn;
        std::vector<short> pcmOut;
        std::vector<complex> freqOut;

        //pcmIn.clear(); // gilgil temp 2016.02.16
        //freqIn.clear(); // gilgil temp 2016.02.16
        //pcmOut.clear(); // gilgil temp 2016.02.16
        //freqOut.clear(); // gilgil temp 2016.02.16

        m_totalBuffer = m_totalBuffer.mid(ProcessSize);

        AudioDataParam param;
        param.pcmIn = &pcmIn;
        param.freqIn = &freqIn;
        param.pcmOut = &pcmOut;
        param.freqOut = &freqOut;
        am.pitchShift(param);

        emit dataAvail(param);

        m_output->write((const char*)pcmOut.data(), (qint64)ProcessSize);
        pcmInFile->write(pcmIn.data(), ProcessSize/sizeof(short));
        pcmOutFile->write(pcmOut.data(), ProcessSize/sizeof(short));
    }
}

void AudioMgr::setPitch(int pitch)
{
    am.setPitch(pitch);
}

void AudioMgr::start()
{
    state = Active;

    //Audio output device
    m_output= m_audioOutput->start();
    //Audio input device
    m_input = m_audioInput->start();

    // get current date time
    QString current = QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss-zzz");
    QString pcmInName = current + "_in.wav";
    QString pcmOutName = current + "_out.wav";

    // Create Wav files
    pcmInFile = new WavOutFile(pcmInName.toStdString().c_str(), 44100, 16, 1);
    pcmOutFile = new WavOutFile(pcmOutName.toStdString().c_str(), 44100, 16, 1);

    //connect readyRead signal to processing slot.
    //Call processing when audio samples fill in inputbuffer
    connect(m_input, SIGNAL(readyRead()), this, SLOT(processing()));
}

void AudioMgr::suspend()
{
    state = Suspended;
}
void AudioMgr::resume()
{
    state = Active;
}

void AudioMgr::stop()
{
    state = Closed;
    disconnect(m_input, SIGNAL(readyRead()), this, SLOT(processing()));

    delete m_audioInput; m_audioInput = 0;
    delete m_audioOutput; m_audioOutput = 0;
    m_input = 0;
    m_output = 0;
    delete pcmInFile; pcmInFile = 0;
    delete pcmOutFile; pcmOutFile = 0;
}

