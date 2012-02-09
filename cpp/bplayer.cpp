#include "bplayer.h"

using namespace std;

BPLayer::BPLayer(unsigned int width)
    : width(width)
{
    bp = new BPCell[width*width];
}

BPLayer::~BPLayer()
{
    delete[] bp;
}
