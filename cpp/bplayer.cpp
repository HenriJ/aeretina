#include "bplayer.h"

BPLayer::BPLayer(unsigned int width)
    : width(width)
{
    bp = new BPCell[width*width];
}

BPLayer::~BPLayer()
{
    delete[] bp;
}

BPCell & BPLayer::c(unsigned int x, unsigned int y)
{
    return bp[y * width + x];
}
