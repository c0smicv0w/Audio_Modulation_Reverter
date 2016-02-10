#include <QFile>

#include "../src/audiomodulator.h"

int main(int argc, char* argv[])
{
    QFile srcFile;

    srcFile.setFileName(argv[1]);
    srcFile.open(QIODevice::ReadOnly);

    char* temp = new char[2048];

    srcFile.read(temp, 2048);

    AudioDataParam param{new QByteArray, new QList<complex>, new QByteArray, new QList<complex>};

    param.pcmIn->setRawData(temp, 2048);

    AudioModulator test;
    test.setPitch(0);
    test.pitchShift(param);

    srcFile.close();

    QFile dstFile;

    dstFile.setFileName(argv[2]);
    dstFile.open(QIODevice::WriteOnly);

    dstFile.write(param.pcmOut->data(), 2048);

    dstFile.close();

    return 0;
}
