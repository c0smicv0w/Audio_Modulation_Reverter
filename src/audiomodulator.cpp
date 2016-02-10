#include "audiomodulator.h"
#include "transform.h"
#include <math.h>

#define SEMITONE 1.0594630943592952645618252949463

double AudioModulator::getPitch() const
{
    return pitch;
}

void AudioModulator::setPitch(double value)
{
    pitch = value;
}

QAudioFormat AudioModulator::getAudioFormat() const
{
    return audioFormat;
}

void AudioModulator::setAudioFormat(const QAudioFormat &value)
{
    audioFormat = value;
}

// Convert from double to integer and saturate
inline int saturate(double dvalue, double minval, double maxval)
{
    if (dvalue > maxval)
    {
        dvalue = maxval;
    }
    else if (dvalue < minval)
    {
        dvalue = minval;
    }
    return (int)dvalue;
}

void AudioModulator::pitchShift(AudioDataParam param)
{
    Transform trans;

    char* tempIn = param.pcmIn->data();
    int numElems = param.pcmIn->size();
    int bytesPerSample = 2;//audioFormat.sampleSize() / 8;
    int size = numElems/bytesPerSample;
    double *buffer = new double[size];
    complex *inputCom = new complex[size];
    short *tempIn2 = (short*)tempIn;                        // pcm convert
    double conv = 1.0 / 32768.0;
    for (int i = 0; i < size; i++)
    {
        short value = tempIn2[i];
        buffer[i] = (double)(value * conv);
    }


            // fft

    for (int i = 0 ; i < size; i++)
    {
        inputCom[i] = buffer[i];
    }

    trans.Forward(inputCom, size);

    for (int i = 0; i < size; i++)
    {
        param.freqIn->append(inputCom[i]);
    }





    complex *outputCom = new complex[size];               // pitch scaling

    for (int i = 0; i < pow(SEMITONE, pitch); i++)
    {
        outputCom[(int)((double)i * (pow(SEMITONE, pitch)))] = inputCom[i];
    }

    for (int i = 0; i < size; i++)
    {
        param.freqOut->append(outputCom[i]);
    }





    trans.Inverse(outputCom, size);                     // Ifft

    short *tempOut = new short[param.pcmIn->size()];

    short *tempOut2 = (short *)tempOut;
    for (int i = 0; i < numElems; i ++)
    {
        short value = (short)saturate(buffer[i] * 32768.0, -32768.0, 32767.0);
        tempOut2[i] = value;
        param.pcmOut->append(tempOut2[i]);
    }





}
