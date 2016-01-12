#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <cstring>
#include <ctype.h>
#include <stdexcept>

#include "WaveFile.h"

using namespace std;

static const char riffStr[] = "RIFF";
static const char waveStr[] = "WAVE";
static const char fmtStr[]  = "fmt";
static const char factStr[] = "fact";
static const char dataStr[] = "data";


//WavFileGeneric
WavFileGeneric::WavFileGeneric()
{
    convBuf = NULL;
    convBufSize = 0;
}

WavFileGeneric::~WavFileGeneric()
{
    delete[] convBuf;
    convBufSize = 0;
}

void *WavFileGeneric::getConvBuf(int sizeByte)
{
    if (convBufSize < sizeByte)
    {
        delete[] convBuf;

        convBufSize = (sizeByte + 15) & -8;
        convBuf = new char[convBufSize];
    }
    return convBuf;
}



//WavinFile
WavInFile::WavInFile(const char *filename)
{
    fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        string msg = "Error: Unalbe to open file \"";
        msg += filename;
        msg += "\" for reading.";
        std::runtime_error(msg.c_str());
    }

    init();
}

WavInFile::WavInFile(FILE *file)
{
    fp = NULL;
    if (!file)
    {
        string msg = "Error: Unable to access input stream for reading";
        std::runtime_error(msg.c_str());
    }

    init();
}

void WavInFile::init()
{
    int hdrsOk;

    assert(fp);

    hdrsOk = readWavHeaders();
    if (hdrsOk != 0)
    {
        string msg = "Input file is corrupt or not a WAV file";
        std::runtime_error(msg.c_str());
    }

    dataRead = 0;
}

WavInFile::~WavInFile()
{
    if (fp)
        fclose(fp);
    fp = NULL;
}

void WavInFile::rewind()
{
    int hdrsOk;

    fseek(fp, 0, SEEK_SET);
    hdrsOk = readWavHeaders();
    assert(hdrsOk == 0);
    dataRead = 0;
}

int WavInFile::checkCharTags() const
{
    if (memcmp(fmtStr, header.format.format, 4) != 0)
        return -1;
    if (memcmp(dataStr, header.data.data_field, 4) != 0)
        return -1;

    return 0;
}

int WavInFile::read(unsigned char *buffer, int maxElems)
{
    int numBytes;
    uint afterDataRead;

    if (header.format.bits_per_sample != 0)
    {
        std::runtime_error("Error: WavInFile::read(char*, int) works only with 8bit samples.");
    }
    assert(sizeof(char) == 1);

    numBytes = maxElems;
    afterDataRead = dataRead + numBytes;
    if (afterDataRead > header.data.data_len)
    {
        numBytes = (int)header.data.data_len - (int)dataRead;
        assert(numBytes >= 0);
    }

    assert(buffer);
    numBytes = (int)fread(buffer, 1, numBytes, fp);
    dataRead += numBytes;

    return numBytes;
}

int WavInFile::read(short *buffer, int maxElems)
{
    unsigned int afterDataRead;
    int numBytes;
    int numElems;

    assert(buffer);
    switch(header.format.bits_per_sample)
    {
    case 8: // 8 bit format
    {
        unsigned char *temp = (unsigned char*)getConvBuf(maxElems);
        int i;

        numElems = read(temp, maxElems);
        // convert from 8 to 16 bit
        for (i = 0; i <numElems; i++)
        {
            buffer[i] = (short)(((short)temp[i] - 128) * 256);
        }
        break;
    }
    case 16: // 16 bit format
    {
        assert(sizeof(short) == 2);

        numBytes = maxElems * 2;
        afterDataRead = dataRead + numBytes;
        if (afterDataRead > header.data.data_len)
        {
            numBytes = (int)header.data.data_len - (int)dataRead;
            assert(numBytes >= 0);
        }

        numBytes = (int)fread(buffer, 1, numBytes, fp);
        dataRead += numBytes;
        numElems = numBytes / 2;

        break;
    }
    default:
        std::runtime_error("read(short*, int) error");
    };

    return numElems;
}

