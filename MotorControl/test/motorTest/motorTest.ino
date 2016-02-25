/**
  * Test the functionality of the motors using a predetermined sequence of turns that involves all motors.
  */

#include "motor-driver.h"

// initializes motor pins
const int stepPinR = 1; const int dirPinR = 2;
const int stepPinL = 3; const int dirPinL = 4;
const int stepPinU = 5; const int dirPinU = 6;
const int stepPinD = 7; const int dirPinD = 8;
const int stepPinF = 9; const int dirPinF = 10;
const int stepPinB = 11; const int dirPinB = 12;

MotorDriver driver(stepPinL, driPinL,
                    stepPinR, driPinR,
                    stepPinU, driPinU,
                    stepPinD, driPinD,
                    stepPinF, driPinF,
                    stepPinB, driPinB);

// takes in solution
char moves[] = "rludfbRLUDFB2r2L2B";
const int numberOfSteps = 15;

/* solve
 *  solves the cube given a list of moves
 *  rotates the corresponding motor based on 
 *  solution list
 */
void solve( char moves[]){
  // loop through list of steps
  int i = 0;
  while (i<numberOfSteps){
    // Deals with clockwise turns
    if      (moves[i]=='r'){driver.step('R',1,HIGH); i++;}
    else if (moves[i]=='l'){driver.step('L',1,HIGH); i++;}
    else if (moves[i]=='u'){driver.step('U',1,HIGH); i++;}
    else if (moves[i]=='d'){driver.step('D',1,HIGH); i++;}
    else if (moves[i]=='f'){driver.step('F',1,HIGH); i++;}
    else if (moves[i]=='b'){driver.step('B',1,HIGH); i++;}
    // Deals with ccw turns
    else if (moves[i]=='R'){driver.step('R',1,LOW); i++;}
    else if (moves[i]=='L'){driver.step('L',1,LOW); i++;}
    else if (moves[i]=='U'){driver.step('U',1,LOW); i++;}
    else if (moves[i]=='D'){driver.step('D',1,LOW); i++;}
    else if (moves[i]=='F'){driver.step('F',1,LOW); i++;}
    else if (moves[i]=='B'){driver.step('B',1,LOW); i++;}
    // Deals with 180 deg turns
    else if (moves[i]=='2'){
      if      (moves[i+1]=='r'){driver.step('R',2,HIGH); i+=2;}
      else if (moves[i+1]=='l'){driver.step('L',2,HIGH); i+=2;}
      else if (moves[i+1]=='u'){driver.step('U',2,HIGH); i+=2;}
      else if (moves[i+1]=='d'){driver.step('D',2,HIGH); i+=2;}
      else if (moves[i+1]=='f'){driver.step('F',2,HIGH); i+=2;}
      else if (moves[i+1]=='b'){driver.step('B',2,HIGH); i+=2;}
    } 
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
  solve(moves);
}
