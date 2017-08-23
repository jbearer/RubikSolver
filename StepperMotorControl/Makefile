ROOT = ../

TARGETS = CubeCameraRead.exe KillMotors StepInput Camera-SlowStepperMotorControl.exe
OBJS = cubeCameraRead.o camera-SlowStepperMotorControl.o \
	EasyPIO.o StepperMotorController.o turnCube.o

CPPFLAGS=-g -std=c++1y -Wall -Wextra -pedantic
LDLIBS=-I/usr/local/include/opencv2 -I/usr/local/include

INCL = -I$(ROOT)CubeSolver/src -I$(ROOT)SerialComm/src -I$(ROOT)MotorControl/src/arduino-incl
CPPFLAGS += $(INCL)

all: $(TARGETS) $(OBJS)

clean:
	rm -rf obj *.o *.exe
	rm -f $(TARGETS)
again: clean all

cubeCameraRead.o: cubeCameraRead.cpp turnCube.hpp
	g++ $(CPPFLAGS) cubeCameraRead.cpp -c -o cubeCameraRead.o $(LDLIBS) -lncurses

EasyPIO.o: EasyPIO.c EasyPIO.h
	gcc -c EasyPIO.c

StepperMotorController.o: StepperMotorController.c StepperMotorController.h
	gcc -c StepperMotorController.c

turnCube.o: turnCube.cpp turnCube.hpp
	g++ $(CPPFLAGS) turnCube.cpp -c -o turnCube.o

camera-SlowStepperMotorControl.o: camera-SlowStepperMotorControl.cpp EasyPIO.h
	g++ -g -std=c++11 -Wall -Wextra camera-SlowStepperMotorControl.cpp  -c -o camera-SlowStepperMotorControl.o;

CubeCameraRead.exe: cubeCameraRead.o
	g++ -o CubeCameraRead.exe EasyPIO.o StepperMotorController.o turnCube.o cubeCameraRead.o `pkg-config --cflags --libs opencv` -lncurses


KillMotors: killMotors.c
	gcc -o KillMotors killMotors.c

StepInput: stepInput.c
	gcc -o StepInput stepInput.c


Camera-SlowStepperMotorControl.exe: camera-SlowStepperMotorControl.o
	g++ -o Camera-SlowStepperMotorControl.exe $(OBJS) -I/usr/local/include/opencv2

