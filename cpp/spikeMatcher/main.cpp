#include <iostream>
#include <sstream>
#include <string>

#include "../aer/doublemat.h"
#include "meareader.h"

int main(int argc, char *argv[])
{
    DoubleMat ** templates = new DoubleMat* [190];

    string staticpath = "/home/riton/marre/templates/t";

    for (int t = 1; t < 190; ++t) {
        stringstream pathstream;
        pathstream << staticpath << t;
        templates[t] = new DoubleMat(pathstream.str());
    }

    MeaReader mr("/home/riton/marre/barmovie.filtered", 32767);

    int size = 10000;

    int * data = new int[size];
    mr.read(0, size, data);

    for (int i = 0; i < size; ++i) {
        cout << data[i] << endl;
    }
    
    return 0;
}
