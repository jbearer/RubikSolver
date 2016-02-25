/**
 * Receive face-turn instructions from Serial port,
 * and send the corresponding instructions to the motors to solve the physical cube.
 */

#include "motor-control.h"
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

void setup() {
  pinMode(stepPinR,OUTPUT); pinMode(dirPinR,OUTPUT);
  pinMode(stepPinL,OUTPUT); pinMode(dirPinL,OUTPUT);
  pinMode(stepPinU,OUTPUT); pinMode(dirPinU,OUTPUT);
  pinMode(stepPinD,OUTPUT); pinMode(dirPinD,OUTPUT);
  pinMode(stepPinF,OUTPUT); pinMode(dirPinF,OUTPUT);
  pinMode(stepPinB,OUTPUT); pinMode(dirPinB,OUTPUT);

  Serial.begin(9600);
}

void loop() {
  FaceTurn next = MotorControl::getNextInstruction();
  next(driver);
}
