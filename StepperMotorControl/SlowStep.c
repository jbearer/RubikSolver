/**
 * Implements a slow step, that is reliable
 * Intended use with Neural Net training
 * 
 * Austin Chun
 * Spring 2017
 */

#include "StepperMotorController.h"
#include <time.h>
#include <stdlib.h>


/**
 * @brief      stepSlow is slow and reliable
 *
 * @param[in]  motor  The motor (0-5)
 * @param[in]  dir    The dir (0 or 1)
 * 
 * @note    Run in 1/32 microstepping
 */
void stepSlow(int motor, int dir)
{
    int i = 0; // Cause C is stupid

    // Define pins to use
    pins p = choosePins(motor);
    int stepPin = p.stepPin;
    int disablePin = p.disablePin;

    // Wake up motor
    digitalWrite(disablePin, LOW);
    // Set the direction
    digitalWrite(dirPin, dir);
    delayMillis(2);

    int deay = 150;
    int stepNum = 50;
    int accelSteps = 35;

    // Pulse motor stepNum times
    for (i = 0; i < stepNum*32; i++) {
    // Pulse HIGH-LOW
    digitalWrite(stepPin, HIGH); delayMicros(deay);
    digitalWrite(stepPin, LOW);  delayMicros(deay);

    if(deay > 75 && i < accelSteps*32){
        deay = deay - 1;
    }
    else if(deay > 150){
        deay = deay + 2;
    }

  }

  delayMillis(1);

  // Turn off motor(s)  
  digitalWrite(disablePin, HIGH);

}


void stepNorm (int motor, int dir)
{
    int i = 0; // Cause C is stupid

    // Define pins to use
    pins p = choosePins(motor);
    int stepPin = p.stepPin;
    int disablePin = p.disablePin;

    // Wake up motor
    digitalWrite(disablePin, LOW);
    // Set the direction
    digitalWrite(dirPin, dir);
    delayMillis(2);

    int deay = 1000;
    int stepNum = 50;
    int accelSteps = 35;

    // Pulse motor stepNum times
    for (i = 0; i < stepNum-1; i++) {
    // Pulse HIGH-LOW
    digitalWrite(stepPin, HIGH); delayMicros(deay);
    digitalWrite(stepPin, LOW);  delayMicros(deay);
/*
    if(deay > 1400 && i < accelSteps){
        deay = deay - 10;
    }
    else if(deay > 1600){
        deay = deay + 20;
    }
*/
  }

  delayMillis(1);

  // Turn off motor(s)  
  digitalWrite(disablePin, HIGH);

}

int main(void)
{
    // Setup GPIO pins
    setup();

    // Turn on Motors 
    srand(time(NULL));   // should only be called once
    int r = 0;

    int i = 0;
    //int motor = 0;
    for(i = 0; i < 10; ++i){
        r = rand()%5;
        digitalWrite(sleepPin, HIGH);
        delayMillis(1);
        stepSlow(1, HIGH);
        digitalWrite(sleepPin, LOW);
        disableAll();
        delayMillis(1000);
    }

    

    return 0;
}
