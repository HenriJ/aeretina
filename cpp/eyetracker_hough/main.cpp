#include <iostream>
#include <string.h>
#include <cmath>

#include <boost/timer.hpp>

#include <GL/glew.h>
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/freeglut.h>
#endif

#include "../aer/event2d.h"
#include "../aer/fileevent2dreader.h"

#include "range.h"
#include "pupil.h"
#include "precompexp.h"

#define ENABLE_DISPLAY 1000

using namespace std;

void wrongUsage() {
    cout << "TODO" << endl;
    exit(-1);
}


/*
  Static vars
*/
static Pupil * pupil;
static int xReflection = 0;
static int yReflection = 0;
static int rReflection = 0;

#if ENABLE_DISPLAY
static unsigned int width = 320;
static unsigned int height = 240;
static unsigned int pixels = width*height;

static double * ON;
static double * OFF;

static void render(void)
{
    static int checker_size = 1;
    glClear(GL_COLOR_BUFFER_BIT);

    int x = 0;
    int y = 0;

    double Xs = pupil->x*pupil->x;
    double Ys = pupil->y*pupil->y;
    double Rs = pupil->r*pupil->r;

    double over;
    for (unsigned int k = 0; k < pixels; ++k) {
        over = 0;

        if (Xs + Ys <= Rs) {
            over += 0.5;//exp(-pupil->confidence/20);
        }

//        if ((x-xReflection)*(x-xReflection) + (y-yReflection)*(y-yReflection) <= rReflection*rReflection*1.2) {
//            over += 0.5;
//        }

        glColor3d(OFF[k], ON[k], over);

        glRecti(x*checker_size, (height-y)*checker_size, (x + 1)*checker_size, (height-y + 1)*checker_size);

        ++x;
        Xs = (x-pupil->x)*(x-pupil->x);
        if (x >= width) {
            x = 0;
            ++y;
            Ys = (y-pupil->y)*(y-pupil->y);
        }
    }
    glFlush();

    glutSwapBuffers();
}
#endif


