/**
 * Austin Chun
 * 
 * twoFaceTurn.cpp
 *      Turn opposite faces in independant directions at the same time
 * 
 */

/*
g++ -g -std=c++11 -Wall -Wextra twoFaceTurn.cpp -c -o twoFaceTurn.o; g++ -o TwoFaceTurn.exe twoFaceTurn.o
*/

extern "C"{
  #include "StepperMotorController.h"
}

#include <stdio.h>
#include <unistd.h>


#define dirPin1 26
#define dirPin2 19

void stepTwoFace(int motor1, int motor2, int dir1, int dir2, int turns) {
  int i = 0; // Cause C is stupid

  // Define pins to use
  pins p1 = choosePins(motor1);
  int stepPinA = p1.stepPin;
  int disablePinA = p1.disablePin;
  pins p2 = choosePins(motor2);
  int stepPinB = p2.stepPin;
  int disablePinB = p2.disablePin;

  int stepNum;
  int accelSteps;

  // Define num steps and num accelSteps
  if(turns == 1) {
    stepNum = 50;
    accelSteps = 40;
  }
  else{
    stepNum = 100;
    accelSteps = 85;
  }
  
  // Wake up motor
  digitalWrite(disablePinA, LOW);
  digitalWrite(disablePinB, LOW);
  // Set the direction
  digitalWrite(dirPin1, dir1);
  digitalWrite(dirPin2, dir2);
  delayMillis(1);

  // Play with this
  int deay = 100;

  // Pulse motor stepNum times
  for (i = 0; i < stepNum*32 - 4; i++) {
    // Pulse HIGH-LOW
    digitalWrite(stepPinA, HIGH); digitalWrite(stepPinB, HIGH); delayMicros(deay);
    digitalWrite(stepPinA, LOW);  digitalWrite(stepPinB, LOW);  delayMicros(deay);

    if(deay > 25 && i < accelSteps*32){
      deay = deay - 1;
    }
    else if(deay < 100 && i > accelSteps*32){
      deay = deay + 2;
    }
    // printf("%d\n", deay);
  }

  delayMillis(1);

  // Turn off motor(s)  
  digitalWrite(disablePin1, HIGH);
  digitalWrite(disablePin1, HIGH);
}


int main(void){

  pioInit();
  setup();

  pinMode(dirPin1, OUTPUT);
  pinMode(dirPin2, OUTPUT);

  printf("3...\n");
  sleep(1);
  printf("2...\n");
  sleep(1);
  printf("1...\n");
  sleep(1);
  printf("Moving\n");
  sleep(2);
  digitalWrite(sleepPin, HIGH); // Turn motors on

  // stepTwoFace(0, 3, 1, 1, 1);

  // Execution
  int i = 0;
  for(i = 0; i < 3; i++){
    stepTwoFace(i, i+3, 0, 1, 2);
    delayMillis(100);
  }
  delayMillis(1000);
  for(i = 0; i < 3; i++){
    stepTwoFace(i, i+3, 1, 1, 2);
    delayMillis(100);
  }

  digitalWrite(sleepPin, LOW);

  return 0;

}



