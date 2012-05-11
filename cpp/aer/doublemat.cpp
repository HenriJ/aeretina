#include "doublemat.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <vector>
#include <math.h>

using namespace std;

DoubleMat::DoubleMat(const DoubleMat & dm)
{
    width_ = dm.width_;
    size_  = dm.size_;
    half_width_ = dm.half_width_;

    ref = dm.ref;
    mat = dm.mat;

    ++(*ref);
}

DoubleMat & DoubleMat::operator= (const DoubleMat & dm)
{
    if (this != &dm) // protect against invalid self-assignment
    {
        width_ = dm.width_;
        size_  = dm.size_;
        half_width_ = dm.half_width_;

        ref = dm.ref;
        mat = dm.mat;

        ++(*ref);
    }
    return *this;
}

DoubleMat::DoubleMat(int width)
    : width_(width), half_width_(width/2), size_(width*width)
{
    ref = new int;
    mat = new double[size_];
}

void DoubleMat::init(const char * path)
{
    ref = new int;

    ifstream file;
    file.open(path);

    if (!file.is_open()) {
        cout << "Unable to open " << path << endl;
        terminate();
    }

    vector<double> v;

    char * buffer = new char[32];
    unsigned int pos = 0;
    while (!file.eof()) {
        char c = file.get();
        if (c == 'e' || c == 'E' || (c >= 48 && c <= 57) || c == '-' || c == '.') { // If a digit
            buffer[pos] = c;
            ++pos;
        } else {
            if (pos > 0) {
                buffer[pos] = '\0';
                v.push_back(atof(buffer));
                pos = 0;
            }
        }
    }

    mat = new double[v.size()];
    size_ = v.size();
    width_ = sqrt(size_);
    half_width_ = width_/2;
    for (unsigned int i = 0; i < v.size(); ++i) {
        mat[i] = v[i];
    }
}

DoubleMat::~DoubleMat()
{
    --ref;
    if (ref==0) {
        delete[] mat;
    }
}
