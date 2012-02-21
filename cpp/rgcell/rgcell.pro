QT       -= gui core

LIBS     += -lopencv_core

QMAKE_CXXFLAGS = -std=c++0x -O0

TARGET   =  rgcell
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    misc.cpp \
    ../aer/event2d.cpp \
    ../aer/event2dreader.cpp \
    ../aer/fileevent2dreader.cpp \
    ../aer/dummyevent2dreader.cpp \
    bplayer.cpp \
    bpcell.cpp \
    precompexp.cpp \
    rgcell.cpp

HEADERS += \
    misc.h \
    ../aer/event2d.h \
    ../aer/event2dreader.h \
    ../aer/fileevent2dreader.h \
    ../aer/dummyevent2dreader.h \
    bplayer.h \
    bpcell.h \
    ../aer/timestamp.h \
    precompexp.h \
    ../aer/Queue.h \
    rgcell.h

