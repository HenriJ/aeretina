#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <cmath>
#include <cstdlib>

#include <boost/timer.hpp>

#include "ini/cpp/INIReader.h"

#include "../aer/event2d.h"
#include "../aer/fileevent2dreader.h"
#include "../aer/dummyevent2dreader.h"

#include "precompexp.h"
#include "doublemat.h"

#include "bplayer.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 5) {
        cout << "Usage: ./rgcell ParameterFile EventsFiles RGCellX RGCellY [MaximumTick]" << endl;
        cout << "Example: ./rgcell OnBeta/OnBeta.params events.aedat 42 42" << endl;
        terminate();
    }


    /*
     * Simulation parameters
     */
    int width = 128;

    {
        ifstream confFile(argv[1]);
        if (!confFile.is_open()) {
            cout << "Unable to open parameters file : " << argv[1] << endl;
            terminate();
        }
    }
    INIReader conf(argv[1]);

    timestamp maxTick = 1000;
    if (argc >= 6)
        maxTick = atol(argv[5]);


    /*
     * Input data
     */
    FileEvent2dReader reader(argv[2]);


    /*
     * RGCell position
     */
    unsigned char rgx = atoi(argv[3]);
    unsigned char rgy = atoi(argv[4]);


    /*
      Bipolar layers
     */
    BPLayer * B1 = new BPLayer(width, rgx, rgy, conf.GetInteger("B1", "tau", 0),
                               DoubleMat(conf.Get("B1", "bipolar", "unspecified B1 bipolar matrix")),
                               DoubleMat(conf.Get("B1", "ganglion", "unspecified B1 ganglion matrix")));
    BPLayer * B2 = new BPLayer(width, rgx, rgy, conf.GetInteger("B2", "tau", 0),
                               DoubleMat(conf.Get("B2", "bipolar", "unspecified B2 bipolar matrix")),
                               DoubleMat(conf.Get("B2", "ganglion", "unspecified B2 ganglion matrix")));
    BPLayer * B3 = new BPLayer(width, rgx, rgy, conf.GetInteger("B3", "tau", 0),
                               DoubleMat(conf.Get("B3", "bipolar", "unspecified B3 bipolar matrix")),
                               DoubleMat(conf.Get("B3", "ganglion", "unspecified B3 ganglion matrix")));
    BPLayer * B4 = new BPLayer(width, rgx, rgy, conf.GetInteger("B4", "tau", 0),
                               DoubleMat(conf.Get("B4", "bipolar", "unspecified B4 bipolar matrix")),
                               DoubleMat(conf.Get("B4", "ganglion", "unspecified B4 ganglion matrix")));


    unsigned event_index = 0;
    timestamp t = 0;

    timestamp last_cout = 0;

    timestamp d1 = conf.GetInteger("B1", "delay", 0);
    timestamp d2 = conf.GetInteger("B2", "delay", 0);
    timestamp d3 = conf.GetInteger("B3", "delay", 0);
    timestamp d4 = conf.GetInteger("B4", "delay", 0);

    queue<Event2d> queue1;
    queue<Event2d> queue2;
    queue<Event2d> queue3;
    queue<Event2d> queue4;

    double temp1;
    double temp2;
    double temp3;
    double temp4;

    double g;

    //boost::timer tic;

    while (true) {

        // Fill queues
        if (queue1.size() == 0 || queue2.size() == 0 || queue3.size() == 0 || queue4.size() == 0) {
            if (reader.hasNext()) {
                Event2d e = reader.readEvent2d();
                ++event_index;

                queue1.push(e.shifted(d1));
                queue2.push(e.shifted(d2));
                queue3.push(e.shifted(d3));
                queue4.push(e.shifted(d4));
            }
        }


        // End loop when no more events
        if (queue1.empty() && queue2.empty() && queue3.empty() && queue4.empty()) {
            break;
        }


        // Find next tick
        t += maxTick;
        if (!queue1.empty() && queue1.front().t < t) {
            t = queue1.front().t;
        }
        if (!queue2.empty() && queue2.front().t < t) {
            t = queue2.front().t;
        }
        if (!queue3.empty() && queue3.front().t < t) {
            t = queue3.front().t;
        }
        if (!queue4.empty() && queue4.front().t < t) {
            t = queue4.front().t;
        }


        // Compute
        g = 0;

        if (t == queue1.front().t) {
            temp1 = B1->feed(queue1.front());
            queue1.pop();
        } else {
            temp1 = B1->feed(t);
        }

        if (t == queue2.front().t) {
            temp2 = B2->feed(queue2.front());
            queue2.pop();
        } else {
            temp2 = B2->feed(t);
        }

        if (t == queue3.front().t) {
            temp3 = B3->feed(queue3.front());
            queue3.pop();
        } else {
            temp3 = B3->feed(t);
        }

        if (t == queue4.front().t) {
            temp4 = B4->feed(queue4.front());
            queue4.pop();
        } else {
            temp4 = B4->feed(t);
        }

        g += temp1 - temp2 - temp3 + temp4;


        // Output
        if (t - last_cout >= maxTick) {
            cout << t << ", " << g << endl;
            last_cout = t;
        }

    }

    //cout << "# " << event_index / tic.elapsed() << "e/s" << endl;

    return 0;
}
