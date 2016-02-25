/**
 * \file motor-control.cpp
 * \brief Implementation of definitions from motor-control.h.
 * \details See motor-control.h.
 */

#include "motor-control.h"
#include <stdexcept>

using MotorControl::SerialMessage;
using MotorControl::FaceTurn;

void MotorControl::LEFT(MotorDriver driver) {
	driver.step(LEFT, 1, CLOCKWISE);
}

void MotorControl::RIGHT(MotorDriver driver) {
	driver.step(RIGHT, 1, CLOCKWISE);
}

void MotorControl::UP(MotorDriver driver) {
	driver.step(UP, 1, CLOCKWISE);
}

void MotorControl::DOWN(MotorDriver driver) {
	driver.step(DOWN, 1, CLOCKWISE);
}

void MotorControl::FRONT(MotorDriver driver) {
	driver.step(FRONT, 1, CLOCKWISE);
}

void MotorControl::BACK(MotorDriver driver) {
	driver.step(BACK, 1, CLOCKWISE);
}

void MotorControl::LEFT_2(MotorDriver driver) {
	driver.step(LEFT, 2, CLOCKWISE);
}

void MotorControl::RIGHT_2(MotorDriver driver) {
	driver.step(RIGHT, 2, CLOCKWISE);
}

void MotorControl::UP_2(MotorDriver driver) {
	driver.step(UP, 2, CLOCKWISE);
}

void MotorControl::DOWN_2(MotorDriver driver) {
	driver.step(DOWN, 2, CLOCKWISE);
}

void MotorControl::FRONT_2(MotorDriver driver) {
	driver.step(FRONT, 2, CLOCKWISE);
}

void MotorControl::BACK_2(MotorDriver driver) {
	driver.step(BACK, 2, CLOCKWISE);
}

void MotorControl::LEFT_INVERTED(MotorDriver driver) {
	driver.step(LEFT, 1, COUNTERCLOCKWISE);
}

void MotorControl::RIGHT_INVERTED(MotorDriver driver) {
	driver.step(RIGHT, 1, COUNTERCLOCKWISE);
}

void MotorControl::UP_INVERTED(MotorDriver driver) {
	driver.step(UP, 1, COUNTERCLOCKWISE);
}

void MotorControl::DOWN_INVERTED(MotorDriver driver) {
	driver.step(DOWN, 1, COUNTERCLOCKWISE);
}

void MotorControl::FRONT_INVERTED(MotorDriver driver) {
	driver.step(FRONT, 1, COUNTERCLOCKWISE);
}

void MotorControl::BACK_INVERTED(MotorDriver driver) {
	driver.step(BACK, 1, COUNTERCLOCKWISE);
}

FaceTurn MotorControl::getAction(MoveInstruction code) {
	switch(code) {
		case LEFT:
			return LEFT;
		case RIGHT:
			return RIGHT;
		case UP:
			return UP;
		case DOWN:
			return DOWN;
		case FRONT:
			return FRONT;
		case BACK:
			return BACK;

		case LEFT_2:
			return LEFT_2;
		case RIGHT_2:
			return RIGHT_2;
		case UP_2:
			return UP_2;
		case DOWN_2:
			return DOWN_2;
		case FRONT_2:
			return FRONT_2;
		case BACK_2:
			return BACK_2;

		case LEFT_INVERTED:
			return LEFT_INVERTED;
		case RIGHT_INVERTED:
			return RIGHT_INVERTED;
		case UP_INVERTED:
			return UP_INVERTED;
		case DOWN_INVERTED:
			return DOWN_INVERTED;
		case FRONT_INVERTED:
			return FRONT_INVERTED;
		case BACK_INVERTED:
			return BACK_INVERTED;

		default:
			throw std::runtime_error("Unknown move instruction.");
	}
}
