INCL = 

CXXFLAGS = -g -std=c++11 -Wall -Wextra $(INCL)
CXX = g++

TARGETS = ArduinoTest.exe
TEST_OBJS = arduino-test.o serial.o failed_read_error.o
SRC_PATH = src
TEST_PATH = test

all: $(TARGETS)
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