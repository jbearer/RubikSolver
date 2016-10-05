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
const Cube::Edge_t Cube::FB_SLICE[4] = { YR, YO, WR, WO };
const Cube::Edge_t Cube::UD_SLICE[4] = { RB, OB, RG, OG };


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

ushort Cube::edgeOrientsCode() const
{
	// Two codes have the same code if the edge orientations are the same
	// Essentially creates a unique base 2 number
	int sum = 0;
	for (int i = 0; i < NUM_EDGES; ++i) {
		sum += edgeOrients_[i] * pow(2, i);
	}
	return sum;
}

ushort Cube::cornerOrientsCode() const
{
	int sum = 0;
	// NUM_CORNERS - 1 because the last corner is defined by the previous 7
	for (int i = 0; i < NUM_CORNERS; ++i) {
		sum += cornerOrients_[i] * pow(3, i);
	}
	return sum;
}



ushort Cube::cornerColorsCode() const
{
	int sum = 0;
	for (int i = 0; i < NUM_CORNERS - 1; ++i) {

		int numGreater = 0;
		for (int j = i + 1; j < NUM_CORNERS; ++j) {

			if (cornerColors_[j] < cornerColors_[i]) {
				++numGreater;
			}
		}
		sum += (numGreater * factorial<double>(8 - i - 1));
	}
	return sum;
}


ushort Cube::edgeOrbitsCode() const
{
	// Hashes {0,0,0,0,_,_...} to 0, {0,0,0,_,0,_...} to 1,
	// ... and {...,_,0,0,0,0} to 494 = (12 choose 4) - 1.
	int sum = 0;
	int offset = 3;
	for (int i = 0; offset >= 0; ++i) {
		
		if (edgeColors_[i] == YB || edgeColors_[i] == YG ||
			edgeColors_[i] == WB || edgeColors_[i] == WG) {
			--offset;
		}
		else {
			sum += binomial_coefficient<double>(12 - i - 1, offset);
		}
	}
	return sum;
}

ushort Cube::edgeColorsCode1() const
{
	int sum = 0;
	int ORBIT_SIZE = 4;

	for (int i = 0; i < ORBIT_SIZE - 1; ++i) {

		int numGreater = 0;
		for (int j = i + 1; j < ORBIT_SIZE; ++j) {

			if (edgeColors_[LR_SLICE[j]] < edgeColors_[LR_SLICE[i]]) {
				++numGreater;
			}
		}
		sum += (numGreater * factorial<double>(ORBIT_SIZE - i - 1));
	}
	return sum;
}

ushort Cube::edgeColorsCode2() const
{
	int sum = 0;
	int ORBIT_SIZE = 8;

	// combined LR_SLICE and FB_SLICE
	Edge_t permArray[8] = { YR, YO, RB, OB, RG, OG, WR, WO };

	for (int i = 0; i < ORBIT_SIZE - 1; ++i) {

		int numGreater = 0;
		for (int j = i + 1; j < ORBIT_SIZE; ++j) {

			if (edgeColors_[permArray[j]] < edgeColors_[permArray[i]]) {
				++numGreater;
			}
		}
		sum += (numGreater * factorial<double>(ORBIT_SIZE - i - 1));
	}
	return sum;
}

bool Cube::nextEdgeColors1()
{
	const size_t orbit_size = 4;

	// todo: use references
	Edge_t lrSliceColors[orbit_size];

	// Store the lr slice
	for (size_t i = 0; i < orbit_size; ++i)
		lrSliceColors[i] = edgeColors_[LR_SLICE[i]];

	// increment the lr slice, if possible
	if (!std::next_permutation(lrSliceColors, lrSliceColors + orbit_size))
		return false;

	// store the lr slice back into the cube
	for (size_t i = 0; i < orbit_size; ++i)
		edgeColors_[LR_SLICE[i]] = lrSliceColors[i];

	return true;
}

bool Cube::nextEdgeColors2()
{
	const size_t orbit_size = 8;

	Edge_t fb_ud_colors[orbit_size] = { YR, YO, RB, OB, RG, OG, WR, WO };
	
	Edge_t currPermutation[orbit_size];

	for (size_t i = 0; i < orbit_size; ++i)
		currPermutation[i] = edgeColors_[fb_ud_colors[i]];

	if (!std::next_permutation(currPermutation, currPermutation + orbit_size))
		return false;

	for (size_t i = 0; i < orbit_size; ++i)
		edgeColors_[fb_ud_colors[i]] = currPermutation[i];

	return true;
}



bool Cube::nextEdgeOrients()
{
	for (int i = 0; i < NUM_EDGES; ++i) {
		
		// if you see a 1, change it to 0 and carry it over
		if (edgeOrients_[i] == 1) {
			edgeOrients_[i] = 0;
		}

		// If you see a 0, increment and return
		else {
			++edgeOrients_[i];
			return true;
		}
	}
	// the permutation was {1,1,1...}
	return false;
}

bool Cube::nextCornerOrients()
{
	for (std::size_t i = 0; i < NUM_CORNERS; ++i) {
		// if you reach a 2, change it to a 0 and carry over
		if (cornerOrients_[i] == 2) {
			cornerOrients_[i] = 0;
		}
		// if you reach a 0 or 1, increment it and return
		else {
			++cornerOrients_[i];
			return true;
		}
	}

	// the permutation was {2,2,2...}
	return false;
}

bool Cube::nextEdgeOrbits()
{
	return std::next_permutation(edgeColors_, edgeColors_ + NUM_EDGES);
}

void Cube::setFirstEdgeOrbits()
{
	// set edge colors to first edge orbits (have code 0)
	// represents a cube with LR slice edges at YB locations
	// and other edges in the YO spaces
	Edge_t newEdgeColors[Cube::NUM_EDGES] = { YB,YB,YB,YB,YO,YO,YO,YO,YO,YO,YO,YO };
	for (size_t i = 0; i < Cube::NUM_EDGES; ++i) {
		edgeColors_[i] = newEdgeColors[i];
	}
}

bool Cube::nextCornerColors()
{
	return std::next_permutation(cornerColors_, cornerColors_ + NUM_CORNERS);
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
