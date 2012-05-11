/*
 * Usage:
 *
 * EventDisplay disp(128, 10000);
 * disp.feed(event);
 *
 */

#ifndef EVENTDISPLAY_H
#define EVENTDISPLAY_H

#include "precompexp.h"
#include "event2d.h"

#include <thread>

class EventDisplay
{
public:
    EventDisplay(int width, int tau);

    void feed(Event2d e);
    void time(timestamp t);

private:
    int width;
    int pixels;

    PrecompExp * exp;

    double * ON;
    double * OFF;

    timestamp * ON_times;
    timestamp * OFF_times;

    timestamp last_t;

    std::thread * renderThread;
    std::mutex timeLock;

     void render(void);
};

#endif // EVENTDISPLAY_H
