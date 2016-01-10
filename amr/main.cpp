#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        cout << "Usage: " << argv[0] << "inputfile outputfile [option]" << endl;
        return 0;
    }
    cout << "Hello World!" << endl;
    return 0;
}

