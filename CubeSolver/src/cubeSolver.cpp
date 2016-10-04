#include "cubeSolver.h"

#include <iostream>
#include <queue>
#include <fstream>
#include <boost/serialization/unordered_map.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <vector>

using std::cout;
using std::endl;

using namespace CubeSolver;
using namespace CommProtocol;

const std::string END_TABLES_PATH = "ser/end_maps.ser";




int SOLVE_STEP_1_COUNTER = 1;
std::deque<Turn> CubeSolver::solveStep1DFS(Cube cube, EndMap1* endMap1)
{
	SOLVE_STEP_1_COUNTER = 1;

	// TODO: assert tables have been loaded

	CubeNumsStep1 currCube(cube);


	int MIN_DEPTH = 0;
	int MAX_DEPTH = 10;

	std::deque<Turn> result;

	for (int depth = MIN_DEPTH; depth < MAX_DEPTH; ++depth) {

		if (solveStep1Helper(depth, currCube, endMap1, result)) {
			return result;
		}
	}
	cout << "path not found :(" << endl;
	return std::deque<Turn>();
}


bool CubeSolver::solveStep1Helper(int depth, const CubeNumsStep1& curr, EndMap1* endMap1, std::deque<Turn>& result)
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
std::deque<Turn> CubeSolver::solveStep2DFS(Cube cube, EndMap2* endMap2)
{
	SOLVE_STEP_2_COUNTER = 1;

	// TODO: assert tables have been loaded

	CubeNumsStep2 currCube(cube);

	int MIN_DEPTH = 0;
	int MAX_DEPTH = 10;

	std::deque<Turn> result;

	for (int depth = MIN_DEPTH; depth < MAX_DEPTH; ++depth) {
		if (solveStep2Helper(depth, currCube, endMap2, result)) {
			return result;
		}
	}

	cout << "path not found :(" << endl;
	return std::deque<Turn>();
}


bool CubeSolver::solveStep2Helper(int depth, const CubeNumsStep2& curr, EndMap2* endMap2, std::deque<Turn>& result)
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

std::deque<Turn> CubeSolver::turnsFromEndMap1(CubeNumsStep1 start, EndMap1* endMap1)
{
	//cout << "end maps" << endl;
	std::deque<Turn> path;
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

	return path;
}

std::deque<Turn> CubeSolver::turnsFromEndMap2(CubeNumsStep2 start, EndMap2* endMap2)
{
	std::deque<Turn> path;
	CubeNumsStep2 currNums = start;

	while (currNums != CubeNumsStep2()) {

		MoveInstruction mi = (*endMap2)[currNums];
		path.push_back(Turn(mi));

		int i = getIndex2(mi);
		currNums = currNums.turn(i);
	}

	return path;
}

std::vector<MoveInstruction> CubeSolver::solve(Cube& cube, EndMap1* endMap1, EndMap2* endMap2)
{
	std::deque<Turn> firstTurns = solveStep1DFS(cube, endMap1);

	// apply turns found in first turns
	
	for (auto turn : firstTurns) {
		cube = cube.turnWith(turn);
	}
	std::deque<Turn> lastTurns = solveStep2DFS(cube, endMap2);
	
	for (auto turn : lastTurns) {
		cube = cube.turnWith(turn);
	}

	std::vector<MoveInstruction> instructions;	

	return instructions;
}

//TODO: get rid of this
int CubeSolver::getIndex1(MoveInstruction mi)
{
	switch (mi) {
	case FRONT: return 0;
		break;
	case RIGHT: return 1;
		break;
	case BACK: return 2;
		break;
	case LEFT: return 3;
		break;
	case UP: return 4;
		break;
	case DOWN: return 5;
		break;
	case FRONT_INVERTED: return 6;
		break;
	case RIGHT_INVERTED: return 7;
		break;
	case BACK_INVERTED: return 8;
		break;
	case LEFT_INVERTED: return 9;
		break;
	case UP_INVERTED: return 10;
		break;
	case DOWN_INVERTED: return 11;
		break;
	default:
		cout << "not an acceptable turn" << endl;
		return 0;
	}
}

int CubeSolver::getIndex2(MoveInstruction mi)
{
	switch (mi) {
	case FRONT_2: return 0;
		break;
	case RIGHT: return 1;
		break;
	case BACK_2: return 2;
		break;
	case LEFT: return 3;
		break;
	case UP_2: return 4;
		break;
	case DOWN_2: return 5;
		break;
	case RIGHT_INVERTED: return 6;
		break;
	case LEFT_INVERTED: return 7;
		break;
	default: 
		cout << "not an acceptable turn" << endl;
		return 0;
	}
}
