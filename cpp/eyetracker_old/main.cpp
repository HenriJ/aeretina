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

#include "circle.h"
#include "precompexp.h"

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

static unsigned int width = 128;

static PrecompExp * pExp;

static double * ON;
static double * OFF;

static Circle * circle;

static int checker_size = 1;

static void render(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    unsigned int x = 0;
    unsigned int y = 0;

    int pixels = width * width;

    unsigned int deltaR = 4;
    double over = 0;

    for (int k = 0; k < pixels; ++k) {
        over = 0;

        //if (circle->confidence < 10) {
            bool outerCircle = ((x-circle->x)*(x-circle->x) + (y-circle->y)*(y-circle->y) <= (circle->r+deltaR)*(circle->r+deltaR)) &&
                               ((x-circle->x)*(x-circle->x) + (y-circle->y)*(y-circle->y) >= (circle->r-deltaR)*(circle->r-deltaR));
            bool innerCircle = ((x-circle->x)*(x-circle->x) + (y-circle->y)*(y-circle->y) <= 250) &&
                               ((x-circle->x)*(x-circle->x) + (y-circle->y)*(y-circle->y) >= 0);

            if (outerCircle || innerCircle) {
                over = exp(-circle->confidence/20);
            }
        //}


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

struct Range {
    unsigned int begin;
    unsigned int end;
};

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
     * Constants
     */
    unsigned int width = 128; // 128px
    timestamp tau = 250; // 250us
    pExp = new PrecompExp(tau);

    PrecompExp * pVarExp = new PrecompExp(2*tau);

    PrecompExp * displayExp = new PrecompExp(100000);

    unsigned int deltaR = 3;

    /*
     * Datastructures needed
     */
    circle = new Circle;
    double * H = new double[width*width*width];
    timestamp * H_times = new timestamp[128*128*128];
    double houghSum = 0;
    timestamp houghSum_last_t = 0;

    ON = new double[width*width];
    OFF = new double[width*width];
    timestamp * ON_times = new timestamp[128*128];
    timestamp * OFF_times = new timestamp[128*128];


    /*
     * OpenGL stuffs
     */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(512, 512);
    glutCreateWindow("EyeTracker");
    gluOrtho2D(0.0, 128, 0.0, 128);

    glutDisplayFunc(&render);
    glutIdleFunc(&render);


    /*
     * Event loop
     */
    unsigned int pos = 0;
    unsigned int i = 0;

    double min = 0;
    unsigned int minPos = 0;

    double max = 0;
    unsigned int maxX = 0;
    unsigned int maxY = 0;
    unsigned int maxR = 0;
    unsigned int maxPos = 0;

    double meanX = 0;
    double meanY = 0;
    double sumX = 0;
    double sumY = 0;

    double posXVar = 0;
    double posYVar = 0;
    timestamp last_t = 0;

    unsigned char innerR = 250;

    boost::timer tic;

    Range rangeX{30, 90};
    Range rangeY{30, 90};


    while (reader.hasNext()) {
        ++i;
        Event2d e = reader.readEvent2d();

        unsigned char temp = e.x;
        e.x = 128-e.y;
        e.y = temp;

        /* HOUGH */
        double delta;
        bool change = false;

        // Update last max to compare with future maxes
        max = H[maxPos] * pExp->val(e.t - H_times[maxPos]);
        min = H[minPos] * pExp->val(e.t - H_times[minPos]);

        houghSum = houghSum * pExp->val(e.t - houghSum_last_t);
        houghSum_last_t = e.t;

        for (unsigned int x = rangeX.begin; x < rangeX.end; ++x) {
            for (unsigned int y = rangeY.begin; y < rangeY.end; ++y) {
                delta = (x-e.x)*(x-e.x) + (y-e.y)*(y-e.y);

                for (unsigned int r = 30; r < 41; ++r) {
                    change = false;

                    if (((delta < (r + deltaR)*(r + deltaR)) && (delta > (r - deltaR)*(r - deltaR))) ||
                        ((delta <= innerR) && (delta >= 50)) ) {
                        pos = r * width * width + y * width + x;
                        H[pos] = H[pos] * pExp->val(e.t - H_times[pos]) + 1;
                        H_times[pos] = e.t;
                        ++houghSum;
                        change = true;
                    }

                    if (((delta < (r - deltaR)*(r - deltaR)) && (delta > innerR))){
                        pos = r * width * width + y * width + x;
                        H[pos] = H[pos] * pExp->val(e.t - H_times[pos]) - 1;
                        H_times[pos] = e.t;
                        --houghSum;
                        change = true;
                    }

                    if (change && H[pos] > max) {
                        max = H[pos];
                        maxX = x;
                        maxY = y;
                        maxR = r;
                        maxPos = pos;
                    }

                    if (change && H[pos] < min) {
                        min = H[pos];
                        minPos = pos;
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


        posXVar = posXVar * pVarExp->val(e.t - last_t) + (meanX - circle->x)*(meanX - circle->x);
        posYVar = posYVar * pVarExp->val(e.t - last_t) + (meanY - circle->y)*(meanY - circle->y);
        last_t = e.t;


        if (posXVar + posYVar < 10) {
            cout << e.t << ", " << meanX << ", " << meanY << endl;
        }

        circle->confidence = posXVar + posYVar;
        circle->x = meanX;
        circle->y = meanY;
        circle->r = maxR;

        if (e.t > 10000000) {
            break;
        }


//        if (i%100000 == 0) {
//            //cout << 100000/tic.elapsed() << "e/s" << endl;
//            tic.restart();
//        }

        /* DISPLAY ON & OFF*/
        pos = e.y * width + e.x;
        if (e.p == 1) {
            ON[pos] = displayExp->val(e.t - ON_times[pos]) * ON[pos] + 1;
            ON_times[pos] = e.t;
        } else {
            OFF[pos] = displayExp->val(e.t - OFF_times[pos]) * OFF[pos] + 1;
            OFF_times[pos] = e.t;
        }

        if (i % 10 == 0) {
            for (unsigned int k = 0; k < width*width; ++k) {
                ON[k] = displayExp->val(e.t - ON_times[k]) * ON[k] / 4;
                ON_times[k] = e.t;
                OFF[k] = displayExp->val(e.t - OFF_times[k]) * OFF[k] / 4;
                OFF_times[k] = e.t;
            }

            //cout << i << " " << e.t << endl;
            render();
            glutMainLoopEvent();
        }



        /* DISPLAY Hough */

//        if (i % 10 == 0) {
//            double temp;
//            unsigned int r = 35;
//            for (unsigned int x = rangeX.begin; x < rangeX.end; ++x) {
//                for (unsigned int y = rangeY.begin; y < rangeY.end; ++y) {
//                    pos = r * width * width + y * width + x;
//                    temp = (pExp->val(e.t - H_times[pos]) * H[pos] - min) / (max - min);
//                    ON[y * width + x] = temp;
//                }
//            }
//            //circle->confidence = 1000;
//            cout << i << ", " << e.t << ", " << houghSum << endl;
//            //cout << i << " " << e.t << endl;
//            render();
//            glutMainLoopEvent();
//        }

    }

    /*
     * Output results
     */
//    for (unsigned int i = 0; i < maxDeltaT; ++i) {
//        cout << delta_times[i] << endl;
//    }

    return 0;
}
