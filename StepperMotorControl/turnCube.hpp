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

#define CW 0
#define CCW 1


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
