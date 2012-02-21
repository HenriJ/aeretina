#include "bpcell.h"

#include <iostream>
#include "misc.h"

BPCell::BPCell()
{
    u = 0;
    v = 0;
    last_t = -999999;
}

BPCell::~BPCell()
{
}

void BPCell::add(const double e_v, const timestamp e_t, const PrecompExp * pExp)
{
    assert(e_t >= last_t);

    double delta_t = e_t - last_t;
    double expd = pExp->val(delta_t);

    v = expd * (v + delta_t * u);
    u = expd * u + e_v;

    last_t = e_t;
}


double BPCell::compute(const timestamp t, const PrecompExp * pExp) const
{
    assert(t >= last_t);

    double val = v;

    if (t != last_t) {
        double delta_t = t - last_t;
        double expd = pExp->val(delta_t);

        val = expd * (v + delta_t * u);
    }

    return (val > 0) ? val : 0;
}

std::vector<double> BPCell::rangeCompute(const timestamp begin_t, const timestamp delta_t, const timestamp end_t, const PrecompExp *pExp) const
{
    assert(begin_t >= last_t);

    std::vector<double> vals((end_t-begin_t)/delta_t);
    double expd = pExp->val(delta_t);

    double loc_v = v;
    double loc_u = u;

    unsigned int i = 0;
    for (timestamp t = begin_t; t < end_t; t += delta_t) {
        vals[i] = (loc_v > 0) ? loc_v : 0;
        ++i;
        if (abs(vals[i] - compute(t, pExp)) > 0) std::cout << abs(vals[i] - compute(t, pExp)) << std::endl;
        loc_v = expd * (loc_v + delta_t * loc_u);
        loc_u = expd * loc_u;
    }

    return vals;
}
