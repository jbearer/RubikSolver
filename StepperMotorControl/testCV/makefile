# A really dumb makefile for now
TARGETS =  Threshold.exe ObjectTrackingTutorial.exe MotionTracking.exe CannyEdgeDetection.exe
OBJS = objectTrackingTutorial.o motionTracking.o cannyEdgeDetection.o

# Build executables:
#	CubeTest.exe: Scramble and solve a test cube, and time the process.
all: $(TARGETS)

clean:
	rm -rf obj *.o *.exe
again: clean all

objectTrackingTutorial.o: objectTrackingTutorial.cpp
	g++ -g -std=c++11 -Wall -Wextra -pedantic objectTrackingTutorial.cpp -c -o objectTrackingTutorial.o -I/usr/local/include/opencv2 -I/usr/local/include

ObjectTrackingTutorial.exe: $(OBJS)
	g++ -o ObjectTrackingTutorial.exe objectTrackingTutorial.o `pkg-config --cflags --libs opencv` 
	# g++ -o Tutorial.exe objectTrackingTutorial.o -I/usr/local/include/opencv2 -I/usr/local/include  -lopencv_core -lopencv_videoio -lopencv_highgui -lopencv_imgcodecs


Threshold.exe: threshold.cpp
	g++ threshold.cpp -o Threshold.exe `pkg-config --cflags --libs opencv` 

motionTracking.o: motionTracking.cpp
	g++ -g -std=c++11 -Wall -Wextra -pedantic motionTracking.cpp -c -o motionTracking.o -I/usr/local/include/opencv2 -I/usr/local/include

MotionTracking.exe: $(OBJS)
	g++ -o MotionTracking.exe motionTracking.o `pkg-config --cflags --libs opencv` 


cannyEdgeDetection.o: cannyEdgeDetection.cpp
	g++ -g -std=c++11 -Wall -Wextra -pedantic cannyEdgeDetection.cpp -c -o cannyEdgeDetection.o -I/usr/local/include/opencv2 -I/usr/local/include

CannyEdgeDetection.exe: $(OBJS)
	g++ -o CannyEdgeDetection.exe cannyEdgeDetection.o `pkg-config --cflags --libs opencv` 

# todo: use `pkg-config --libs opencv2`