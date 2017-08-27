# usage:
# make: build executables
# make test: make and execute normal tests
# make test_perf: make and execute performance tests

ROOT = ../
include $(ROOT)config.mk

SRC_PATH = src
TEST_PATH = test
OBJ_PATH = obj

INCL += -I$(SRC_PATH)
CXXFLAGS += $(INCL)

GTEST_DIR = /usr/src/gtest/googletest
TEST_INCL = -isystem $(GTEST_DIR)/include

# Google Test variables, from sample makefile
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

GTEST_OBJ = test/test_obj

# Objects and targets:
OBJ_FILES = cube turn cube_nums cubeSolver turn_tables end_maps cubeEncoder translate
OBJS = $(addsuffix .o,$(addprefix $(OBJ_PATH)/,$(OBJ_FILES)))
TEST_OBJS = $(OBJS) $(OBJ_PATH)/cube-test.o $(GTEST_OBJ)/gtest_main.a

TARGETS = Cube.exe CubeTest.exe

# Flags passed to the preprocessor.
# We have no idea here whether pthreads is available in the system, so
# disable its use.
GTEST_FLAGS += -DGTEST_HAS_PTHREAD=0

# Build executables:

all: $(TARGETS)

# build and run tests:
# make test: make and execute normal tests
test: CubeTest.exe
	./CubeTest.exe --gtest_filter=-Build.*:CubeSolverPerf.*

# make test_build: make and execute tests for serializing solver resources
test_build: CubeTest.exe
	./CubeTest.exe --gtest_filter=Build.*

# make test_perf: make and execute performance test (time solve for 500 cubes)
test_perf: CubeTest.exe
	./CubeTest.exe --gtest_filter=CubeSolverPerf.*

# Build objects necessary to link external code with code in this directory
# Does not build an executable
lib: $(OBJS)

clean:
	rm -rf obj *.exe $(TARGETS)
again: clean all

#######################################
############# SRC FILES ###############
#######################################

SRC_RECIPE = @$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJ_PATH)/turn.o: $(SRC_PATH)/turn.cpp $(SRC_PATH)/turn.h
	@mkdir -p $(@D)
	$(SRC_RECIPE)

$(OBJ_PATH)/cube.o: $(SRC_PATH)/cube.cpp $(SRC_PATH)/cube.h $(SRC_PATH)/turn.h
	@mkdir -p $(@D)
	$(SRC_RECIPE)

$(OBJ_PATH)/translate.o: $(SRC_PATH)/translate.cpp $(SRC_PATH)/translate.h \
		$(SRC_PATH)/cube.h
	@mkdir -p $(@D)
	$(SRC_RECIPE)

$(OBJ_PATH)/cubeEncoder.o: $(SRC_PATH)/cubeEncoder.cpp $(SRC_PATH)/cubeEncoder.h \
		$(SRC_PATH)/cube.h
	@mkdir -p $(@D)
	$(SRC_RECIPE)

$(OBJ_PATH)/turn_tables.o: $(SRC_PATH)/turn_tables.cpp $(SRC_PATH)/turn_tables.h \
		$(SRC_PATH)/cube.h $(SRC_PATH)/turn.h
	@mkdir -p $(@D)
	$(SRC_RECIPE)

$(OBJ_PATH)/end_maps.o: $(SRC_PATH)/end_maps.cpp $(SRC_PATH)/cubeSolver.h
	@mkdir -p $(@D)
	$(SRC_RECIPE)

$(OBJ_PATH)/cube_nums.o: $(SRC_PATH)/cube_nums.cpp $(SRC_PATH)/cube_nums.h \
		$(SRC_PATH)/cubeSolver.h $(SRC_PATH)/cube.h
	@mkdir -p $(@D)
	$(SRC_RECIPE)

$(OBJ_PATH)/cubeSolver.o: $(SRC_PATH)/cubeSolver.cpp $(SRC_PATH)/cubeSolver.h \
		$(SRC_PATH)/cube.h $(SRC_PATH)/cube_nums.h $(SRC_PATH)/cubeEncoder.h \
		$(SRC_PATH)/turn.h $(SRC_PATH)/turn_tables.h $(SRC_PATH)/translate.h
	@mkdir -p $(@D)
	$(SRC_RECIPE)

$(OBJ_PATH)/main.o: $(SRC_PATH)/main.cpp
	@mkdir -p $(@D)
	$(SRC_RECIPE)

#####################################
########## TEST FILES ###############
#####################################

# Adapted from googletest sample makefile

$(GTEST_OBJ)/gtest-all.o : $(GTEST_SRCS_)
	@mkdir -p $(@D)
	$(CXX) $(GTEST_FLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc -o $@

$(GTEST_OBJ)/gtest_main.o : $(GTEST_SRCS_)
	@mkdir -p $(@D)
	$(CXX) $(GTEST_FLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc -o $@

$(GTEST_OBJ)/gtest.a : $(GTEST_OBJ)/gtest-all.o
	@mkdir -p $(@D)
	$(AR) $(ARFLAGS) $@ $^ -o $@

$(GTEST_OBJ)/gtest_main.a : $(GTEST_OBJ)/gtest-all.o $(GTEST_OBJ)/gtest_main.o
	@mkdir -p $(@D)
	$(AR) $(ARFLAGS) $@ $^ -o $@

$(OBJ_PATH)/cube-test.o: $(TEST_PATH)/cube-test.cpp $(SRC_PATH)/cubeSolver.h $(GTEST_HEADERS)
	@mkdir -p $(@D)
	$(CXX) $(GTEST_FLAGS) $(CXXFLAGS) $(TEST_INCL) -c $< -o $@

#######################################
######### EXECUTABLES #################
#######################################

Cube.exe: $(OBJS) $(OBJ_PATH)/main.o
	$(CXX) -o $@ $^ $(LINKERFLAGS)

CubeTest.exe: $(TEST_OBJS)
	$(CXX) $(GTEST_FLAGS) -o $@ $(CXXFLAGS) $^ $(LINKERFLAGS)
