# usage:
# make: build executables
# make doc: build doxygen

include config.mk

# Local includes
INCL += -IMotorControl/src/arduino-incl -isystem -ISerialComm/src -ISerialComm/test -ICubeSolver/src -IImageProcess/
INCL += -I/usr/local/include/opencv2 -I/usr/local/include
ARDINCL +=

CXXFLAGS += $(INCL)
ARDCXXFLAGS += $(ARDINCL)

TARGETS = main.exe #CommProtocolTest.exe SerialTest.exe main.exe

COMM_PROTOCOL_TEST_OBJS = comm-protocol-test.o

SERIAL_TEST_OBJS = serial-test.o

MAIN_OBJS = main.o

MOTOR_CONTROL_OBJS = StepperMotorControl/EasyPIO.o \
					 StepperMotorControl/StepperMotorController.o \
					 StepperMotorControl/turnCube.o

EXTERNAL_OBJS = CubeSolver/obj/cube.o CubeSolver/obj/turn.o CubeSolver/obj/cube_nums.o CubeSolver/obj/cubeSolver.o CubeSolver/obj/cubeEncoder.o \
				CubeSolver/obj/turn_tables.o ImageProcess/colorFromTemplate.o ImageProcess/startup.o \
				CubeSolver/obj/translate.o CubeSolver/obj/end_maps.o \
				$(MOTOR_CONTROL_OBJS)

#EXTERNAL_OBJS = SerialComm/failed_read_error.o SerialComm/serial.o

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
	# cd SerialComm; make lib
	cd MotorControl; make lib
	cd CubeSolver; make lib

# comm-test.o: comm-test.cpp
# 	$(CXX) -c $(CXXFLAGS) $<

# serial-test.o: serial-test.cpp
# 	$(CXX) -c $(CXXFLAGS) $<

# CommProtocolTest.exe: $(COMM_PROTOCOL_TEST_OBJS)
# 	$(CXX) -o $@ $(CXXFLAGS) $^ $(EXTERNAL_OBJS)

# SerialTest.exe: $(SERIAL_TEST_OBJS)
# 	$(CXX) -o $@ $(CXXFLAGS) $^ $(EXTERNAL_OBJS)

main.o: main.cpp
	$(CXX) -c $(CXXFLAGS) $<

main.exe: $(MAIN_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^ $(EXTERNAL_OBJS) $(LINKERFLAGS) `pkg-config --cflags --libs opencv`

doc: FORCE
	doxygen doxygen.config; cd doc/latex; make
	cp doc/latex/refman.pdf ./doc
	mv doc/refman.pdf doc/doc.pdf

FORCE:
