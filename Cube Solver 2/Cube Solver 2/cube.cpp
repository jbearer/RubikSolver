#include "cube.h"

#include <assert.h>
#include <iostream>
#include <algorithm>
#include <queue>
#include <ctime>
#include <map>
#include <fstream>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <bitset>
#include <vector>
#include "comm-protocol.h"


using namespace std;
using CommProtocol::MoveInstruction;

// Vector of allowable turns for each step (easy to iterate through)
const Cube::Turn Cube::OK_TURNS1[Cube::NUM_TURNS_STEP1] = {
	Cube::Turn(MoveInstruction::FRONT), Cube::Turn(MoveInstruction::RIGHT), Cube::Turn(MoveInstruction::BACK), Cube::Turn(MoveInstruction::LEFT),
	Cube::Turn(MoveInstruction::UP), Cube::Turn(MoveInstruction::DOWN), Cube::Turn(MoveInstruction::FRONT_INVERTED), Cube::Turn(MoveInstruction::RIGHT_INVERTED),
	Cube::Turn(MoveInstruction::BACK_INVERTED), Cube::Turn(MoveInstruction::LEFT_INVERTED), Cube::Turn(MoveInstruction::UP_INVERTED), Cube::Turn(MoveInstruction::DOWN_INVERTED) };

const Cube::Turn Cube::OK_TURNS2[Cube::NUM_TURNS_STEP2] = {
	Cube::Turn(MoveInstruction::FRONT_2), Cube::Turn(MoveInstruction::RIGHT), Cube::Turn(MoveInstruction::BACK_2), Cube::Turn(MoveInstruction::LEFT),
	Cube::Turn(MoveInstruction::UP_2), Cube::Turn(MoveInstruction::DOWN_2), Cube::Turn(MoveInstruction::RIGHT_INVERTED), Cube::Turn(MoveInstruction::LEFT_INVERTED) };

int Cube::FAC[12];
int Cube::CHOOSE[12][12];

// Cube constructor
Cube::Cube(Edge_t edgeColors[12], char edgeOrients[12],
	Corner_t cornerColors[8], char cornerOrients[8])
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

//////////////////// INITIALIZING //////////////////////

int Cube::fac(int n)
{
	return n == 0 ? 1 : n*fac(n - 1);
}

void Cube::initChoose()
{
	// Memoize values for FAC and CHOOSE tables
	for (int n = 0; n < 12; ++n) {
		FAC[n] = fac(n);
	}

	for (int n = 0; n < 12; ++n) {
		for (int k = 0; k <= n; ++k) {
			CHOOSE[n][k] = FAC[n] / (FAC[k] * FAC[n - k]);
		}
	}
}


void Cube::print() const
{
	std::cout << "//// EDGES ////" << endl;

	// cast to an int, since they're chars
	for (size_t i = 0; i < NUM_EDGES; ++i) {
		std::cout << (int)edgeColors_[i] << "   ";
		std::cout << (int)edgeOrients_[i] << endl;

	}
	std::cout << endl;

	std::cout << " /// CORNERS ////" << endl;
	for (size_t i = 0; i < NUM_CORNERS; ++i) {
		std::cout << (int)cornerColors_[i] << "   ";
		std::cout << (int)cornerOrients_[i] << endl;
	}

	std::cout << endl << endl;
}


Cube::ushort Cube::edgeOrientsCode() const
{
	// Two codes have the same code if the edge orientations are the same
	// Essentially creates a unique base 2 number
	int sum = 0;
	for (int i = 0; i < NUM_EDGES; ++i) {
		sum += edgeOrients_[i] * pow(2, i);
	}
	return sum;
}

Cube::ushort Cube::cornerOrientsCode() const
{
	int sum = 0;
	// NUM_CORNERS - 1 because the last corner is defined by the previous 7
	for (int i = 0; i < NUM_CORNERS; ++i) {
		sum += cornerOrients_[i] * pow(3, i);
	}
	return sum;
}



Cube::ushort Cube::cornerColorsCode() const
{
	initChoose();

	int sum = 0;
	for (int i = 0; i < NUM_CORNERS - 1; ++i) {

		int numGreater = 0;
		for (int j = i + 1; j < NUM_CORNERS; ++j) {

			if (cornerColors_[j] < cornerColors_[i]) {
				++numGreater;
			}
		}
		sum += (numGreater * FAC[8 - i - 1]);
	}
	return sum;
}

Cube::ushort Cube::edgeOrbitsCode() const
{
	// Hashes {0,0,0,0,_,_...} to 0, {0,0,0,_,0,_...} to 1,
	// ... and {...,_,0,0,0,0} to 494 = (12 choose 4) - 1.
	int sum = 0;
	int offset = 3;
	for (int i = 0; offset >= 0; ++i) {
		Edge_t currEdge = edgeColors_[i];
		if (edgeColors_[i] == YB || edgeColors_[i] == YG ||
			edgeColors_[i] == WB || edgeColors_[i] == WG) {
			--offset;
		}
		else {
			sum += CHOOSE[12 - i - 1][offset];
		}
	}
	return sum;
}

