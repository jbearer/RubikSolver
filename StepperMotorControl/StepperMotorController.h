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
pins choosePins(int motor);

/**
 * @brief      step function (full arguments)
 *
 * @param[in]  motor    motor number
 * @param[in]  dir      direction
 * @param[in]  steps    number of steps
 * @param[in]  deay     time delay (us)
 * @param[in]  uFactor  microstep Factor (1 = normal, 32 = 1/32 stepping)
 */
void step(int motor, int dir, int steps, int deay, int uFactor);

/**
 * @brief      Step
 *
 * @param[in]  motor  motor number
 * @param[in]  dir    direction
 * @param[in]  steps  number of steps
 */
void stepE(int motor, int dir, int steps);

void disableAll();

//////////////////////////////////////////////////////////////
//////////////////////// Setup ///////////////////////////////
//////////////////////////////////////////////////////////////

void setup();


#endif
