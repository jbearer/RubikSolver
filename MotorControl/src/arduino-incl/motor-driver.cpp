/**
 * \file motor-driver.cpp
 * \brief Implementation of MotorDriver class.
 * \details See motor-driver.h.
 * \note This file uses Arduino methods and thus should only be compiled in
 *  Arduino editor.
 */

#include "Arduino.h"
#include "HardwareSerial.h"
#include "motor-driver.h"
#include "comm-protocol.h"

MotorDriver::MotorDriver(const PinID stepPinL, const PinID dirPinL,
                const PinID stepPinR, const PinID dirPinR,
                const PinID stepPinU, const PinID dirPinU,
                const PinID stepPinD, const PinID dirPinD,
                const PinID stepPinF, const PinID dirPinF,
                const PinID stepPinB, const PinID dirPinB)
: stepPinL_{stepPinL}, dirPinL_{dirPinL},
  stepPinR_{stepPinR}, dirPinR_{dirPinR},
  stepPinU_{stepPinU}, dirPinU_{dirPinU},
  stepPinD_{stepPinD}, dirPinD_{dirPinD},
  stepPinF_{stepPinF}, dirPinF_{dirPinF},
  stepPinB_{stepPinB}, dirPinB_{dirPinB} {

    // Nothing else to do.
}

void MotorDriver::step(
	const MotorID motor, const int turns, const MotorDirection dir) {
	switch (turns) {
	case 1:
		quarterTurn(motor, dir);
	case 2:
		halfTurn(motor, dir);
	}
}

void MotorDriver::quarterTurn(
    const MotorID motor, const MotorDirection dir) {

	const int maxDeay = 425;  // Starting delay time
	const int stage1 = 350;   // from -10 to -5
	const int minDeay = 140;  // Fastest (capped) speed

    int stepPin = 0;
    int dirPin = 0;

    // Determine motor, and set step & dir pins to that motor.
    switch(motor) {
        case RIGHT_MOTOR:
            stepPin = stepPinR_;
            dirPin = dirPinR_;
            break;
        case LEFT_MOTOR:
            stepPin = stepPinL_;
            dirPin = dirPinL_;
            break;
        case UP_MOTOR:
            stepPin = stepPinU_;
            dirPin = dirPinU_;
            break;
        case DOWN_MOTOR:
            stepPin = stepPinD_;
            dirPin = dirPinD_;
            break;
        case FRONT_MOTOR:
            stepPin = stepPinF_;
            dirPin = dirPinF_;
            break;
        case BACK_MOTOR:
            stepPin = stepPinB_;
            dirPin = dirPinB_;
            break;
        default:
            Serial.println("ERROR: Unexpected motor ID.");
    }
	  
	digitalWrite(dirPin, dir);  // Define Direction
	int deay = maxDeay;         // Always reset deay when function called (important)
	  
	////// Accelerating
	for (int counter = 0; counter < 40; counter++){
	    digitalWrite(stepPin,HIGH); delayMicroseconds(deay); // Pulse HIGH then LOW
	    digitalWrite(stepPin,LOW); delayMicroseconds(deay);  // This is where 'speed' is determined 
	    if (deay > stage1) {  // Update deay when in phase 1
	    	deay = deay - 10;
	    }  
	    else {                // Update deay when in "phase 2" (slower acceleration)
	      	deay = deay - 5;
		}
	    // No need minDeay b/c never gets to max speed (minDeay)
	}
	  
	////// Deceleration
	for (int counter = 0; counter < 10; counter++){
		digitalWrite(stepPin,HIGH); delayMicroseconds(deay); 
	    digitalWrite(stepPin,LOW); delayMicroseconds(deay);
	    deay = deay + 20; // Just decelerated linearly (w.r.t deay)
	}
}

void MotorDriver::halfTurn(
	const MotorID motor, const MotorDirection dir) {

	const int maxDeay = 425;  // slowest speed
  	const int stage1 = 350;   // from -10 to -5
  	const int stage2 = 150;   // from -5 to -1
  	const int minDeay = 140;  // fastest speed

    int stepPin = 0;
    int dirPin = 0;

    // Determine motor, and set step & dir pins to that motor.
    switch(motor) {
        case RIGHT_MOTOR:
            stepPin = stepPinR_;
            dirPin = dirPinR_;
            break;
        case LEFT_MOTOR:
            stepPin = stepPinL_;
            dirPin = dirPinL_;
            break;
        case UP_MOTOR:
            stepPin = stepPinU_;
            dirPin = dirPinU_;
            break;
        case DOWN_MOTOR:
            stepPin = stepPinD_;
            dirPin = dirPinD_;
            break;
        case FRONT_MOTOR:
            stepPin = stepPinF_;
            dirPin = dirPinF_;
            break;
        case BACK_MOTOR:
            stepPin = stepPinB_;
            dirPin = dirPinB_;
            break;
        default:
            Serial.println("ERROR: Unexpected motor ID.");
    }
	  
	digitalWrite(dirPin, dir);  // Define Direction
	int deay = maxDeay;         // Always reset deay when function called (important)

	////// Accelerating
	for (int counter = 0; counter < 85; counter++){ // Counters experimentally determined
	    digitalWrite(stepPin,HIGH); delayMicroseconds(deay); 
	    digitalWrite(stepPin,LOW); delayMicroseconds(deay);
	    if (deay > stage1){ // Update deay based on speed (stage 1)
	      deay = deay - 10;
	    }
	    else if (deay > stage2){ // Stage 2 decreases acceleation
	      deay = deay - 5;
	    }
	    else if (deay > minDeay){
	      deay = deay -1;
	    }
	    // Afterwards, speed plateaus at minDeay, max speed
	}
	  
	////// Deceleration
	for (int counter = 0; counter < 15; counter++){
	    digitalWrite(stepPin,HIGH); delayMicroseconds(deay); 
	    digitalWrite(stepPin,LOW); delayMicroseconds(deay);
	    deay = deay + 20;   
	}
}
