/**
* \file cube.cpp
*/

#include <utility>
#include <iostream>
#include <boost/math/special_functions/binomial.hpp>
#include "cube.h"

using CommProtocol::MoveInstruction;
using CubeSolver::Turn;
using boost::math::binomial_coefficient;
using boost::math::factorial;

using namespace CubeSolver;

// Vectors containing numbers that correspond to faces
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
Cube::Cube(Edge_t edgeColors[NUM_EDGES], char edgeOrients[NUM_EDGES],
	Corner_t cornerColors[NUM_CORNERS], char cornerOrients[NUM_CORNERS])
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

	// cast to an int, since they're chars
	for (size_t i = 0; i < NUM_EDGES; ++i) {
		std::cout << (int)edgeColors_[i] << "   ";
		std::cout << (int)edgeOrients_[i] << std::endl;

	}
	std::cout << std::endl;

	std::cout << " /// CORNERS ////" << std::endl;
	for (size_t i = 0; i < NUM_CORNERS; ++i) {
		std::cout << (int)cornerColors_[i] << "   ";
		std::cout << (int)cornerOrients_[i] << std::endl;
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

// todo: make friends instead (?)
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


Cube Cube::turnWith(Turn inputTurn)
{
	switch (inputTurn.repr) {

		case MoveInstruction::FRONT: return front(*this);
		case MoveInstruction::RIGHT: return right(*this);
		case MoveInstruction::BACK: return back(*this);
		case MoveInstruction::LEFT: return left(*this);
		case MoveInstruction::UP: return up(*this);
		case MoveInstruction::DOWN: return down(*this);
		
		case MoveInstruction::FRONT_INVERTED: return frontI(*this);
		case MoveInstruction::RIGHT_INVERTED: return rightI(*this);
		case MoveInstruction::BACK_INVERTED: return backI(*this);
		case MoveInstruction::LEFT_INVERTED: return leftI(*this);
		case MoveInstruction::UP_INVERTED: return upI(*this);
		case MoveInstruction::DOWN_INVERTED: return downI(*this);
		
		case MoveInstruction::FRONT_2: return front2(*this);
		case MoveInstruction::RIGHT_2: return right2(*this);
		case MoveInstruction::BACK_2: return back2(*this);
		case MoveInstruction::LEFT_2: return left2(*this);
		case MoveInstruction::UP_2: return up2(*this);
		case MoveInstruction::DOWN_2: return down2(*this);
		
		default: return Cube();
	}
}


Cube Cube::front(Cube cube)
{
	cube.turn(FRONT_EDGES, FRONT_CORNERS);
	cube.orientCorners(FRONT_CORNERS);
	return cube;
}

Cube Cube::right(Cube cube)
{
	cube.turn(RIGHT_EDGES, RIGHT_CORNERS);
	return cube;
}

Cube Cube::back(Cube cube)
{
	cube.turn(BACK_EDGES, BACK_CORNERS);
	cube.orientCorners(BACK_CORNERS);
	return cube;
}

Cube Cube::left(Cube cube)
{
	cube.turn(LEFT_EDGES, LEFT_CORNERS);
	return cube;
}

Cube Cube::up(Cube cube)
{
	cube.turn(UP_EDGES, UP_CORNERS);
	cube.orientEdges(UP_EDGES);
	cube.orientCorners(UP_CORNERS);
	return cube;
}

Cube Cube::down(Cube cube)
{
	cube.turn(DOWN_EDGES, DOWN_CORNERS);
	cube.orientEdges(DOWN_EDGES);
	cube.orientCorners(DOWN_CORNERS);
	return cube;
}

void Cube::turn(const Edge_t faceEdges[4], const Corner_t faceCorners[4])
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

Cube Cube::frontI(Cube cube)
{
	cube.turnI(FRONT_EDGES, FRONT_CORNERS);
	cube.orientCorners(FRONT_CORNERS);
	return cube;
}

Cube Cube::rightI(Cube cube)
{
	cube.turnI(RIGHT_EDGES, RIGHT_CORNERS);
	return cube;
}

Cube Cube::backI(Cube cube)
{
	cube.turnI(BACK_EDGES, BACK_CORNERS);
	cube.orientCorners(BACK_CORNERS);
	return cube;
}

Cube Cube::leftI(Cube cube)
{
	cube.turnI(LEFT_EDGES, LEFT_CORNERS);
	return cube;
}

Cube Cube::upI(Cube cube)
{
	cube.turnI(UP_EDGES, UP_CORNERS);
	cube.orientEdges(UP_EDGES);
	cube.orientCorners(UP_CORNERS);
	return cube;
}

Cube Cube::downI(Cube cube)
{
	cube.turnI(DOWN_EDGES, DOWN_CORNERS);
	cube.orientEdges(DOWN_EDGES);
	cube.orientCorners(DOWN_CORNERS);
	return cube;
}


void Cube::turnI(const Edge_t faceEdges[4],
	const Corner_t faceCorners[4])
{
	backwardCycle(faceEdges, edgeColors_);
	backwardCycle(faceEdges, edgeOrients_);
	
	backwardCycle(faceCorners, cornerColors_);
	backwardCycle(faceCorners, cornerOrients_);
}

template <typename Index_t, typename Cubie_t>
void Cube::backwardCycle(const Index_t positions[4], Cubie_t* cubies)
{
	Cubie_t temp = cubies[positions[0]];
	for (size_t i = 0; i < 3; ++i) {
		cubies[positions[i]] = cubies[positions[i + 1]];
	}
	cubies[positions[3]] = temp;
}



Cube Cube::front2(Cube cube)
{
	cube.turn2(FRONT_EDGES, FRONT_CORNERS);
	return cube;
}

Cube Cube::right2(Cube cube)
{
	cube.turn2(RIGHT_EDGES, RIGHT_CORNERS);
	return cube;
}

Cube Cube::back2(Cube cube)
{
	cube.turn2(BACK_EDGES, BACK_CORNERS);
	return cube;
}

Cube Cube::left2(Cube cube)
{
	cube.turn2(LEFT_EDGES, LEFT_CORNERS);
	return cube;
}

Cube Cube::up2(Cube cube)
{
	cube.turn2(UP_EDGES, UP_CORNERS);
	return cube;
}

Cube Cube::down2(Cube cube)
{
	cube.turn2(DOWN_EDGES, DOWN_CORNERS);
	return cube;
}

void Cube::turn2(const Edge_t faceEdges[4], const Corner_t faceCorners[4])
{
	// do double turn by swapping pairs of cubies.  No change in orientation.
	std::swap(edgeColors_[faceEdges[0]], edgeColors_[faceEdges[2]]);
	std::swap(edgeColors_[faceEdges[1]], edgeColors_[faceEdges[3]]);
	std::swap(edgeOrients_[faceEdges[0]], edgeOrients_[faceEdges[2]]);
	std::swap(edgeOrients_[faceEdges[1]], edgeOrients_[faceEdges[3]]);
	
	std::swap(cornerColors_[faceCorners[0]], cornerColors_[faceCorners[2]]);
	std::swap(cornerColors_[faceCorners[1]], cornerColors_[faceCorners[3]]);
	std::swap(cornerOrients_[faceCorners[0]], cornerOrients_[faceCorners[2]]);
	std::swap(cornerOrients_[faceCorners[1]], cornerOrients_[faceCorners[3]]);
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
