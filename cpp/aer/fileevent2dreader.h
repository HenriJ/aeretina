/* FileEvent2dReader
*
* Maintainer : Henri Jouhaud <henri.jouhaud@polytechnique.org>
*
* This class allows you to sequentially read the events from an aer dump file
*/

#ifndef FILEEVENT2DREADER_H
#define FILEEVENT2DREADER_H

#include "event2dreader.h"

class FileEvent2dReader : public Event2dReader
{
public:
    /*
     * @path to the aer file
     * @discard number of events to discard at the begining of the file
     */
    FileEvent2dReader(const char * path, unsigned int discard = 0, int format = 0);

    Event2d rawEvent2d();
    virtual Event2d readEvent2d();
    virtual bool hasNext(bool sync = false);

    Event2d peekEvent2d();

private:
    std::ifstream file;
    int format;
    timestamp shift_t;
    timestamp last_t;
};

#endif // FILEEVENT2DREADER_H
