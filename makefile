include config.mk

# Local includes
INCL += 
ARDINCL +=

CXXFLAGS += $(INCL)
ARDCXXFLAGS += $(ARDINCL)

TARGETS = CommTest.exe

COMM_TEST_OBJS = comm-test.o

EXTERNAL_OBJS = SerialComm/failed_read_error.o SerialComm/serial.o

# Build executables:
#	CommTest.exe: Sends a predetermined sequence of commands via the serial port
#	to the Arduino to trigger the motors. 
all: subdirectories $(TARGETS)
clean:
	cd SerialComm; make clean
	cd MotorControl; make clean
	cd "Cube Solver 2/Cube Solver 2"; make clean
	rm -rf *.o *.exe $(TARGETS)
again: clean all

# Build objects from subdirectories
subdirectories:
	cd SerialComm; make lib
	cd MotorControl; make lib
	cd "Cube Solver 2/Cube Solver 2"; make lib

comm-test.o: comm-test.cpp
	$(CXX) -c $(CXXFLAGS) $<

CommTest.exe: $(COMM_TEST_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^ $(EXTERNAL_OBJS)