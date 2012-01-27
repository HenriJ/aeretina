#include "fileevent2dreader.h"

FileEvent2dReader::FileEvent2dReader(const char * path)
{
    file.open(path);
    while (file.peek() == '#') {
        while(file.get() != '\n') {}
    }

//    while (hasNext()) {
//        Event2d e = readEvent2d();
//        if (e.t < 10000) {
//            break;
//        }
//    }
}

bool FileEvent2dReader::hasNext()
{
    file.peek();
    return !file.eof();
}

Event2d FileEvent2dReader::readEvent2d()
{
    char * buffer = new char[8];
    file.read(buffer, 8);

    return bufferToEvent2d(buffer);
}
