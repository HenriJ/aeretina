#include <iostream>
#include <string.h>

#include "../aer/event2d.h"
#include "../aer/fileevent2dreader.h"

using namespace std;

void wrongUsage() {
    cout << "Synopsis:" << endl;
    cout << "  Computes the time between two events for each 128*128 pixels" << endl;
    cout << "  Outputs an histogram of theses time differences between 0 and 1sec" << endl;
    cout << endl;
    cout << "Usage:" << endl;
    cout << "  $ interspiker datas.aedat [ON|OFF] > interspikes.csv" << endl;
    cout << "  ON  means only consider ON  events" << endl;
    cout << "  OFF means only   consider OFF events" << endl;
    cout << "  To indiscriminately consider all events, don't specify a second argument" << endl;
    cout << endl;
    cout << "Author:" << endl;
    cout << "  Henri Jouhaud <henri.jouhaud@polytechnique.org>" << endl;
    exit(-1);
}

int main(int argc, char *argv[])
{
    /*
     * Retrieve arguments
     */
    if (argc < 2 || argc > 3) {
        wrongUsage();
    }

    FileEvent2dReader reader(argv[1]);

    char pol = 0;
    if (argc == 3) {
        if (strcmp(argv[2], "OFF") == 0) {
            pol = -1;
        } else if (strcmp(argv[2], "ON") == 0) {
            pol = 1;
        } else {
            wrongUsage();
        }
    }


    /*
     * Constants
     */
    unsigned int width = 128; // 128px
    timestamp maxDeltaT = 1000000; // 1s max


    /*
     * Datastructures needed
     */
    timestamp * last_times = new timestamp[128*128];
    unsigned int * delta_times = new unsigned[maxDeltaT + 1];


    /*
     * Event loop
     */
    unsigned int pos = 0;
    timestamp delta = 0;

    while (reader.hasNext()) {
        Event2d e = reader.readEvent2d();

        if ((pol == 0) || (pol == e.p)) {
            pos = e.y * width + e.x;

            delta = e.t - last_times[pos];
            if (delta > maxDeltaT) {
                delta = maxDeltaT;
            }

            delta_times[delta]++;

            last_times[pos] = e.t;
        }
    }


    /*
     * Output results
     */
    for (unsigned int i = 0; i < maxDeltaT; ++i) {
        cout << delta_times[i] << endl;
    }

    return 0;
}
