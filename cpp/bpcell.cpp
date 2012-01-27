#include "bpcell.h"

#include <cmath>
#include <iostream>
#include "misc.h"

BPCell::BPCell()
{
    maxSize = 5000;
    size_ = 0;
    events = new BPEvent[maxSize];
    a = 0;
    b = 0;
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

void BPCell::clean(unsigned int oldT)
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

double BPCell::compExp(double t)
{
    double sum = 0;

    for (unsigned int i = a; i < b; ++i) {
        if (i == maxSize) {
            i = 0;
        }
        BPEvent & e = events[i];
        sum += e.v * cachedExp(t - e.t, 20000);
    }

    return sum;
}

double BPCell::compute(double t)
{
    double sum = 0;

    for (unsigned int i = a; i < b; ++i) {
        if (i == maxSize) {
            i = 0;
        }
        BPEvent & e = events[i];
        sum += e.v * cachedPropExp(t - e.t, 20000);
    }

    return sum;
}

std::vector<double> BPCell::rangeCompute(unsigned int start_t, unsigned int end_t, unsigned int delta_t)
{
    unsigned int iters = (end_t - start_t) / delta_t;
    std::vector<double> vals(iters);

    double delta = double(delta_t)/20000;
    double K = exp(-delta);

    double B = compute(start_t);
    double A = compExp(start_t);

    vals[0] = B;

    for (unsigned int k = 1; k < iters; ++k) {
        B = K * (B + delta * A);
        vals[k] = B;
        A *= K;
    }

    return vals;
}
