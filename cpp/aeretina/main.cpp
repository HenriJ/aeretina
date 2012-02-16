#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <thread>
#include <mutex>

#include <condition_variable>

#include <boost/timer.hpp>

#include <opencv2/core/core.hpp>

#include <GL/glew.h>
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define __CL_ENABLE_EXCEPTIONS
#include <CL/cl.hpp>



#include "common/event2d.h"
#include "common/fileevent2dreader.h"
#include "common/dummyevent2dreader.h"

#include "precompexp.h"

#include "misc.h"
#include "bplayer.h"

#define ENABLE_DISPLAY 0

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

    TimedModif2dSet() {}
    TimedModif2dSet(unsigned int reserve) { modifs.reserve(reserve); }
    TimedModif2dSet(unsigned int t, vector<Modif2d> modifs)
        : t(t), modifs(modifs) {}
};

TimedModif2dSet spatialTrans(const Event2d & e, Mat * m, int maxWidth) {
    TimedModif2dSet set(m->cols * m->rows);
    set.t = e.t;
    vector<Modif2d> & modifs = set.modifs;

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

    return set;
}

struct SimuParam {
    unsigned int width;

    Mat dGONe;
    Mat dGONi;
    Mat dGOFFe;
    Mat dGOFFi;

    int timeShift;

    PrecompExp * pExpONe;
    PrecompExp * pExpONi;
    PrecompExp * pExpOFFe;
    PrecompExp * pExpOFFi;
};

void buildParams(SimuParam & p) {
    p.width = 128;

    Mat A = kernel_gaussian(15, 2);
    Mat B = kernel_gaussian(15, 5);
    Mat dGON = A - B;
    Mat dGOFF = -dGON;
    p.dGONe  = dGON;
    p.dGONi  = dGON;
    p.dGOFFe = dGOFF;
    p.dGOFFi = dGOFF;

    p.timeShift = 50000;

    p.pExpONe  = new PrecompExp(20000);
    p.pExpONi  = new PrecompExp(20000);
    p.pExpOFFe = new PrecompExp(20000);
    p.pExpOFFi = new PrecompExp(20000);
}

static SimuParam * sParam;
static timestamp last_t;

#if ENABLE_DISPLAY
static float * displayBufferR;
static float * displayBufferG;
static float * displayBufferB;
static float maxCols = 1000;

static int checker_size = 1;


static void render(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    unsigned int x = 0;
    unsigned int y = 0;

    int pixels = sParam->width * sParam->width;

    for (int k = 0; k < pixels; ++k) {
            glColor3f(displayBufferR[k] / maxCols, displayBufferG[k] / maxCols , displayBufferB[k] / maxCols);

            glRecti(y*checker_size, x*checker_size, (y + 1)*checker_size, (x + 1)*checker_size);

            ++x;
            if (x >= sParam->width) {
                x = 0;
                ++y;
            }
    }
    glFlush();

    glutSwapBuffers();
}
#endif

const char * kernelSrc =
  "__kernel void squareArray(__global float* input, "
  "                          __global float* output)"
  "{"
  "  output[get_global_id(0)] = input[get_global_id(0)] * input[get_global_id(0)];"
  "}";


