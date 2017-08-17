# A really dumb makefile for now
TARGETS =  CubeCapture.exe ColorFromTemplate.exe
OBJS = cubeCapture.o colorFromTemplate.o

CPPFLAGS=-g -std=c++11 -Wall -Wextra -pedantic
LDLIBS=-I/usr/local/include/opencv2 -I/usr/local/include

all: $(TARGETS)

clean:
	rm -rf obj *.o *.exe
again: clean all

cubeCapture.o: cubeCapture.cpp
	g++ $(CPPFLAGS) cubeCapture.cpp -c -o cubeCapture.o $(LDLIBS) -lncurses

CubeCapture.exe: cubeCapture.o
	g++ -o CubeCapture.exe cubeCapture.o `pkg-config --cflags --libs opencv` -lncurses


colorFromTemplate.o: colorFromTemplate.cpp faceCoords.hpp
	g++ $(CPPFLAGS) colorFromTemplate.cpp -c -o colorFromTemplate.o $(LDLIBS)

ColorFromTemplate.exe: colorFromTemplate.o
	g++ -o ColorFromTemplate.exe colorFromTemplate.o `pkg-config --cflags --libs opencv`
