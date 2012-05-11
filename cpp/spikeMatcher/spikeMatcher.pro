#-------------------------------------------------
#
# Project created by QtCreator 2012-03-13T16:11:12
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = build/spikeMatcher
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    ../aer/doublemat.cpp \
    meareader.cpp

HEADERS += \
    ../aer/doublemat.h \
    meareader.h