int main(int argc, char *argv[])
{
    /*
     * OpenCL
     */


    /*
     * Simulation parameters
     */
    SimuParam p;
    sParam = &p;
    buildParams(p);


    /*
     * Input data
     */
    //DummyEvent2dReader reader(p.width, 1000000);
    FileEvent2dReader reader("/home/riton/NC1.dat");
    //FileEvent2dReader reader("/home/riton/ntest3.dat");
    //FileEvent2dReader reader("/home/riton/iv/saccades_variables_bluredleft/p2_d64.dat"); //8.4E6 e
    //FileEvent2dReader reader("/home/riton/sacgratings.dat"); // 12.7E6 e


    /*
     * Bipolar layers init
     */
    BPLayer bpONe (p.width);
    BPLayer bpONi (p.width);
    BPLayer bpOFFe(p.width);
    BPLayer bpOFFi(p.width);


    #if ENABLE_DISPLAY
    /*
     * Display parameters
     */
    displayBufferR = new float[p.width*p.width];
    displayBufferG = new float[p.width*p.width];
    displayBufferB = new float[p.width*p.width];

    /*
     * OpenGL stuffs
     */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(512, 512);
    glutCreateWindow("BP");
    gluOrtho2D(0.0, 128, 0.0, 128);

    glutDisplayFunc(&render);
    glutIdleFunc(&render);

    thread glutThread(glutMainLoop);
    #endif


    /* DEBUG */
    boost::timer tic;


    unsigned event_index = 0;

    last_t = 0;
    timestamp last_t_shift = 0;

    while (reader.hasNext()) {
        ++event_index;

        Event2d e = reader.readEvent2d();

        if (e.t != last_t) {
            last_t = e.t;
            last_t_shift = last_t - p.timeShift;
        }

        #if ENABLE_DISPLAY
        if (event_index % ENABLE_DISPLAY == 0) {
            cout << "Event: " << event_index << ", time: " << last_t << endl;
            unsigned int k = 0;
            float val;
            for (unsigned int x = 0; x < p.width; ++x) {
                for (unsigned int y = 0; y < p.width; ++y) {
                    val = bpONe.c(x, y).compute(last_t, sParam->pExpONe);
                    displayBufferR[k] = val;

                    val = bpOFFe.c(x, y).compute(last_t, sParam->pExpOFFe);
                    displayBufferG[k] = val;

                    val = bpONi.c(x, y).compute(last_t, sParam->pExpONi);
                    displayBufferB[k] = val;

                    ++k;
                }
            }
        }
        #endif

        TimedModif2dSet setONe  = spatialTrans(e, &p.dGONe,  p.width);
        TimedModif2dSet setONi  = spatialTrans(e, &p.dGONi,  p.width);
        TimedModif2dSet setOFFe = spatialTrans(e, &p.dGOFFe, p.width);
        TimedModif2dSet setOFFi = spatialTrans(e, &p.dGOFFi, p.width);

        for (unsigned int i = 0; i < setONe.modifs.size(); ++i) {
            Modif2d & m = setONe.modifs[i];
            BPCell & cell = bpONe.c(m.x, m.y);
            cell.add(m.v, last_t, p.pExpONe);
        }

        for (unsigned int i = 0; i < setONi.modifs.size(); ++i) {
            Modif2d & m = setONi.modifs[i];
            BPCell & cell = bpONi.c(m.x, m.y);
            cell.add(m.v, last_t_shift, p.pExpONi);
        }

        for (unsigned int i = 0; i < setOFFe.modifs.size(); ++i) {
            Modif2d & m = setOFFe.modifs[i];
            BPCell & cell = bpOFFe.c(m.x, m.y);
            cell.add(m.v, last_t, p.pExpOFFe);
        }

        for (unsigned int i = 0; i < setOFFi.modifs.size(); ++i) {
            Modif2d & m = setOFFi.modifs[i];
            BPCell & cell = bpOFFi.c(m.x, m.y);
            cell.add(m.v, last_t_shift, p.pExpOFFi);
        }

//        int ix = 42;
//        int iy = 42;
//        vector<double> valsONe   = bpONe.c(ix, iy).rangeCompute(last_t, 100, last_t+200000, p.pExpONe);
//        vector<double> valsONi   = bpONe.c(ix, iy).rangeCompute(last_t, 100, last_t+200000, p.pExpONi);
//        vector<double> valsOFFe  = bpONe.c(ix, iy).rangeCompute(last_t, 100, last_t+200000, p.pExpOFFe);
//        vector<double> valsOFFi  = bpONe.c(ix, iy).rangeCompute(last_t, 100, last_t+200000, p.pExpOFFi);

//        double last_val = 0;
//        for (unsigned int i = 0; i < valsONe.size(); ++i) {

//        }
    }
    cout << sizeof(double) << endl;
    cout << event_index / tic.elapsed() << "e/s" << endl;

    #if ENABLE_DISPLAY
    glutThread.join();
    #endif

    return 0;
}
