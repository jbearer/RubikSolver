include ../../config.mk

INCL += -I../../include

CXXFLAGS += $(INCL)

CLEAN_XTNS +=

# Objects needed to link against these files
LIB_OBJS = unit-test.o

# Objects needed to build unit tests
TEST_OBJS =

TARGETS = $(LIB_OBJS) $(TEST_OBJS)

# Build objects for all source files
all: obj $(TARGETS)
	mkdir -p ../../test-obj; find . -type f -name '*.o' -exec mv {} ../../test-obj/ \;

# Build objects needed to link against headers in this directory
obj: $(LIB_OBJS)
	mkdir -p ../../obj; find . -type f -name '*.o' -exec mv {} ../../obj/ \;

clean:
	rm -rf $(CLEAN_XTNS) $(TARGETS)

again: clean all

unit-test.o: src/unit-test.cc src/unit-test.h
	$(CXX) -c $(CXXFLAGS) $<
