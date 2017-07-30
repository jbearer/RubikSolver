ROOT = ../

include $(ROOT)config.mk

INCL += -I$(ROOT)CubeSolver/src/ -I$(ROOT)MotorControl/src/arduino-incl
CXXFLAGS += $(INCL)

TARGETS = Train.exe
OBJS = train.o ../CubeSolver/obj/cube.o

# Build executables:
#	CubeTest.exe: Scramble and solve a test cube, and time the process.
all: $(TARGETS)

clean:
	rm -rf obj *.o *.exe $(TARGETS)
again: clean all

train.o: train.cpp
	$(CXX) $(CXXFLAGS) train.cpp -c -o train.o -I/usr/local/include/opencv2 -I/usr/local/include

Train.exe: $(OBJS)
	$(CXX) -o $@ $^ $(LINKERFLAGS) -I/usr/local/include/opencv2 -I/usr/local/include \
	 -lopencv_core -lopencv_videoio -lopencv_highgui -lopencv_imgcodecs

# todo: use `pkg-config --libs opencv2`
