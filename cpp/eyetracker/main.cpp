#include <iostream>
#include <string.h>

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

    unsigned int deltaR = 1;
    double over = 0;

    for (int k = 0; k < pixels; ++k) {
        over = 0;
        if (circle->confidence >= 700) {
            bool outerCircle = ((x-circle->x)*(x-circle->x) + (y-circle->y)*(y-circle->y) <= (circle->r+deltaR)*(circle->r+deltaR)) &&
                               ((x-circle->x)*(x-circle->x) + (y-circle->y)*(y-circle->y) >= (circle->r-deltaR)*(circle->r-deltaR));
            bool innerCircle = ((x-circle->x)*(x-circle->x) + (y-circle->y)*(y-circle->y) <= 30) &&
                               ((x-circle->x)*(x-circle->x) + (y-circle->y)*(y-circle->y) >= 0);
            if (outerCircle || innerCircle) {
                over = 1;
            }
        }

        glColor3d(OFF[k] / 6, ON[k] / 6, over);


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

int main(int argc, char *argv[])
{
    /*
     * Retrieve arguments
     */
    if (argc < 2) {
        wrongUsage();
    }

    FileEvent2dReader reader(argv[1], 1276);

    /*
     * Constants
     */
    unsigned int width = 128; // 128px
    timestamp tau = 5000; // 1ms
    pExp = new PrecompExp(10*tau);
    unsigned int delta_display = 25;
    unsigned int deltaR = 2;

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

    double max = 0;
    unsigned int maxX = 0;
    unsigned int maxY = 0;
    unsigned int maxR = 0;
    unsigned int maxPos = 0;

    while (reader.hasNext()) {
        ++i;
        Event2d e = reader.readEvent2d();

        unsigned char temp = e.x;
        e.x = 128-e.y;
        e.y = temp;

        /* HOUGH */
        double delta;

        // Update last max to compare with future maxes
        max = H[maxPos] * pExp->val(e.t - H_times[maxPos]);

        houghSum = houghSum * pExp->val(e.t - houghSum_last_t);

        for (unsigned int x = 30; x < 98; ++x) {
            for (unsigned int y = 30; y < 98; ++y) {
                for (unsigned int r = 25; r < 30; ++r) {
                    delta = (x-e.x)*(x-e.x) + (y-e.y)*(y-e.y);
                    if (((delta < (r + deltaR)*(r + deltaR)) && (delta > (r - deltaR)*(r - deltaR))) ||
                        ((delta <= 30) && (delta >= 0)) ){
                        pos = r * width * width + y * width + x;
                        H[pos] = H[pos] * pExp->val(e.t - H_times[pos]) + 1;
                        H_times[pos] = e.t;
                        ++houghSum;
                    }

                    if (H[pos] > max) {
                        max = H[pos];
                        maxX = x;
                        maxY = y;
                        maxR = r;
                        maxPos = pos;
                    }
                }
            }
        }

        houghSum_last_t = e.t;

        circle->confidence = max;
        //cout << circle->confidence << endl;
        circle->x = maxX;
        circle->y = maxY;
        circle->r = maxR;

        cout << e.t << ", " << maxX << ", " << maxY << ", " << maxR << ", " << max << ", " << houghSum << ", " << circle->confidence << endl;

        /* DISPLAY */
        pos = e.y * width + e.x;
        if (e.p == 1) {
            ON[pos] = pExp->val(e.t - ON_times[pos]) * ON[pos] + 1;
            ON_times[pos] = e.t;
        } else {
            OFF[pos] = pExp->val(e.t - OFF_times[pos]) * OFF[pos] + 1;
            OFF_times[pos] = e.t;
        }

        if (i % delta_display == 0) {
            for (unsigned int k = 0; k < width*width; ++k) {
                ON[k] = pExp->val(e.t - ON_times[k]) * ON[k];
                ON_times[k] = e.t;
                OFF[k] = pExp->val(e.t - OFF_times[k]) * OFF[k];
                OFF_times[k] = e.t;
            }

            //cout << i << " " << e.t << endl;
            render();
            glutMainLoopEvent();
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
