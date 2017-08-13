
// Include Guards
#ifndef STEPPER_CONTROL
#define STEPPER_CONTROL

// Inlcude libraries 
#include "EasyPIO.h" // GPIO interfaces
#include <time.h> // time for seeding random num gen


///////////////////////////////////////////////////////////////////////
////////////////////// Define Constants ///////////////////////////////
///////////////////////////////////////////////////////////////////////

#define LOW  0
#define HIGH 1

// Define Raspi pins
// GPIO: 2, 3, 4, 17, 27, 22, 10, 9, 11, 5, 6, 13, 19, 26, 21, 20, 16, 12, 7, 8, 25, 24, 23, 18
//       2-27 (no 14, 15)

/// RUBIKS CUBE 
// const int dirPin1 = 4;
// const int dirPin2 = 17;
const int dirPin = 26;
// const int microStep = 26;

const int stepPin0  = 21;
const int sleepPin0 = 20;
const int stepPin1  = 16;
const int sleepPin1 = 12;
const int stepPin2  = 7;
const int sleepPin2 = 8;
const int stepPin3  = 25;
const int sleepPin3 = 24;
const int stepPin4  = 23;
const int sleepPin4 = 18;
const int stepPin5  = 3;
const int sleepPin5 = 2;

#define randMoveNum 40
int randFaces[randMoveNum];
int randDir[randMoveNum];
int randSteps[randMoveNum];
int randType[randMoveNum];


// Define some structs for easy function returns

// Struct for switch-case statement for pin selection
typedef struct {
  int stepPin;
  int sleepPin;
} pins;

// Struct for switch-case statement converting Turns to turnInfo
typedef struct{
  int motor;
  int dir;
  int turns;
} turnInfo;

// Enum of Turns (consistent with Daniel's naming)
enum Turn{
  F, R, B, L, U, D,
  F2, R2, B2, L2, U2, D2,
  Fi, Ri, Bi, Li, Ui, Di
};

enum Function{
  SLOW, FASTISH, FASTISHER, MIDLOCK, FAST, STEPSYM, LOCKSYM, LINACCLOCKSYM
};


///////////////////////////////////////////////////////////////////////
////////////////////// Create Random Moves ////////////////////////////
///////////////////////////////////////////////////////////////////////

void generateRandomMoves();
void randomMixUp(enum Function fn);
void randomSolve(enum Function fn);
pins choosePins(int motor);
void lockMotorsMidTurn(int motor1, int motor2, int motor3, int state);
void stepSlow(int motor, int dir, int turns);
void stepFastish(int motor, int dir, int type);
void stepFastisher(int motor, int dir, int type) ;
void stepSym(int motor, int dir, int type);
void stepMidLock(int motor, int dir, int type, int next);
void stepLockSym(int motor, int dir, int type, int next);
void stepLinAccLockSym(int motor, int dir, int type, int next);
void setup();
void main(void);



#endif