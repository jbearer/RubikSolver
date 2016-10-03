ROOT = ../

include $(ROOT)config.mk

INCL += -I$(ROOT)MotorControl/src/arduino-incl -Isrc -isystem $(GTEST_DIR)/include
ARDINCL += 

CXXFLAGS += $(INCL)
ARDCXXFLAGS += $(ARDINCL)

TARGETS = Cube.exe CubeTest.exe
OBJS = obj/cube.o obj/turn.o obj/cube_nums.o obj/cubeSolver.o obj/turn_tables.o obj/end_maps.o
TEST_OBJS = $(OBJS) obj/cube-test.o $(TEST_OBJ)/gtest_main.a
SRC_PATH = src
TEST_PATH = test
GTEST_DIR = /usr/src/gtest/googletest

SOLVER_OBJS = obj/cube.o obj/turn.o obj/cube_nums.o obj/cubeSolver.o
TEST_OBJS = $(OBJS) obj/turn_tables.o obj/end_maps.o obj/cube-test.o $(TEST_OBJ)/gtest_main.a


# Flags passed to the preprocessor.
# We have no idea here whether pthreads is available in the system, so
# disable its use.
CPPFLAGS += -DGTEST_HAS_PTHREAD=0

TESTS = CubeTest.exe



# Build executables:
#	CubeTest.exe: Scramble and solve a test cube, and time the process.
all: $(TARGETS)

# build and run tests
test: CubeTest.exe
	./CubeTest.exe --gtest_filter=-Build.*:CubeSolverPerf.*

test_build: CubeTest.exe
	./CubeTest.exe --gtest_filter=Build.*

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

obj/cube.o: $(SRC_PATH)/cube.cpp $(SRC_PATH)/cube.h
	@mkdir -p $(@D)
	$(CXX) -c $(CXXFLAGS) $< -o $@

obj/cubeSolver.o: $(SRC_PATH)/cubeSolver.cpp $(SRC_PATH)/cubeSolver.h
	@mkdir -p $(@D)
	$(CXX) -c $(CXXFLAGS) $< -o $@

obj/turn.o: $(SRC_PATH)/turn.cpp $(SRC_PATH)/cube.h
	@mkdir -p $(@D)
	@$(CXX) -c $(CXXFLAGS) $< -o $@

obj/turn_tables.o: $(SRC_PATH)/turn_tables.cpp $(SRC_PATH)/cubeSolver.h
	@mkdir -p $(@D)
	$(CXX) -c $(CXXFLAGS) $< -o $@

obj/end_maps.o: $(SRC_PATH)/end_maps.cpp $(SRC_PATH)/cubeSolver.h
	@mkdir -p $(@D)
	@$(CXX) -c $(CXXFLAGS) $< -o $@

obj/cube_nums.o: $(SRC_PATH)/cube_nums.cpp $(SRC_PATH)/cubeSolver.h
	@mkdir -p $(@D)
	@$(CXX) -c $(CXXFLAGS) $< -o $@

#####################################
########## TEST FILES ###############
#####################################


# Adapted from googletest sample makefile

# All Google Test headers.  Usually you shouldn't change this
# definition.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

TEST_OBJ = test/test_obj

$(TEST_OBJ)/gtest-all.o : $(GTEST_SRCS_)
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest-all.cc -o $@

$(TEST_OBJ)/gtest_main.o : $(GTEST_SRCS_)
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
            $(GTEST_DIR)/src/gtest_main.cc -o $@

$(TEST_OBJ)/gtest.a : $(TEST_OBJ)/gtest-all.o
	@mkdir -p $(@D)
	$(AR) $(ARFLAGS) $@ $^ -o $@

$(TEST_OBJ)/gtest_main.a : $(TEST_OBJ)/gtest-all.o $(TEST_OBJ)/gtest_main.o
	@mkdir -p $(@D)
	$(AR) $(ARFLAGS) $@ $^ -o $@

obj/cube-test.o: $(TEST_PATH)/cube-test.cpp $(SRC_PATH)/cubeSolver.h $(GTEST_HEADERS)
	@mkdir -p $(@D)
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

obj/main.o: $(SRC_PATH)/main.cpp
	@mkdir -p $(@D)
	$(CXX) -c $(CXXFLAGS) $< -o $@


#######################################
######### EXECUTABLES #################
#######################################

Cube.exe: $(OBJS) obj/main.o
	$(CXX) -o $@ $^ $(LINKERFLAGS)

CubeTest.exe: $(TEST_OBJS)
	@$(CXX) $(CPPFLAGS) -o $@ $(CXXFLAGS) $^ $(LINKERFLAGS)
