INCL = 
CXXFLAGS = -g -std=c++11 -Wall -Wextra $(INCL)
CXX = g++

# Path to source files
SRC_PATH = src

# Objects which need to be built for linking with external sources
OBJS = motor-control.o

# Build objects necessary to link external code with code in this directory
# Does not build an executable
lib: $(OBJS)
clean:
	rm -rf *.o *.exe $(OBJS)
again: clean all

motor-control.o: $(SRC_PATH)/motor-control.cpp $(SRC_PATH)/motor-control.h \
	$(SRC_PATH)/motor-driver.h
	$(CXX) -c $(CXXFLAGS) $<