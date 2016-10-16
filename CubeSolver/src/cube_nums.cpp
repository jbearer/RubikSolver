#include <iostream>

#include "cubeSolver.h"

using CubeSolver::Cube;
using CommProtocol::MoveInstruction;

using namespace CubeSolver;
using namespace CubeSolver::TurnTables;

const std::string TURN_TABLES_PATH = "ser/turn_tables.ser";


CubeNumsStep1::CubeNumsStep1() : // initialize with solved cube
edgeOrients_{ 0 }, cornerOrients_{ 0 }, edgeOrbits_{ 105 }
{
	//	nothing to do
}

CubeNumsStep2::CubeNumsStep2() : // initialize with solved cube
	cornerColors_{ 0 }, edgeColors1_{ 0 }, edgeColors2_{ 0 }
{
	//	nothing to do
}


CubeNumsStep1::CubeNumsStep1(ushort in1, ushort in2, ushort in3) :
	edgeOrients_{ in1 }, cornerOrients_{ in2 }, edgeOrbits_{ in3 }
{
	// nothing to do
}

CubeNumsStep2::CubeNumsStep2(ushort in1, ushort in2, ushort in3) :
	cornerColors_{ in1 }, edgeColors1_{ in2 }, edgeColors2_{ in3 }
{
	// nothing to do
}

CubeNumsStep1::CubeNumsStep1(const Cube& cube) :
	edgeOrients_{ CubeEncoder::edgeOrientsCode(cube) },
	cornerOrients_{ CubeEncoder::cornerOrientsCode(cube) },
	edgeOrbits_{ CubeEncoder::edgeOrbitsCode(cube) }
{
	// nothing else to do
}

CubeNumsStep2::CubeNumsStep2(const Cube& cube) :
	cornerColors_{ CubeEncoder::cornerColorsCode(cube) },
	edgeColors1_{ CubeEncoder::edgeColorsCode1(cube) },
	edgeColors2_{ CubeEncoder::edgeColorsCode2(cube) }
{
	// nothing else to do
}

bool CubeNumsStep1::operator==(const CubeNumsStep1& rhs) const
{

	return
		edgeOrients_ == rhs.edgeOrients_ &&
		cornerOrients_ == rhs.cornerOrients_ &&
		edgeOrbits_ == rhs.edgeOrbits_;
}

bool CubeNumsStep1::operator!=(const CubeNumsStep1& rhs) const
{
	return !(*this == rhs);
}



bool CubeNumsStep2::operator==(const CubeNumsStep2& rhs) const
{

	return
		cornerColors_ == rhs.cornerColors_ &&
		edgeColors1_ == rhs.edgeColors1_ &&
		edgeColors2_ == rhs.edgeColors2_;
}

bool CubeNumsStep2::operator!=(const CubeNumsStep2& rhs) const
{
	return !(*this == rhs);
}

size_t CubeNumsStep1::Hash::operator()(const CubeNumsStep1& cube) const
{
	return cube.edgeOrients_ +
		cube.cornerOrients_ * NUM_EDGE_ORIENTS +
		cube.edgeOrbits_* NUM_EDGE_ORIENTS * NUM_CORNER_ORIENTS;
}


size_t CubeNumsStep2::Hash::operator()(const CubeNumsStep2& cube) const
{
	return cube.cornerColors_ +
		cube.edgeColors1_ * NUM_CORNER_COLORS * NUM_EDGE_COLORS2 +
		cube.edgeColors2_ * NUM_CORNER_COLORS;
}

CubeNumsStep1 CubeNumsStep1::turn(const CubeNumsStep1& cube, int i)
{
	return CubeNumsStep1(
		TurnTables::EDGE_ORIENTS_TABLE[cube.edgeOrients_][i],
		TurnTables::CORNER_ORIENTS_TABLE[cube.cornerOrients_][i],
		TurnTables::EDGE_ORBITS_TABLE[cube.edgeOrbits_][i]);
}

CubeNumsStep2 CubeNumsStep2::turn(const CubeNumsStep2& cube, int i)
{
	return CubeNumsStep2(
		TurnTables::CORNER_COLORS_TABLE[cube.cornerColors_][i],
		TurnTables::EDGE_COLORS_TABLE1[cube.edgeColors1_][i],
		TurnTables::EDGE_COLORS_TABLE2[cube.edgeColors2_][i]);
}

std::ostream& CubeSolver::operator<<(std::ostream& out, const CubeNumsStep1& cube)
{
    out << cube.edgeOrients_ << " ";
    out << cube.cornerOrients_ << " ";
    out << cube.edgeOrbits_ << " ";

    return out;
}

std::ostream& CubeSolver::operator<<(std::ostream& out, const CubeNumsStep2& cube)
{
    out << cube.cornerColors_ << " ";
    out << cube.edgeColors1_ << " ";
    out << cube.edgeColors2_ << " ";

    return out;
}
