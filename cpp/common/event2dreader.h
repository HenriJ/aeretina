/* Event2dReader
*
* Maintainer : Henri Jouhaud <henri.jouhaud@polytechnique.org>
*
* Event2dReader is an abstract class representing a way to read event from a source
*/

#ifndef EVENT2DREADER_H
#define EVENT2DREADER_H

#include <iostream>
#include <fstream>
#include "event2d.h"

class Event2dReader
{
public:
    Event2dReader();
    virtual ~Event2dReader();

    // Returns the next event
    virtual Event2d readEvent2d() = 0;

    // Blocking, returns true if there is a next event
    virtual bool hasNext() = 0;

protected:
    // Helper function to convert an 8 bytes buffer to an event object
    Event2d bufferToEvent2d(char * buffer) const;
};

#endif // EVENT2DREADER_H
