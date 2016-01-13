#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ParseParameter.h"
#include "WaveFile.h"
#include "Transform.h"

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

void analyze(WavInFile *inFile)
{
    Transform test;

    int size;

    while (inFile->eof() == 0)
    {
        int windowSize = 4096;
        double *re = new double[windowSize];
        memset(re, 0, sizeof(double)*windowSize);

        complex *com = new complex[windowSize];
        complex *output = new complex[windowSize];

        size = inFile->read(re, windowSize);
        int nSamples = size / (int)inFile->getNumChannels();

        for (int i = 0; i < size; i++)
        {
            com[i] = re[i];
        }

        cout << "\n\n\n\n\nbefore transform" << endl;
        for (int i = 0; i < size; i++)
        {
            cout << "real: " << com[i].re() << " image: " << com[i].im() << endl;

        }



        test.Forward(com, output, nSamples);



        cout << "\n\n\n\n\nafter transform" << endl;
        for (int i = 0; i < size; i++)
        {
            cout << "real: " << output[i].re() << " image: " << output[i].im() << endl;
        }

        delete[] re;
        delete[] com;
        delete[] output;
    }
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

    analyze(inFile);

    return 0;
}

