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


class WavInFile : protected WavFileGeneric
{
private:
    FILE *fp;
    long pos;
    long dataRead;
    WavHeader header;

    void init();
    int readWavHeaders();
    int checkCharTags() const;
    int readHeaderBlock();
    int readRIFFBlock();

public:
    WavInFile(const char *filename);
    WavInFile(FILE *file);

    ~WavInFile();

    void rewind();

    uint getSampleRate() const;
    uint getNumBits() const;
    uint getDataSizeInBytes() const;
    uint getNumSamples() const;
    uint getBytesPerSample() const;
    uint getNumChannels() const;
    uint getLengthMS() const;
    uint getElapsedMs() const;

    int read(unsigned char *buffer, int macElems);
    int read(short *buffer, int maxElems);

    int eof() const;
};

class WavOutFile : protected WavFileGeneric
{
private:
    FILE *fp;
    WavHeader header;
    int bytesWritten;

    void fillInHeader(const uint sampleRate, const uint bits, const uint channels);
    void finishHeader();
    void writeHeader();

public:
    WaveOutFile(const char *filename, int sampleRate, int bits, int channels);
    WaveOutFile(File *file, int sampleRate, int bits, int channels);
    ~WaveOutFile();

    void write(const unsigned char *buffer, int numElems);
    void write(const double *buffer, int numElems);

};


#endif // WAVEFILE

