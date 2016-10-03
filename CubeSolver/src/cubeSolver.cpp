#include "cubeSolver.h"
//#include "cube_end_tables.h"

#include <assert.h>
#include <iostream>
#include <algorithm>
#include <queue>
//#include <ctime>
#include <map>
#include <fstream>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <bitset>
#include <vector>
//#include "comm-protocol.h"


using namespace std;

// Vector of allowable turns for each step (easy to iterate through)
/*
Turn OK_TURNS1[NUM_TURNS_STEP1] = {
	Turn(MoveInstruction::FRONT), Turn(MoveInstruction::RIGHT), Turn(MoveInstruction::BACK), Turn(MoveInstruction::LEFT),
	Turn(MoveInstruction::UP), Turn(MoveInstruction::DOWN), Turn(MoveInstruction::FRONT_INVERTED), Turn(MoveInstruction::RIGHT_INVERTED),
	Turn(MoveInstruction::BACK_INVERTED), Turn(MoveInstruction::LEFT_INVERTED), Turn(MoveInstruction::UP_INVERTED), Turn(MoveInstruction::DOWN_INVERTED) };

Turn OK_TURNS2[NUM_TURNS_STEP2] = {
	Turn(MoveInstruction::FRONT_2), Turn(MoveInstruction::RIGHT), Turn(MoveInstruction::BACK_2), Turn(MoveInstruction::LEFT),
	Turn(MoveInstruction::UP_2), Turn(MoveInstruction::DOWN_2), Turn(MoveInstruction::RIGHT_INVERTED), Turn(MoveInstruction::LEFT_INVERTED) };
*/

/*
void init()
{
	// todo: if no tables/maps, then create such maps
	readTurnTables();
	readEndMaps();
}
*/

const string END_TABLES_PATH = "ser/end_maps.ser";


void readEndMaps(string pathToFile, EndMap1*& endMap1, EndMap2*& endMap2)
{
	ifstream is(pathToFile, ios::binary);
	boost::archive::binary_iarchive iarch(is);

	endMap1 = new EndMap1();

	cout << "reading map 1" << endl;
	iarch >> *endMap1;
	cout << endMap1->size() << endl;

	endMap2 = new EndMap2();

	cout << "reading map 2" << endl;
	iarch >> *endMap2;
	cout << endMap2->size() << endl;

	is.close();
}


/*
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
*/

int SOLVE_STEP_1_COUNTER = 1;
deque<Turn> solveStep1DFS(Cube cube, EndMap1* endMap1)
{
	SOLVE_STEP_1_COUNTER = 1;
	/*
	Cube solvedCube;

	assert(STEP1MAP.count(solvedCube.cubeNums1()) == 1);
	assert(STEP2MAP.count(solvedCube.cubeNums2()) == 1);
	assert(EDGE_COLORS_TABLE2[3000][3] == 13466);
	*/
	CubeNumsStep1 currCube(cube);


	int MIN_DEPTH = 0;
	int MAX_DEPTH = 10;

	deque<Turn> result;

	for (int depth = MIN_DEPTH; depth < MAX_DEPTH; ++depth) {

		if (solveStep1Helper(depth, currCube, endMap1, result)) {
			return result;
		}
	}
	cout << "path not found :(" << endl;
	return deque<Turn>();
}


bool solveStep1Helper(int depth, const CubeNumsStep1& curr, EndMap1* endMap1, deque<Turn>& result)
{
	if (SOLVE_STEP_1_COUNTER % 100000 == 0) {
		cout << "step 1: " << SOLVE_STEP_1_COUNTER << endl;
		++SOLVE_STEP_1_COUNTER;
	}
	
	if (depth == 0) {
		if (endMap1->count(curr) > 0) {
			//cout << "found it" << endl;
			result = turnsFromEndMap1(curr, endMap1);

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
			if (solveStep1Helper(depth - 1, turned, endMap1, result)) {
				Turn currTurn = OK_TURNS1[i];

				result.push_front(currTurn);
				return true;
			}
		}
		return false;
	}
}

int SOLVE_STEP_2_COUNTER = 1;
deque<Turn> solveStep2DFS(Cube cube, EndMap2* endMap2)
{
	SOLVE_STEP_2_COUNTER = 1;
	/*
	Cube solvedCube;
	assert(STEP1MAP.count(solvedCube.cubeNums1()) == 1);
	assert(STEP2MAP.count(solvedCube.cubeNums2()) == 1);
	assert(EDGE_COLORS_TABLE2[3000][3] == 13466);
	*/
	CubeNumsStep2 currCube(cube);

	int MIN_DEPTH = 0;
	int MAX_DEPTH = 10;

	deque<Turn> result;

	for (int depth = MIN_DEPTH; depth < MAX_DEPTH; ++depth) {
		if (solveStep2Helper(depth, currCube, endMap2, result)) {
			return result;
		}
	}

	cout << "path not found :(" << endl;
	return deque<Turn>();
}


