/**
 * \file motor-control.h
 * \brief Defines constants and functions used by Arduino sketches that operate
 *  the motor. Should be included by an Arduino program. The associated
 *  definitions use Arduino library functions, and so the code should only be
 *  compiled in the Arduino environment.
 */

#pragma once

#include "motor-driver.h"
#include "comm-protocol.h"

namespace MotorControl {

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
    FaceTurn getAction(CommProtocol::MoveInstruction);

    /**
     * \brief Read the next instruction from the serial part, and return the
     *  associated function.
     */
    FaceTurn getNextAction();

}
