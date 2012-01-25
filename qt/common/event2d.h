#ifndef EVENT2D_H
#define EVENT2D_H

#include <iostream>

class Event2d
{
public:
    unsigned char x;
    unsigned char y;
    char p;
    unsigned int t;

    Event2d(unsigned char x, unsigned char y,
             char p, unsigned int t);
};

std::ostream& operator<<(std::ostream& output, const Event2d& mat);

#endif // EVENT2D_H
