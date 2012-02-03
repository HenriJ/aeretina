#ifndef BPEVENT_H
#define BPEVENT_H

#include "common/timestamp.h"

class BPEvent
{
public:
    double v;
    timestamp t;

    BPEvent();
    BPEvent(double v, timestamp t);
};

#endif // BPEVENT_H
