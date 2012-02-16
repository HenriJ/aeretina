/* DummyEvent2dReader
*
* Maintainer : Henri Jouhaud <henri.jouhaud@polytechnique.org>
*
* A convenient way to generate random events
*/

#ifndef DUMMYEVENT2DREADER_H
#define DUMMYEVENT2DREADER_H

#include <cstdlib>
#include "event2dreader.h"

class DummyEvent2dReader : public Event2dReader
{
public:
    DummyEvent2dReader(int spatialSize = 128, int limit = 1000000);
    virtual Event2d readEvent2d();
    virtual bool hasNext();

private:
    int spatialSize;
    int limit;
    int iter;
};

#endif // DUMMYEVENT2DREADER_H
