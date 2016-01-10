#include <iostream>
#include "ParseParameter.h"

using namespace std;

int main(int argc, char* argv[])
{
    ParseParameter *param;

    if (argc < 4)
    {
        cout << "Usage: " << argv[0] << "inputfile outputfile [option]" << endl;
        return 0;
    }

    param = new ParseParameter(argc, argv);


    return 0;
}

