/**
* \file turn.cpp
*/

#include <iostream>
#include <vector>
#include <boost/assign/list_of.hpp>
#include <map>

#include "turn.h"

using namespace boost::assign;
using namespace CubeSolver;

std::map<Turn, Turn> turnToOppTurn = map_list_of
	(F, Fi)
	(R, Ri)
	(B, Bi)
	(L, Li)
	(U, Ui)
	(D, Di)
	(Fi, F)
	(Ri, R)
	(Bi, B)
	(Li, L)
	(Ui, U)
	(Di, D);

Turn CubeSolver::oppTurn(Turn turn)
{
	if (turnToOppTurn.count(turn) > 0)
		return turnToOppTurn[turn];
	else
		// Turn is its own inverse (e.g. F2)
		return turn;
}

std::map<Turn, std::string> turnToString = map_list_of
	(F, "F")
	(R, "R")
	(B, "B")
	(L, "L")
	(U, "U")
	(D, "D")
	(F2, "F2")
	(R2, "R2")
	(B2, "B2")
	(L2, "L2")
	(U2, "U2")
	(D2, "D2")
	(Fi, "F'")
	(Ri, "R'")
	(Bi, "B'")
	(Li, "L'")
	(Ui, "U'")
	(Di, "D'");

std::ostream& CubeSolver::operator<<(std::ostream& out, Turn turn)
{
	out << turnToString[turn] << " ";
	return out;
}




