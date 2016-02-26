INCL = 
CXXFLAGS = -g -std=c++11 -Wall -Wextra $(INCL)
CXX = g++

TARGETS = CommTest.exe

COMM_TEST_OBJS = comm-test.o

EXTERNAL_OBJS = SerialComm/failed_read_error.o SerialComm/serial.o

# Build executables:
#	CommTest.exe: Sends a predetermined sequence of commands via the serial port
#	to the Arduino to trigger the motors. 
all: Dependencies $(TARGETS)
clean:
	cd SerialComm; make clean
	cd MotorControl; make clean
	rm -rf *.o *.exe $(TARGETS)
again: clean all

# Build objects from subdirectories
Dependencies:
	cd SerialComm; make lib
	cd MotorControl; make lib

comm-test.o: comm-test.cpp
	$(CXX) -c $(CXXFLAGS) $<

CommTest.exe: $(COMM_TEST_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^ $(EXTERNAL_OBJS)