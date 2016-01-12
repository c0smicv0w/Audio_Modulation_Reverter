#include <iostream>
#include <stdio.h>
#include <string.h>
#include "ParseParameter.h"
#include "WaveFile.h"

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


    return 0;
}

