#include <stdexcept>
#include "ParseParameter.h"

using namespace std;

// Constructor
ParseParameter::ParseParameter(const int argc, const char * const argv[])
{
    setInFileName((char*)argv[1]);
    setOutFileName((char*)argv[2]);

}

static int _toLowerCase(int c)
{
    if (c >= 'A' && c <= 'Z')
    {
        c += 'a' - 'A';
    }
    return c;
}

void ParseParameter::parseSwitchParam(const string &str)
{
    int opt;

    if (str[0] != '-')
    {
        string msg = "ERROR: Illegal parameter";
        std::runtime_error(msg.c_str());
    }

    opt = _toLowerCase(str[1]);

    switch (opt)
    {
    case 'p':
        // '-pitch=xx'
        setPitch(parseOption(str));
        break;
    default:
        string msg = "ERROR: Illegal parameter";
        std::runtime_error(msg.c_str());
    }

}

double ParseParameter::parseOption(const string &str) const
{
    int equal;

    equal = (int)str.find_first_of('=');
    if (equal < 0)
    {
        string msg = "ERROR: Illegal parameter";
        std::runtime_error(msg.c_str());
    }

    return (double)atof(str.substr(equal + 1).c_str());
}

void ParseParameter::checkLimits()
{
    double pitch;

    pitch = getPitch();

    if (pitch < -60.0)
    {
        setPitch(-60.0);
    }
    else if (pitch > 60.0)
    {
        setPitch(60.0);
    }

}
