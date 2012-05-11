QT       -= gui core

LIBS     += -L/usr/X11R6/lib -lGL -lglut -lGLEW

QMAKE_CXXFLAGS = -std=c++0x -O3

TARGET   =  build/eyetracker
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    ../aer/event2d.cpp \
    ../aer/event2dreader.cpp \
    ../aer/fileevent2dreader.cpp \
    ../aer/dummyevent2dreader.cpp \
    precompexp.cpp \
    precompsquare.cpp

HEADERS += \
    ../aer/event2d.h \
    ../aer/event2dreader.h \
    ../aer/fileevent2dreader.h \
    ../aer/dummyevent2dreader.h \
    ../aer/timestamp.h \
    precompexp.h \
    precompsquare.h \
    pupil.h \
    range.h

OTHER_FILES +=
