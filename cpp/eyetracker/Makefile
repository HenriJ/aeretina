#############################################################################
# Makefile for building: build/eyetracker
# Generated by qmake (2.01a) (Qt 4.7.4) on: Mon Feb 20 17:44:39 2012
# Project:  eyetracker.pro
# Template: app
# Command: /home/riton/QtSDK/Desktop/Qt/474/gcc/bin/qmake -spec ../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/linux-g++ -o Makefile eyetracker.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_NO_DEBUG
CFLAGS        = -pipe -O2 -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -std=c++0x -O3 -O2 -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/linux-g++ -I. -I../../../../QtSDK/Desktop/Qt/474/gcc/include -I.
LINK          = g++
LFLAGS        = -Wl,-O1 -Wl,-rpath,/home/riton/QtSDK/Desktop/Qt/474/gcc/lib
LIBS          = $(SUBLIBS)  -L/home/riton/QtSDK/Desktop/Qt/474/gcc/lib -L/usr/X11R6/lib -lGL -lglut -lGLEW -lpthread 
AR            = ar cqs
RANLIB        = 
QMAKE         = /home/riton/QtSDK/Desktop/Qt/474/gcc/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = main.cpp \
		../aer/event2d.cpp \
		../aer/event2dreader.cpp \
		../aer/fileevent2dreader.cpp \
		../aer/dummyevent2dreader.cpp \
		circle.cpp \
		precompexp.cpp 
OBJECTS       = main.o \
		event2d.o \
		event2dreader.o \
		fileevent2dreader.o \
		dummyevent2dreader.o \
		circle.o \
		precompexp.o
DIST          = ../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/common/g++.conf \
		../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/common/unix.conf \
		../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/common/linux.conf \
		../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/qconfig.pri \
		../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/modules/qt_webkit_version.pri \
		../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/qt_functions.prf \
		../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/qt_config.prf \
		../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/exclusive_builds.prf \
		../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/default_pre.prf \
		../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/release.prf \
		../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/default_post.prf \
		../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/warn_on.prf \
		../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/qt.prf \
		../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/unix/thread.prf \
		../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/moc.prf \
		../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/resources.prf \
		../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/uic.prf \
		../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/yacc.prf \
		../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/lex.prf \
		../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/include_source_dir.prf \
		eyetracker.pro
QMAKE_TARGET  = eyetracker
DESTDIR       = build/
TARGET        = build/eyetracker

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET):  $(OBJECTS)  
	@$(CHK_DIR_EXISTS) build/ || $(MKDIR) build/ 
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: eyetracker.pro  ../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/linux-g++/qmake.conf ../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/common/g++.conf \
		../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/common/unix.conf \
		../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/common/linux.conf \
		../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/qconfig.pri \
		../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/modules/qt_webkit_version.pri \
		../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/qt_functions.prf \
		../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/qt_config.prf \
		../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/exclusive_builds.prf \
		../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/default_pre.prf \
		../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/release.prf \
		../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/default_post.prf \
		../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/warn_on.prf \
		../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/qt.prf \
		../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/unix/thread.prf \
		../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/moc.prf \
		../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/resources.prf \
		../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/uic.prf \
		../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/yacc.prf \
		../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/lex.prf \
		../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/include_source_dir.prf
	$(QMAKE) -spec ../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/linux-g++ -o Makefile eyetracker.pro
../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/common/g++.conf:
../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/common/unix.conf:
../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/common/linux.conf:
../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/qconfig.pri:
../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/modules/qt_webkit_version.pri:
../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/qt_functions.prf:
../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/qt_config.prf:
../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/exclusive_builds.prf:
../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/default_pre.prf:
../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/release.prf:
../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/default_post.prf:
../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/warn_on.prf:
../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/qt.prf:
../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/unix/thread.prf:
../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/moc.prf:
../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/resources.prf:
../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/uic.prf:
../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/yacc.prf:
../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/lex.prf:
../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/features/include_source_dir.prf:
qmake:  FORCE
	@$(QMAKE) -spec ../../../../QtSDK/Desktop/Qt/474/gcc/mkspecs/linux-g++ -o Makefile eyetracker.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/eyetracker1.0.0 || $(MKDIR) .tmp/eyetracker1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/eyetracker1.0.0/ && $(COPY_FILE) --parents ../aer/event2d.h ../aer/event2dreader.h ../aer/fileevent2dreader.h ../aer/dummyevent2dreader.h ../aer/timestamp.h circle.h precompexp.h .tmp/eyetracker1.0.0/ && $(COPY_FILE) --parents main.cpp ../aer/event2d.cpp ../aer/event2dreader.cpp ../aer/fileevent2dreader.cpp ../aer/dummyevent2dreader.cpp circle.cpp precompexp.cpp .tmp/eyetracker1.0.0/ && (cd `dirname .tmp/eyetracker1.0.0` && $(TAR) eyetracker1.0.0.tar eyetracker1.0.0 && $(COMPRESS) eyetracker1.0.0.tar) && $(MOVE) `dirname .tmp/eyetracker1.0.0`/eyetracker1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/eyetracker1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all:
compiler_moc_header_clean:
compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: 

####### Compile

main.o: main.cpp ../aer/event2d.h \
		../aer/timestamp.h \
		../aer/fileevent2dreader.h \
		../aer/event2dreader.h \
		circle.h \
		precompexp.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

event2d.o: ../aer/event2d.cpp ../aer/event2d.h \
		../aer/timestamp.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o event2d.o ../aer/event2d.cpp

event2dreader.o: ../aer/event2dreader.cpp ../aer/event2dreader.h \
		../aer/event2d.h \
		../aer/timestamp.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o event2dreader.o ../aer/event2dreader.cpp

fileevent2dreader.o: ../aer/fileevent2dreader.cpp ../aer/fileevent2dreader.h \
		../aer/event2dreader.h \
		../aer/event2d.h \
		../aer/timestamp.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o fileevent2dreader.o ../aer/fileevent2dreader.cpp

dummyevent2dreader.o: ../aer/dummyevent2dreader.cpp ../aer/dummyevent2dreader.h \
		../aer/event2dreader.h \
		../aer/event2d.h \
		../aer/timestamp.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o dummyevent2dreader.o ../aer/dummyevent2dreader.cpp

circle.o: circle.cpp circle.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o circle.o circle.cpp

precompexp.o: precompexp.cpp precompexp.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o precompexp.o precompexp.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:
