# 
# Copyright (c) 2013 No Face Press, LLC
# License http://opensource.org/licenses/mit-license.php MIT License
#

#This makefile is used to test the other Makefiles


PROG = povocop
SRC = povocop_main.cpp JobundleHandler.cpp

TOP = ../
CIVETWEB_LIB = libcivetweb.a

CFLAGS = -I$(TOP)/include $(COPT) 
CFLAGS += -g -DLUA_COMPAT_ALL -DUSE_LUA -DUSE_LUA_SQLITE3 -DUSE_LUA_FILE_SYSTEM -std=c++11
LIBS = -lpthread PJDL/libpjdl.a

include $(TOP)/resources/Makefile.in-os

ifeq ($(TARGET_OS),LINUX) 
	LIBS += -ldl libzip.a 
endif

all: PJDL  $(PROG) 

PJDL: PJDL/*cpp 
	make -C PJDL

$(PROG): $(CIVETWEB_LIB) $(SRC) PJDL/libpjdl.a
	$(CXX) -o $@ $(CFLAGS) $(LDFLAGS) $(SRC) $(CIVETWEB_LIB) $(LIBS)

$(CIVETWEB_LIB):
	$(MAKE) -C $(TOP) clean lib WITH_CPP=1
	cp $(TOP)/$(CIVETWEB_LIB) .

clean:
	rm -f $(CIVETWEB_LIB) $(PROG)

.PHONY: all clean
