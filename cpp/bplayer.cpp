#include "bplayer.h"

using namespace std;

BPLayer::BPLayer(unsigned int width)
    : width(width)
{
    bp = new BPCell[width*width];
    for (unsigned int i = 0; i < width * width; ++i) {
        bp[i].init();
    }
}

BPLayer::~BPLayer()
{
    delete[] bp;
}
