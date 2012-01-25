#include "event2d.h"

Event2d::Event2d(unsigned char x, unsigned char y,
                   char p, unsigned int t)
                  : x(x), y(y), p(p), t(t)
{
}

std::ostream& operator<<(std::ostream& output, const Event2d& e)
{
    output << "Event2d: (";
    output << (int) e.x << ", " << (int) e.y << ", ";
    output << (int) e.p << ", " << e.t;
    output << ")";
    return output;
}
