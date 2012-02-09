#include "bpcell.h"

#include <iostream>
#include "misc.h"

BPCell::BPCell()
{
    u = 0;
    v = 0;
    last_t = 0;
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
