#ifndef PRECOMPPROPEXP_H
#define PRECOMPPROPEXP_H

#include <iostream>

class PrecompPropExp
{
public:
    PrecompPropExp(int max, double tau);
    ~PrecompPropExp();
    inline double val(int t) const {
        if (t > max) {
            std::cout << "PrecompPropExp overflow, asking for " << t << " > " << max << "\n";
            return 0;
        }
        return precomp[t];
    }

private:
    double * precomp;
    int max;
};

#endif // PRECOMPPROPEXP_H
