#include "dummyevent2dreader.h"

unsigned char rand_char(unsigned char max) {
    return (unsigned char) (((double) rand() / RAND_MAX) * max);
}

DummyEvent2dReader::DummyEvent2dReader(int spatialSize, int limit)
    : spatialSize(spatialSize), limit(limit), iter(0)
{
    srand(42); // For the repetability
}

bool DummyEvent2dReader::hasNext()
{
    return (iter < limit);
}

Event2d DummyEvent2dReader::readEvent2d()
{
    ++iter;

    unsigned char x = rand_char(spatialSize);
    unsigned char y = rand_char(spatialSize);
    char p = rand_char(2) * 2 - 1;

    return Event2d(x, y, p, iter);
}
