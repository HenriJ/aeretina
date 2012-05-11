#include "meareader.h"

#include "../aer/doublemat.h"

using namespace std;

MeaReader::MeaReader(const char * path, int offset)
    : offset(offset)
{
    file.open(path);

    if (!file.is_open()) {
        cout << "Unable to open " << path << endl;
        terminate();
    }

    int pos = 0;
    int step = 0;

    while(true) {
        ++pos;

        if (file.eof() || pos > 10000) {
            cout << "Not a mea file" << endl;
            terminate();
        }

        char * buffer = new char[1];
        file.read(buffer, 1);

        if (*buffer == 'E') {
            step = 1;
        }
        else if (step == 1 && *buffer == 'O') {
            step = 2;
        }
        else if (step == 2 && *buffer == 'H') {
            break;
        }
    }

    start = pos+2;
}

void MeaReader::read(int begin, int size, int * data)
{
    file.seekg(begin+start, ios_base::beg);

    char * buffer = new char[2];

    for (int i = 0; i < size; ++i) {
        file.read(buffer, 2);

        unsigned int part0 = 0x000000FF&buffer[0];
        unsigned int part1 = 0x000000FF&buffer[1];

        int temp = (part1<<8)|(part0);

        data[i] = temp - offset;
    }
}
