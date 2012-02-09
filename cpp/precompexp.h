#ifndef PRECOMPEXP_H
#define PRECOMPEXP_H

#include <iostream>
#include <assert.h>

#include "common/timestamp.h"

class PrecompExp
{
public:
    PrecompExp(double tau);
    ~PrecompExp();
    inline double val(timestamp t) const {
        assert(t >= 0);
        if (t >= size) {
            return 0;
        }
        return precomp[t];
    }

private:
    double * precomp;
    timestamp size;
};

#endif // PRECOMPEXP_H
