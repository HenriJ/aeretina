#include "fileevent2dreader.h"

using namespace std;

FileEvent2dReader::FileEvent2dReader(const char * path, unsigned int discard, int format)
    : format(format)
{
    file.open(path);

    if (!file.is_open()) {
        cout << "Unable to open " << path << endl;
        terminate();
    }

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

bool FileEvent2dReader::hasNext(bool sync)
{
    file.peek();

    if (file.eof()) {
        return false;
    }

    if (sync) {
        Event2d e = peekEvent2d();
        timestamp delta = e.t-last_t;
        if (delta > 0) {
            usleep(e.t-last_t);
        }
    }

    return true;
}

Event2d FileEvent2dReader::rawEvent2d()
{
    char * buffer = new char[8];
    file.read(buffer, 8);

    if (format == 0)
        return bufferToEvent2d(buffer);
    else
        return atisToEvent2d(buffer);
}

Event2d FileEvent2dReader::readEvent2d()
{
    char * buffer = new char[8];
    file.read(buffer, 8);

    Event2d e = (format == 0) ? bufferToEvent2d(buffer) : atisToEvent2d(buffer);

    if (e.t < last_t) {
        shift_t += last_t;
    }

    last_t = e.t;

    e.t += shift_t;

    return e;
}

Event2d FileEvent2dReader::peekEvent2d()
{
    Event2d e = readEvent2d();

    file.seekg(-8, ios::cur);

    return e;
}