int main(int argc, char *argv[])
{
    /*
     * Retrieve arguments
     */
    if (argc < 5) {
        wrongUsage();
    }

    FileEvent2dReader reader(argv[1], 13670, 1);
    double steps = strtod(argv[2], NULL); // 1
    int centroid = strtol(argv[3], NULL, 10); // 5
    timestamp tau = strtol(argv[4], NULL, 10); // 1000us

    steps = 1;
    /*
     * Init
     */
    PrecompExp * pExp = new PrecompExp(10000);

    pupil = new Pupil(180, 129, 19);

    Range rangeX(100, 250, steps);
    Range rangeY(50, 200, steps);
    Range rangeR(19, 19, steps);


    unsigned int H_size = rangeX.size()*rangeY.size()*rangeR.size();

    double * H          = new double[H_size];
    timestamp * H_times = new timestamp[H_size];


    Range rangeRReflection(3, 6 , 1);
    unsigned int HReflection_size = rangeX.size()*rangeY.size()*rangeRReflection.size();

    double * HReflection = new double[HReflection_size];
    timestamp * HReflection_times = new timestamp[HReflection_size];




    /*
     * Display init
     */
    #if ENABLE_DISPLAY
    PrecompExp * displayExp = new PrecompExp(10000);

    ON  = new double[pixels];
    OFF = new double[pixels];

    timestamp * ON_times  = new timestamp[pixels];
    timestamp * OFF_times = new timestamp[pixels];


    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(512, 512);
    glutCreateWindow("EyeTracker");
    gluOrtho2D(0.0, width, 0.0, width);

    glutDisplayFunc(&render);
    glutIdleFunc(&render);
    #endif



    /*
     * Event loop
     */
    unsigned int i = 0;

    double max = 0;
    unsigned int maxPos  = 0;
    unsigned int maxPosX = 0;
    unsigned int maxPosY = 0;
    unsigned int maxPosR = 0;

    double maxReflection = 0;
    unsigned int maxPosReflection = 0;

    double meanX = 0;
    double meanY = 0;
    double sum = 0;

    boost::timer tic;


    unsigned int pos;
    bool change;
    double Xs;
    double Ys;
    double Rs;

    double maxX;
    double maxY;
    double maxR;

    RangeIt x;
    RangeIt y;
    RangeIt r;

    double distance = 0;
    while (reader.hasNext()) {
        ++i;
        Event2d e = reader.readEvent2d();

        /*
          Reflection
         */
        /*maxReflection = HReflection[maxPosReflection] * pExp->val(e.t - HReflection_times[maxPosReflection]);
        for (x = rangeX.it(); x.check(); x.iter()) {
            Xs = (x.v()-e.x)*(x.v()-e.x);
            for (y = rangeY.it(); y.check(); y.iter()) {
                Ys = (y.v()-e.y)*(y.v()-e.y);
                for (r = rangeRReflection.it(); r.check(); r.iter()) {
                    Rs = r.v()*r.v();
                    change = false;

                    if (Xs + Ys <= Rs) {
                        pos = x.p() + rangeX.size() * (y.p() + rangeY.size() * r.p());

                        HReflection[pos] = HReflection[pos] * pExp->val(e.t - HReflection_times[pos]) + 1/Rs;
                        HReflection_times[pos] = e.t;
                        change = true;
                    }


                    if (Xs + Ys > Rs && Xs + Ys <= 2*Rs) {
                        pos = x.p() + rangeX.size() * (y.p() + rangeY.size() * r.p());

                        HReflection[pos] = HReflection[pos] * pExp->val(e.t - HReflection_times[pos]) - 1/Rs;
                        HReflection_times[pos] = e.t;
                        change = true;
                    }

                    if (change && HReflection[pos] > maxReflection) {
                        maxReflection = HReflection[pos];
                        maxPosReflection = pos;

                        xReflection = x.v();
                        yReflection = y.v();
                        rReflection = r.v();
                    }
                }
            }
        }
        if ((e.x - xReflection)*(e.x - xReflection) + (e.y - yReflection)*(e.y - yReflection) <= rReflection*rReflection*1.2) {
            continue;
        }*/


        /*
          Pupil
         */
        // Update last max to compare with future maxes
        max = H[maxPos] * pExp->val(e.t - H_times[maxPos]);
        distance = 1;//exp(-5*((pupil->x - e.x)*(pupil->x - e.x) + (pupil->y - e.y)*(pupil->y - e.y))/((pupil->r)*(pupil->r)));

        for (x = rangeX.it(); x.check(); x.iter()) {
            Xs = (x.v()-e.x)*(x.v()-e.x);
            for (y = rangeY.it(); y.check(); y.iter()) {
                Ys = (y.v()-e.y)*(y.v()-e.y);
                for (r = rangeR.it(); r.check(); r.iter()) {
                    Rs = r.v()*r.v();
                    change = false;

                    if (Xs + Ys <= 1.2 * Rs && Xs + Ys >= 0.8 * Rs) {
                        pos = x.p() + rangeX.size() * (y.p() + rangeY.size() * r.p());

                        H[pos] = H[pos] * pExp->val(e.t - H_times[pos]) + 1;//distance/r.v();
                        H_times[pos] = e.t;
                        change = true;
                    } else
                    if (Xs + Ys < 0.4 * Rs) {
                        pos = x.p() + rangeX.size() * (y.p() + rangeY.size() * r.p());

                        H[pos] = H[pos] * pExp->val(e.t - H_times[pos]) - 1;//distance/r.v();
                        H_times[pos] = e.t;
                        change = true;
                    }
//                    else
//                    if (Xs + Ys < 1 * Rs && Xs + Ys >= 0.7 * Rs) {
//                        pos = x.p() + rangeX.size() * (y.p() + rangeY.size() * r.p());

//                        H[pos] = H[pos] * pExp->val(e.t - H_times[pos]) - 1;//distance/r.v();
//                        H_times[pos] = e.t;
//                        change = true;
//                    }
//                    else
//                    if (Xs + Ys < 0.7 * Rs) {
//                        pos = x.p() + rangeX.size() * (y.p() + rangeY.size() * r.p());

//                        H[pos] = H[pos] * pExp->val(e.t - H_times[pos]) + 1;//distance/r.v();
//                        H_times[pos] = e.t;
//                        change = true;
//                    }

                    if (change && H[pos] > max) {
                        max    = H[pos];
                        maxPos = pos;

                        maxPosX = x.p();
                        maxPosY = y.p();
                        maxPosR = r.p();

                        maxX = x.v();
                        maxY = y.v();
                        maxR = r.v();
                    }
                }
            }
        }

//        sum   = 0;
//        meanX = 0;
//        meanY = 0;

//        for (x = rangeX.it(maxPosX-centroid, maxPosX+centroid); x.check(); x.iter()) {
//            for (y = rangeX.it(maxPosY-centroid, maxPosY+centroid); y.check(); y.iter()) {
//                pos = x.p() + rangeX.size() * (y.p() + rangeY.size() * maxPosR);

//                meanX += H[pos] * pExp->val(e.t - H_times[pos]) * x.v();
//                meanY += H[pos] * pExp->val(e.t - H_times[pos]) * y.v();
//                sum   += H[pos] * pExp->val(e.t - H_times[pos]);
//            }
//        }

        pupil->x = maxX;
        pupil->y = maxY;
        pupil->r = maxR;

        /*pupil->x = 0.001 * maxX + 0.999*pupil->x;
        pupil->y = 0.001 * maxY + 0.999*pupil->y;
        pupil->r = 0.001 * maxR + 0.999*pupil->r;
*/
        cout << i << ", " << e.t << ", " << *pupil << endl;
        //cout << e.t << ", " << *pupil << (int)e.x << " | " << (int)e.y << " | " << (((pupil->x-e.x)*(pupil->x-e.x) +  (pupil->y-e.y)*(pupil->y-e.y))<=pupil->r*pupil->r) << endl;
/*
        if (e.t > 2000000) {
            //cout << 2000000/tic.elapsed() << "e/s" << endl;
            break;
        }
*/

        #if ENABLE_DISPLAY
        pos = e.y * width + e.x;
        if (e.p == 1) {
            ON[pos] = displayExp->val(e.t - ON_times[pos]) * ON[pos] + 1;
            ON_times[pos] = e.t;
        } else {
            OFF[pos] = displayExp->val(e.t - OFF_times[pos]) * OFF[pos] + 1;
            OFF_times[pos] = e.t;
        }

        if (i % ENABLE_DISPLAY == 0) {
            for (unsigned int k = 0; k < pixels; ++k) {
                ON[k]  = displayExp->val(e.t - ON_times[k]) * ON[k] * 0.95;
                ON_times[k]  = e.t;
                OFF[k] = displayExp->val(e.t - OFF_times[k]) * OFF[k] * 0.95;
                OFF_times[k] = e.t;
            }

            render();
            glutMainLoopEvent();
        }


        /* DISPLAY Hough */

//        if (i % ENABLE_DISPLAY == 0) {
//            double temp;
//            rangeA.begin();
//            rangeB.begin();
//            rangeTheta.begin();
//            for (double x = rangeX.begin(); x <= rangeX.end(); x = rangeX.iter()) {
//                for (double y = rangeY.begin(); y <= rangeY.end(); y = rangeY.iter()) {
//                    pos = rangeX.pos() + rangeX.size() * (rangeY.pos() + rangeY.size() * (rangeA.pos() + rangeA.size() * (rangeB.pos() + rangeB.size() * rangeTheta.pos())));
//                    temp = (pExp->val(e.t - H_times[pos]) * H[pos] - min) / (max - min);
//                    ON[(int) (y * width + x)] = temp;
//                }
//            }
//            //circle->confidence = 1000;
//            //cout << i << ", " << e.t << ", " << houghSum << endl;
//            //cout << i << " " << e.t << endl;
//            render();
//            glutMainLoopEvent();
//        }
        #endif

    }

    return 0;
}
