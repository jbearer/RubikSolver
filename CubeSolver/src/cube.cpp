/**
* \file cube.cpp
*/

#include "cube.h"

#include <utility>
#include <iostream>
#include <boost/math/special_functions/binomial.hpp>

using boost::math::binomial_coefficient;
using boost::math::factorial;

using namespace CommProtocol;
using namespace CubeSolver;

MoveInstruction CubeSolver::OK_TURNS1[NUM_TURNS_STEP1] = {
	FRONT, RIGHT, BACK, LEFT, UP, DOWN, 
	FRONT_INVERTED, RIGHT_INVERTED, BACK_INVERTED,
	LEFT_INVERTED, UP_INVERTED, DOWN_INVERTED };

MoveInstruction CubeSolver::OK_TURNS2[NUM_TURNS_STEP2] = {
	FRONT_2, RIGHT, BACK_2, LEFT, UP_2, DOWN_2, 
	RIGHT_INVERTED, LEFT_INVERTED };


// Vectors containing enums that correspond to faces
const Cube::Edge_t Cube::FRONT_EDGES[4] = { YG, OG, WG, RG };
const Cube::Edge_t Cube::RIGHT_EDGES[4] = { YO, OB, WO, OG };
const Cube::Edge_t Cube::BACK_EDGES[4] = { YB, RB, WB, OB };
const Cube::Edge_t Cube::LEFT_EDGES[4] = { YR, RG, WR, RB };
const Cube::Edge_t Cube::UP_EDGES[4] = { YB, YO, YG, YR };
const Cube::Edge_t Cube::DOWN_EDGES[4] = { WG, WO, WB, WR };

const Cube::Corner_t Cube::FRONT_CORNERS[4] = { YRG, YOG, WOG, WRG };
const Cube::Corner_t Cube::RIGHT_CORNERS[4] = { YOG, YOB, WOB, WOG };
const Cube::Corner_t Cube::BACK_CORNERS[4] = { YOB, YRB, WRB, WOB };
const Cube::Corner_t Cube::LEFT_CORNERS[4] = { YRB, YRG, WRG, WRB };
const Cube::Corner_t Cube::UP_CORNERS[4] = { YRB, YOB, YOG, YRG };
const Cube::Corner_t Cube::DOWN_CORNERS[4] = { WRG, WOG, WOB, WRB };

// Corresponding edges in the slices
const Cube::Edge_t Cube::LR_SLICE[4] = { YB, YG, WB, WG };

// Cube constructor
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

void Cube::print() const
{
	std::cout << "//// EDGES ////" << std::endl;

	for (size_t i = 0; i < NUM_EDGES; ++i) {
		std::cout << edgeColors_[i] << "   ";
		std::cout << edgeOrients_[i] << std::endl;

	}
	std::cout << std::endl;

	std::cout << " /// CORNERS ////" << std::endl;
	for (size_t i = 0; i < NUM_CORNERS; ++i) {
		std::cout << cornerColors_[i] << "   ";
		std::cout << cornerOrients_[i] << std::endl;
	}

	std::cout << std::endl << std::endl;
}

bool Cube::isSolved()
{
	Cube solvedCube;
	return *this == solvedCube;
}

/*
Cube Cube::turnWith(Turn inputTurn)
{
	return turn(*this, inputTurn.repr);
}
*/

Cube Cube::turn(Cube cube, MoveInstruction mi) {
	switch (mi) {
		case FRONT: cube.turnFrontOrBack(FRONT_EDGES, FRONT_CORNERS); 
			break;
		case RIGHT: cube.turnRightOrLeft(RIGHT_EDGES, RIGHT_CORNERS); 
			break;
		case BACK: cube.turnFrontOrBack(BACK_EDGES, BACK_CORNERS); 
			break;
		case LEFT: cube.turnRightOrLeft(LEFT_EDGES, LEFT_CORNERS); 
			break;
		case UP: cube.turnUpOrDown(UP_EDGES, UP_CORNERS); 
			break;
		case DOWN: cube.turnUpOrDown(DOWN_EDGES, DOWN_CORNERS); 
			break;

		case FRONT_2: cube.turn2(FRONT);
			break;
		case RIGHT_2: cube.turn2(RIGHT);
			break;
		case BACK_2: cube.turn2(BACK);
			break;
		case LEFT_2: cube.turn2(LEFT);
			break;
		case UP_2: cube.turn2(UP);
			break;
		case DOWN_2: cube.turn2(DOWN);
			break;

		case FRONT_INVERTED: cube.turnI(FRONT);
			break;
		case RIGHT_INVERTED: cube.turnI(RIGHT);
			break;
		case BACK_INVERTED: cube.turnI(BACK);
			break;
		case LEFT_INVERTED: cube.turnI(LEFT);
			break;
		case UP_INVERTED: cube.turnI(UP);
			break;
		case DOWN_INVERTED: cube.turnI(DOWN);
			break;
		default:
			cube = Cube();

	}
	return cube;
}

void Cube::turn2(MoveInstruction mi)
{
	*this = turn(*this, mi);
	*this = turn(*this, mi);
}

void Cube::turnI(MoveInstruction mi)
{
	*this = turn(*this, mi);
	*this = turn(*this, mi);
	*this = turn(*this, mi);
}

void Cube::turnFrontOrBack(const Edge_t edges[4], const Corner_t corners[4])
{
	turnCubies(edges, corners);
	orientCorners(corners);
}

void Cube::turnRightOrLeft(const Edge_t edges[4], const Corner_t corners[4])
{
	turnCubies(edges, corners);
}

void Cube::turnUpOrDown(const Edge_t edges[4], const Corner_t corners[4])
{
	turnCubies(edges, corners);
	orientEdges(edges);
	orientCorners(corners);
}

void Cube::turnCubies(const Edge_t faceEdges[4], const Corner_t faceCorners[4])
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

void Cube::orientEdges(const Edge_t edges[4])
{
	// edge orientations are changed identically (wrt the face) for U,D,U',D'
	for (int i = 0; i < 4; ++i) {
		edgeOrients_[edges[i]] = !edgeOrients_[edges[i]];
	}
}

void Cube::orientCorners(const Corner_t pos[4])
{
	// corner orientations also change the same for U,D,U',D',L,R,L',R'
	cornerOrients_[pos[0]] = (cornerOrients_[pos[0]] + 1) % 3;
	cornerOrients_[pos[1]] = (cornerOrients_[pos[1]] + 2) % 3;
	cornerOrients_[pos[2]] = (cornerOrients_[pos[2]] + 1) % 3;
	cornerOrients_[pos[3]] = (cornerOrients_[pos[3]] + 2) % 3;
}
