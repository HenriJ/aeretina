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

double * display;

static Circle * circle;

static int checker_size = 1;

static void render(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    unsigned int x = 0;
    unsigned int y = 0;

    int pixels = width * width;

    for (int k = 0; k < pixels; ++k) {
        glColor3d(display[k], display[k], display[k]);


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
    if (argc < 2) {
        wrongUsage();
    }

    FileEvent2dReader reader(argv[1], 1);

    /*
     * Constants
     */
    unsigned int width = 128; // 128px
    timestamp tau = 250; // 1ms
    pExp = new PrecompExp(tau);

    unsigned int deltaR = 3;

    /*
     * Datastructures needed
     */
    circle = new Circle;
    double * H = new double[width*width];
    timestamp * H_times = new timestamp[128*128];
    display = new double[width*width];
    double houghSum = 0;
    timestamp houghSum_last_t = 0;


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

    unsigned char innerR = 250;

    Range rangeX{30, 60};
    Range rangeY{30, 60};

    unsigned int maxR = 35;

    while (reader.hasNext()) {
        ++i;
        Event2d e = reader.readEvent2d();

        unsigned char temp = e.x;
        e.x = 128-e.y;
        e.y = temp;

        /* HOUGH */
        double delta;
        double max = 0;

        houghSum = houghSum * pExp->val(e.t - houghSum_last_t);

        for (unsigned int x = rangeX.begin; x < rangeX.end; ++x) {
            for (unsigned int y = rangeY.begin; y < rangeY.end; ++y) {
                delta = (x-e.x)*(x-e.x) + (y-e.y)*(y-e.y);

                if (((delta < (maxR + deltaR)*(maxR + deltaR)) && (delta > (maxR - deltaR)*(maxR - deltaR))) ||
                    ((delta <= innerR) && (delta >= 50)) ) {
                    pos = y * width + x;
                    H[pos] = H[pos] * pExp->val(e.t - H_times[pos]) + 1;
                    H_times[pos] = e.t;
                    ++houghSum;
                }

//                if (((delta < (maxR - deltaR)*(maxR - deltaR)) && (delta > innerR))){
//                    pos = y * width + x;
//                    H[pos] = H[pos] * pExp->val(e.t - H_times[pos]) - 1;
//                    H_times[pos] = e.t;
//                    --houghSum;
//                }

                if (H[pos] > max) {
                    max = H[pos];
                }


            }
        }

        houghSum_last_t = e.t;


        /* DISPLAY */

        if (i % 1 == 0) {
            for (unsigned int k = 0; k < width*width; ++k) {
                cout << H[pos] << endl;
                display[k] = H[pos] * pExp->val(e.t - H_times[pos]) / max;
            }

            //cout << display[k] << endl;
            cout << i << " " << e.t << endl;
            render();
            glutMainLoopEvent();
            sleep(1);
        }
    }

    /*
     * Output results
     */
//    for (unsigned int i = 0; i < maxDeltaT; ++i) {
//        cout << delta_times[i] << endl;
//    }

    return 0;
}
