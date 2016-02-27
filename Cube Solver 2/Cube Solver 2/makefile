
# Path to boost root directory on local PC
BOOST_PATH = C:/Program_Files/boost_1_60_0

INCL = -I$(dir $(lastword $(MAKEFILE_LIST)))../../MotorControl/src/arduino-incl \
-I/$(BOOST_PATH)

CXXFLAGS = -g -std=c++11 -Wall -Wextra $(INCL)
CXX = g++

TARGETS = CubeTest.exe
OBJS = cube-test.o cube.o cube_turn.o

# Build executables:
#	CubeTest.exe: Scramble and solve a test cube, and time the process.
all: $(TARGETS)

# Build objects necessary to link external code with code in this directory
# Does not build an executable
lib: $(OBJS)

clean:
	rm -rf *.o *.exe $(TARGETS)

again: clean all

cube-test.o: cube-test.cpp cube.h
	$(CXX) -c $(CXXFLAGS) $<

cube.o: cube.cpp cube.h
	$(CXX) -c $(CXXFLAGS) $<

cube_turn.o: cube_turn.cpp cube.h
	$(CXX) -c $(CXXFLAGS) $<

CubeTest.exe: $(OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^