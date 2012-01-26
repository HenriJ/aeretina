#include "bpcell.h"

BPCell::BPCell()
{
    maxSize = 200;
    events = new BPEvent[200];
    a = 0;
    b = 1;
}

BPCell::~BPCell()
{
    delete[] events;
}

void BPCell::add(BPEvent e)
{
    events[a] = e;
    ++a;
    if (a >= maxSize) {
        a = 0;
    }
}

void BPCell::clean(unsigned int oldT)
{
//    std::vector<BPEvent>::iterator it = events->begin();
//    for (; it < events->end(); ++it) {
//        if ((*it).t > oldT) {
//            break;
//        }
//    }
//    if (it != events->begin()) {
//        events->erase(events->begin(), it);
//    }
}

unsigned int BPCell::size() const
{
    return b-a;
}
