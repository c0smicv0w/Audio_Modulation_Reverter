#ifndef PARSEPARAMETER_H
#define PARSEPARAMETER_H

class ParseParameter
{
private:
    char *inFileName = NULL;
    char *outFileName = NULL;
    double pitch = 0.0;

public:
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


char *ParseParameter::getOutFileName() const
{
    return outFileName;
}

void ParseParameter::setOutFileName(char *value)
{
    outFileName = value;
}

double ParseParameter::getPitch() const
{
return pitch;
}

void ParseParameter::setPitch(double value)
{
pitch = value;
}
char *ParseParameter::getInFileName() const
{
    return inFileName;
}

void ParseParameter::setInFileName(char *value)
{
    inFileName = value;
}
