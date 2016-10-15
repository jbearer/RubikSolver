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
			CubeNumsStep1 turned = CubeNumsStep1::turn(curr, i);
			// cube found: push back current move and return true
			if (solveStep1Helper(depth - 1, turned, endMap1, result)) {
				Turn currTurn = TURNS_STEP1[i];

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

			CubeNumsStep2 turnedCube = CubeNumsStep2::turn(curr, i);

			// cube found: push back current move and return true
			if (solveStep2Helper(depth - 1, turnedCube, endMap2, result)) {
				Turn currTurn = TURNS_STEP2[i];

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
		Turn t = (*endMap1)[currNums];
		path.push_back(t);

		int i = getIndex1(t);
		currNums = CubeNumsStep1::turn(currNums, i);

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

		Turn t = (*endMap2)[currNums];
		path.push_back(t);

		int i = getIndex2(t);
		currNums = CubeNumsStep2::turn(currNums, i);
	}

	return path;
}

std::vector<Turn> CubeSolver::solve(Cube& cube, EndMap1* endMap1, EndMap2* endMap2)
{
	std::vector<Turn> allTurns;

	std::deque<Turn> firstTurns = solveStep1DFS(cube, endMap1);

	// apply turns found in first turns
	for (auto turn : firstTurns) {
		cube = Cube::turn(cube, turn);
		allTurns.push_back(turn);
	}
	std::deque<Turn> lastTurns = solveStep2DFS(cube, endMap2);

	for (auto turn : lastTurns) {
		cube = Cube::turn(cube, turn);
		allTurns.push_back(turn);
	}

	return allTurns;
}

int CubeSolver::getIndex1(Turn mi)
{
	for (size_t i = 0; i < NUM_TURNS_STEP1; ++i) {
		if (TURNS_STEP1[i] == mi) {
			return i;
		}
	}
	cout << "turn not found" << endl;
	return 0;
}

int CubeSolver::getIndex2(Turn mi)
{
	for (size_t i = 0; i < NUM_TURNS_STEP2; ++i) {
		if (TURNS_STEP2[i] == mi) {
			return i;
		}
	}
	cout << "turn not found" << endl;
	return 0;
}
