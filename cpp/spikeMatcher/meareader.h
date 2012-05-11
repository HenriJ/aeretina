#ifndef MEAREADER_H
#define MEAREADER_H

#include <iostream>
#include <fstream>

class MeaReader
{
public:
    MeaReader(const char * path, int offset);
    void read(int begin, int size, int * data);

private:
    std::ifstream file;
    int offset;
    int start;

};

#endif // MEAREADER_H
