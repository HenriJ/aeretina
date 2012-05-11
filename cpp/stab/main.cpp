#include <iostream>

#include "spikes.h"
#include "carnelldist.h"

using namespace std;

int main()
{
    Spikes s;
    s.push(10);

    Spikes x;
    x.push(10);
    x.push(20);

    CarnellDist d(10);

    cout << d.dist(s, x) << endl;
    return 0;
}
