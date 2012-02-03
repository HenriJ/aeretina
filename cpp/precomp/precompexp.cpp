#include "precompexp.h"

#include <cmath>

PrecompExp::PrecompExp(int max, double tau)
    : max(max)
{
    precomp = new double[max+1];
    for (int t = 0; t <= max; ++t) {
        precomp[t] = exp(-t / tau);
    }
}

PrecompExp::~PrecompExp()
{
    delete[] precomp;
}


