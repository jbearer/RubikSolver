ROOT = ../

include $(ROOT)config.mk

INCL += -I$(ROOT)MotorControl/src/arduino-incl -Isrc -isystem -I$(ROOT)CubeSolver/src -I$(ROOT)StepperMotorControl
ARDINCL +=

CXXFLAGS += $(INCL)
ARDCXXFLAGS += $(ARDINCL)

CUBE_SRC = $(ROOT)CubeSolver/src/
CUBE_OBJ = $(ROOT)CubeSolver/obj/

TARGETS =  CubeCapture.exe #ColorFromTemplate.exe
OBJS = cubeCapture.o colorFromTemplate.o startup.o

#CPPFLAGS=-g -std=c++11 -Wall -Wextra -pedantic
LDLIBS=-I/usr/local/include/opencv2 -I/usr/local/include

all: $(TARGETS)

clean:
	rm -rf obj *.o *.exe
again: clean all

cubeCapture.o: cubeCapture.cpp
	g++ $(CXXFLAGS) cubeCapture.cpp -c -o cubeCapture.o $(LDLIBS) -lncurses

colorFromTemplate.o: colorFromTemplate.hpp colorFromTemplate.cpp faceCoords.hpp
	g++ $(CXXFLAGS) colorFromTemplate.cpp -c -o colorFromTemplate.o $(LDLIBS)

startup.o: startup.hpp startup.cpp
	g++ $(CXXFLAGS) startup.cpp -c -o startup.o $(LDLIBS) -I$(ROOT)CubeSolver/src

CubeCapture.exe: cubeCapture.o
	g++ -o CubeCapture.exe cubeCapture.o `pkg-config --cflags --libs opencv` -lncurses

# ColorFromTemplate.exe: colorFromTemplate.o
# 	g++ -o ColorFromTemplate.exe colorFromTemplate.o $(CUBE_OBJ)cube.o `pkg-config --cflags --libs opencv`

# Startup.exe: startup.o
# 	$(MAKE) --directory=$(ROOT)CubeSolver
# 	g++ -o startup.exe startup.o $(CUBE_OBJ)cube.o `pkg-config --cflags --libs opencv`
