#include "cubeSolver.h"

#include <iostream>
#include <queue>
#include <algorithm>
#include <fstream>
#include <memory>
#include <boost/serialization/unordered_map.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <vector>

using std::cout;
using std::endl;

using namespace CubeSolver;

const std::string END_TABLES_PATH = "ser/end_maps.ser";


Solver::Solver()
{
	readTurnTables();
	e1_ = std::move(std::unique_ptr<EndMap1>(new EndMap1));
	e2_ = std::move(std::unique_ptr<EndMap2>(new EndMap2));

	// HACK to add a destination to the end maps.  FRONT
	// should be a dummy move instruction
	(*e1_)[CubeNumsStep1()] = FRONT;
	(*e2_)[CubeNumsStep2()] = FRONT;

}

Solver::Solver(std::string file)
{
	readTurnTables();
	e1_ = std::move(std::unique_ptr<EndMap1>(new EndMap1));
	e2_ = std::move(std::unique_ptr<EndMap2>(new EndMap2));
	readEndMaps(file, e1_, e2_);
}

int SOLVE_STEP_1_COUNTER = 1;
std::deque<Turn> Solver::solveStep1DFS(Cube cube)
{
	SOLVE_STEP_1_COUNTER = 1;

	// TODO: assert tables have been loaded

	CubeNumsStep1 currCube(cube);


	int MIN_DEPTH = 0;
	int MAX_DEPTH = 10;

	std::deque<Turn> result;

	for (int depth = MIN_DEPTH; depth < MAX_DEPTH; ++depth) {

		if (solveStep1Helper(depth, currCube, result)) {
			return result;
		}
	}
	cout << "path not found :(" << endl;
	throw std::exception();
	//return std::deque<Turn>();
}


bool Solver::solveStep1Helper(int depth, const CubeNumsStep1& curr, std::deque<Turn>& result)
{
	if (SOLVE_STEP_1_COUNTER % 100000 == 0) {
		cout << "step 1: " << SOLVE_STEP_1_COUNTER << endl;
		++SOLVE_STEP_1_COUNTER;
	}

	if (depth == 0) {
		if (e1_->count(curr) > 0) {
			//cout << "found it" << endl;
			result = turnsFromEndMap1(curr, e1_);

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
			if (solveStep1Helper(depth - 1, turned, result)) {
				Turn currTurn = TURNS_STEP1[i];

				result.push_front(currTurn);
				return true;
			}
		}
		return false;
	}
}

int SOLVE_STEP_2_COUNTER = 1;
std::deque<Turn> Solver::solveStep2DFS(Cube cube)
{
	SOLVE_STEP_2_COUNTER = 1;

	// TODO: assert tables have been loaded

	CubeNumsStep2 currCube(cube);

	int MIN_DEPTH = 0;
	int MAX_DEPTH = 10;

	std::deque<Turn> result;

	for (int depth = MIN_DEPTH; depth < MAX_DEPTH; ++depth) {
		if (solveStep2Helper(depth, currCube, result)) {
			return result;
		}
	}

	cout << "path not found :(" << endl;
	throw std::exception();
	return std::deque<Turn>();
}


bool Solver::solveStep2Helper(int depth, const CubeNumsStep2& curr, std::deque<Turn>& result)
{

	if (SOLVE_STEP_2_COUNTER % 100000 == 0) {
		cout << "step 2: " << SOLVE_STEP_2_COUNTER << endl;
		++SOLVE_STEP_2_COUNTER;
	}

	if (depth == 0) {
		if (e2_->count(curr) > 0) {

			result = turnsFromEndMap2(curr, e2_);
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
			if (solveStep2Helper(depth - 1, turnedCube, result)) {
				Turn currTurn = TURNS_STEP2[i];

				result.push_front(currTurn);
				return true;
			}
		}
		return false;
	}
}

std::deque<Turn> CubeSolver::turnsFromEndMap1(CubeNumsStep1 start, std::unique_ptr<EndMap1>& e1)
{
	//cout << "end maps" << endl;
	std::deque<Turn> path;
	CubeNumsStep1 currNums = start;
	//cout << "finding turns" << endl;
	//start.print();
	int pathLength = 0;

	while (currNums != CubeNumsStep1()) {
		Turn t = (*e1)[currNums];
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

std::deque<Turn> CubeSolver::turnsFromEndMap2(CubeNumsStep2 start, std::unique_ptr<EndMap2>& e2)
{
	std::deque<Turn> path;
	CubeNumsStep2 currNums = start;

	while (currNums != CubeNumsStep2()) {

		Turn t = (*e2)[currNums];
		path.push_back(t);

		int i = getIndex2(t);
		currNums = CubeNumsStep2::turn(currNums, i);
	}

	return path;
}

std::vector<Turn> Solver::solve(Cube& cube)
{
	std::vector<Turn> allTurns;

	if (cube == Cube()){
		return allTurns;
	}

	std::deque<Turn> firstTurns = solveStep1DFS(cube);

	// apply turns found in first turns
	for (auto turn : firstTurns) {
		cube = Cube::turn(cube, turn);
		allTurns.push_back(turn);
	}

	std::deque<Turn> lastTurns = solveStep2DFS(cube);

	for (auto turn : lastTurns) {
		cube = Cube::turn(cube, turn);
		allTurns.push_back(turn);
	}

	return allTurns;
}

std::vector<Turn> Solver::solveToCube(Cube& start, Cube end)
{
	// turns to get from start to solved;
	std::vector<Turn> allTurns = solve(start);

	// get from solved to end
	std::vector<Turn> turnsToEnd = solve(end);
	for (size_t i = 0; i < turnsToEnd.size(); ++i) {
		allTurns.push_back(oppTurn(turnsToEnd[turnsToEnd.size() - i - 1]));
	}

	// Condense the result, so we don't have to go through solved
	Cube newCube;
	for (size_t i = 0; i < allTurns.size(); ++i) {
		newCube = Cube::turn(newCube, oppTurn(allTurns[allTurns.size() - i - 1]));
	}

	return solve(newCube);

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
