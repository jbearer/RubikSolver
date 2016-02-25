/**
 * \file motor-driver.cpp
 * \brief Implementation of MotorDriver class.
 * \details See motor-driver.h.
 * \note This file uses Arduino methods and thus should only be compiled in
 *  Arduino editor.
 */

#include "motor-driver.h"

MotorDriver::MotorDriver(const PinID stepPinL, const PinID dirPinL,
                const PinID stepPinR, const PinID dirPinR,
                const PinID stepPinU, const PinID dirPinU,
                const PinID stepPinD, const PinID dirPinD,
                const PinID stepPinF, const PinID dirPinF,
                const PinID stepPinB, const PinID dirPinB)
: stepPinL_{stepPinL}, driPinL_{dirPinL},
  stepPinR_{stepPinR}, driPinR_{dirPinR},
  stepPinU_{stepPinU}, driPinU_{dirPinU},
  stepPinD_{stepPinD}, driPinD_{dirPinD},
  stepPinF_{stepPinF}, driPinF_{dirPinF},
  stepPinB_{stepPinB}, driPinB_{dirPinB} {

    // Nothing else to do.
}

void MotorDriver::step(
    const MotorID motor, const int turns, const MotorDirection dir) const {
    int stepPin = 0;
    int dirPin = 0;

    // Determine motor, and set step & dir pins to that motor.
    switch(motor) {
        case RIGHT_MOTOR:
            stepPin = stepPinR_;
            dirPin = stepPinR_;
            break;
        case LEFT_MOTOR:
            stepPin = stepPinL_;
            dirPin = stepPinL_;
            break;
        case UP_MOTOR:
            stepPin = stepPinU_;
            dirPin = stepPinU_;
            break;
        case DOWN_MOTOR:
            stepPin = stepPinD_;
            dirPin = stepPinD_;
            break;
        case FRONT_MOTOR:
            stepPin = stepPinF_;
            dirPin = stepPinF_;
            break;
        case BACK_MOTOR:
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
