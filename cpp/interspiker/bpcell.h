#ifndef BPCELL_H
#define BPCELL_H

#include <vector>

#include "precompexp.h"

class BPCell
{
public:
    BPCell();
    virtual ~BPCell();

    void add(const double v, const timestamp t, const PrecompExp * pExp);

    /* DEBUG */
    inline timestamp get_last_t() const { return last_t; }

    double compute(const timestamp t, const PrecompExp *pExp) const;
    std::vector<double> rangeCompute(const timestamp begin_t, const timestamp delta_t, const timestamp end_t, const PrecompExp *pExp) const;

private:
    double u;
    double v;
    timestamp last_t;
};

#endif // BPCELL_H
