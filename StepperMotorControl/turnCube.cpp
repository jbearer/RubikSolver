/**
 * 
 */


#include "turnCube.hpp"

using namespace CubeSolver;
using namespace CommProtocol;

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
  int deay = 400;

  int microStep = 8;

  // Pulse motor stepNum times
  for (i = 0; i < stepNum*microStep; i++) {
    digitalWrite(stepPin, HIGH); delayMicros(deay);
    digitalWrite(stepPin, LOW); delayMicros(deay);
    if(deay > 300 && i < accelSteps*microStep){
      deay = deay - 1;
    }
    else if(deay < 400 && i > accelSteps*microStep){
      deay = deay + 1;
    }

  }

  delayMillis(1);

  // Turn off motor(s)  
  digitalWrite(disablePin, HIGH);
}

void turn(Turn t)
{
  // LEFT = clockwise
  // Green = front
  // White = Bottom
  switch(t){
    case FRONT:
      step(1,CW,1); break;
    case RIGHT:
      step(5,CW,1); break;
    case DOWN:
      step(3,CW,1); break;
    case BACK:
      step(4,CW,1); break;
    case LEFT:
      step(2,CW,1); break;
    case UP:
      step(0,CW,1); break;
    case FRONT_INVERTED:
      step(1,CCW,1); break;
    case RIGHT_INVERTED:
      step(5,CCW,1); break;
    case DOWN_INVERTED:
      step(3,CCW,1); break;
    case BACK_INVERTED:
      step(4,CCW,1); break;
    case LEFT_INVERTED:
      step(2,CCW,1); break;
    case UP_INVERTED:
      step(0,CCW,1); break;
    case FRONT_2:
      step(1,CW,2); break;
    case RIGHT_2:
      step(5,CW,2); break;
    case DOWN_2:
      step(3,CW,2); break;
    case BACK_2:
      step(4,CW,2); break;
    case LEFT_2:
      step(2,CW,2); break;
    case UP_2:
      step(0,CW,2); break;
    // default:
    //   // std::cerr << "ERROR: Invalid Turn enum passed to turn()" << std::endl; 
    //   break;
  }
}