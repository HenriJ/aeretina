#ifndef BPCELL_H
#define BPCELL_H

#include <vector>
#include "bpevent.h"

class BPCell
{
public:
    BPCell();
    virtual ~BPCell();

    void add(const BPEvent e);
    void clean(unsigned int oldT);
    unsigned int size() const;

    double compExp(double t);
    double compute(double t);
    std::vector<double> rangeCompute(unsigned int start_t, unsigned int end_t, unsigned int delta_t);

private:
    unsigned int maxSize;
    unsigned int size_;
    unsigned int a;
    unsigned int b;
    BPEvent * events;
};

#endif // BPCELL_H
