#pragma once

/**
* \file turn.h
*/

#include <string>
#include <vector>
#include <map>
#include <iostream>

#include "../../MotorControl/src/arduino-incl/comm-protocol.h"

namespace CubeSolver {

const int NUM_TURNS_STEP1 = 12;
const int NUM_TURNS_STEP2 = 8;

CommProtocol::MoveInstruction oppMove(CommProtocol::MoveInstruction mi);
std::ostream& operator<<(std::ostream& out, CommProtocol::MoveInstruction mi);



} // end namespace CubeSolver
