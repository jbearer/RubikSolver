/**
 *
 */


#include "turnCube.hpp"

using namespace CubeSolver;

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
  // L = clockwise
  // Green = front
  // White = Bottom
  switch(t){
    case F:
      step(FRONT_MOTOR,CW,1); break;
    case R:
      step(RIGHT_MOTOR,CW,1); break;
    case D:
      step(DOWN_MOTOR,CW,1); break;
    case B:
      step(BACK_MOTOR,CW,1); break;
    case L:
      step(LEFT_MOTOR,CW,1); break;
    case U:
      step(UP_MOTOR,CW,1); break;
    case Fi:
      step(FRONT_MOTOR,CCW,1); break;
    case Ri:
      step(RIGHT_MOTOR,CCW,1); break;
    case Di:
      step(DOWN_MOTOR,CCW,1); break;
    case Bi:
      step(BACK_MOTOR,CCW,1); break;
    case Li:
      step(LEFT_MOTOR,CCW,1); break;
    case Ui:
      step(UP_MOTOR,CCW,1); break;
    case F2:
      step(FRONT_MOTOR,CW,2); break;
    case R2:
      step(RIGHT_MOTOR,CW,2); break;
    case D2:
      step(DOWN_MOTOR,CW,2); break;
    case B2:
      step(BACK_MOTOR,CW,2); break;
    case L2:
      step(LEFT_MOTOR,CW,2); break;
    case U2:
      step(UP_MOTOR,CW,2); break;
    // default:
    //   // std::cerr << "ERROR: Invalid Turn enum passed to turn()" << std::endl;
    //   break;
  }
}
