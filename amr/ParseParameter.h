#ifndef PARSEPARAMETER_H
#define PARSEPARAMETER_H

#include <string>

using namespace std;

class ParseParameter
{
public:
    double pitch;
    char *inFileName;
    char *outFileName;

    char *getInFileName() const;
    void setInFileName(char *value);
    char *getOutFileName() const;
    void setOutFileName(char *value);
    double getPitch() const;
    void setPitch(double value);

    ParseParameter(const int argc, const char* const argv[]);

    void parseSwitchParam(const string &str);
    double parseOption(const string &str) const;
    void checkLimits();
};


#endif // PARSEPARAMETER_H
