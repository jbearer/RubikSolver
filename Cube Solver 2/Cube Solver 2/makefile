include ../../config.mk

INCL += -I../../MotorControl/src/arduino-incl
ARDINCL += 

CXXFLAGS += $(INCL)
ARDCXXFLAGS += $(ARDINCL)

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