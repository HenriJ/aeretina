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

#define ENABLE_DISPLAY 100

using namespace std;

void wrongUsage() {
    cout << "TODO" << endl;
    exit(-1);
}


/*
  Static vars
*/
static Pupil * pupil;

#if ENABLE_DISPLAY
static unsigned int width = 128;
static unsigned int pixels = 128*128;

static double * ON;
static double * OFF;

static void render(void)
{
    static int checker_size = 1;
    glClear(GL_COLOR_BUFFER_BIT);

    unsigned int x = 0;
    unsigned int y = 0;

    double Xa;
    double Yb;
    double delta;
    double over;
    for (unsigned int k = 0; k < pixels; ++k) {
        over = 0;
        Xa = ((x-pupil->x)*cos(pupil->theta) + (y-pupil->y)*sin(pupil->theta)) / pupil->a;
        Yb = ((x-pupil->x)*sin(pupil->theta) - (y-pupil->y)*cos(pupil->theta)) / pupil->b;
        delta = Xa*Xa + Yb*Yb;

        if (delta <= 1) {
            over = 1;//exp(-pupil->confidence/20);
        }

        glColor3d(OFF[k], ON[k], over);

        glRecti(y*checker_size, x*checker_size, (y + 1)*checker_size, (x + 1)*checker_size);

        ++x;
        if (x >= width) {
            x = 0;
            ++y;
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
    if (argc < 3) {
        wrongUsage();
    }

    FileEvent2dReader reader(argv[1], 1276);
    int centroid = strtol(argv[2], NULL, 10);



    /*
     * Init
     */
    unsigned int width = 128; // 128px
    unsigned int deltaR = 3;

    timestamp tau = 250; // 1000us
    PrecompExp * pExp;
    PrecompExp * pExpFast = new PrecompExp(250);
    PrecompExp * pExpSlow = new PrecompExp(1000);

    pExp = pExpSlow;

    Range rangeX(20, 100, 1);
    Range rangeY(20, 100, 1);
    Range rangeA(15, 15, 1);
    Range rangeB(15, 15, 1);
    Range rangeTheta(0, 0, 3.14159/20);

    pupil = new Pupil(56, 70, 10, 10, 0);

    unsigned int H_size = rangeX.size()*rangeY.size()*rangeA.size()*rangeB.size()*rangeTheta.size();
    double * H          = new double[H_size];
    timestamp * H_times = new timestamp[H_size];




    /*
     * Display init
     */
    #if ENABLE_DISPLAY
    PrecompExp * displayExp = new PrecompExp(250);

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

    double min = 0;
    unsigned int minPos = 0;

    double max = 0;
    unsigned int maxPos = 0;
    Pupil maxPupil;

    double meanX = 0;
    double meanY = 0;
    double sumX = 0;
    double sumY = 0;

    double posXVar = 0;
    double posYVar = 0;
    timestamp last_t = 0;

    boost::timer tic;


    unsigned int pos;
    bool change;
    unsigned char swap;
    double Xa;
    double Yb;
    double delta;

    double eventsQ = 0;

    double distance = 0;
    while (reader.hasNext()) {
        ++i;
        Event2d e = reader.readEvent2d();

        swap = e.x;
        e.x = 128-e.y;
        e.y = swap;

        // Update last max to compare with future maxes
        max = H[maxPos] * pExp->val(e.t - H_times[maxPos]);
        min = H[minPos] * pExp->val(e.t - H_times[minPos]);

//        rangeX = Range(pupil->x-5, pupil->x+5, 1);
//        rangeY = Range(pupil->y-5, pupil->y+5, 1);

        distance = exp(-5*((pupil->x - e.x)*(pupil->x - e.x) + (pupil->y - e.y)*(pupil->y - e.y))/((pupil->a)*(pupil->b)));

        for (double x = rangeX.begin(); x <= rangeX.end(); x = rangeX.iter()) {
            for (double y = rangeY.begin(); y <= rangeY.end(); y = rangeY.iter()) {
                for (double a = rangeA.begin(); a <= rangeA.end(); a = rangeA.iter()) {
                    for (double b = rangeB.begin(); b <= rangeB.end(); b = rangeB.iter()) {
                        for (double theta = rangeTheta.begin(); theta <= rangeTheta.end(); theta = rangeTheta.iter()) {
                            Xa = ((x-e.x)*cos(theta) + (y-e.y)*sin(theta)) / a;
                            Yb = ((x-e.x)*sin(theta) - (y-e.y)*cos(theta)) / b;
                            delta = Xa*Xa + Yb*Yb;

                            change = false;

                            if (delta <= 1) {
                                pos = rangeX.pos() + rangeX.size() * (rangeY.pos() + rangeY.size() * (rangeA.pos() + rangeA.size() * (rangeB.pos() + rangeB.size() * theta)));

                                H[pos] = H[pos] * pExp->val(e.t - H_times[pos]) + distance/(a*b);
                                H_times[pos] = e.t;
                                change = true;
                            }

                            if (delta > 1 &&  delta <= 2) {
                                pos = rangeX.pos() + rangeX.size() * (rangeY.pos() + rangeY.size() * (rangeA.pos() + rangeA.size() * (rangeB.pos() + rangeB.size() * theta)));

                                H[pos] = H[pos] * pExp->val(e.t - H_times[pos]) - distance/(a*b);
                                H_times[pos] = e.t;
                                change = true;
                            }

                            if (change && H[pos] > max) {
                                max = H[pos];

                                maxPupil.x = x;
                                maxPupil.y = y;
                                maxPupil.a = a;
                                maxPupil.b = b;
                                maxPupil.theta = theta;

                                maxPos = pos;
                            }

                            if (change && H[pos] < min) {
                                min = H[pos];
                                minPos = pos;
                            }
                        }
                    }
                }
            }
        }



        meanX = 0;
        sumX = 0;
        meanY = 0;
        sumY = 0;
        for (int x = maxX - centroid; x <= maxX + centroid; ++x) {
            for (int y = maxY - centroid; y <= maxY + centroid; ++y) {
                pos = maxR * width * width + y * width + x;
                meanX += H[pos] * pExp->val(e.t - H_times[pos]) * (x - (int)maxX);
                sumX += H[pos] * pExp->val(e.t - H_times[pos]);
                meanY += H[pos] * pExp->val(e.t - H_times[pos]) * (y - (int)maxY);
                sumY += H[pos] * pExp->val(e.t - H_times[pos]);
            }
        }
        meanX = maxX + meanX/sumX;
        meanY = maxY + meanY/sumY;


//        posXVar = posXVar * pVarExp->val(e.t - last_t) + (meanX - circle->x)*(meanX - circle->x);
//        posYVar = posYVar * pVarExp->val(e.t - last_t) + (meanY - circle->y)*(meanY - circle->y);
//        last_t = e.t;


        //if (posXVar + posYVar < 10) {
        //    cout << e.t << ", " << meanX << ", " << meanY << endl;
        //}
        last_t = e.t;


        pupil = new Pupil(maxPupil);

        cout << e.t << ", " << *pupil << endl;

        if (e.t > 10000000) {
            break;
        }


//        if (i%100000 == 0) {
//            //cout << 100000/tic.elapsed() << "e/s" << endl;
//            tic.restart();
//        }




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
            for (unsigned int k = 0; k < width*width; ++k) {
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
