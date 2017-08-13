/**
 * Austin Chun
 * Spring 2017
 * 
 * Rubiks Cube Stepper
 * 
 */


// Include Guards
#ifndef RUBIKS_CUBE
#define RUBIKS_CUBE

// Inlcude libraries 
#include "EasyPIO.h" // GPIO interfaces
#include <time.h> // time for seeding random num gen
#include <math.h>

///////////////////////////////////////////////////////////////////////
////////////////////// Define Constants ///////////////////////////////
///////////////////////////////////////////////////////////////////////

#define LOW  0
#define HIGH 1

// Define Raspi pins
#define disablePin0 3
#define disablePin1 2
#define disablePin2 18
#define disablePin3 23
#define disablePin4 24
#define disablePin5 25

#define stepPin0 8
#define stepPin1 7
#define stepPin2 12
#define stepPin3 16
#define stepPin4 20
#define stepPin5 21

#define dirPin    26
#define sleepPin  4

// Struct for pin selection
typedef struct {
  int stepPin;
  int disablePin;
} pins;

// Random Moves List
#define randMoveNum 40
int randFaces[randMoveNum];
int randDir[randMoveNum];
int randSteps[randMoveNum];
int randType[randMoveNum];


/// Define some structs for easy function returns

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

////////////////////////////////////////////////////////////////////
////////////////////// Define Functions ////////////////////////////
////////////////////////////////////////////////////////////////////
void generateRandomMoves();
void randomMixUp(enum Function fn);
void randomSolve(enum Function fn);
pins choosePins(int motor);
void lockMotorsMidTurn(int motor1, int motor2, int motor3, int state);
void stepSlow(int motor, int dir, int turns);
void stepFastish(int motor, int dir, int type);
void stepFastisher(int motor, int dir, int type);
void stepSym(int motor, int dir, int type);
void stepMidLock(int motor, int dir, int type, int next);
void stepLockSym(int motor, int dir, int type, int next);
void stepLinAccLockSym(int motor, int dir, int type, int next);
void setup();

#endif