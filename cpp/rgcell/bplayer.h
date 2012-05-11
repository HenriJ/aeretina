#ifndef BPCELL_H
#define BPCELL_H

#include "../aer/event2d.h"
#include "doublemat.h"
#include "precompexp.h"
#include <math.h>

class BPLayer
{
public:
    BPLayer(int width, int rgx, int rgy, timestamp tau, DoubleMat bipolar, DoubleMat ganglion);
    virtual ~BPLayer();

    inline double feed(timestamp t)
    {
        if (!enabled) {
            return 0;
        }
        assert(t >= last_t);

        timestamp delta_t = t - last_t;
        double expStep = pExp->val(delta_t);

        double g = 0;

        int x = 0;
        int y = 0;
        for (int k = 0; k < pixels; ++k) {
            v[k] = expStep * (v[k] + delta_t * u[k]);
            u[k] = expStep * u[k];

            if (v[k] > 0) {
                g += v[k];
            }

            ++x;
            if (x == width) {
                x = 0;
                ++y;
            }
        }

        last_t = t;

        return g;
    }

    inline double feed(const Event2d e)
    {
        if (!enabled) {
            return 0;
        }
        assert(e.t >= last_t);

        timestamp delta_t = e.t - last_t;
        double expStep = pExp->val(delta_t);

        double g = 0;

        int x = 0;
        int y = 0;
        for (int k = 0; k < pixels; ++k) {
            v[k] = expStep * (v[k] + delta_t * u[k]);
            u[k] = expStep * u[k] + invtausquare * e.p * bipolar.c(x-e.x, y-e.y) * ganglion.c(rgx-x, rgy-y);

            if (v[k] > 0) {
                g += v[k];
            }

            ++x;
            if (x == width) {
                x = 0;
                ++y;
            }
        }

        last_t = e.t;

        return g;
    }


private:
    bool enabled;

    int width;
    int pixels;

    int rgx;
    int rgy;

    double invtausquare;
    PrecompExp * pExp;

    DoubleMat bipolar;
    DoubleMat ganglion;

    double * u;
    double * v;
    timestamp last_t;
};

#endif // BPLAYER_H
