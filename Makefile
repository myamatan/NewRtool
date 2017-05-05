TARGET	= Rtool

ROOTCFLAGS	:= $(shell root-config --cflags)
ROOTGLIBS	:= $(shell root-config --glibs)

CXXFLAGS 	= -Wall -O2 $(ROOTCFLAGS)

FLUKECC = Rtool.cc
OS := ${shell uname}

all: COMPILE_ENV $(TARGET)

COMPILE_ENV:
ifeq ($(OS),Darwin)
CC := clang++
CXXLIBS = -lboost_system -I/usr/local/Cellar/boost/1.61.0/ $(ROOTGLIBS) # 1.61.1
else
CC := g++
CXXLIBS = -lboost_system -I/usr/include/boost/ $(ROOTGLIBS)
endif

RtoolDict.cc: Rtool.hh RtoolLinkDef.h
	rootcint -f RtoolDict.cc -c Rtool.hh RtoolLinkDef.h

Rtool.o: $(FLUKECC)
	$(CC) -o $@ $(CXXFLAGS) -DRELAY2 -c $<

Rtool: Rtool.o RtoolDict.o
	$(CC) -o $@ $^ $(CXXLIBS)

.cc.o:
	$(CC) $(CXXFLAGS) -c $<

clean:
	rm -f $(TARGET) $(OBJS) RtoolDict.* *.o *~
