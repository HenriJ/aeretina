#include "precompexp.h"

#include <cmath>
#include <climits>
#include <assert.h>

PrecompExp::PrecompExp(double tau)
{
    if (tau == 0) {
        size = 0;
        precomp = new double[1];
    } else {
        size = ceil(10 * tau);

        assert(size < INT_MAX);

        precomp = new double[size];
        for (int t = 0; t < size; ++t) {
            precomp[t] = exp(-t / tau);
        }
    }
}

PrecompExp::~PrecompExp()
{
    delete[] precomp;
}


