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

#include "../aer/precompexp.h"
#include "../aer/event2d.h"
#include "../aer/fileevent2dreader.h"
#include "../aer/eventdisplay.h"

#include "range.h"
#include "pupil.h"

#define ENABLE_DISPLAY 0

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
    if (argc < 6) {
        wrongUsage();
    }

    FileEvent2dReader reader(argv[1], 1);

    double searchRadius = strtod(argv[2], NULL); // 1.5
    double updateCoef  = strtod(argv[3], NULL);   // 0.005
    double updateCoefR = strtod(argv[4], NULL);  // 0.0005
    double distribCoef = strtod(argv[5], NULL);  // 2

    EventDisplay disp(128, 10000);


    /*
     * Init
     */
    pupil = new Pupil(56, 70, 18, 18, 0);


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
    #if ENABLE_DISPLAY
    unsigned int i = 0;
    unsigned int pos;
    #endif


    double Xa;
    double Yb;
    double delta;

    double alpha = updateCoef;
    double malpha = 1 - alpha;

    double alphaR = updateCoefR;
    double malphaR = 1 - alphaR;

    boost::timer tic;
    unsigned event_index = 0;

    unsigned char swap;
    while (reader.hasNext()) {
        Event2d e = reader.readEvent2d();
        ++event_index;usleep(10);

        swap = e.x;
        e.x  = 128-e.y;
        e.y  = swap;

        Xa = ((e.x-pupil->x)*cos(pupil->theta) + (e.y-pupil->y)*sin(pupil->theta)) / pupil->a;
        Yb = ((e.x-pupil->x)*sin(pupil->theta) - (e.y-pupil->y)*cos(pupil->theta)) / pupil->b;
        delta = Xa*Xa + Yb*Yb;

        if (delta <= searchRadius) {
            pupil->x = alpha * e.x + malpha * pupil->x;
            pupil->y = alpha * e.y + malpha * pupil->y;

            pupil->a = alphaR * distribCoef * abs(Xa) * pupil->a + malphaR * pupil->a;
            pupil->b = alphaR * distribCoef * abs(Yb) * pupil->b + malphaR * pupil->b;

            cout << e.t << ", " << *pupil << " " <<  Xa << endl;
        }

        disp.feed(e);


        #if ENABLE_DISPLAY
        ++i;

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
        #endif

    }

    //cout << "# " << event_index / tic.elapsed() << "e/s" << endl;
    return 0;
}
