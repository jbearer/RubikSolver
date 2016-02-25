INCL = 
CXXFLAGS = -g -std=c++11 -Wall -Wextra $(INCL)
CXX = g++

include SerialComm/makefile
include MotorControl/makefile

TARGETS = CommTest.exe

COMM_TEST_OBJS = comm-test.o

# Build executables:
#	CommTest.exe: Sends a predetermined sequence of commands via the serial port
#	to the Arduino to trigger the motors. 
all: Dependencies $(TARGETS)
clean:
	rm -rf *.o *.exe $(COMM_TEST_OBJS)
again: clean all

# Build objects from subdirectories
Dependencies:
	cd SerialComm; make lib
	cd MotorControl; make lib

comm-test.o: comm-test.cpp SerialComm/src/serialstream.h \
	MotorControl/src/motor-control.h
	$(CXX) -c $(CXXFLAGS) $<

CommTest.exe: $(COMM_TEST_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^