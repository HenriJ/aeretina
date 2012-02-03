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
    unsigned char x;
    unsigned char y;
    char p;
    timestamp t;

    Event2d(unsigned char x, unsigned char y,
             char p, timestamp t);
};

std::ostream& operator<<(std::ostream& output, const Event2d& mat);

#endif // EVENT2D_H
