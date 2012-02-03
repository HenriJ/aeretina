#include "precomppropexp.h"

#include <cmath>

PrecompPropExp::PrecompPropExp(int max, double tau)
    : max(max)
{
    precomp = new double[max+1];

    for (int t = 0; t <= max; ++t) {
        double temp = t / tau;
        precomp[t] = temp * exp(-temp);
    }
}

PrecompPropExp::~PrecompPropExp()
{
    delete[] precomp;
}
