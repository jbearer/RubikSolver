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


struct Turn {
	/// Constructors
	Turn() = delete;
	Turn(CommProtocol::MoveInstruction c);

	/// Data members
	std::string toString;
	CommProtocol::MoveInstruction repr;
	CommProtocol::MoveInstruction oppTurn;

	/// Construct a vector of Turns from a vector of MoveInstructions
	static std::vector<Turn> movesToTurns(std::vector<CommProtocol::MoveInstruction> moves);

	//static std::map<CommProtocol::MoveInstruction, std::string> turnToString;
	static CommProtocol::MoveInstruction oppMove(CommProtocol::MoveInstruction mi);
};

std::ostream& operator<<(std::ostream& out, CommProtocol::MoveInstruction mi);



} // end namespace CubeSolver
