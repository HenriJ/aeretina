#ifndef FILEEVENT2DREADER_H
#define FILEEVENT2DREADER_H

#include "event2dreader.h"

class FileEvent2dReader : public Event2dReader
{
public:
    FileEvent2dReader(char * path);
    virtual Event2d readEvent2d();
    virtual bool hasNext();

private:
    std::ifstream file;
};

#endif // FILEEVENT2DREADER_H
