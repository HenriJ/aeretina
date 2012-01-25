QT       += core

QT       -= gui

LIBS     += -lopencv_core

TARGET   =  qt
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    misc.cpp \
    common/event2d.cpp \
    common/event2dreader.cpp \
    common/fileevent2dreader.cpp \
    common/dummyevent2dreader.cpp

HEADERS += \
    misc.h \
    common/event2d.h \
    common/event2dreader.h \
    common/fileevent2dreader.h \
    common/dummyevent2dreader.h

OTHER_FILES += \
    junk.txt
