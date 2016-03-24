/**
 * \class MotorDriver
 * \brief Interface for the hardware.
 * \details Exposes a method to turn any of the six attached motors. Abstracts
 *  details such as the pins each motor is connected to.
 */

#pragma once
#include "Arduino.h"

typedef int PinID;

enum MotorID : char {
    LEFT_MOTOR,
    RIGHT_MOTOR,
    UP_MOTOR,
    DOWN_MOTOR,
    FRONT_MOTOR,
    BACK_MOTOR
};

enum MotorDirection : int {
    CLOCKWISE = HIGH,
    COUNTERCLOCKWISE = LOW
};

class MotorDriver {
public:

    /**
     * \brief Create an instance of a MotorDriver and configure it for the
     *  given hardware setup.
     * \param stepPinX The pin for motor X indicating how many turns to make.
     * \param dirPinX The pin for motor X indicating which direction to rotate.
     */
    MotorDriver(const PinID stepPinL, const PinID dirPinL,
                const PinID stepPinR, const PinID dirPinR,
                const PinID stepPinU, const PinID dirPinU,
                const PinID stepPinD, const PinID dirPinD,
                const PinID stepPinF, const PinID dirPinF,
                const PinID stepPinB, const PinID dirPinB);

    /**
     * \brief Turn a motor.
     * \param motor ID of the motor to turn.
     * \param turns The number of 90 degree turns to make.
     * \param dir The direction of the turns.
     */
    void step(
        const MotorID motor, const int turns, const MotorDirection dir);

private:

	void quarterTurn(const MotorID motor, const MotorDirection dir);
	void halfTurn(const MotorID motor, const MotorDirection dir);

    /// Hardware connections.
    PinID stepPinL_;
    PinID dirPinL_;

    PinID stepPinR_;
    PinID dirPinR_;

    PinID stepPinU_;
    PinID dirPinU_;

    PinID stepPinD_;
    PinID dirPinD_;

    PinID stepPinF_;
    PinID dirPinF_;

    PinID stepPinB_;
    PinID dirPinB_;
};