# usage:
# make: build executables
# make doc: build doxygen

include config.mk

# Local includes
INCL += -IMotorControl/src/arduino-incl -ISerialComm/src -ISerialComm/test
ARDINCL +=

CXXFLAGS += $(INCL)
ARDCXXFLAGS += $(ARDINCL)

TARGETS = CommProtocolTest.exe SerialTest.exe #main.exe

COMM_PROTOCOL_TEST_OBJS = comm-protocol-test.o

SERIAL_TEST_OBJS = serial-test.o

MAIN_OBJS = main.o

EXTERNAL_OBJS = SerialComm/failed_read_error.o SerialComm/serial.o

# Build executables:
#	CommTest.exe: Sends a predetermined sequence of commands via the serial port
#	to the Arduino to trigger the motors. 
all: subdirectories $(TARGETS)
clean:
	cd SerialComm; make clean
	cd MotorControl; make clean
	cd CubeSolver; make clean
	rm -rf doc
	rm -rf *.o *.exe $(TARGETS)
again: clean all

# Build objects from subdirectories
subdirectories:
	cd SerialComm; make lib
	cd MotorControl; make lib
	#cd CubeSolver; make lib

comm-test.o: comm-test.cpp
	$(CXX) -c $(CXXFLAGS) $<

serial-test.o: serial-test.cpp
	$(CXX) -c $(CXXFLAGS) $<

CommProtocolTest.exe: $(COMM_PROTOCOL_TEST_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^ $(EXTERNAL_OBJS)

SerialTest.exe: $(SERIAL_TEST_OBJS)
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