#include "bplayer.h"

#include <iostream>

BPLayer::BPLayer(int width, int rgx, int rgy, timestamp tau, DoubleMat bipolar, DoubleMat ganglion)
    : width(width), pixels(width*width), rgx(rgx), rgy(rgy),
      bipolar(bipolar), ganglion(ganglion)
{
    if (tau == 0) {
        enabled = false;
    } else {
        invtausquare = 1./(tau*tau);
        pExp = new PrecompExp(tau);

        u = new double[pixels];
        v = new double[pixels];
        last_t = 0;

        enabled = true;
    }
}

BPLayer::~BPLayer()
{
    if (enabled) {
        delete[] u;
        delete[] v;
        delete pExp;
    }
}
