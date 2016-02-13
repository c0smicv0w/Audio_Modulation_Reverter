#include "audiomodulator.h"
#include "transform.h"
#include <math.h>
#include <string.h>

#define SEMITONE 1.0594630943592952645618252949463

double AudioModulator::getPitch() const
{
    return pitch;
}

void AudioModulator::setPitch(double value)
{
    pitch = value;
}

// Convert from double to integer and saturate
inline int ceil(double dvalue, double minval, double maxval)
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

    char* tempIn = (char*)param.pcmIn->data();
    int numElems = param.pcmIn->size();
    int bytesPerSample = 2; // just considered about 16bit sampling data.
    int size = numElems/bytesPerSample;
    double *buffer = new double[size];
    complex *inputCom = new complex[size];



    // pcm convert
    short *tempIn2 = new short[size];

    memcpy(tempIn2, (short*)tempIn, sizeof(short)*size);

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

    param.freqIn->clear();
    param.freqIn->reserve(size);

    for (int i = 0; i < size; i++)
    {
        param.freqIn->push_back(inputCom[i]);
    }



    complex *outputCom = new complex[size];               // pitch scaling

    double shiftTerm = pow(SEMITONE, pitch);

    if (shiftTerm > 1.0) // up-scaling
    {
        for (int i = 0; i < size / shiftTerm; i++)
        {
            outputCom[(int)((double)i * shiftTerm)] = inputCom[i];
        }
    }
    else if (shiftTerm < 1.0) // down-scaling
    {
        for (int i = 0; i < size; i++)
        {
            outputCom[(int)((double)i * shiftTerm)] = inputCom[i];
        }
    }
    else
    {
        for (int i = 0; i < size; i++) //do nothing
        {
            outputCom[i] = inputCom[i];
        }
    }



    param.freqOut->clear();
    param.freqOut->reserve(size);

    for (int i = 0; i < size; i++)
    {
        param.freqOut->push_back(outputCom[i]);
    }



    trans.Inverse(outputCom, size);                     // Ifft

    short *tempOut = new short[size];


    for (int i = 0; i < size; i ++)
    {
        short value = (short)ceil(outputCom[i].re() * 32768.0, -32768.0, 32767.0);
        tempOut[i] = value;
    }

    param.pcmOut->clear();
    param.pcmOut->reserve(numElems);

    for (int i = 0; i < numElems; i++)
    {
        param.pcmOut->push_back(tempOut[i]);
    }

    delete[] buffer;
    delete[] inputCom;
    delete[] outputCom;
    delete[] tempOut;
    delete[] tempIn2;
}
