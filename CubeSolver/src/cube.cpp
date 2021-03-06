/**
* \file cube.cpp
*/

#include <iostream>
#include "cube.h"

using std::cout;
using std::endl;

using namespace CubeSolver;

Turn CubeSolver::TURNS_STEP1[NUM_TURNS_STEP1] = {
	F, R, B, L, U, D, Fi, Ri, Bi, Li, Ui, Di };

Turn CubeSolver::TURNS_STEP2[NUM_TURNS_STEP2] = {
	F2, R, B2, L, U2, D2, Ri, Li };

// Vectors containing enums that correspond to faces
const Cube::Edge_t Cube::FRONT_EDGES[EDGES_PER_FACE] = { YG, OG, WG, RG };
const Cube::Edge_t Cube::RIGHT_EDGES[EDGES_PER_FACE] = { YO, OB, WO, OG };
const Cube::Edge_t Cube::BACK_EDGES[EDGES_PER_FACE] = { YB, RB, WB, OB };
const Cube::Edge_t Cube::LEFT_EDGES[EDGES_PER_FACE] = { YR, RG, WR, RB };
const Cube::Edge_t Cube::UP_EDGES[EDGES_PER_FACE] = { YB, YO, YG, YR };
const Cube::Edge_t Cube::DOWN_EDGES[EDGES_PER_FACE] = { WG, WO, WB, WR };

const Cube::Corner_t Cube::FRONT_CORNERS[CORNERS_PER_FACE] = { YRG, YOG, WOG, WRG };
const Cube::Corner_t Cube::RIGHT_CORNERS[CORNERS_PER_FACE] = { YOG, YOB, WOB, WOG };
const Cube::Corner_t Cube::BACK_CORNERS[CORNERS_PER_FACE] = { YOB, YRB, WRB, WOB };
const Cube::Corner_t Cube::LEFT_CORNERS[CORNERS_PER_FACE] = { YRB, YRG, WRG, WRB };
const Cube::Corner_t Cube::UP_CORNERS[CORNERS_PER_FACE] = { YRB, YOB, YOG, YRG };
const Cube::Corner_t Cube::DOWN_CORNERS[CORNERS_PER_FACE] = { WRG, WOG, WOB, WRB };


Cube::Cube(Edge_t edgeColors[NUM_EDGES], int edgeOrients[NUM_EDGES],
	Corner_t cornerColors[NUM_CORNERS], int cornerOrients[NUM_CORNERS])
{
	for (int i = 0; i < NUM_EDGES; ++i) {
		edgeColors_[i] = edgeColors[i];
		edgeOrients_[i] = edgeOrients[i];
	}
	for (int i = 0; i < NUM_CORNERS; ++i) {
		cornerColors_[i] = cornerColors[i];
		cornerOrients_[i] = cornerOrients[i];
	}
}

bool Cube::operator==(const Cube& rhs)
{
	for (int i = 0; i < NUM_EDGES; ++i) {
		if (edgeColors_[i] != rhs.edgeColors_[i] ||
			edgeOrients_[i] != rhs.edgeOrients_[i])
			return false;
	}
	for (int i = 0; i < NUM_CORNERS; ++i)
		if (cornerColors_[i] != rhs.cornerColors_[i] ||
			cornerOrients_[i] != rhs.cornerOrients_[i])
			return false;

	return true;
}

bool Cube::operator!=(const Cube& rhs)
{
	return !(*this == rhs);
}

bool Cube::isSolved()
{
	Cube solvedCube;
	return *this == solvedCube;
}


