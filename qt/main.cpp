#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cstdlib>

#include <opencv2/core/core.hpp>

#include "misc.h"
#include "common/event2d.h"
#include "common/fileevent2dreader.h"
#include "common/dummyevent2dreader.h"

using namespace std;
using namespace cv;

int main(/*int argc, char *argv[]*/)
{
    Mat A = kernel_gaussian(2, 1);
    Mat B = kernel_gaussian(2, 0.99);
    Mat dG = A - B;
    //cout << dG << endl;

    //DummyEvent2dReader reader;
    FileEvent2dReader reader("/home/riton/demo.aer");
    while (reader.hasNext()) {
        cout << reader.readEvent2d() << endl;
    }

    // Simulation
    //vector<bpEvent> bpONe[WIDTH][WIDTH];


    return 0;
}
