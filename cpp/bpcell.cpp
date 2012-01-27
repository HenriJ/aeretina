#include "bpcell.h"

#include <cmath>
#include <iostream>

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

void BPCell::add(BPEvent e)
{
    events[b] = e;
    ++b;
    if (b >= maxSize) {
        b = 0;
    }
    ++size_;
    if (size_ > maxSize) {
        std::cout << size_ << "\n";
        std::cout << "BPCell overflow\n";
    }
}

void BPCell::clean(unsigned int oldT)
{
    int k = 0;
    while (events[k].t <= oldT && a != b) {
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