bool solveStep2Helper(int depth, const CubeNumsStep2& curr, EndMap2* endMap2, deque<Turn>& result)
{

	if (SOLVE_STEP_2_COUNTER % 100000 == 0) {
		cout << "step 2: " << SOLVE_STEP_2_COUNTER << endl;
		++SOLVE_STEP_2_COUNTER;
	}

	if (depth == 0) {
		if (endMap2->count(curr) > 0) {
			
			result = turnsFromEndMap2(curr, endMap2);
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
			if (solveStep2Helper(depth - 1, turnedCube, endMap2, result)) {
				Turn currTurn = OK_TURNS2[i];

				result.push_front(currTurn);
				return true;
			}
		}
		return false;
	}
}

deque<Turn> turnsFromEndMap1(CubeNumsStep1 start, EndMap1* endMap1)
{
	//cout << "end maps" << endl;
	deque<Turn> path;
	CubeNumsStep1 currNums = start;
	//cout << "finding turns" << endl;
	//start.print();
	int pathLength = 0;
	
	while (currNums != CubeNumsStep1()) {
		MoveInstruction mi = (*endMap1)[currNums];
		path.push_back(Turn(mi));

		int i = getIndex1(mi);
		currNums = currNums.turn(i);

		if (pathLength > 20) {
			cout << "error in turnsFromEndMap1" << endl;
			break;
		}
		++pathLength;
	}
	//cout << "found path" << endl;

	return path;
}

deque<Turn> turnsFromEndMap2(CubeNumsStep2 start, EndMap2* endMap2)
{
	deque<Turn> path;
	CubeNumsStep2 currNums = start;

	while (currNums != CubeNumsStep2()) {

		MoveInstruction mi = (*endMap2)[currNums];
		path.push_back(Turn(mi));

		int i = getIndex2(mi);
		currNums = currNums.turn(i);
	}

	return path;
}
/*
bool Cube::isSolved()
{
	Cube solvedCube;
	return *this == solvedCube;
}
*/

std::vector<MoveInstruction> solve(Cube& cube, EndMap1* endMap1, EndMap2* endMap2)
{
	deque<Turn> firstTurns = solveStep1DFS(cube, endMap1);

	// apply turns found in first turns
	
	for (auto turn : firstTurns) {
		cube = cube.turnWith(turn);
	}
	deque<Turn> lastTurns = solveStep2DFS(cube, endMap2);
	
	for (auto turn : lastTurns) {
		cube = cube.turnWith(turn);
	}

	vector<MoveInstruction> instructions;
	
	// print and fill vector with move instructions to return
	/*
	for (auto turn : firstTurns) {
		cout << turn.toString << " ";
		instructions.push_back(turn.repr);
	} cout << endl;
	
	for (auto turn : lastTurns) {
		cout << turn.toString << " ";
		instructions.push_back(turn.repr);
	} cout << endl;
	*/	

	return instructions;
}

//TODO: get rid of this
int getIndex1(MoveInstruction mi)
{
	switch (mi) {
	case MoveInstruction::FRONT: return 0;
		break;
	case MoveInstruction::RIGHT: return 1;
		break;
	case MoveInstruction::BACK: return 2;
		break;
	case MoveInstruction::LEFT: return 3;
		break;
	case MoveInstruction::UP: return 4;
		break;
	case MoveInstruction::DOWN: return 5;
		break;
	case MoveInstruction::FRONT_INVERTED: return 6;
		break;
	case MoveInstruction::RIGHT_INVERTED: return 7;
		break;
	case MoveInstruction::BACK_INVERTED: return 8;
		break;
	case MoveInstruction::LEFT_INVERTED: return 9;
		break;
	case MoveInstruction::UP_INVERTED: return 10;
		break;
	case MoveInstruction::DOWN_INVERTED: return 11;
		break;
	default:
		cout << "not an acceptable turn" << endl;
		return 0;
	}
}

int getIndex2(MoveInstruction mi)
{
	switch (mi) {
	case MoveInstruction::FRONT_2: return 0;
		break;
	case MoveInstruction::RIGHT: return 1;
		break;
	case MoveInstruction::BACK_2: return 2;
		break;
	case MoveInstruction::LEFT: return 3;
		break;
	case MoveInstruction::UP_2: return 4;
		break;
	case MoveInstruction::DOWN_2: return 5;
		break;
	case MoveInstruction::RIGHT_INVERTED: return 6;
		break;
	case MoveInstruction::LEFT_INVERTED: return 7;
		break;
	default: 
		cout << "not an acceptable turn" << endl;
		return 0;
	}
}

/*
void Cube::testDFS()
{
	readTurnTables();
	readEndMaps();
	
	Cube cube;

	vector<Turn> maneuver = {
		Turn(MoveInstruction::DOWN),
		Turn(MoveInstruction::RIGHT),
		Turn(MoveInstruction::FRONT_INVERTED),
		Turn(MoveInstruction::UP_INVERTED),
		Turn(MoveInstruction::FRONT_INVERTED),
		Turn(MoveInstruction::BACK),
		Turn(MoveInstruction::LEFT)
		};
		
	for (auto turn : maneuver) {
		cube = cube.turnWith(turn);
	}
	
	cout << "Starting..." << endl;

	clock_t t;
	t = clock();

	cube.solve();

	t = clock() - t;
	cout << "time for searching: " << (float)t / CLOCKS_PER_SEC << endl;
	
	}
*/
/*int main()
{

	Cube::testDFS();
	
	//std::system("pause");
	return 0;
}*/



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