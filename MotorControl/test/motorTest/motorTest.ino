/**
  * Test the functionality of the motors using a predetermined sequence of turns that involves all motors.
  */

#include "motor-driver.h"
#include "motor-control.h"

using namespace MotorControl;

// initializes motor pins
const int stepPinR = 1; const int dirPinR = 2;
const int stepPinL = 3; const int dirPinL = 4;
const int stepPinU = 5; const int dirPinU = 6;
const int stepPinD = 7; const int dirPinD = 8;
const int stepPinF = 9; const int dirPinF = 10;
const int stepPinB = 11; const int dirPinB = 12;

MotorDriver driver(stepPinL, dirPinL,
                    stepPinR, dirPinR,
                    stepPinU, dirPinU,
                    stepPinD, dirPinD,
                    stepPinF, dirPinF,
                    stepPinB, dirPinB);

// a test sequence of actions to perform on the cube
const int NUM_STEPS = 18;
FaceTurn moves[NUM_STEPS] = {
        TURN_LEFT, TURN_RIGHT, TURN_UP, TURN_DOWN, TURN_FRONT, TURN_BACK,

        TURN_LEFT_INVERTED, TURN_RIGHT_INVERTED, TURN_UP_INVERTED, 
        TURN_DOWN_INVERTED, TURN_FRONT_INVERTED, TURN_BACK_INVERTED,

        TURN_LEFT_2, TURN_RIGHT_2, TURN_UP_2, TURN_DOWN_2, TURN_FRONT_2, 
        TURN_BACK_2
    };


// Perform a sequence of actions on the cube
void act( MotorControl::FaceTurn moves[], const int steps){
  // loop through list of steps
  for (int i = 0; i < steps; ++i) {
   moves[i](driver);
  }
}


/* setup
 *  establishes pins as outputs
 */
void setup() {
  pinMode(stepPinR,OUTPUT); pinMode(dirPinR,OUTPUT);
  pinMode(stepPinL,OUTPUT); pinMode(dirPinL,OUTPUT);
  pinMode(stepPinU,OUTPUT); pinMode(dirPinU,OUTPUT);
  pinMode(stepPinD,OUTPUT); pinMode(dirPinD,OUTPUT);
  pinMode(stepPinF,OUTPUT); pinMode(dirPinF,OUTPUT);
  pinMode(stepPinB,OUTPUT); pinMode(dirPinB,OUTPUT);
}

/* loop
 *  solves the cube
 */
void loop() {
  delay(3000);
  act(moves, NUM_STEPS);
}
