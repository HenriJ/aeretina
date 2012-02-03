#ifndef BPLAYER_H
#define BPLAYER_H

#include <vector>

#include "bpevent.h"
#include "bpcell.h"

class BPLayer
{
public:
    BPLayer(unsigned int width);
    virtual ~BPLayer();

    inline BPCell & c(unsigned int x, unsigned int y) { return bp[y * width + x]; }

private:
    unsigned int width;
    BPCell * bp;
};

#endif // BPLAYER_H
