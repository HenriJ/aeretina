#include "bpcell.h"

#include <iostream>
#include "misc.h"

BPCell::BPCell()
{
    size_ = 0;
    a = 0;
    b = 0;
}

void BPCell::init(unsigned int maxSize)
{
    this->maxSize = maxSize;
    events = new BPEvent[maxSize];
}

BPCell::~BPCell()
{
    delete[] events;
}

void BPCell::add(const BPEvent e)
{
    events[b] = e;
    ++b;
    if (b >= maxSize) {
        b = 0;
    }
    ++size_;
    if (size_ > maxSize - 1) {
        std::cout << size_ << "\n";
        std::cout << "BPCell overflow\n";
    }
}

void BPCell::clean(timestamp oldT)
{
    while (events[a].t <= oldT && a!= b) {
        ++a;
        if (a >= maxSize) {
            a = 0;
        }
        --size_;
    }
}

unsigned int BPCell::size() const
{
    return size_;
}

double BPCell::compExp(double t, PrecompExp * pExp)
{
    double sum = 0;

    for (unsigned int i = a; i < b; ++i) {
        if (i == maxSize) {
            i = 0;
        }
        BPEvent & e = events[i];
        sum += e.v * pExp->val(t - e.t);
    }

    return sum;
}

double BPCell::compute(double t, PrecompPropExp * pPropExp)
{
    double sum = 0;

    for (unsigned int i = a; i < b; ++i) {
        if (i == maxSize) {
            i = 0;
        }
        BPEvent & e = events[i];
        sum += e.v * pPropExp->val(t - e.t);
    }

    return sum;
}

std::vector<double> BPCell::rangeCompute(timestamp start_t, timestamp end_t, timestamp delta_t,
                                         PrecompExp * pExp, PrecompPropExp * pPropExp)
{
    unsigned int iters = (end_t - start_t) / delta_t;
    std::vector<double> vals(iters);

    double delta = double(delta_t)/20000;
    double K = exp(-delta);

    double B = compute(start_t, pPropExp);
    double A = compExp(start_t, pExp);

    vals[0] = B;

    for (unsigned int k = 1; k < iters; ++k) {
        B = K * (B + delta * A);
        vals[k] = std::max(B, 0.);
        A *= K;
    }

    return vals;
}
