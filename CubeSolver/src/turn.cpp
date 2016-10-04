/**
* \file turn.cpp
*/

#include <iostream>
#include <vector>

#include "turn.h"

using CommProtocol::MoveInstruction;

using namespace CubeSolver;

Turn::Turn(MoveInstruction c) : repr{ c }
{
	repr = c;
	switch (c) {

	case MoveInstruction::FRONT:
		toString = "F"; oppTurn = MoveInstruction::FRONT_INVERTED;
		break;
	case MoveInstruction::RIGHT:
		toString = "R"; oppTurn = MoveInstruction::RIGHT_INVERTED;
		break;
	case MoveInstruction::BACK:
		toString = "B"; oppTurn = MoveInstruction::BACK_INVERTED;
		break;
	case MoveInstruction::LEFT:
		toString = "L"; oppTurn = MoveInstruction::LEFT_INVERTED;
		break;
	case MoveInstruction::UP:
		toString = "U"; oppTurn = MoveInstruction::UP_INVERTED;
		break;
	case MoveInstruction::DOWN:
		toString = "D"; oppTurn = MoveInstruction::DOWN_INVERTED;
		break;

	case MoveInstruction::FRONT_2:
		toString = "F2"; oppTurn = MoveInstruction::FRONT_2;
		break;
	case MoveInstruction::RIGHT_2:
		toString = "R2"; oppTurn = MoveInstruction::RIGHT_2;
		break;
	case MoveInstruction::BACK_2:
		toString = "B2"; oppTurn = MoveInstruction::BACK_2;
		break;
	case MoveInstruction::LEFT_2:
		toString = "L2"; oppTurn = MoveInstruction::LEFT_2;
		break;
	case MoveInstruction::UP_2:
		toString = "U2"; oppTurn = MoveInstruction::UP_2;
		break;
	case MoveInstruction::DOWN_2:
		toString = "D2"; oppTurn = MoveInstruction::DOWN_2;
		break;

	case MoveInstruction::FRONT_INVERTED:
		toString = "F'"; oppTurn = MoveInstruction::FRONT;
		break;
	case MoveInstruction::RIGHT_INVERTED:
		toString = "R'"; oppTurn = MoveInstruction::RIGHT;
		break;
	case MoveInstruction::BACK_INVERTED:
		toString = "B'"; oppTurn = MoveInstruction::BACK;
		break;
	case MoveInstruction::LEFT_INVERTED:
		toString = "L'"; oppTurn = MoveInstruction::LEFT;
		break;
	case MoveInstruction::UP_INVERTED:
		toString = "U'"; oppTurn = MoveInstruction::UP;
		break;
	case MoveInstruction::DOWN_INVERTED:
		toString = "D'"; oppTurn = MoveInstruction::DOWN;
		break;

	default: std::cout << "error: not a valid char" << std::endl;
	}
}


std::vector<Turn> Turn::movesToTurns(std::vector<MoveInstruction> moves)
{
	std::vector<Turn> turns;
	for (MoveInstruction m : moves) {
		turns.push_back(Turn(m));
	}
	return turns;
}


