/* Event2d
*
* Maintainer : Henri Jouhaud <henri.jouhaud@polytechnique.org>
*
* Event2d represents a tuple (x, y, p, t)
*/

#ifndef EVENT2D_H
#define EVENT2D_H

#include <iostream>
#include "timestamp.h"

class Event2d
{
public:
    unsigned short x;
    unsigned short y;
    short p;
    timestamp t;

    inline Event2d() {}
    inline Event2d(unsigned short x, unsigned short y, short p, timestamp t)
        : x(x), y(y), p(p), t(t) {}

    inline Event2d shifted(timestamp dt)
    {
        return Event2d(x, y, p, t+dt);
    }

    inline bool operator<(const Event2d &e) const {
        return t < e.t;
    }

    inline bool operator<=(const Event2d &e) const {
        return t <= e.t;
    }

    inline bool operator>(const Event2d &e) const {
        return t > e.t;
    }

    inline bool operator>=(const Event2d &e) const {
        return t >= e.t;
    }

    friend std::ostream& operator<< (std::ostream& output, const Event2d& e)
    {
        output << "Event2d: (";
        output << (int) e.x << ", " << (int) e.y << ", ";
        output << (int) e.p << ", " << e.t;
        output << ")";
        return output;
    }
};

#endif // EVENT2D_H
