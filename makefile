# usage:
# make: build executables
# make doc: build doxygen

include config.mk

# Local includes
INCL += -IMotorControl/src/arduino-incl
ARDINCL +=

CXXFLAGS += $(INCL)
ARDCXXFLAGS += $(ARDINCL)

TARGETS = CommTest.exe #main.exe

COMM_TEST_OBJS = comm-test.o

MAIN_OBJS = main.o

EXTERNAL_OBJS = SerialComm/failed_read_error.o SerialComm/serial.o

# Build executables:
#	CommTest.exe: Sends a predetermined sequence of commands via the serial port
#	to the Arduino to trigger the motors. 
all: subdirectories $(TARGETS)
clean:
	cd SerialComm; make clean
	cd MotorControl; make clean
	cd "Cube Solver 2/Cube Solver 2"; make clean
	rm -rf doc
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

main.o: main.cpp
	$(CXX) -c $(CXXFLAGS) $<

main.exe: $(MAIN_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^ $(EXTERNAL_OBJS)

doc: FORCE
	doxygen doxygen.config; cd doc/latex; make
	cp doc/latex/refman.pdf ./doc
	mv doc/refman.pdf doc/doc.pdf

FORCE: