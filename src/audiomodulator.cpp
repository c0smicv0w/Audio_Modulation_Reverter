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
inline double ceil(double dvalue, double minval, double maxval)
{
    if (dvalue > maxval)
    {
        dvalue = maxval;
    }
    else if (dvalue < minval)
    {
        dvalue = minval;
    }
    return dvalue;
}

void AudioModulator::pitchShift(AudioDataParam param)
{
    Transform trans;
    int size = param.pcmIn->size();

    //
    // pcm convert
    //
    param.freqIn->resize(size);
    complex *inputCom = param.freqIn->data();
    static double conv = 1.0 / 32768.0;
    for (int i = 0; i < size; i++)
    {
        inputCom[i] = (double)((*(param.pcmIn))[i] * conv);
    }

    //
    // fft
    //
    trans.Forward(inputCom, size);

    //
    // Apply nyquist theorem
    //
    if (pitch < 0)
    {
      for (int i = size / 2; i < size; i++)
      inputCom[i] = 0;
    }

    //
    // pitch shift
    //
    param.freqOut->resize(size);
    complex *outputCom = param.freqOut->data();
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

    //
    // ifft
    //
    std::vector<complex> tempCom = *(param.freqOut);
    trans.Inverse(tempCom.data(), size);

    //
    // pcm convert
    //
    param.pcmOut->resize(size);
    for (int i = 0; i < size; i++)
    {
        double d = tempCom[i].re();
        d = ceil(d * 32768.0, -32768.0, 32767.0);
        (*param.pcmOut)[i] = (short)d;
    }
}
