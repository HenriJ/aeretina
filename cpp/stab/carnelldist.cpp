#include "carnelldist.h"

timestamp abs(timestamp t) {
    return (t>=0) ? t : -t;
}

CarnellDist::CarnellDist(timestamp tau)
{
    exp = new PrecompExp(tau);
}

CarnellDist::~CarnellDist()
{
    delete exp;
}

double CarnellDist::dist(Spikes a, Spikes b)
{
    double d = 0;

    int la = a.length();
    int lb = b.length();

    for (int i = 1; i < la; ++i) {
        for (int j = 1; j < la; ++j) {
            d = d + exp->val(abs(a(i) - a(j)));
        }
    }

    for (int i = 1; i < lb; ++i) {
        for (int j = 1; j < lb; ++j) {
            d = d + exp->val(abs(b(i) - b(j)));
        }
    }

    for (int i = 1; i < la; ++i) {
        for (int j = 1; j < lb; ++j) {
            d = d - 2 * exp->val(abs(a(i) - b(j)));
        }
    }

    return d;
}
