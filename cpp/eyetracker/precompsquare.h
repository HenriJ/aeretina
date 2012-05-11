#ifndef PRECOMPSQUARE_H
#define PRECOMPSQUARE_H

#include <assert.h>
#include <math.h>

class PrecompSquare
{
public:
    PrecompSquare(int size);
    ~PrecompSquare();

    inline double val(int t) const {
        assert(t <= size_);
        return precomp[t];
    }

private:
    double * precomp;
    int size_;
};

#endif // PRECOMPSQUARE_H
