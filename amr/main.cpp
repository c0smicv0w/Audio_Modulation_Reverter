#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ParseParameter.h"
#include "WaveFile.h"
#include "Transform.h"


#define SEMITONE 1.0594630943592952645618252949463

using namespace std;

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

void analyze(WavInFile *inFile, WavOutFile *outFile)
{
    Transform test;

    int size;

    while (inFile->eof() == 0)
    {
        int windowSize = 1024;
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

        //cout << "\n\n\n\n\nbefore transform" << endl;
        //for (int i = 0; i < size; i++)
        //{
            //cout << "real: " << com[i].re() << " image: " << com[i].im() << endl;

        //}

        int pitch = 4;



        test.Forward(com, temp, size);


        for (int i = 0; i < size / pow(SEMITONE, pitch); i++)
        {
            output[(int)((double)i * (pow(SEMITONE, pitch)))] = temp[i];
        }


        test.Inverse(output, size);


        for (int i = 1; i < size - 1; i++)
        {
            if (output[i].re() == 0)
            {
                output[i] = (double)((output[i-1].re() + output[i+1].re())/2.0);
            }
        }


        for (int i = 0; i < size; i++)
        {
            re[i] = output[i].re();
        }


        outFile->write(re, size);
/*
        cout << "\n\n\n\n\nafter transform" << endl;
        for (int i = 0; i < size; i++)
        {
            cout << "real: " << output[i].re() << " image: " << output[i].im() << endl;
        }
*/
        delete[] re;
        delete[] com;
        delete[] output;
    }
    outFile->~WavOutFile();
}

int main(int argc, char* argv[])
{
    ParseParameter *param;
    WavInFile *inFile;
    WavOutFile *outFile;

    if (argc < 4)
    {
        cout << "Usage: " << argv[0] << "inputfile outputfile [option]" << endl;
        return 0;
    }

    param = new ParseParameter(argc, argv);

    openFiles(&inFile, &outFile, param);

    analyze(inFile, outFile);


    return 0;
}

