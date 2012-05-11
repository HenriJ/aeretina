#include "eventdisplay.h"

#include <GL/glew.h>
#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/freeglut.h>
#endif


void EventDisplay::render(void)
{
    static int checker_size = 1;
    glClear(GL_COLOR_BUFFER_BIT);

    int x = 0;
    int y = 0;

    timeLock.lock();
    timestamp local_t = last_t;

    for (int k = 0; k < pixels; ++k) {
        glColor3d(exp->val(local_t - OFF_times[k]) * OFF[k], exp->val(local_t - ON_times[k]) * ON[k], 0);

        glRecti(y*checker_size, x*checker_size, (y + 1)*checker_size, (x + 1)*checker_size);

        ++x;
        if (x >= width) {
            x = 0;
            ++y;
        }
    }
    timeLock.unlock();
    glFlush();

    glutSwapBuffers();
}

EventDisplay::EventDisplay(int width, int tau)
    : width(width)
{
    pixels = width*width;

    exp = new PrecompExp(tau);

    ON  = new double[pixels];
    OFF = new double[pixels];

    ON_times  = new timestamp[pixels];
    OFF_times = new timestamp[pixels];

    glutInit(new int, 0);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(512, 512);
    glutCreateWindow("EventDisplay");
    gluOrtho2D(0.0, width, 0.0, width);

    renderThread = new std::thread([this]() {
        while(true) {
            usleep(10000);

            render();
            glutMainLoopEvent();
        }
    });
}

void EventDisplay::feed(Event2d e)
{
    timeLock.lock();
    int pos = e.y * width + e.x;
    if (e.p == 1) {
        ON[pos] = exp->val(e.t - ON_times[pos]) * ON[pos] + 1;
        ON_times[pos] = e.t;
    } else {
        OFF[pos] = exp->val(e.t - OFF_times[pos]) * OFF[pos] + 1;
        OFF_times[pos] = e.t;
    }

    last_t = e.t;
    timeLock.unlock();
}

void EventDisplay::time(timestamp t)
{
    timeLock.lock();
    last_t = t;
    timeLock.unlock();
}
