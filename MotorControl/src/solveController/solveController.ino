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

/** 
 *  \brief Read an incoming instruction from the serial port.
 *  Instruction will be in the form of a function pointer, so 8 bytes are read and then
 *  concatenated into the pointer. If the pointer is valid, the function is executed and
 *  true is returned. A nullptr signifies the end of solving, and false is returned.
 *  
 *  \note Assumes 64-bit architecture.
 */
bool nextInstruction() {
  // The number of bytes in a pointer
  const int NUM_BYTES = 8;
  
  union {
    char[NUM_BYTES] bytes;
    MotorControl::FaceTurn function;
  } ptrBytes;

  for (int i = 0; i < NUM_BYTES; ++i) {
    while (!Serial.available()) {}
    ptrBytes.bytes[i] = Serial.read();
  }

  if (!ptrBytes.function) return false;
  ptrBytes.function(driver);
  return true;
}

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
  while(nextInstruction()) {}
}
