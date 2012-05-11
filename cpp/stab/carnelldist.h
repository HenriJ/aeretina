#ifndef CARNELLDIST_H
#define CARNELLDIST_H

#include "../aer/timestamp.h"
#include "../aer/precompexp.h"

#include "spikes.h"

class CarnellDist
{
public:
    CarnellDist(timestamp tau);
    ~CarnellDist();

    double dist(Spikes a, Spikes b);

private:
    PrecompExp * exp;
};

#endif // CARNELLDIST_H
