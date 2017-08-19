/**
* \file 	cubeEncoder.cpp
* \brief	Contains the implementation of cubeEncoder.h
*/
#include <utility>
#include <iostream>
#include <boost/math/special_functions/binomial.hpp>

#include "cubeEncoder.h"
#include "cube.h"

using std::cout;
using std::endl;
using boost::math::binomial_coefficient;
using boost::math::factorial;

using namespace CubeSolver;

// Corresponding edges in the LR-Slice (meridian parallel to L and R)
const Cube::Edge_t CubeEncoder::LR_SLICE[ORBIT_SIZE] =
			{ Cube::YB, Cube::YG, Cube::WB, Cube::WG };


ushort CubeEncoder::edgeOrientsCode(const Cube& cube)
{
	size_t sum = 0;
	for (size_t i = 0; i < Cube::NUM_EDGES; ++i) {
		sum += cube.edgeOrients_[i] * pow(2, i);
	}
	return sum;
}

ushort CubeEncoder::cornerOrientsCode(const Cube& cube)
{
	size_t sum = 0;
	for (size_t i = 0; i < Cube::NUM_CORNERS; ++i) {
		sum += cube.cornerOrients_[i] * pow(3, i);
	}
	return sum;
}


ushort CubeEncoder::edgeOrbitsCode(const Cube& cube)
{
	size_t sum = 0;
	int offset = 3;

	for (size_t i = 0; offset >= 0; ++i) {
		// If this edge is a member of the LR_SLICE edges, decrease offset
		if (std::count(LR_SLICE, LR_SLICE + ORBIT_SIZE, cube.edgeColors_[i])) {
			--offset;
		}
		// if it's not an LR_SLICE edge, then skip over the possible ways that
		// it could have been an LR_SLICE edge, calculated below
		else {
			sum +=
				binomial_coefficient<double>(Cube::NUM_EDGES - i - 1, offset);
		}
	}
	return sum;
}

ushort CubeEncoder::edgeColorsCode1(const Cube& cube)
{
	int sum = 0;

	for (size_t i = 0; i < ORBIT_SIZE - 1; ++i) {

		// Counts the number of elements in the LR_SLICE after i,
		// that i is greater than.  For example, in {3,4,2,1},
		// numGreater(3) = 2 and numGreater(2) = 1
		size_t numGreater = 0;
		for (size_t j = i + 1; j < ORBIT_SIZE; ++j) {

			if (cube.edgeColors_[LR_SLICE[j]] < cube.edgeColors_[LR_SLICE[i]]) {
				++numGreater;
			}
		}
		sum += (numGreater * factorial<double>(ORBIT_SIZE - i - 1));
	}
	return sum;
}

ushort CubeEncoder::edgeColorsCode2(const Cube& cube)
{
	int sum = 0;
	const size_t fb_ud_size = 8;

	// combined FB_SLICE and UD_SLICE
	Cube::Edge_t fb_ud_slice[fb_ud_size] = {
		Cube::YR, Cube::YO, Cube::RB, Cube::OB,
		Cube::RG, Cube::OG, Cube::WR, Cube::WO };

	for (size_t i = 0; i < fb_ud_size - 1; ++i) {

		int numGreater = 0;
		for (size_t j = i + 1; j < fb_ud_size; ++j) {

			if (cube.edgeColors_[fb_ud_slice[j]] <
				cube.edgeColors_[fb_ud_slice[i]]) {
				++numGreater;
			}
		}
		sum += (numGreater * factorial<double>(fb_ud_size - i - 1));
	}
	return sum;
}


ushort CubeEncoder::cornerColorsCode(const Cube& cube)
{
	size_t sum = 0;
	for (size_t i = 0; i < Cube::NUM_CORNERS - 1; ++i) {

		size_t numGreater = 0;
		for (size_t j = i + 1; j < Cube::NUM_CORNERS; ++j) {

			if (cube.cornerColors_[j] < cube.cornerColors_[i]) {
				++numGreater;
			}
		}
		sum += (numGreater * factorial<double>(Cube::NUM_CORNERS - i - 1));
	}
	return sum;
}

bool CubeEncoder::nextEdgeOrients(Cube& cube)
{
	for (int i = 0; i < Cube::NUM_EDGES; ++i) {

		// if you see a 1, change it to 0 and carry it over
		if (cube.edgeOrients_[i] == 1) {
			cube.edgeOrients_[i] = 0;
		}

		// If you see a 0, increment and return
		else {
			++cube.edgeOrients_[i];
			return true;
		}
	}
	// the permutation was {1,1,1...}
	return false;
}

bool CubeEncoder::nextCornerOrients(Cube& cube)
{
	for (std::size_t i = 0; i < Cube::NUM_CORNERS; ++i) {
		// if you reach a 2, change it to a 0 and carry over
		if (cube.cornerOrients_[i] == 2) {
			cube.cornerOrients_[i] = 0;
		}
		// if you reach a 0 or 1, increment it and return
		else {
			++cube.cornerOrients_[i];
			return true;
		}
	}

	// the permutation was {2,2,2...}
	return false;
}

bool CubeEncoder::nextEdgeOrbits(Cube& cube)
{
	// Assumes that edgeColors_ is only consists of two enums, one representing
	// on the LR_SLICE and the other off the LR_SLICE.
	return std::next_permutation(
		cube.edgeColors_, cube.edgeColors_ + Cube::NUM_EDGES);
}

void CubeEncoder::setFirstEdgeOrbits(Cube& cube)
{
	// set edge colors to first edge orbits (have code 0)
	// represents a cube with LR slice edges at YB locations
	// and other edges in the YO spaces
	for (size_t i = 0; i < Cube::NUM_EDGES; ++i)
		cube.edgeColors_[i] = i < 4 ? Cube::YB : Cube::YO;
}

bool CubeEncoder::nextEdgeColors1(Cube& cube)
{
	const size_t orbit_size = 4;

	// todo: use references
	Cube::Edge_t lrSliceColors[orbit_size];

	// Store the lr slice
	for (size_t i = 0; i < orbit_size; ++i)
		lrSliceColors[i] = cube.edgeColors_[LR_SLICE[i]];

	// increment the lr slice, if possible
	if (!std::next_permutation(lrSliceColors, lrSliceColors + orbit_size))
		return false;

	// store the lr slice back into the cube
	for (size_t i = 0; i < orbit_size; ++i)
		cube.edgeColors_[LR_SLICE[i]] = lrSliceColors[i];

	return true;
}

bool CubeEncoder::nextEdgeColors2(Cube& cube)
{
	const size_t orbit_size = 8;

	Cube::Edge_t fb_ud_colors[orbit_size] =
		{ Cube::YR, Cube::YO, Cube::RB, Cube::OB,
		  Cube::RG, Cube::OG, Cube::WR, Cube::WO };

	Cube::Edge_t currPermutation[orbit_size];

	for (size_t i = 0; i < orbit_size; ++i)
		currPermutation[i] = cube.edgeColors_[fb_ud_colors[i]];

	if (!std::next_permutation(currPermutation, currPermutation + orbit_size))
		return false;

	for (size_t i = 0; i < orbit_size; ++i)
		cube.edgeColors_[fb_ud_colors[i]] = currPermutation[i];

	return true;
}



bool CubeEncoder::nextCornerColors(Cube& cube)
{
	return std::next_permutation(
		cube.cornerColors_, cube.cornerColors_ + Cube::NUM_CORNERS);
}


