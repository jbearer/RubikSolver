include ../config.mk

# Local includes
INCL +=
ARDINCL +=

CXXFLAGS += $(INCL)
ARDCXXFLAGS += $(ARDINCL)

TARGETS = ArduinoTest.exe
OBJS = serial.o failed_read_error.o
TEST_OBJS = arduino-test.o serial.o failed_read_error.o
SRC_PATH = src
TEST_PATH = test

# Build executables:
#	ArduinoTest.exe: send some data via the serial port to the Arduino. Runs in
#	conjunction with test/arduino-test/arduino-test.ino
all: $(TARGETS)

# Build objects necessary to link external code with code in this directory
# Does not build an executable
lib: $(OBJS)

clean:
	rm -rf *.o *.exe $(TARGETS)
again: clean all

failed_read_error.o: $(SRC_PATH)/failed_read_error.cpp $(SRC_PATH)/failed_read_error.h
	$(CXX) -c $(CXXFLAGS) $<

serial.o: $(SRC_PATH)/serial.cpp $(SRC_PATH)/serial.h
	$(CXX) -c $(CXXFLAGS) $<

arduino-test.o: $(TEST_PATH)/arduino-test.cpp $(TEST_PATH)/arduino-test/test-constants.h $(SRC_PATH)/serialstream.h
	$(CXX) -c $(CXXFLAGS) $<

ArduinoTest.exe: $(TEST_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^