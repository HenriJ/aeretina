QT       -= gui core

LIBS     += -L/usr/X11R6/lib -lGL -lglut -lGLEW

QMAKE_CXXFLAGS = -std=c++0x -O3

TARGET   =  build/eyetracker
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    ../aer/event2dreader.cpp \
    ../aer/fileevent2dreader.cpp \
    ../aer/dummyevent2dreader.cpp \
    ../aer/precompexp.cpp \
    ../aer/eventdisplay.cpp


HEADERS += \
    ../aer/event2d.h \
    ../aer/event2dreader.h \
    ../aer/fileevent2dreader.h \
    ../aer/dummyevent2dreader.h \
    ../aer/timestamp.h \
    ../aer/precompexp.h \
    pupil.h \
    range.h \
    ../aer/eventdisplay.h


OTHER_FILES +=
