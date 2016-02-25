/**
 * \file motor-driver.cpp
 * \brief Implementation of MotorDriver class.
 * \details See motor-driver.h.
 * \note This file uses Arduino methods and thus should only be compiled in
 *  Arduino editor.
 */

#include "motor-driver.h"

MotorDriver::MotorDriver(PinID stepPinL, PinID dirPinL,
                PinID stepPinR, PinID dirPinR,
                PinID stepPinU, PinID dirPinU,
                PinID stepPinD, PinID dirPinD,
                PinID stepPinF, PinID dirPinF,
                PinID stepPinB, PinID dirPinB)
: stepPinL_{stepPinL}, driPinL_{dirPinL},
  stepPinR_{stepPinR}, driPinR_{dirPinR},
  stepPinU_{stepPinU}, driPinU_{dirPinU},
  stepPinD_{stepPinD}, driPinD_{dirPinD},
  stepPinF_{stepPinF}, driPinF_{dirPinF},
  stepPinB_{stepPinB}, driPinB_{dirPinB} {

    // Nothing else to do.
}

void MotorDriver::step(MotorID motor, int turns, MotorDirection dir) {
    int stepPin = 0;
    int dirPin = 0;

    // Determine motor, and set step & dir pins to that motor.
    switch(motor) {
        case RIGHT:
            stepPin = stepPinR_;
            dirPin = stepPinR_;
            break;
        case LEFT:
            stepPin = stepPinL_;
            dirPin = stepPinL_;
            break;
        case UP:
            stepPin = stepPinU_;
            dirPin = stepPinU_;
            break;
        case DOWN:
            stepPin = stepPinD_;
            dirPin = stepPinD_;
            break;
        case FRONT:
            stepPin = stepPinF_;
            dirPin = stepPinF_;
            break;
        case BACK:
            stepPin = stepPinB_;
            dirPin = stepPinB_;
            break;
        case Serial.println("ERROR: Unexpected motor ID.");
    }

    // Step the motor.
    digitalWrite(dirPin, dir);
    for (int counter = 0; counter < turns*100; counter++){
        digitalWrite(stepPin,HIGH); 
        delayMicroseconds(DELAY); 
        digitalWrite(stepPin,LOW); 
        delayMicroseconds(DELAY);
    }
}
