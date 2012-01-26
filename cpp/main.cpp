#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cstdlib>

#include <boost/thread.hpp>
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/timer.hpp>

#include <opencv2/core/core.hpp>

#include <GL/glew.h>
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include "misc.h"
#include "common/event2d.h"
#include "common/fileevent2dreader.h"
#include "common/dummyevent2dreader.h"
#include "bpevent.h"
#include "bplayer.h"

using namespace std;
using namespace cv;

static void render(void)
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glutSwapBuffers();
}

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

TimedModif2dSet spatialTrans(const Event2d e, Mat * m, int maxWidth) {
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

//void modifMerger(vector<BPEvent>[128]* bp) {
//    cout << "plop" << bp[0][0].size() << endl;
//}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(300, 300);
    glutCreateWindow("BP");
    glutDisplayFunc(&render);
    //glutIdleFunc(&update_fade_factor);

    glewInit();
    if (!GLEW_VERSION_2_0) {
        fprintf(stderr, "OpenGL 2.0 not available\n");
        return 1;
    }

    //boost::thread glutThread( glutMainLoop );

    Mat A = kernel_gaussian(2, 1);
    Mat B = kernel_gaussian(2, 0.99);
    Mat dG = A - B;
    //cout << dG << endl;

    DummyEvent2dReader reader;

    const unsigned int WIDTH = 128;
    BPLayer bpONe(WIDTH);

    boost::timer tic;
    unsigned total = 0;

    int maxT = 100000;

    boost::interprocess::message_queue::remove("modifs");

    boost::interprocess::message_queue queue(boost::interprocess::create_only, "modifs", 100, sizeof(Modif2d));

//    boost::thread_group tg;
//    modifMerger(bpONe);
//    tg.create_thread();

    while (reader.hasNext()) {

        ++total;
        if (total % 1000 == 0) {
            int maxEvents = 0;
            for (int x = 0; x < WIDTH; ++x) {
                for (int y = 0; y < WIDTH; ++y) {
                    if (bpONe.c(x, y).size() > maxEvents) {
                        maxEvents = bpONe.c(x, y).size();
                    }
                }
            }
            cout << maxEvents << endl;
        }


        TimedModif2dSet set = spatialTrans(reader.readEvent2d(), &dG, WIDTH);
        unsigned int oldTime = std::max((int) (set.t) - maxT, 0);


        for (unsigned int i = 0; i < set.modifs.size(); ++i) {
            //queue.send(&set.modifs[i], sizeof(set.modifs[i]), 0);

            Modif2d & m = set.modifs[i];
            BPCell & cell = bpONe.c(m.x, m.y);

            cell.clean(oldTime);
            cell.add(BPEvent(m.v, set.t));
        }
    }


    cout << total << " in " << tic.elapsed() << endl;


    cout << sizeof(BPEvent) << endl;
    sleep(100);


    //glutThread.join();

    return 0;
}
