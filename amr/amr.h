#ifndef AMR_H
#define AMR_H

#include "FIFOSamplePipe.h"

typedef unsigned int    uint;

namespace amr
{

class amr : FIFOProcessor
{
private:
    class RateTransposer *pRateTransposer;
    class TDStretch *pTDStretch;

    double virtualRate;
    double virtualTempo;
    double virtualPitch;

    bool  bSrateSet;

    double samplesExpectedOut;
    long   samplesOutput;

    void calcEffectiveRateAndTempo();

protected :
    uint  channels;
    double rate;
    double tempo;

public:
    amr();
    virtual ~amr();

    void setRate(double newRate);
    void setTempo(double newTempo);
    void setRateChange(double newRate);
    void setTempoChange(double newTempo);

    void setPitch(double newPitch);
    void setPitchOctaves(double newPitch);
    void setPitchSemiTones(int newPitch);
    void setPitchSemiTones(double newPitch);

    void setChannels(uint numChannels);
    void setSampleRate(uint srate);

    void flush();

    virtual void putSamples(const SAMPLETYPE *samples, uint numSamples);

    virtual uint receiveSamples(SAMPLETYPE *output, uint maxSamples);
    virtual uint receiveSamples(uint maxSamples);

    virtual void clear();

    bool setSetting(int settingId, int value);
    int getSetting(int settingId) const;

    virtual uint numUnprocessedSamples() const;
};

}
#endif // AMR_H
