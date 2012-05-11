TEMPLATE = app
CONFIG += console
CONFIG -= qt

LIBS     += -L/usr/X11R6/lib -lGL -lglut -lGLEW

QMAKE_CXXFLAGS = -std=c++0x -O3

TARGET = ./saccadic

SOURCES += main.cpp \
    ../aer/fileevent2dreader.cpp \
    ../aer/event2dreader.cpp \
    ../aer/dvsevent2dreader.cpp \
    ../aer/precompexp.cpp \
    ../aer/eventdisplay.cpp

HEADERS += \
    ../aer/timestamp.h \
    ../aer/fileevent2dreader.h \
    ../aer/event2dreader.h \
    ../aer/event2d.h \
    ../aer/dvsevent2dreader.h \
    ../aer/precompexp.h \
    ../aer/eventdisplay.h

