# makefile
# use make or make all to
#	1. Copy include files into the proper Arduino sketch folders.
#	2. Build a binary for Arduino using AVR, and show compiler output without
#		having to build in Arduino IDE
#
# use make silent to do 1. but not 2.
#	make silent prepares sketches to be run from Arduino IDE

include ../config.mk

# Local includes
INCL +=
ARDINCL +=

# Path to source files
SRC_PATH = src

# Path to files used for testing but not used in the final build of the project
TEST_PATH = test

# Path to files included by arduino
ARD_INCL_PATH = $(SRC_PATH)/arduino-incl

SKETCHES = 

# Objects which need to be built for linking with external sources
# The library is currently header only. No objects need to be built.
# (Non-header files are meant only for use by Arduino, and thus are compiled
# separately in the Arduino environmnet.)
OBJS = triangle.o

TEST_OBJS = triangle-test.o

TESTS = TriangleTest.exe

# Copy arduino include files into each sketch directory, so that the sketches
# are ready to compile. Compiles but does not link all .cpp files included by
# arduino, so that compiler errors can be known simply by running make
# Does not build an executable
all: $(SKETCHES) $(OBJS) $(TEST_OBJS) $(TESTS)

lib: $(SKETCHES) $(OBJS)

clean:
	rm -rf *.o *.exe $(OBJS)
again: clean all

# Object targets

column-vector.o: src/column-vector.cc src/column-vector.h
	$(CXX) -c $(CXXFLAGS) $(INCL) $<

triangle.o: src/triangle.cc src/triangle.h src/column-vector.h src/column-vector-private.h
	$(CXX) -c $(CXXFLAGS) $(INCL) $<

triangle-test.o: test/triangle-test.cc src/triangle.h
	$(CXX) -c $(CXXFLAGS) $(INCL) $<

TriangleTest.exe: $(TEST_OBJS) $(OBJS)
	$(CXX) -o $@ $(LINKERFLAGS) $^ 