int WavInFile::read(double *buffer, int maxElems)
{
    unsigned int afterDataRead;
    int numBytes;
    int numElems;
    int bytesPerSample;
    
    assert(buffer);
    
    bytesPerSample = header.format.bits_per_sample / 8;
    
    if ((bytesPerSample < 1) || (bytesPerSample > 4))
    {
        std::runtime_error("sample wav not supported. Can't open WAV file");
    }
    
    numBytes = maxElems * bytesPerSample;
    afterDataRead = dataRead + numBytes;
    if (afterDataRead > header.data.data_len)
    {
        numBytes = (int)header.data.data_len - (int)dataRead;
        assert(numBytes >= 0);
    }
    
    char *temp = (char*)getConvBuf(numBytes);
    numBytes = (int)fread(temp, 1, numBytes, fp);
    dataRead += numBytes;

    numElems = numBytes / bytesPerSample;

    switch (bytesPerSample)
    {
    case 1:
    {
        unsigned char *temp2 = (unsigned char*)temp;
        double conv = 1.0 / 128.0;
        for (int i = 0; i < numElems; i++)
        {
            buffer[i] = (double)(temp2[i] * conv - 1.0);
        }
        break;
    }
    case 2:
    {
        short *temp2 = (short*)temp;
        double conv = 1.0 / 32768.0;
        for (int i = 0; i < numElems; i++)
        {
            short value = temp2[i];
            buffer[i] = (double)(value * conv);
        }
        break;
    }
    case 3:
    {
        char *temp2 = (char*)temp;
        double conv = 1.0 / 8388608.0;
        for (int i = 0; i < numElems; i++)
        {
            int value = *((int*)temp2);
            value = value & 0x00ffffff;
            value |= (value & 0x00800000) ? 0xff000000 : 0;
            buffer[i] = (double)(value * conv);
            temp2 += 3;
        }
        break;
    }
    case 4:
    {
        int *temp2 = (int *)temp;
        double conv = 1.0 / 2147483648.0;
        assert(sizeof(int) == 4);
        for (int i = 0; i < numElems; i++)
        {
            int value = temp2[i];
            buffer[i] = (double)(value * conv);
        }
        break;
    }
    }

    return numElems;

}



int WavInFile::eof() const
{
    return (dataRead == header.data.data_len || feof(fp));
}

// test if character code is between a white space ' ' and little 'z'
static int isAlpha(char c)
{
    return (c >= ' ' && c <= 'z') ? 1 : 0;
}


// test if all characters are between a white space ' ' and little 'z'
static int isAlphaStr(const char *str)
{
    char c;

    c = str[0];
    while (c)
    {
        if (isAlpha(c) == 0) return 0;
        str ++;
        c = str[0];
    }

    return 1;
}


int WavInFile::readRIFFBlock()
{
    if (fread(&(header.riff), sizeof(WavRiff), 1, fp) != 1) return -1;

    // header.riff.riff_char should equal to 'RIFF');
    if (memcmp(riffStr, header.riff.riff_char, 4) != 0) return -1;
    // header.riff.wave should equal to 'WAVE'
    if (memcmp(waveStr, header.riff.wave, 4) != 0) return -1;

    return 0;
}

int WavInFile::readHeaderBlock()
{
    char label[5];
    string sLabel;

    // Read label string
    if (fread(label, 1, 4, fp) !=4)
        return -1;

    label[4] = 0;

    if (isAlphaStr(label) == 0)
        return -1;    // not a valid label

    // Decode blocks according to their label
    if (strcmp(label, fmtStr) == 0)
    {
        int nLen, nDump;

        // 'fmt ' block
        memcpy(header.format.format, fmtStr, 4);

        // read length of the format field
        if (fread(&nLen, sizeof(int), 1, fp) != 1) return -1;

        header.format.format_len = nLen;

        // calculate how much length differs from expected
        nDump = nLen - ((int)sizeof(header.format) - 8);

        // if format_len is larger than expected, read only as much data as we've space for
        if (nDump > 0)
        {
            nLen = sizeof(header.format) - 8;
        }

        // read data
        if (fread(&(header.format.fixed), nLen, 1, fp) != 1) return -1;

        // if format_len is larger than expected, skip the extra data
        if (nDump > 0)
        {
            fseek(fp, nDump, SEEK_CUR);
        }

        return 0;
    }
    else if (strcmp(label, factStr) == 0)
    {
        int nLen, nDump;

        // 'fact' block
        memcpy(header.fact.fact_field, factStr, 4);

        // read length of the fact field
        if (fread(&nLen, sizeof(int), 1, fp) != 1)
            return -1;

        header.fact.fact_len = nLen;

        // calculate how much length differs from expected
        nDump = nLen - ((int)sizeof(header.fact) - 8);

        // if format_len is larger than expected, read only as much data as we've space for
        if (nDump > 0)
        {
            nLen = sizeof(header.fact) - 8;
        }

        // read data
        if (fread(&(header.fact.fact_sample_len), nLen, 1, fp) != 1) return -1;


        // if fact_len is larger than expected, skip the extra data
        if (nDump > 0)
        {
            fseek(fp, nDump, SEEK_CUR);
        }

        return 0;
    }
    else if (strcmp(label, dataStr) == 0)
    {
        // 'data' block
        memcpy(header.data.data_field, dataStr, 4);
        if (fread(&(header.data.data_len), sizeof(uint), 1, fp) != 1) return -1;

        return 1;
    }
    else
    {
        uint len, i;
        uint temp;
        // unknown block

        // read length
        if (fread(&len, sizeof(len), 1, fp) != 1) return -1;
        // scan through the block
        for (i = 0; i < len; i ++)
        {
            if (fread(&temp, 1, 1, fp) != 1) return -1;
            if (feof(fp)) return -1;   // unexpected eof
        }
    }
    return 0;
}

