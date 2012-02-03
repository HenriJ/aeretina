#ifndef PRECOMPEXP_H
#define PRECOMPEXP_H

#include <iostream>

class PrecompExp
{
public:
    PrecompExp(int max, double tau);
    ~PrecompExp();
    inline double val(int t) const {
        if (t > max) {
            std::cout << "PrecompExp overflow, asking for " << t << " > " << max << "\n";
            return 0;
        }
        return precomp[t];
    }

private:
    double * precomp;
    int max;
};

#endif // PRECOMPEXP_H
