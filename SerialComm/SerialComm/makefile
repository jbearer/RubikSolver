CXXFLAGS = -g -std=c++11 -Wall -Wextra 
CXX = g++

TARGETS = ArduinoTest.exe
TEST_OBJS = arduino-test.o serial.o failed_read_error.o

all: $(TARGETS)
clean:
	rm -rf *.o *.exe $(TARGETS)
again: clean all

failed_read_error.o: failed_read_error.cpp failed_read_error.h
	$(CXX) -c $(CXXFLAGS) $<

serial.o: serial.cpp serial.h
	$(CXX) -c $(CXXFLAGS) $<

arduino-test.o: arduino-test.cpp test-constants.h serialstream.h
	$(CXX) -c $(CXXFLAGS) $<

ArduinoTest.exe: $(TEST_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^