int WavInFile::readWavHeaders()
{
    int res;

    memset(&header, 0, sizeof(header));

    res = readRIFFBlock();
    if (res) return 1;
    // read header blocks until data block is found
    do
    {
        // read header blocks
        res = readHeaderBlock();
        if (res < 0) return 1;  // error in file structure
    } while (res == 0);
    // check that all required tags are legal
    return checkCharTags();
}

uint WavInFile::getNumChannels() const
{
    return header.format.channel;
}


uint WavInFile::getNumBits() const
{
    return header.format.bits_per_sample;
}


uint WavInFile::getBytesPerSample() const
{
    return getNumChannels() * getNumBits() / 8;
}


uint WavInFile::getSampleRate() const
{
    return header.format.samplerate;
}



uint WavInFile::getDataSizeInBytes() const
{
    return header.data.data_len;
}


uint WavInFile::getNumSamples() const
{
    if (header.format.byte_per_sample == 0)
        return 0;

    if (header.format.fixed > 1)
        return header.fact.fact_sample_len;

    return header.data.data_len / (unsigned short)header.format.byte_per_sample;
}


uint WavInFile::getLengthMS() const
{
    double numSamples;
    double sampleRate;

    numSamples = (double)getNumSamples();
    sampleRate = (double)getSampleRate();

    return (uint)(1000.0 * numSamples / sampleRate + 0.5);
}


// Returns how milliseconds of audio have proceeded
uint WavInFile::getElapsedMS() const
{
    return (uint)(1000.0 * (double)dataRead / (double)header.format.byterate);
}



// Class WavOutFile
WavOutFile::WavOutFile(const char *filename, int sampleRate, int bits, int channels)
{
    bytesWritten = 0;
    fp = fopen(filename, "wb");

    if (fp == NULL)
    {
        std::runtime_error("Unable to open file for writng");
    }

    fillInHeader(sampleRate, bits, channels);
    writeHeader();
}

WavOutFile::WavOutFile(FILE *file, int sampleRate, int bits, int channels)
{
    bytesWritten = 0;
    fp = file;

    if (fp == NULL)
    {
        std::runtime_error("Error: Unable to access output file stream");
    }

    fillInHeader(sampleRate, bits, channels);
    writeHeader();
}

WavOutFile::~WavOutFile()
{
    finishHeader();
    if (fp)
        fclose(fp);
    fp = NULL;
}

void WavOutFile::fillInHeader(const uint sampleRate, const uint bits, const uint channels)
{
    // fill in the 'riff' part

    // copy string 'RIFF' to riff_char
    memcpy(&(header.riff.riff_char), riffStr, 4);
    // package_len unknown
    header.riff.package_len = 0;
    // copy string 'WAVE' to wave
    memcpy(&(header.riff.wave), waveStr, 4);

    // fill in the 'format' part

    // copy string 'fmt ' to format
    memcpy(&(header.format.format), fmtStr, 4);

    header.format.format_len = 0x10;
    header.format.fixed = 1;
    header.format.channel = (short)channels;
    header.format.samplerate = (int)sampleRate;
    header.format.bits_per_sample = (short)bits;
    header.format.byte_per_sample = (short)(bits * channels / 8);
    header.format.byterate = header.format.byte_per_sample * (int)sampleRate;
    header.format.samplerate = (int)sampleRate;

    // fill in the 'fact' part
    memcpy(&(header.fact.fact_field), factStr, 4);
    header.fact.fact_len = 4;
    header.fact.fact_sample_len = 0;

    // fill in the 'data' part

    // copy string 'data' to data_field
    memcpy(&(header.data.data_field), dataStr, 4);
    header.data.data_len = 0;
}