Cube Cube::turn(Cube cube, Turn inputTurn) {
	switch (inputTurn) {
		case F: cube.turnFrontOrBack(FRONT_EDGES, FRONT_CORNERS);
			break;
		case R: cube.turnRightOrLeft(RIGHT_EDGES, RIGHT_CORNERS);
			break;
		case B: cube.turnFrontOrBack(BACK_EDGES, BACK_CORNERS);
			break;
		case L: cube.turnRightOrLeft(LEFT_EDGES, LEFT_CORNERS);
			break;
		case U: cube.turnUpOrDown(UP_EDGES, UP_CORNERS);
			break;
		case D: cube.turnUpOrDown(DOWN_EDGES, DOWN_CORNERS);
			break;

		case F2: cube.turn2(F);
			break;
		case R2: cube.turn2(R);
			break;
		case B2: cube.turn2(B);
			break;
		case L2: cube.turn2(L);
			break;
		case U2: cube.turn2(U);
			break;
		case D2: cube.turn2(D);
			break;

		case Fi: cube.turnI(F);
			break;
		case Ri: cube.turnI(R);
			break;
		case Bi: cube.turnI(B);
			break;
		case Li: cube.turnI(L);
			break;
		case Ui: cube.turnI(U);
			break;
		case Di: cube.turnI(D);
			break;
		default:
			cube = Cube();

	}
	return cube;
}

void Cube::turn2(Turn inputTurn)
{
	*this = turn(*this, inputTurn);
	*this = turn(*this, inputTurn);
}

void Cube::turnI(Turn inputTurn)
{
	for (size_t i = 0; i < 3; ++i)
		*this = turn(*this, inputTurn);
}

void Cube::turnFrontOrBack(const Edge_t edges[EDGES_PER_FACE],
						   const Corner_t corners[CORNERS_PER_FACE])
{
	turnCubies(edges, corners);
	orientCorners(corners);
}

void Cube::turnRightOrLeft(const Edge_t edges[EDGES_PER_FACE],
						   const Corner_t corners[CORNERS_PER_FACE])
{
	turnCubies(edges, corners);
}

void Cube::turnUpOrDown(const Edge_t edges[EDGES_PER_FACE],
						const Corner_t corners[CORNERS_PER_FACE])
{
	turnCubies(edges, corners);
	orientEdges(edges);
	orientCorners(corners);
}

void Cube::turnCubies(const Edge_t faceEdges[EDGES_PER_FACE],
					  const Corner_t faceCorners[CORNERS_PER_FACE])
{
	forwardCycle(faceEdges, edgeColors_);
	forwardCycle(faceEdges, edgeOrients_);

	forwardCycle(faceCorners, cornerColors_);
	forwardCycle(faceCorners, cornerOrients_);
}

template <typename Index_t, typename Cubie_t>
void Cube::forwardCycle(const Index_t positions[4], Cubie_t* cubies)
{
	Cubie_t temp = cubies[positions[3]];
	for (size_t i = 3; i > 0; --i) {
		cubies[positions[i]] = cubies[positions[i - 1]];
	}
	cubies[positions[0]] = temp;
}

void Cube::orientEdges(const Edge_t pos[EDGES_PER_FACE])
{
	// edge orientations are changed identically (wrt the face) for U,D,U',D'
	for (int i = 0; i < EDGES_PER_FACE; ++i) {
		edgeOrients_[pos[i]] = !edgeOrients_[pos[i]];
	}
}

void Cube::orientCorners(const Corner_t pos[CORNERS_PER_FACE])
{
	// corner orientations also change the same for U,D,U',D',L,R,L',R'
	cornerOrients_[pos[0]] = (cornerOrients_[pos[0]] + 1) % 3;
	cornerOrients_[pos[1]] = (cornerOrients_[pos[1]] + 2) % 3;
	cornerOrients_[pos[2]] = (cornerOrients_[pos[2]] + 1) % 3;
	cornerOrients_[pos[3]] = (cornerOrients_[pos[3]] + 2) % 3;
}

std::ostream& CubeSolver::operator<<(std::ostream& out, const Cube& cube)
{
	out << "//// EDGES ////" << endl;

	for (size_t i = 0; i < Cube::NUM_EDGES; ++i) {
		out << cube.edgeColors_[i] << "   ";
		out << cube.edgeOrients_[i] << endl;

	}
	out << endl;

	out << " /// CORNERS ////" << endl;
	for (size_t i = 0; i < Cube::NUM_CORNERS; ++i) {
		out << cube.cornerColors_[i] << "   ";
		out << cube.cornerOrients_[i] << endl;
	}

	out << endl << endl;

	return out;
}

