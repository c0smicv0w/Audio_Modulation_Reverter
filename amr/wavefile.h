#ifndef WAVEFILE
#define WAVEFILE

#include <stdio.h>

#ifndef uint
typedef unsigned int uint;
#endif

typedef struct // riff section header
{
    char    riff_char[4];
    int     package_len;
    char    wave[4];
} WavRiff;

typedef struct // format section header
{
    char    format[4];
    int     format_len;
    short   fixed;
    short   channel;
    int     samplerate;
    int     byterate;
    short   byte_per_sample;
    short   bits_per_sample;
} WavFormat;

typedef struct // fact section header
{
    char    fact_field[4];
    int     fact_len;
    uint    fact_sample_len;
} WavFact;

typedef struct // data section header
{
    char    data_field[4];
    uint    data_len;
} WavData;

typedef struct
{
    WavRiff     riff;
    WavFormat   format;
    WavFact     fact;
    WavData     data;
} WavHeader;

class WavFileGeneric
{
private:
    char *convBuf;
    int convBufSize;

protected:
    WavFileGeneric();
    virtual ~WavFileGeneric();

    void *getConvBuf(int sizeByte);
};



#endif // WAVEFILE

