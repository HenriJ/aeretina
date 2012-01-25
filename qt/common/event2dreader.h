#ifndef EVENT2DREADER_H
#define EVENT2DREADER_H

#include <iostream>
#include <fstream>
#include "event2d.h"

class Event2dReader
{
public:
    Event2dReader();
    virtual Event2d readEvent2d() = 0;
    virtual bool hasNext() = 0;

protected:
    Event2d bufferToEvent2d(char * buffer);
};

#endif // EVENT2DREADER_H
