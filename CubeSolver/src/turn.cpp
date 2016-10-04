/**
* \file turn.cpp
*/

#include <iostream>
#include <vector>

#include "turn.h"

using namespace CommProtocol;

using namespace CubeSolver;

Turn::Turn(MoveInstruction c) : repr{ c }
{
	repr = c;
	switch (c) {

	case FRONT:
		toString = "F"; oppTurn = FRONT_INVERTED;
		break;
	case RIGHT:
		toString = "R"; oppTurn = RIGHT_INVERTED;
		break;
	case BACK:
		toString = "B"; oppTurn = BACK_INVERTED;
		break;
	case LEFT:
		toString = "L"; oppTurn = LEFT_INVERTED;
		break;
	case UP:
		toString = "U"; oppTurn = UP_INVERTED;
		break;
	case DOWN:
		toString = "D"; oppTurn = DOWN_INVERTED;
		break;

	case FRONT_2:
		toString = "F2"; oppTurn = FRONT_2;
		break;
	case RIGHT_2:
		toString = "R2"; oppTurn = RIGHT_2;
		break;
	case BACK_2:
		toString = "B2"; oppTurn = BACK_2;
		break;
	case LEFT_2:
		toString = "L2"; oppTurn = LEFT_2;
		break;
	case UP_2:
		toString = "U2"; oppTurn = UP_2;
		break;
	case DOWN_2:
		toString = "D2"; oppTurn = DOWN_2;
		break;

	case FRONT_INVERTED:
		toString = "F'"; oppTurn = FRONT;
		break;
	case RIGHT_INVERTED:
		toString = "R'"; oppTurn = RIGHT;
		break;
	case BACK_INVERTED:
		toString = "B'"; oppTurn = BACK;
		break;
	case LEFT_INVERTED:
		toString = "L'"; oppTurn = LEFT;
		break;
	case UP_INVERTED:
		toString = "U'"; oppTurn = UP;
		break;
	case DOWN_INVERTED:
		toString = "D'"; oppTurn = DOWN;
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


