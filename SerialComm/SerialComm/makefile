CXXFLAGS = -g -std=c++11 -Wall -Wextra 
CXX = g++

TARGETS = ArduinoTest.exe
TEST_OBJS = serialstream.o arduino-test.o serial.o

all: $(TARGETS)
clean:
	rm -rf *.o *.exe $(TARGETS)
again: clean all

serial.o: serial.cpp serial.h
	$(CXX) -c $(CXXFLAGS) $<

serialstream.o: serialstream.cpp serialstream.h serial.h
	$(CXX) -c $(CXXFLAGS) $<

arduino-test.o: arduino-test.cpp test-constants.h serialstream.h
	$(CXX) -c $(CXXFLAGS) $<

ArduinoTest.exe: $(TEST_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^