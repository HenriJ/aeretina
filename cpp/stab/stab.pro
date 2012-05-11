TEMPLATE = app
CONFIG += console
CONFIG -= qt

TARGET = stab

QMAKE_CXXFLAGS += -std=c++0x

SOURCES += main.cpp \
    spikes.cpp \
    carnelldist.cpp \
    ../aer/precompexp.cpp

HEADERS += \
    spikes.h \
    ../aer/timestamp.h \
    carnelldist.h \
    ../aer/precompexp.h \
    ../aer/mat.h

