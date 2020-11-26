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

RELVERSION  = $(shell cat .release)

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
INSTALLDIR = $(MakefileDirFullPath)/install.$(RELVERSION)/
INSTALLDIR_USR_BIN = /usr/local/bin/
INSTALLDIR_USR_LIB = /usr/local/lib/
INSTALLDIR_USR_INC = /usr/local/include/
INSTALLDIR_USR_ECT = /usr/local/ect/

.PHONY: all makedir clean printmakeinfo

#----------------------------------------------------#

all: makedir ex01_push ex02_plot

makedir:
	mkdir -p $(OUTLIB);

printmakeinfo:
	$(info CXX                  = "$(CXX)")
	$(info CXXFLAGS             = "$(CXXFLAGS)")
	$(info MakefileFullPath     = "$(MakefileFullPath)")
	$(info MakefileDirFullPath  = "$(MakefileDirFullPath)")
	$(info OUTLIB               = "$(OUTLIB)")
	$(info INSTALLDIR           = "$(INSTALLDIR)")
	$(info INSTALLDIR_USR_BIN   = "$(INSTALLDIR_USR_BIN)")
	$(info INSTALLDIR_USR_LIB   = "$(INSTALLDIR_USR_LIB)")
	$(info INSTALLDIR_USR_INC   = "$(INSTALLDIR_USR_INC)")
	$(info INSTALLDIR_USR_ECT   = "$(INSTALLDIR_USR_ECT)")
	$(info RELVERSION           = "$(RELVERSION)")

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
	cp $(OUTLIB)libpushVectorInRoot.so $(INSTALLDIR)libpushVectorInRoot.so
	cp $(MakefileDirFullPath)/libpushVectorInRoot.h $(INSTALLDIR)libpushVectorInRoot.h
	sudo ln -s $(INSTALLDIR)libpushVectorInRoot.so $(INSTALLDIR_USR_LIB)libpushVectorInRoot.so
	sudo ln -s $(INSTALLDIR)libpushVectorInRoot.h $(INSTALLDIR_USR_INC)libpushVectorInRoot.h

cleaninstall:
	rm -rf $(INSTALLDIR)
	sudo rm -rf $(INSTALLDIR_USR_LIB)libpushVectorInRoot.so
	sudo rm -rf $(INSTALLDIR_USR_INC)libpushVectorInRoot.h

clean:
	rm -f *~
	rm -f .*~
	rm -f ex01_push
	rm -f ex02_plot
	rm -rf obj
	rm -f $(OUTLIB)*
