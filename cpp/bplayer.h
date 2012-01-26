#ifndef BPLAYER_H
#define BPLAYER_H

#include "bpevent.h"
#include "bpcell.h"

class BPLayer
{
public:
    BPLayer(unsigned int width);
    virtual ~BPLayer();

    BPCell & c(unsigned int x, unsigned int y);

private:
    unsigned int width;
    BPCell * bp;
};

#endif // BPLAYER_H
