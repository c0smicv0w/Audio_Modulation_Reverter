#ifndef AUDIOMODULATOR
#define AUDIOMODULATOR

#include <vector>
#include "complex.h"

typedef struct {
    std::vector<short> *pcmIn;
    std::vector<complex> *freqIn;
    std::vector<short> *pcmOut;
    std::vector<complex> *freqOut;
}AudioDataParam;

class AudioModulator
{
private:
    double pitch;

public:
    AudioModulator() {  pitch = 0;  }
    void pitchShift(AudioDataParam param);

    double getPitch() const;
    void setPitch(double value);
};

#endif // AUDIOMODULATOR

