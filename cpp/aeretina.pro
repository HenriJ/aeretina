QT       += core

QT       -= gui

LIBS     += -lopencv_core -L/usr/X11R6/lib -lGL -lglut -lGLEW -lboost_thread

TARGET   =  aeretina
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    misc.cpp \
    common/event2d.cpp \
    common/event2dreader.cpp \
    common/fileevent2dreader.cpp \
    common/dummyevent2dreader.cpp \
    bplayer.cpp \
    bpevent.cpp \
    bpcell.cpp

HEADERS += \
    misc.h \
    common/event2d.h \
    common/event2dreader.h \
    common/fileevent2dreader.h \
    common/dummyevent2dreader.h \
    bplayer.h \
    bpevent.h \
    bpcell.h

OTHER_FILES += \
    junk.txt