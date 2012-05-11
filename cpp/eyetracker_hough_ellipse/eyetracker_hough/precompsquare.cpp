#include "precompsquare.h"

PrecompSquare::PrecompSquare(int size)
    : size_(size)
{
    precomp = new double[size];
    for (int t = 0; t < size; ++t) {
        precomp[t] = t*t;
    }
}

PrecompSquare::~PrecompSquare()
{
    delete[] precomp;
}