void WavOutFile::finishHeader()
{
    header.riff.package_len = bytesWritten + sizeof(WavHeader) -  sizeof(WavRiff) + 4;
    header.data.data_len = bytesWritten;
    header.fact.fact_sample_len = bytesWritten / header.format.byte_per_sample;

    writeHeader();
}

void WavOutFile::writeHeader()
{
    WavHeader hdrTmp;
    int res;
    hdrTmp = header;

    // write the supplemented header in the beginning of the file
    fseek(fp, 0, SEEK_SET);
    res = (int)fwrite(&hdrTmp, sizeof(hdrTmp), 1, fp);
    if (res != 1)
    {
        std::runtime_error("Error while writing to a wav file");
    }

    fseek(fp, 0, SEEK_END);

}


void WavOutFile::write(const unsigned char *buffer, int numElems)
{
    int res;

    if (header.format.bits_per_sample != 0)
    {
        std::runtime_error("Error: WavOutFile::write(const char*, int) accepts ont 8bit samples");
    }
    assert(sizeof(char) == 1);

    res = (int)fwrite(buffer, 1, numElems, fp);
    if (res != numElems)
    {
        std::runtime_error("Error while writng to a wav file");
    }

    bytesWritten += numElems;
}

void WavOutFile::write(const short *buffer, int numElems)
{
    int res;

    // 16 bit sample
    if (numElems < 1) // nothing to do
        return;

    switch (header.format.bits_per_sample)
    {
    case 8:
    {
        int i;
        unsigned char *temp = (unsigned char *)getConvBuf(numElems);
        // convert from 16bit format to 8bit format
        for (i = 0; i < numElems; i++)
        {
            temp[i] = (unsigned char)(buffer[i] / 256 + 128);
        }
        // write in 8bit format
        write(temp, numElems);
        break;
    }
    case 16:
    {
        // 16 bit format

        //use temp buffer to swap byte order if necssary
        short *pTemp = (short *)getConvBuf(numElems * sizeof(short));
        memcpy(pTemp, buffer, numElems * 2);

        res = (int)fwrite(pTemp, 2, numElems, fp);

        if (res != numElems)
        {
            std::runtime_error("Error while writing to a wav file.");
        }
        bytesWritten += 2 * numElems;
        break;
    }
    default:
    {
        std::runtime_error("unsupported sample wav file. can't open wave file");
    }
    }
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

void WavOutFile::write(const double *buffer, int numElems)
{
    int numBytes;
    int bytesPerSample;

    if (numElems == 0) return;

    bytesPerSample = header.format.bits_per_sample / 8;
    numBytes = numElems * bytesPerSample;
    short *temp = (short*)getConvBuf(numBytes);

    switch (bytesPerSample)
    {
        case 1:
        {
            unsigned char *temp2 = (unsigned char *)temp;
            for (int i = 0; i < numElems; i ++)
            {
                temp2[i] = (unsigned char)saturate(buffer[i] * 128.0 + 128.0, 0.0, 255.0);
            }
            break;
        }

        case 2:
        {
            short *temp2 = (short *)temp;
            for (int i = 0; i < numElems; i ++)
            {
                short value = (short)saturate(buffer[i] * 32768.0, -32768.0, 32767.0);
                temp2[i] = value;
            }
            break;
        }

        case 3:
        {
            char *temp2 = (char *)temp;
            for (int i = 0; i < numElems; i ++)
            {
                int value = saturate(buffer[i] * 8388608.0, -8388608.0, 8388607.0);
                *((int*)temp2) = value;
                temp2 += 3;
            }
            break;
        }

        case 4:
        {
            int *temp2 = (int *)temp;
            for (int i = 0; i < numElems; i ++)
            {
                int value = saturate(buffer[i] * 2147483648.0, -2147483648.0, 2147483647.0);
                temp2[i] = value;
            }
            break;
        }

        default:
            assert(false);
    }

    int res = (int)fwrite(temp, 1, numBytes, fp);

    if (res != numBytes)
    {
        std::runtime_error("Error while writing to a wav file.");
    }
    bytesWritten += numBytes;
}




