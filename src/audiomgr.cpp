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

        std::vector<short> pcmIn;
        std::vector<complex> freqIn;
        std::vector<short> pcmOut;
        std::vector<complex> freqOut;

        pcmIn.clear();
        freqIn.clear();
        pcmOut.clear();
        freqOut.clear();

        QByteArray temp;
        temp = m_totalBuffer.left(ProcessSize);

        const short* begin = reinterpret_cast<short*>(temp.data());
        const short* end = begin + temp.length();
        std::vector<short> tempPcmIn(begin, end);

        pcmIn = tempPcmIn;

        m_totalBuffer = m_totalBuffer.mid(ProcessSize);

        AudioDataParam param;
        param.pcmIn = &pcmIn;
        param.freqIn = &freqIn;
        param.pcmOut = &pcmOut;
        param.freqOut = &freqOut;
        am.pitchShift(param);

        emit dataAvail(param);




        QByteArray outPcm;
        outPcm.fromRawData(reinterpret_cast<char*>(param.pcmOut->data()), sizeof(short)*param.pcmOut->size());

        m_output->write(outPcm);
        pcmInFile->write((short*)pcmIn.data(), ProcessSize/sizeof(short));
        pcmOutFile->write((short*)param.pcmOut->data(), ProcessSize/sizeof(short));

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
    QDateTime current = QDateTime::currentDateTime();
    QString pcmInName = current.toString() + "_In.wav";
    QString pcmOutName = current.toString() + "_Out.wav";

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

