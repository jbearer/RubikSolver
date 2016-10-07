/**
* \file turn.cpp
*/

#include <iostream>
#include <vector>
#include <boost/assign/list_of.hpp>
#include <map>

#include "turn.h"

using namespace boost::assign;
using namespace CommProtocol;
using namespace CubeSolver;

std::map<Turn, Turn> turnToOppTurn = map_list_of
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

Turn CubeSolver::oppTurn(Turn turn)
{
	if (turnToOppTurn.count(turn) > 0)
		return turnToOppTurn[turn];
	else
		// Turn is its own inverse (e.g. FRONT_2)
		return turn;
}

std::map<Turn, std::string> turnToString = map_list_of
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

std::ostream& CubeSolver::operator<<(std::ostream& out, Turn turn)
{
	out << turnToString[turn] << " ";
	return out;
}




