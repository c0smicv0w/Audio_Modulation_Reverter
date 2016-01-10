#ifndef AMR_H
#define AMR_H

namespace amr
{

class amr
{
private:


    double virtualPitch;



    bool  bSrateSet;


    double samplesExpectedOut;


    long   samplesOutput;


    void calcEffectiveRateAndTempo();

protected :
    int  channels;
    double rate;
    double tempo;

public:
    amr();
    virtual ~amr();

};

}
#endif // AMR_H
