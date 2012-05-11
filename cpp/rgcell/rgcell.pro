QT       -= gui core

LIBS     += -lopencv_core

QMAKE_CXXFLAGS = -std=c++0x -O3

TARGET   =  build/rgcell
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    ini/ini.c \
    ini/cpp/INIReader.cpp \
    ../aer/event2dreader.cpp \
    ../aer/fileevent2dreader.cpp \
    ../aer/dummyevent2dreader.cpp \
    precompexp.cpp \
    doublemat.cpp \
    bplayer.cpp

HEADERS += \
    ini/ini.h \
    ini/cpp/INIReader.h \
    ../aer/event2d.h \
    ../aer/event2dreader.h \
    ../aer/fileevent2dreader.h \
    ../aer/dummyevent2dreader.h \
    ../aer/timestamp.h \
    precompexp.h \
    doublemat.h \
    bplayer.h

