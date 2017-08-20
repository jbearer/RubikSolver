ROOT = ../

TARGETS = CubeCameraRead.exe KillMotors StepInput Camera-SlowStepperMotorControl.exe
OBJS = cubeCameraRead.o camera-SlowStepperMotorControl.o

CPPFLAGS=-g -std=c++11 -Wall -Wextra -pedantic
LDLIBS=-I/usr/local/include/opencv2 -I/usr/local/include 


all: $(TARGETS)

clean:
	rm -rf obj *.o *.exe
	rm -f $(TARGETS)
again: clean all

cubeCameraRead.o: cubeCameraRead.cpp
	g++ $(CPPFLAGS) cubeCameraRead.cpp -c -o cubeCameraRead.o $(LDLIBS) -lncurses

CubeCameraRead.exe: cubeCameraRead.o
	g++ -o CubeCameraRead.exe cubeCameraRead.o `pkg-config --cflags --libs opencv` -lncurses

KillMotors: killMotors.c
	gcc -o KillMotors killMotors.c

StepInput: stepInput.c
	gcc -o StepInput stepInput.c

camera-SlowStepperMotorControl.o: camera-SlowStepperMotorControl.cpp EasyPIO.h
	g++ -g -std=c++11 -Wall -Wextra camera-SlowStepperMotorControl.cpp  -c -o camera-SlowStepperMotorControl.o; 

Camera-SlowStepperMotorControl.exe: camera-SlowStepperMotorControl.o
	g++ -o Camera-SlowStepperMotorControl.exe camera-SlowStepperMotorControl.o -I/usr/local/include/opencv2



# CC = gcc
# CCFLAGS = -I -Wall

# SOURCES=$(wildcard *.c)
# OBJECTS=$(SOURCES:.c=.o)
# TARGET=des

# OBJDIR = obj

# all: $(TARGET)

# $(TARGET): $(OBJECTS)
# 	$(CC) -o $@ $^ $(LDFLAGS) 

# # $(OBJDIR)/%.o: %.c %.h
# # 	$(CC) $(CCFLAGS) -c $<

# $(OBJDIR)/%.o: %.c
# 	$(CC) $(CCFLAGS) -c $<

# clean:
# 	rm -f *.o $(TARGET)



##############################
# TARGET = prog
# LIBS = -lm
# CC = gcc
# CFLAGS = -g -Wall

# .PHONY: default all clean

# default: $(TARGET)
# all: default

# OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
# HEADERS = $(wildcard *.h)

# %.o: %.c $(HEADERS)
# 	$(CC) $(CFLAGS) -c $< -o $@

# .PRECIOUS: $(TARGET) $(OBJECTS)

# $(TARGET): $(OBJECTS)
# 	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

# clean:
# 	-rm -f *.o
# 	-rm -f $(TARGET)