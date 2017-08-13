/**
 * Austin Chun
 * 
 * Stepper Motor Controller Header (PCB)
 * 
 */

// Include Guards
#ifndef STEPPER_CONTROL
#define STEPPER_CONTROL

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


/**
 * @brief      case-switch statement to designate appropriate stepPin and disablePin
 *
 * @param[in]  motor  The motor number
 *
 * @return     struct with stepPin and disablePin
 */
pins choosePins(int motor) {
  int stepPin;
  int disablePin;
  // Define pins to use
  switch (motor) {
    case 0:
      stepPin = stepPin0; disablePin = disablePin0;
      break;
    case 1:
      stepPin = stepPin1; disablePin = disablePin1;
      break;
    case 2:
      stepPin = stepPin2; disablePin = disablePin2;
      break;
    case 3:
      stepPin = stepPin3; disablePin = disablePin3;
      break;
    case 4:
      stepPin = stepPin4; disablePin = disablePin4;
      break;
    case 5:
      stepPin = stepPin5; disablePin = disablePin5;
      break;
    default:
      stepPin = -1; disablePin = -1; // Undefined
      break;
  }
  pins p = {stepPin, disablePin};
  return p;
}


/**
 * @brief      step function (full arguments)
 *
 * @param[in]  motor    motor number
 * @param[in]  dir      direction
 * @param[in]  steps    number of steps
 * @param[in]  deay     time delay (us)
 * @param[in]  uFactor  microstep Factor (1 = normal, 32 = 1/32 stepping)
 */
void step(int motor, int dir, int steps, int deay, int uFactor) {
  int i = 0; 

  // Define pins to use
  pins p = choosePins(motor);
  int stepPin = p.stepPin;
  int disablePin = p.disablePin;
  
  // Wake up motor
  digitalWrite(disablePin, LOW);
  // Set the direction
  digitalWrite(dirPin, dir);
  delayMillis(1);

  // Pulse motor stepNum times
  for (i = 0; i < steps*uFactor ; i++) {
    // Pulse HIGH-LOW
    digitalWrite(stepPin, HIGH); delayMicros(deay);
    digitalWrite(stepPin, LOW);  delayMicros(deay);
  }

  delayMillis(1);

  // Turn off motor(s)  
  digitalWrite(disablePin, HIGH);
}

/**
 * @brief      Step
 *
 * @param[in]  motor  motor number
 * @param[in]  dir    direction
 * @param[in]  steps  number of steps
 */
void stepE(int motor, int dir, int steps) {
  int i = 0; 
  int deay = 600;

  // Define pins to use
  pins p = choosePins(motor);
  int stepPin = p.stepPin;
  int disablePin = p.disablePin;

  // Wake up motor
  digitalWrite(disablePin, LOW);
  digitalWrite(sleepPin, HIGH);

  // Set the direction
  digitalWrite(dirPin, dir);
  delayMillis(1);

  // Pulse motor stepNum times
  for (i = 0; i < steps; i++) {
    // Pulse HIGH-LOW
    digitalWrite(stepPin, HIGH); delayMicros(deay);
    digitalWrite(stepPin, LOW);  delayMicros(deay);
  }
  delayMillis(1);

  // Turn off motor(s)  
  digitalWrite(disablePin, HIGH);
  digitalWrite(sleepPin, LOW);
}


void disableAll(){
        digitalWrite(disablePin0, HIGH);
        digitalWrite(disablePin1, HIGH);
        digitalWrite(disablePin2, HIGH);
        digitalWrite(disablePin3, HIGH);
        digitalWrite(disablePin4, HIGH);
        digitalWrite(disablePin5, HIGH);
}

//////////////////////////////////////////////////////////////
//////////////////////// Setup ///////////////////////////////
//////////////////////////////////////////////////////////////

void setup(){
  // Initialize GPIO
  pioInit();
  
  // Set all pins as outputs
  pinMode(dirPin, OUTPUT);
  pinMode(sleepPin, OUTPUT);

  pinMode(stepPin0, OUTPUT);
  pinMode(disablePin0, OUTPUT);
  pinMode(stepPin1, OUTPUT);
  pinMode(disablePin1, OUTPUT);
  pinMode(stepPin2, OUTPUT);
  pinMode(disablePin2, OUTPUT);
  pinMode(stepPin3, OUTPUT);
  pinMode(disablePin3, OUTPUT);
  pinMode(stepPin4, OUTPUT);
  pinMode(disablePin4, OUTPUT);
  pinMode(stepPin5, OUTPUT);
  pinMode(disablePin5, OUTPUT);

  // Start with motor off
  digitalWrite(disablePin0, HIGH);
  digitalWrite(disablePin1, HIGH);
  digitalWrite(disablePin2, HIGH);
  digitalWrite(disablePin3, HIGH);
  digitalWrite(disablePin4, HIGH);
  digitalWrite(disablePin5, HIGH);

}


#endif
