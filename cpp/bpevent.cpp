#include "bpevent.h"

BPEvent::BPEvent()
    : v(0), t(0)
{
}

BPEvent::BPEvent(double v, unsigned int t)
    : v(v), t(t)
{
}
