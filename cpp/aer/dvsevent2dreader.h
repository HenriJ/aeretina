#ifndef DVSEVENT2DREADER_H
#define DVSEVENT2DREADER_H

#include "event2dreader.h"

#include <iostream>

class DvsEvent2dReader : public Event2dReader
{
public:
    DvsEvent2dReader();
    ~DvsEvent2dReader();

    void close();

    Event2d rawEvent2d();
    virtual Event2d readEvent2d();
    virtual bool hasNext();

private:
    std::fstream file;

    timestamp shift_t;
    timestamp last_t;
};

#endif // DVSEVENT2DREADER_H
