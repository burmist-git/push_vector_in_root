########################################################################
#                                                                      #
# Copyright(C) 2020 - LBS - (Single person developer.)                 #
# Tue Mar 24 11:27:47 CET 2020                                         #
# Autor: Leonid Burmistrov                                             #
#                                                                      #
# Script description:                                                  #
#                                                                      #
# Input paramete:                                                      #
#                                                                      #
# This software is provided "as is" without any warranty.              #
########################################################################

ROOTCFLAGS  = $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOTLIBS    = $(shell $(ROOTSYS)/bin/root-config --libs)
ROOTGLIBS   = $(shell $(ROOTSYS)/bin/root-config --glibs)
ROOTLDFLAGS = $(shell $(ROOTSYS)/bin/root-config --ldflags)

CXX  = g++
CXX += -I./

CXXFLAGS  = -g -Wall -fPIC -Wno-deprecated
CXXFLAGS += $(ROOTCFLAGS)
CXXFLAGS += $(ROOTLIBS)
CXXFLAGS += $(ROOTGLIBS)
CXXFLAGS += $(ROOTLDFLAGS)

MakefileFullPath = $(abspath $(lastword $(MAKEFILE_LIST)))
MakefileDirFullPath = $(shell dirname $(MakefileFullPath))
OUTLIB = $(MakefileDirFullPath)/obj/
INSTALLDIR = $(MakefileDirFullPath)/install/

.PHONY: all makedir clean printmakeinfo

#----------------------------------------------------#

all: makedir ex01_push ex02_plot

makedir:
	mkdir -p $(OUTLIB);

printmakeinfo:
	$(info CXX                 = "$(CXX)")
	$(info CXXFLAGS            = "$(CXXFLAGS)")
	$(info MakefileFullPath    = "$(MakefileFullPath)")
	$(info MakefileDirFullPath = "$(MakefileDirFullPath)")
	$(info OUTLIB              = "$(OUTLIB)")

ex01_push: ex01_push.cc obj/libpushVectorInRoot.so
	$(CXX) -o $@ $< $(OUTLIB)*.so $(CXXFLAGS)

ex02_plot: ex02_plot.cc obj/libpushVectorInRoot.so
	$(CXX) -o $@ $< $(OUTLIB)*.so $(CXXFLAGS)

obj/%.o: %.cc libpushVectorInRoot.h
	$(CXX) $(CXXFLAGS) -c -I. -o $@ $<

obj/libpushVectorInRoot.so: obj/pushVectorInRoot.o
	$(CXX) -shared -o $@ $^ $(ROOTLIBS) $(ROOTGLIBS)

install: makedir obj/libpushVectorInRoot.so
	mkdir -p $(INSTALLDIR);
	ln -s $(OUTLIB)libpushVectorInRoot.so $(INSTALLDIR)libpushVectorInRoot.so
	ln -s $(MakefileDirFullPath)/libpushVectorInRoot.h $(INSTALLDIR)libpushVectorInRoot.h

clean:
	rm -f *~
	rm -f .*~
	rm -f ex01_push
	rm -f ex02_plot
	rm -rf install
	rm -rf obj
	rm -f $(OUTLIB)*
