#include "cube.h"


Cube::CubeNumsStep1::CubeNumsStep1() : // initialize with solved cube
	edgeOrients_{0}, cornerOrients_{0}, edgeOrbits_{105} //TODO: FIX EDGE ORBITS
	{
		//	nothing to do
	}

Cube::CubeNumsStep2::CubeNumsStep2() : // initialize with solved cube
	cornerColors_{ 0 }, edgeColors1_{ 0 }, edgeColors2_{ 0 } //TODO: FIX EDGE ORBITS
{
	//	nothing to do
}


Cube::CubeNumsStep1::CubeNumsStep1(ushort in1, ushort in2, ushort in3) :
	edgeOrients_{ in1 }, cornerOrients_{ in2 }, edgeOrbits_{ in3 }
{
	// NOTHING TO DO
}

Cube::CubeNumsStep2::CubeNumsStep2(ushort in1, ushort in2, ushort in3) :
	cornerColors_{ in1 }, edgeColors1_{ in2 }, edgeColors2_{ in3 }
{
	// NOTHING TO DO
}

Cube::CubeNumsStep1::CubeNumsStep1(const Cube& cube) :
	edgeOrients_{ cube.edgeOrientsCode() },
	cornerOrients_{ cube.cornerOrientsCode() },
	edgeOrbits_{ cube.edgeOrbitsCode() }
{
	// nothing else to do
}

Cube::CubeNumsStep2::CubeNumsStep2(const Cube& cube) :
	cornerColors_{ cube.cornerColorsCode() },
	edgeColors1_{ cube.edgeColorsCode1() },
	edgeColors2_{ cube.edgeColorsCode2() }
{
	// nothing else to do
}

bool Cube::CubeNumsStep1::operator==(const CubeNumsStep1& rhs) const
{

	return
		edgeOrients_ == rhs.edgeOrients_ &&
		cornerOrients_ == rhs.cornerOrients_ &&
		edgeOrbits_ == rhs.edgeOrbits_;
}

bool Cube::CubeNumsStep1::operator!=(const CubeNumsStep1& rhs) const
{
	return !(*this == rhs);
}



bool Cube::CubeNumsStep2::operator==(const CubeNumsStep2& rhs) const
{

	return
		cornerColors_ == rhs.cornerColors_ &&
		edgeColors1_ == rhs.edgeColors1_ &&
		edgeColors2_ == rhs.edgeColors2_;
}

bool Cube::CubeNumsStep2::operator!=(const CubeNumsStep2& rhs) const
{
	return !(*this == rhs);
}

size_t Cube::CubeNumsStep1::Hash::operator()(const CubeNumsStep1& cube) const
{
	return cube.edgeOrients_ +
		cube.cornerOrients_ * NUM_EDGE_ORIENTS +
		cube.edgeOrbits_* NUM_EDGE_ORIENTS * NUM_CORNER_ORIENTS;
}


size_t Cube::CubeNumsStep2::Hash::operator()(const CubeNumsStep2& cube) const
{
	return cube.cornerColors_ +
		cube.edgeColors1_ * NUM_CORNER_COLORS * NUM_EDGE_COLORS2 +
		cube.edgeColors2_ * NUM_CORNER_COLORS;
}


Cube::CubeNumsStep1 Cube::CubeNumsStep1::turn(int i) const
{
	return CubeNumsStep1(
		EDGE_ORIENTS_TABLE[edgeOrients_][i],
		CORNER_ORIENTS_TABLE[cornerOrients_][i],
		EDGE_ORBITS_TABLE[edgeOrbits_][i]);
}

Cube::CubeNumsStep2 Cube::CubeNumsStep2::turn(int i) const
{
	return CubeNumsStep2(
		CORNER_COLORS_TABLE[cornerColors_][i],
		EDGE_COLORS_TABLE1[edgeColors1_][i],
		EDGE_COLORS_TABLE2[edgeColors2_][i]);
}

/*
Cube::CubeNums Cube::cubeNums1()
{
	return CubeNums(
		edgeOrientsCode(),
		cornerOrientsCode(),
		edgeOrbitsCode());
}


Cube::CubeNums Cube::cubeNums2()
{
	return CubeNums(
		cornerColorsCode(),
		edgeColorsCode1(),
		edgeColorsCode2());
}
*/