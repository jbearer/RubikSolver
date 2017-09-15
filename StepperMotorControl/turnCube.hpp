/**
 * Austin Chun
 *
 * Simple interface to turn the cube SLOWLY
 */

#pragma once

extern "C"{
  #include "StepperMotorController.h"
}

#include <stdio.h>
#include <unistd.h>

#include "turn.h"

#define CW 1
#define CCW 0

#define FRONT_MOTOR = 1;
#define RIGHT_MOTOR = 5;
#define DOWN_MOTOR = 3;
#define BACK_MOTOR = 4;
#define LEFT_MOTOR = 2;
#define UP_MOTOR = 0;

/**
 * @brief      Slow stepping function (should always work)
 *
 * @param[in]  motor  The motor
 * @param[in]  dir    The dir
 * @param[in]  turns  The turns
 *
 * @note ~0.1s per quarter turn
 */
void step(int motor, int dir, int turns);

void turn(CubeSolver::Turn t);
