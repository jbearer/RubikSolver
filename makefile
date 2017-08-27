# usage:
# make: build executables
# make doc: build doxygen

include config.mk

# Local includes
INCL += -ICubeSolver/src -IImageProcess/
INCL += -I/usr/local/include/opencv2 -I/usr/local/include

CXXFLAGS += $(INCL)

TARGETS = main.exe test.exe

MAIN_OBJS = main.o

MOTOR_CONTROL_OBJS = $(addprefix StepperMotorControl/,EasyPIO StepperMotorController turnCube)

CUBE_SOLVER_OBJS = $(addprefix CubeSolver/obj/,cube turn cube_nums cubeSolver \
				   cubeEncoder turn_tables translate end_maps)

IMAGE_PROC_OBJS = $(addprefix ImageProcess/,colorFromTemplate startup)

EXTERNAL_OBJS = $(addsuffix .o,$(CUBE_SOLVER_OBJS) $(IMAGE_PROC_OBJS) $(MOTOR_CONTROL_OBJS))

# Build executables:
	# main.exe
all: subdirectories $(TARGETS)
clean:
	cd CubeSolver; make clean
	cd ImageProcess; make clean
	rm -rf doc
	rm -rf *.o *.exe $(TARGETS)
again: clean all

# Build objects from subdirectories
subdirectories:
	cd CubeSolver; make lib
	cd ImageProcess; make lib

main.o: main.cpp
	$(CXX) -c $(CXXFLAGS) $<

test.o: test.cpp
	$(CXX) -c $(CXXFLAGS) $<

main.exe: $(MAIN_OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $^ $(EXTERNAL_OBJS) $(LINKERFLAGS) `pkg-config --cflags --libs opencv` -lncurses

test.exe: test.o
	$(CXX) -o $@ $(CXXFLAGS) $^ $(EXTERNAL_OBJS) $(LINKERFLAGS) `pkg-config --cflags --libs opencv` -lncurses


doc: FORCE
	doxygen doxygen.config; cd doc/latex; make
	cp doc/latex/refman.pdf ./doc
	mv doc/refman.pdf doc/doc.pdf

FORCE:
