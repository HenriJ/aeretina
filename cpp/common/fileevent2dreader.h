/* FileEvent2dReader
*
* Maintainer : Henri Jouhaud <henri.jouhaud@polytechnique.org>
*
* This class allow you to sequentially read the events from an aer dump file
*/

#ifndef FILEEVENT2DREADER_H
#define FILEEVENT2DREADER_H

#include "event2dreader.h"

class FileEvent2dReader : public Event2dReader
{
public:
    // path to the aer file
    FileEvent2dReader(const char * path);

    virtual Event2d readEvent2d();
    virtual bool hasNext();

private:
    std::ifstream file;
};

#endif // FILEEVENT2DREADER_H