Cube::ushort Cube::edgeColorsCode1() const
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
		sum += (numGreater * FAC[ORBIT_SIZE - i - 1]);
	}
	return sum;
}

Cube::ushort Cube::edgeColorsCode2() const
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
		sum += (numGreater * FAC[ORBIT_SIZE - i - 1]);
	}
	return sum;
}

deque<Cube::Turn> Cube::solveStep1DFS()
{
	/*
	Cube solvedCube;

	assert(STEP1MAP.count(solvedCube.cubeNums1()) == 1);
	assert(STEP2MAP.count(solvedCube.cubeNums2()) == 1);
	assert(EDGE_COLORS_TABLE2[3000][3] == 13466);
	*/
	CubeNumsStep1 currCube(*this);


	int MIN_DEPTH = 4;
	int MAX_DEPTH = 8;

	deque<Turn> result;

	for (int depth = MIN_DEPTH; depth < MAX_DEPTH; ++depth) {
		if (solveStep1Helper(depth, currCube, result)) {
			return result;
		}
	}
	cout << "path not found :(" << endl;
	return deque<Turn>();
}
int SOLVE_STEP_1_COUNTER = 0;
bool Cube::solveStep1Helper(int depth, const CubeNumsStep1& curr, deque<Turn>& result)
{
	//if (SOLVE_STEP_1_COUNTER % 1000 == 0) cout << SOLVE_STEP_1_COUNTER << endl;
	//++SOLVE_STEP_1_COUNTER;
	if (depth == 0) {
		if (STEP1MAP.count(curr) > 0) {
			//cout << "found it" << endl;
			result = turnsFromEndMap1(curr);

			return true;
		}
		else {
			// not found, so return false
			return false;
		}
	}
	else {
		// iterate through all the ok turns
		for (int i = 0; i < NUM_TURNS_STEP1; ++i) {

			CubeNumsStep1 turned = curr.turn(i);

			// cube found: push back current move and return true
			if (solveStep1Helper(depth - 1, turned, result)) {
				Turn currTurn = OK_TURNS1[i];

				result.push_front(currTurn);
				return true;
			}
		}
		return false;
	}
}


deque<Cube::Turn> Cube::solveStep2DFS()
{
	/*
	Cube solvedCube;
	assert(STEP1MAP.count(solvedCube.cubeNums1()) == 1);
	assert(STEP2MAP.count(solvedCube.cubeNums2()) == 1);
	assert(EDGE_COLORS_TABLE2[3000][3] == 13466);
	*/
	CubeNumsStep2 currCube(*this);

	int MIN_DEPTH = 4;
	int MAX_DEPTH = 10;

	deque<Turn> result;

	for (int depth = MIN_DEPTH; depth < MAX_DEPTH; ++depth) {
		if (solveStep2Helper(depth, currCube, result)) {
			return result;
		}
	}

	cout << "path not found :(" << endl;
	return deque<Turn>();
}

bool Cube::solveStep2Helper(int depth, const CubeNumsStep2& curr, deque<Turn>& result)
{
	if (depth == 0) {
		if (STEP2MAP.count(curr) > 0) {
			
			result = turnsFromEndMap2(curr);
			return true;
		}
		else {
			return false;
		}
	}
	else {
		// iterate through all ok turns
		for (int i = 0; i < NUM_TURNS_STEP2; ++i) {

			CubeNumsStep2 turnedCube = curr.turn(i);

			// cube found: push back current move and return true
			if (solveStep2Helper(depth - 1, turnedCube, result)) {
				Turn currTurn = OK_TURNS2[i];

				result.push_front(currTurn);
				return true;
			}
		}
		return false;
	}
}


std::vector<MoveInstruction> Cube::solve()
{
	deque<Turn> firstTurns = solveStep1DFS();

	// apply turns found in first turns
	
	for (auto turn : firstTurns) {
		*this = (turn.turnFunc)(*this);
	}
	deque<Turn> lastTurns = solveStep2DFS();
	
	vector<MoveInstruction> instructions;
	
	// print and fill vector with move instructions to return
	for (auto turn : firstTurns) {
		cout << turn.toString << " ";
		instructions.push_back(turn.repr);
	} cout << endl;
	
	for (auto turn : lastTurns) {
		cout << turn.toString << " ";
		instructions.push_back(turn.repr);
	} cout << endl;
	

	return instructions;
}




/*
To do:

1) Build move tables
2) Shorten to two-phase
3) Serialize 6-face solve hash tables
4) Create different executables for building maps and solving cube
5) Account for symmetries



Notes:

1) 3/17: average solve: 1.78 sec.  Longest: 24 sec
2) First step: average solve: .017, Longest: .187
*/