# A really dumb makefile for now
TARGETS = Train.exe
OBJS = train.o 

# Build executables:
#	CubeTest.exe: Scramble and solve a test cube, and time the process.
all: $(TARGETS)

clean:
	rm -rf obj *.o *.exe
again: clean all

train.o: train.cpp
	g++ -g -std=c++11 -Wall -Wextra -pedantic train.cpp -c -o train.o -I/usr/local/include/opencv2 -I/usr/local/include

Train.exe: $(OBJS)
	 g++ -o Train.exe train.o -I/usr/local/include/opencv2 -I/usr/local/include  -lopencv_core -lopencv_videoio -lopencv_highgui -lopencv_imgcodecs

# todo: use `pkg-config --libs opencv2`
