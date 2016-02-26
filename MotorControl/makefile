# makefile
# use make or make all to
#	1. Copy include files into the proper Arduino sketch folders.
#	2. Build a binary for Arduino using AVR, and show compiler output without
#		having to build in Arduino IDE
#
# use make silent to do 1. but not 2.
#	make silent prepares sketches to be run from Arduino IDE

# External dependencies (optional - used for viewing compiler output without
# building in Arduino IDE):
#	Arduino cores: path depends on local PC configuration
#	winAVR

# Path to arduino libraries. Specific to local PC
AVR_PATH = C:/Program_Files_x86/Arduino/hardware/arduino/avr

# Pin layout (standard = Arduino Uno)
PIN_LAYOUT = standard

INCL = -I$(AVR_PATH)/cores/arduino -I$(AVR_PATH)/variants/standard

# Arduino Uno architecture
MCU = atmega328p

# Arduino processor frequency
F_CPU = 16000000UL

CXXFLAGS = -Wall -Wextra -pedantic -Os -DF_CPU=$(F_CPU) -std=c++11 -mmcu=$(MCU) $(INCL)
CXX = avr-g++

# Path to source files
SRC_PATH = src

# Path to files used for testing but not used in the final build of the project
TEST_PATH = test

# Path to files included by arduino
ARD_INCL_PATH = $(SRC_PATH)/arduino-incl

SKETCHES = $(TEST_PATH)/motorTest $(SRC_PATH)/solveController

# Objects which need to be built for linking with external sources
# The library is currently header only. No objects need to be built.
# (Non-header files are meant only for use by Arduino, and thus are compiled
# separately in the Arduino environmnet.)
OBJS = motor-control.o motor-driver.o 

# Copy arduino include files into each sketch directory, so that the sketches
# are ready to compile. Compiles but does not link all .cpp files included by
# arduino, so that compiler errors can be known simply by running make
# Does not build an executable
all: $(SKETCHES) $(OBJS)

# Copy arduino files into sketch directories, but do not compile anything;
# no compiler output
lib: $(SKETCHES)

clean:
	rm -rf *.o *.exe $(OBJS) $(TEST_PATH)/motorTest/motor-control.h \
	$(TEST_PATH)/motorTest/motor-control.cpp \
	$(TEST_PATH)/motorTest/motor-driver.h \
	$(TEST_PATH)/motorTest/motor-driver.cpp \
	$(TEST_PATH)/motorTest/comm-protocol.h \
	$(SRC_PATH)/solveController/motor-control.h \
	$(SRC_PATH)/solveController/motor-control.cpp \
	$(SRC_PATH)/solveController/motor-driver.h \
	$(SRC_PATH)/solveController/motor-driver.cpp \
	$(SRC_PATH)/solveController/comm-protocol.h
again: clean all

# Object targets

motor-control.o: $(ARD_INCL_PATH)/motor-control.cpp \
$(ARD_INCL_PATH)/motor-control.h $(ARD_INCL_PATH)/motor-driver.h \
$(ARD_INCL_PATH)/comm-protocol.h
	$(CXX) -c $(CXXFLAGS) $<

motor-driver.o: $(ARD_INCL_PATH)/motor-driver.cpp $(ARD_INCL_PATH)/motor-driver.h
	$(CXX) -c $(CXXFLAGS) $<

# Rules for motorTest sketch:

$(TEST_PATH)/motorTest/motor-control.h: $(ARD_INCL_PATH)/motor-control.h
	cp $< $(TEST_PATH)/motorTest

$(TEST_PATH)/motorTest/motor-control.cpp: $(ARD_INCL_PATH)/motor-control.cpp
	cp $< $(TEST_PATH)/motorTest

$(TEST_PATH)/motorTest/motor-driver.h: $(ARD_INCL_PATH)/motor-driver.h
	cp $< $(TEST_PATH)/motorTest

$(TEST_PATH)/motorTest/motor-driver.cpp: $(ARD_INCL_PATH)/motor-driver.cpp
	cp $< $(TEST_PATH)/motorTest

$(TEST_PATH)/motorTest/comm-protocol.h: $(ARD_INCL_PATH)/comm-protocol.h
	cp $< $(TEST_PATH)/motorTest

$(TEST_PATH)/motorTest: $(TEST_PATH)/motorTest/motor-control.h \
$(TEST_PATH)/motorTest/motor-control.cpp \
$(TEST_PATH)/motorTest/motor-driver.h \
$(TEST_PATH)/motorTest/motor-driver.cpp \
$(TEST_PATH)/motorTest/comm-protocol.h

# Rules for solveController sketch:

$(SRC_PATH)/solveController/motor-control.h: $(ARD_INCL_PATH)/motor-control.h
	cp $< $(SRC_PATH)/solveController

$(SRC_PATH)/solveController/motor-control.cpp: $(ARD_INCL_PATH)/motor-control.cpp
	cp $< $(SRC_PATH)/solveController

$(SRC_PATH)/solveController/motor-driver.h: $(ARD_INCL_PATH)/motor-driver.h
	cp $< $(SRC_PATH)/solveController

$(SRC_PATH)/solveController/motor-driver.cpp: $(ARD_INCL_PATH)/motor-driver.cpp
	cp $< $(SRC_PATH)/solveController

$(SRC_PATH)/solveController/comm-protocol.h: $(ARD_INCL_PATH)/comm-protocol.h
	cp $< $(SRC_PATH)/solveController

$(SRC_PATH)/solveController: $(SRC_PATH)/solveController/motor-control.h \
$(SRC_PATH)/solveController/motor-control.cpp \
$(SRC_PATH)/solveController/motor-driver.h \
$(SRC_PATH)/solveController/motor-driver.cpp \
$(SRC_PATH)/solveController/comm-protocol.h