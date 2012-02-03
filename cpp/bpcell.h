#ifndef BPCELL_H
#define BPCELL_H

#include <vector>

#include "precomp/precompexp.h"
#include "precomp/precomppropexp.h"

#include "bpevent.h"

class BPCell
{
public:
    BPCell();
    void init(unsigned int maxSize = 2500);
    virtual ~BPCell();

    void add(const BPEvent e);
    void clean(timestamp oldT);

    unsigned int size() const;

    double compExp(double t, PrecompExp * pExp);
    double compute(double t, PrecompPropExp * pPropExp);
    std::vector<double> rangeCompute(timestamp start_t, timestamp end_t, timestamp delta_t,
                                     PrecompExp * pExp, PrecompPropExp * pPropExp);

private:
    unsigned int maxSize;
    unsigned int size_;
    unsigned int a;
    unsigned int b;
    BPEvent * events;
};

#endif // BPCELL_H
