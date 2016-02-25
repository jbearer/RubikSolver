/**
 * \file motor-control.h
 * \brief Defines constants and functions used by Arduino sketches that operate
 *  the motor. Defines a protocol for serial communication between such a sketch
 *  and a PC program.
 */

#pragma once

#include "motor-driver.h"

namespace MotorControl {

    typedef char SerialMessage;

    /**
     * \enum MoveInstruction
     * \brief Constants representing a single cube action.
     */
    enum MoveInstruction : SerialMessage {
        LEFT,
        RIGHT,
        UP,
        DOWN,
        FRONT,
        BACK,

        LEFT_2,
        RIGHT_2,
        UP_2,
        DOWN_2,
        FRONT_2,
        BACK_2,

        LEFT_INVERTED,
        RIGHT_INVERTED,
        UP_INVERTED,
        DOWN_INVERTED,
        FRONT_INVERTED,
        BACK_INVERTED
    };

    typedef void(*FaceTurn)(MotorDriver);

    /// Functions that activate the motors.
    void TURN_LEFT(MotorDriver);
    void TURN_RIGHT(MotorDriver);
    void TURN_UP(MotorDriver);
    void TURN_DOWN(MotorDriver);
    void TURN_FRONT(MotorDriver);
    void TURN_BACK(MotorDriver);

    void TURN_LEFT_2(MotorDriver);
    void TURN_RIGHT_2(MotorDriver);
    void TURN_UP_2(MotorDriver);
    void TURN_DOWN_2(MotorDriver);
    void TURN_FRONT_2(MotorDriver);
    void TURN_BACK_2(MotorDriver);

    void TURN_LEFT_INVERTED(MotorDriver);
    void TURN_RIGHT_INVERTED(MotorDriver);
    void TURN_UP_INVERTED(MotorDriver);
    void TURN_DOWN_INVERTED(MotorDriver);
    void TURN_FRONT_INVERTED(MotorDriver);
    void TURN_BACK_INVERTED(MotorDriver);

    /**
     * \brief Get the function associated with a MoveInstruction code.
     */
    FaceTurn getAction(MoveInstruction);

}
