# TODO have clean delete the copies of the Arduino include files
# TODO make each Arduino include target dependent not only on the original files
#	but on the existence of the copies in each sketch folder

INCL = 
CXXFLAGS = -g -std=c++11 -Wall -Wextra $(INCL)
CXX = g++

# Path to source files
SRC_PATH = src

# Path to files used for testing but not used in the final build of the project
TEST_PATH = test

# Path to files included by arduino
ARD_INCL_PATH = $(SRC_PATH)/arduino-incl

ARD_INCLUDES = motor-control motor-driver comm-protocol

# Objects which need to be built for linking with external sources
# The library is currently header only. No objects need to be built.
# (Non-header files are meant only for use by Arduino, and thus are compiled
# separately in the Arduino environmnet.)
OBJS = 

# Build objects necessary to link external code with code in this directory.
# Copy arduino include files into each sketch directory, so that the sketches
# are ready to compile.
# Does not build an executable
all: $(OBJS) $(ARD_INCLUDES)
clean:
	rm -rf *.o *.exe $(OBJS)
again: clean all

motor-control: $(ARD_INCL_PATH)/motor-control.cpp $(ARD_INCL_PATH)/motor-control.h
	cp $(ARD_INCL_PATH)/$@.h $(SRC_PATH)/solveController;\
	cp $(ARD_INCL_PATH)/$@.cpp $(SRC_PATH)/solveController;\
	cp $(ARD_INCL_PATH)/$@.h $(TEST_PATH)/motorTest;\
	cp $(ARD_INCL_PATH)/$@.cpp $(TEST_PATH)/motorTest;\

motor-driver: $(ARD_INCL_PATH)/motor-driver.cpp $(ARD_INCL_PATH)/motor-driver.h
	cp $(ARD_INCL_PATH)/$@.h $(SRC_PATH)/solveController;\
	cp $(ARD_INCL_PATH)/$@.cpp $(SRC_PATH)/solveController;\
	cp $(ARD_INCL_PATH)/$@.h $(TEST_PATH)/motorTest;\
	cp $(ARD_INCL_PATH)/$@.cpp $(TEST_PATH)/motorTest;\

comm-protocol: $(ARD_INCL_PATH)/comm-protocol.h
	cp $(ARD_INCL_PATH)/$@.h $(SRC_PATH)/solveController;\
	cp $(ARD_INCL_PATH)/$@.h $(TEST_PATH)/motorTest;\