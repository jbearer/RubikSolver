/**
 * \file motor-control.cpp
 * \brief Implementation of definitions from motor-control.h.
 * \details See motor-control.h.
 */

#include "motor-control.h"
#include "motor-driver.h"
#include <stdexcept>

using MotorControl::MoveInstruction;
using MotorControl::FaceTurn;

void MotorControl::TURN_LEFT(MotorDriver driver) {
    driver.step(LEFT_MOTOR, 1, CLOCKWISE);
}

void MotorControl::TURN_RIGHT(MotorDriver driver) {
    driver.step(RIGHT_MOTOR, 1, CLOCKWISE);
}

void MotorControl::TURN_UP(MotorDriver driver) {
    driver.step(UP_MOTOR, 1, CLOCKWISE);
}

void MotorControl::TURN_DOWN(MotorDriver driver) {
    driver.step(DOWN_MOTOR, 1, CLOCKWISE);
}

void MotorControl::TURN_FRONT(MotorDriver driver) {
    driver.step(FRONT_MOTOR, 1, CLOCKWISE);
}

void MotorControl::TURN_BACK(MotorDriver driver) {
    driver.step(BACK_MOTOR, 1, CLOCKWISE);
}

void MotorControl::TURN_LEFT_2(MotorDriver driver) {
    driver.step(LEFT_MOTOR, 2, CLOCKWISE);
}

void MotorControl::TURN_RIGHT_2(MotorDriver driver) {
    driver.step(RIGHT_MOTOR, 2, CLOCKWISE);
}

void MotorControl::TURN_UP_2(MotorDriver driver) {
    driver.step(UP_MOTOR, 2, CLOCKWISE);
}

void MotorControl::TURN_DOWN_2(MotorDriver driver) {
    driver.step(DOWN_MOTOR, 2, CLOCKWISE);
}

void MotorControl::TURN_FRONT_2(MotorDriver driver) {
    driver.step(FRONT_MOTOR, 2, CLOCKWISE);
}

void MotorControl::TURN_BACK_2(MotorDriver driver) {
    driver.step(BACK_MOTOR, 2, CLOCKWISE);
}

void MotorControl::TURN_LEFT_INVERTED(MotorDriver driver) {
    driver.step(LEFT_MOTOR, 1, COUNTERCLOCKWISE);
}

void MotorControl::TURN_RIGHT_INVERTED(MotorDriver driver) {
    driver.step(RIGHT_MOTOR, 1, COUNTERCLOCKWISE);
}

void MotorControl::TURN_UP_INVERTED(MotorDriver driver) {
    driver.step(UP_MOTOR, 1, COUNTERCLOCKWISE);
}

void MotorControl::TURN_DOWN_INVERTED(MotorDriver driver) {
    driver.step(DOWN_MOTOR, 1, COUNTERCLOCKWISE);
}

void MotorControl::TURN_FRONT_INVERTED(MotorDriver driver) {
    driver.step(FRONT_MOTOR, 1, COUNTERCLOCKWISE);
}

void MotorControl::TURN_BACK_INVERTED(MotorDriver driver) {
    driver.step(BACK_MOTOR, 1, COUNTERCLOCKWISE);
}

FaceTurn MotorControl::getAction(MoveInstruction code) {
    switch(code) {
        case LEFT:
            return TURN_LEFT;
        case RIGHT:
            return TURN_RIGHT;
        case UP:
            return TURN_UP;
        case DOWN:
            return TURN_DOWN;
        case FRONT:
            return TURN_FRONT;
        case BACK:
            return TURN_BACK;

        case LEFT_2:
            return TURN_LEFT_2;
        case RIGHT_2:
            return TURN_RIGHT_2;
        case UP_2:
            return TURN_UP_2;
        case DOWN_2:
            return TURN_DOWN_2;
        case FRONT_2:
            return TURN_FRONT_2;
        case BACK_2:
            return TURN_BACK_2;

        case LEFT_INVERTED:
            return TURN_LEFT_INVERTED;
        case RIGHT_INVERTED:
            return TURN_RIGHT_INVERTED;
        case UP_INVERTED:
            return TURN_UP_INVERTED;
        case DOWN_INVERTED:
            return TURN_DOWN_INVERTED;
        case FRONT_INVERTED:
            return TURN_FRONT_INVERTED;
        case BACK_INVERTED:
            return TURN_BACK_INVERTED;

        default:
            throw std::runtime_error("Unknown move instruction.");
    }
}