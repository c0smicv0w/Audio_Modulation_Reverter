#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <cmath>

#include "ParseParameter.h"
#include "WaveFile.h"
#include "Transform.h"


#define SEMITONE 1.0594630943592952645618252949463

using namespace std;

double logTone(double x)
{
    return std::log(x) / std::log(SEMITONE);
}

static void openFiles(WavInFile **inFile, WavOutFile **outFile, const ParseParameter *params)
{
    int bits, samplerate, channels;

    if (strcmp(params->inFileName, "stdin") == 0)
    {
        // used 'stdin' as input file
        *inFile = new WavInFile(stdin);
    }
    else
    {
        // open input file...
        *inFile = new WavInFile(params->inFileName);
    }

    // ... open output file with same sound parameters
    bits = (int)(*inFile)->getNumBits();
    samplerate = (int)(*inFile)->getSampleRate();
    channels = (int)(*inFile)->getNumChannels();

    if (params->outFileName)
    {
        if (strcmp(params->outFileName, "stdout") == 0)
        {
            *outFile = new WavOutFile(stdout, samplerate, bits, channels);
        }
        else
        {
            *outFile = new WavOutFile(params->outFileName, samplerate, bits, channels);
        }
    }
    else
    {
        *outFile = NULL;
    }
}

void analyze(WavInFile *inFile, WavOutFile *outFile, ParseParameter *params)
{
    Transform test;

    int size;
    int count = 0;
    int FreqArray[2000] = {0,};


    while (inFile->eof() == 0)
    {
        int windowSize = 16384;
        double *re = new double[windowSize];
        memset(re, 0, sizeof(double)*windowSize);

        complex *com = new complex[windowSize];
        complex *temp = new complex[windowSize];

        size = inFile->read(re, windowSize);

        for (int i = 0; i < size; i++)
        {
            com[i] = re[i];
        }

        test.Forward(com, temp, windowSize);


        double max = 0;
        int maxIndex = -1;

        for (int i = 0; i < windowSize - 1000; i++)
        {

            if (max < temp[i].norm())
            {
                max = temp[i].norm();
                maxIndex = i;
            }
        }


        FreqArray[int((maxIndex * 1.35)/10)]++;

        //cout << "\nlargest frequency: " << maxIndex * 1.35 <<"Hz" <<endl << endl;


        count++;

        delete[] re;
        delete[] com;
        delete[] temp;
    }

    int maxFreq = 0;
    int maxCount = FreqArray[0];

    for (int i = 1; i < 2000; i++)
    {
        if (FreqArray[i] >= maxCount) {
            maxCount = FreqArray[i];
            maxFreq = i;
        }
    }

    cout << maxFreq*10 << endl;

    params->pitch = logTone(600.0/(maxFreq *10));
    cout <<"pitch: " <<params->pitch << endl;
    inFile->rewind();

    while (inFile->eof() == 0)
    {
        int windowSize = 16384;
        double *re = new double[windowSize];
        memset(re, 0, sizeof(double)*windowSize);

        complex *com = new complex[windowSize];
        complex *output = new complex[windowSize];
        complex *temp = new complex[windowSize];

        size = inFile->read(re, windowSize);

        for (int i = 0; i < size; i++)
        {
            com[i] = re[i];
        }

        test.Forward(com, temp, windowSize);


        for (int i = 0; i < (windowSize / pow(SEMITONE,4)); i++)
        {
            output[(int)((double)i * (pow(SEMITONE, 4)))] = temp[i];
        }

        for (int i = 1; i < size - 1; i++)
        {
            if (output[i].re() < 0.000001 && output[i].re() > -0.000001)
            {
                output[i] = (double)((output[i-1].re() + output[i+1].re())/2.0);
            }
        }

        test.Inverse(output, windowSize);



        for (int i = 0; i < size; i++)
        {
            re[i] = output[i].re();
        }


        outFile->write(re, size);


        delete[] re;
        delete[] com;
        delete[] temp;
        delete[] output;
    }

    outFile->~WavOutFile();
}

int main(int argc, char* argv[])
{
    ParseParameter *param;
    WavInFile *inFile;
    WavOutFile *outFile;

    if (argc < 3)
    {
        cout << "Usage: " << argv[0] << "inputfile outputfile [option]" << endl;
        return 0;
    }

    param = new ParseParameter(argc, argv);

    openFiles(&inFile, &outFile, param);

    analyze(inFile, outFile, param);


    return 0;
}

