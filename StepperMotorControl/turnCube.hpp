/**
 * Austin Chun
 * 
 * Simple interface to turn the cube SLOWLY
 */


extern "C"{
  #include "StepperMotorController.h"
}

#include <stdio.h>
#include <unistd.h>


/**
 * @brief      Slow stepping function (should always work)
 *
 * @param[in]  motor  The motor
 * @param[in]  dir    The dir
 * @param[in]  turns  The turns
 * 
 * @note ~0.1s per quarter turn
 */
void step(int motor, int dir, int turns) {
  int i = 0; // Cause C is stupid

  // Define pins to use
  pins p = choosePins(motor);
  int stepPin = p.stepPin;
  int disablePin = p.disablePin;
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
  digitalWrite(disablePin, LOW);
  // Set the direction
  digitalWrite(dirPin, dir);
  delayMillis(1);

  // Play with this
  int deay = 100;

  // Pulse motor stepNum times
  for (i = 0; i < stepNum*32 - 4; i++) {
    // Pulse HIGH-LOW
    digitalWrite(stepPin, HIGH); delayMicros(deay);
    digitalWrite(stepPin, LOW);  delayMicros(deay);

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
  digitalWrite(disablePin, HIGH);
}
