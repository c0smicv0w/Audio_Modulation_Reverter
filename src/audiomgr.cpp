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
    m_buffer(InputSize, 0),
    pcmInFile(0),
    pcmOutFile(0)
{
    initializeAudio();
}

AudioMgr::~AudioMgr()
{
  if (m_audioOutput != 0) {
    delete m_audioOutput;
    m_audioOutput = 0;
  }

  if (m_audioInput != 0) {
    delete m_audioInput;
    m_audioInput = 0;
  }
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

    m_audioOutput = new QAudioOutput(m_Outputdevice, m_format, this);
    m_audioOutput->setBufferSize(OutputSize * 2);
    m_audioInput = new QAudioInput(m_Inputdevice, m_format, this);
    m_audioInput->setBufferSize(InputSize);
}

void AudioMgr::processing()
{
    //Return if audio input is null
    if(!m_audioInput)
        return;

    //Check the number of samples in input buffer
    qint64 len = m_audioInput->bytesReady();

    //Limit sample size
    if(len > InputSize)
        len = InputSize;

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
        if(  size < OutputSize )
            break;
        size_t numElems = OutputSize / sizeof(short);

        const short* begin = reinterpret_cast<short*>(m_totalBuffer.data());
        const short* end = begin + numElems;

        std::vector<short> pcmIn(begin, end);
        std::vector<complex> freqIn;
        std::vector<short> pcmOut;
        std::vector<complex> freqOut;

        m_totalBuffer = m_totalBuffer.mid(OutputSize);

        AudioDataParam param;
        param.pcmIn = &pcmIn;
        param.freqIn = &freqIn;
        param.pcmOut = &pcmOut;
        param.freqOut = &freqOut;
        am.pitchShift(param);

        emit dataAvail(param);

        Q_ASSERT(pcmIn.size() == numElems);
        Q_ASSERT(pcmOut.size() == numElems);
        pcmInFile->write(pcmIn.data(), numElems);
        pcmOutFile->write(pcmOut.data(), numElems);
        qint64 writeLen = m_output->write((const char*)pcmOut.data(), (qint64)OutputSize);
        if (writeLen != OutputSize)
            qWarning() << QString("m_output->write return %1 (should be %2)").arg(writeLen).arg(OutputSize);
    }
}

void AudioMgr::setPitch(int pitch)
{
    am.setPitch(pitch);
}

void AudioMgr::start()
{
    state = Active;

    // get current date time
    QString current = QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss-zzz");
    QString pcmInName = current + "_in.wav";
    QString pcmOutName = current + "_out.wav";

    // Create Wav files
    pcmInFile = new WavOutFile(pcmInName.toStdString().c_str(), 44100, 16, 1);
    pcmOutFile = new WavOutFile(pcmOutName.toStdString().c_str(), 44100, 16, 1);

    // Audio output device
    m_output= m_audioOutput->start();
    Q_ASSERT(m_output != 0);

    // Audio input device
    m_input = m_audioInput->start();
    Q_ASSERT(m_input != 0);
    // m_audioInput->setVolume(0.01); // gilgil temp 2016.02.14

    // Connect readyRead signal to processing slot.
    // Call processing when audio samples fill in inputbuffer
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

    m_audioInput->stop();
    m_audioOutput->stop();

    m_input = 0;
    m_output = 0;

    delete pcmInFile; pcmInFile = 0;
    delete pcmOutFile; pcmOutFile = 0;
}

