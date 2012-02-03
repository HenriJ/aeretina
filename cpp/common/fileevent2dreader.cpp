#include "fileevent2dreader.h"

FileEvent2dReader::FileEvent2dReader(const char * path, unsigned int discard)
{
    file.open(path);

    // Discard file commentaries
    while (file.peek() == '#') {
        while(file.get() != '\n') {}
    }

    shift_t = 0;
    last_t = 0;

    // Discard first events as specified
    while (discard > 0 && hasNext()) {
        --discard;
        rawEvent2d();
    }

    Event2d e = rawEvent2d();
    shift_t = -e.t;
    last_t = e.t;
}

bool FileEvent2dReader::hasNext()
{
    file.peek();
    return !file.eof();
}

Event2d FileEvent2dReader::rawEvent2d()
{
    char * buffer = new char[8];
    file.read(buffer, 8);

    return bufferToEvent2d(buffer);
}

Event2d FileEvent2dReader::readEvent2d()
{
    char * buffer = new char[8];
    file.read(buffer, 8);

    Event2d e = bufferToEvent2d(buffer);

    if (e.t < last_t) {
        shift_t += last_t;
    }

    last_t = e.t;

    e.t += shift_t;

    return e;
}
