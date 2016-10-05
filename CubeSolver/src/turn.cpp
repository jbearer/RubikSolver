/**
* \file turn.cpp
*/

#include <iostream>
#include <vector>
#include <boost/assign/list_of.hpp>
#include <map>

#include "turn.h"

using CommProtocol::MoveInstruction;

using namespace boost::assign;
using namespace CommProtocol;
using namespace CubeSolver;

std::map<MoveInstruction, std::string> turnToString = map_list_of
	(FRONT, "F")
	(RIGHT, "R")
	(BACK, "B")
	(LEFT, "L")
	(UP, "U")
	(DOWN, "D")
	(FRONT_2, "F2")
	(RIGHT_2, "R2")
	(BACK_2, "B2")
	(LEFT_2, "L2")
	(UP_2, "U2")
	(DOWN_2, "D2")
	(FRONT_INVERTED, "F'")
	(RIGHT_INVERTED, "R'")
	(BACK_INVERTED, "B'")
	(LEFT_INVERTED, "L'")
	(UP_INVERTED, "U'")
	(DOWN_INVERTED, "D'");

std::map<MoveInstruction, MoveInstruction> moveToOppMove = map_list_of
	(FRONT, FRONT_INVERTED)
	(RIGHT, RIGHT_INVERTED)
	(BACK, BACK_INVERTED)
	(LEFT, LEFT_INVERTED)
	(UP, UP_INVERTED)
	(DOWN, DOWN_INVERTED)
	(FRONT_INVERTED, FRONT)
	(RIGHT_INVERTED, RIGHT)
	(BACK_INVERTED, BACK)
	(LEFT_INVERTED, LEFT)
	(UP_INVERTED, UP)
	(DOWN_INVERTED, DOWN);

std::ostream& CubeSolver::operator<<(std::ostream& out, MoveInstruction mi)
{
	out << turnToString[mi] << " ";
	return out;
}

MoveInstruction CubeSolver::oppMove(MoveInstruction mi)
{
	if (moveToOppMove.count(mi) > 0)
		return moveToOppMove[mi];
	else
		// MoveInstruction is its own inverse (e.g. FRONT_2)
		return mi;
}


