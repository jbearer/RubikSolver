#include "cube.h"


Cube::CubeNums::CubeNums() :	// initialize with invalid codes
	first {USHRT_MAX }, second{ USHRT_MAX }, third{ USHRT_MAX }
	{
		//	nothing to do
	}


Cube::CubeNums::CubeNums(ushort in1, ushort in2, ushort in3) :
	first{ in1 }, second{ in2 }, third{ in3 }
{
	// NOTHING TO DO
}

size_t Cube::CubeNums::Hash1::operator()(const CubeNums& cube) const
{
	return cube.first + cube.second * NUM_EDGE_ORIENTS + cube.third * NUM_EDGE_ORIENTS * NUM_CORNER_ORIENTS;
}

size_t Cube::CubeNums::Hash2::operator()(const CubeNums& cube) const
{
	return cube.first + cube.second * NUM_CORNER_COLORS * NUM_EDGE_COLORS2 + cube.third * NUM_CORNER_COLORS;
}


Cube::CubeNums Cube::CubeNums::turn1(int i) const
{
	return CubeNums(
		EDGE_ORIENTS_TABLE[first][i],
		CORNER_ORIENTS_TABLE[second][i],
		EDGE_ORBITS_TABLE[third][i]);
}

Cube::CubeNums Cube::CubeNums::turn2(int i) const
{
	return CubeNums(
		CORNER_COLORS_TABLE[first][i],
		EDGE_COLORS_TABLE1[second][i],
		EDGE_COLORS_TABLE2[third][i]);
}

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