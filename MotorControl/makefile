INCL = 
CXXFLAGS = -g -std=c++11 -Wall -Wextra $(INCL)
CXX = g++

# Path to source files
SRC_PATH = src

# Path to files used for testing but not used in the final build of the project
TEST_PATH = test

# Path to files included by arduino
ARD_INCL_PATH = $(SRC_PATH)/arduino-incl

ARD_INCLUDES = $(ARD_INCL_PATH)/motor-control.h \
$(ARD_INCL_PATH)/motor-control.cpp $(ARD_INCL_PATH)/motor-driver.h \
$(ARD_INCL_PATH)/motor-driver.cpp $(ARD_INCL_PATH)/comm-protocol.h

ARD_INCLUDES_LIST = {motor-control.h, motor-control.cpp, motor-driver.h, \
motor-driver.cpp, comm-protocol.h}

SKETCHES = $(SRC_PATH)/solveController $(TEST_PATH)/motorTest

# Objects which need to be built for linking with external sources
# The library is currently header only. No objects need to be built.
# (Non-header files are meant only for use by Arduino, and thus are compiled
# separately in the Arduino environmnet.)
OBJS = 

# Build objects necessary to link external code with code in this directory.
# Copy arduino include files into each sketch directory, so that the sketches
# are ready to compile.
# Does not build an executable
all: $(OBJS) $(SKETCHES)
clean:
	rm -rf *.o *.exe $(OBJS) $(SRC_PATH)/solveController/$(ARD_INCLUDES_LIST) \
	$(TEST_PATH)/motorTest/$(ARD_INCLUDES_LIST)
again: clean all

$(SRC_PATH)/solveController: $(ARD_INCLUDES)
	rm $</$(ARD_INCLUDES_LIST); \
	cp $(ARD_INCL_PATH)/$(ARD_INCLUDES_LIST) $@

$(TEST_PATH)/motorTest: $(ARD_INCLUDES)
	rm $</$(ARD_INCLUDES_LIST); \
	cp $(ARD_INCL_PATH)/$(ARD_INCLUDES_LIST) $@
