include ../../config.mk

INCL += ../../include

CXXFLAGS += $(INCL)

CLEAN_XTNS +=

# Objects needed from this subdirectory to build the library
LIB_OBJS =

TARGETS = $(LIB_OBJS)

# Build objects needed from this subdirectory to compile the library
all: $(TARGETS)
	mkdir -p ../../obj; find . -type f -name '*.o' -exec mv {} ../../obj/ \;

clean:
	rm -rf $(CLEAN_XTNS) $(TARGETS)

again: clean all
