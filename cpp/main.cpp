#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <thread>

#include <boost/timer.hpp>
#include <boost/thread/condition_variable.hpp>

#include <opencv2/core/core.hpp>

#include "common/event2d.h"
#include "common/fileevent2dreader.h"
#include "common/dummyevent2dreader.h"

#include "precomp/precompexp.h"
#include "precomp/precomppropexp.h"

#include "misc.h"
#include "bpevent.h"
#include "bplayer.h"

using namespace std;
using namespace cv;

struct Modif2d {
    unsigned char x;
    unsigned char y;
    double v;

    Modif2d(unsigned char x, unsigned char y, double v)
        : x(x), y(y), v(v) {}
};

struct TimedModif2dSet {
    unsigned int t;
    vector<Modif2d> modifs;

    TimedModif2dSet(unsigned int t, vector<Modif2d> modifs)
        : t(t), modifs(modifs) {}
};

TimedModif2dSet spatialTrans(const Event2d & e, Mat * m, int maxWidth) {
    vector<Modif2d> modifs;

    int shift = m->cols / 2;

    for (int x = 0; x < m->cols; ++x) {
        for (int y = 0; y < m->rows; ++y) {
            int i = x - shift + e.x;
            int j = y - shift + e.y;
            // Check boundaries
            if (i >= 0 && i < maxWidth && j >= 0 && j < maxWidth) {
                modifs.push_back(Modif2d(i, j, m->at<double>(x, y) * e.p));
            }
        }
    }

    return TimedModif2dSet(e.t, modifs);
}

int main(int argc, char *argv[])
{
    Mat A = kernel_gaussian(2, 1);
    Mat B = kernel_gaussian(2, 0.99);
    Mat dGON = A - B;
    Mat dGOFF = -dGON;
    //cout << dGON << endl;


    DummyEvent2dReader reader(128, 1000000);
    //FileEvent2dReader reader("/home/riton/NC1.dat");

    const unsigned int WIDTH = 128;
    BPLayer bpONe (WIDTH);
    BPLayer bpONi (WIDTH);
    BPLayer bpOFFe(WIDTH);
    BPLayer bpOFFi(WIDTH);

    boost::timer tic;
    unsigned total = 0;

    int maxT = 100000;
    int timeShift = 5000;

    unsigned int last_t = 0;
    unsigned int last_t_shift = 0;
    unsigned int oldTime = 0;
    unsigned int oldTimeShift = 0;

    PrecompExp * pExp = new PrecompExp(100000, 20000);
    PrecompPropExp * pPropExp = new PrecompPropExp(100000, 20000);


    boost::condition_variable cond;

    thread t1([] {

        cout << ".";
    });

    while (reader.hasNext()) {
        ++total;

// DEBUG: max number of BPEvents in a BPCell
//        if (total % 1000 == 0) {
//            unsigned int maxEvents = 0;
//            BPCell * c;
//            for (unsigned int x = 0; x < WIDTH; ++x) {
//                for (unsigned int y = 0; y < WIDTH; ++y) {
//                    if (bpONe.c(x, y).size() > maxEvents) {
//                        c = & bpONe.c(x, y);
//                        maxEvents = c->size();
//                    }
//                }
//            }
//            cout << maxEvents << endl;
//        }

        Event2d e = reader.readEvent2d();

        if (e.t != last_t) {
            last_t = e.t;
            last_t_shift = last_t - timeShift;
            oldTime = std::max((int) (last_t) - maxT, 0);
            oldTimeShift = oldTime - timeShift;
        }

        TimedModif2dSet setON  = spatialTrans(e, &dGON, WIDTH);
        TimedModif2dSet setOFF = spatialTrans(e, &dGOFF, WIDTH);

//        for (unsigned int i = 0; i < setON.modifs.size(); ++i) {
//            Modif2d & m = setON.modifs[i];
//            BPCell & cell = bpONe.c(m.x, m.y);
//            cell.clean(oldTime);
//            cell.add(BPEvent(m.v, last_t));
//        }

//        for (unsigned int i = 0; i < setON.modifs.size(); ++i) {
//            Modif2d & m = setON.modifs[i];
//            BPCell & cell = bpONi.c(m.x, m.y);
//            cell.clean(oldTimeShift);
//            cell.add(BPEvent(m.v, last_t_shift));
//        }

//        for (unsigned int i = 0; i < setOFF.modifs.size(); ++i) {
//            Modif2d & m = setOFF.modifs[i];
//            BPCell & cell = bpOFFe.c(m.x, m.y);
//            cell.clean(oldTime);
//            cell.add(BPEvent(m.v, last_t));
//        }

//        for (unsigned int i = 0; i < setOFF.modifs.size(); ++i) {
//            Modif2d & m = setOFF.modifs[i];
//            BPCell & cell = bpOFFi.c(m.x, m.y);
//            cell.clean(oldTimeShift);
//            cell.add(BPEvent(m.v, last_t_shift));
//        }

//        std::vector<double> valsONe  = bpONe.c(e.x, e.y).rangeCompute(last_t, last_t+100000, 1000, pExp, pPropExp);
//        std::vector<double> valsONi  = bpONi.c(e.x, e.y).rangeCompute(last_t, last_t+100000, 1000, pExp, pPropExp);
//        std::vector<double> valsOFFe = bpOFFe.c(e.x, e.y).rangeCompute(last_t, last_t+100000, 1000, pExp, pPropExp);
//        std::vector<double> valsOFFi = bpOFFi.c(e.x, e.y).rangeCompute(last_t, last_t+100000, 1000, pExp, pPropExp);

    }

    cout << (double)(total) / tic.elapsed() << "e/s" << endl;

    sleep(100);

    return 0;
}
