#include <iostream>
#include <stdlib.h>
#include <signal.h>

#include "../aer/dvsevent2dreader.h"
#include "../aer/fileevent2dreader.h"
#include "../aer/eventdisplay.h"
#include "../aer/precompexp.h"

using namespace std;


DvsEvent2dReader * dvspointer;

void signal_callback_handler(int signum)
{
    if (dvspointer != NULL) {
        dvspointer->close();
    }

    exit(signum);
}


int main(int argc, char *argv[])
{
    signal(SIGINT, signal_callback_handler);

    FileEvent2dReader r("/home/riton/henriSaccades.aedat");

    PrecompExp exp(1000);

    EventDisplay disp(128, 10000);

    double threshold = strtod(argv[1], NULL);

    double eps = 0;
    timestamp last_t = 0;

    while (r.hasNext(true)) {
        //usleep(1000);
        Event2d e = r.readEvent2d();

        eps = eps * exp.val(e.t - last_t) + 1;
        last_t = e.t;
        cout << eps << endl;

        if (eps < threshold) {
            disp.feed(e);
        } else {
            disp.time(last_t);
        }

    }

    return 0;
}

