#include "cube.h"

#include <assert.h>
#include <unordered_map>
#include <assert.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <boost/serialization/vector.hpp>	// needed for serializing maps
#include <boost/serialization/unordered_map.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <vector>
#include <tuple>

using namespace std;
using CommProtocol::MoveInstruction;

unordered_map<Cube::CubeNumsStep1,
	MoveInstruction,
	Cube::CubeNumsStep1::Hash> Cube::STEP1MAP;
	
unordered_map<Cube::CubeNumsStep2,
	MoveInstruction,
	Cube::CubeNumsStep2::Hash> Cube::STEP2MAP;

void Cube::readEndMaps()
{
	ifstream is("endTables.ser", ios::binary);
	boost::archive::binary_iarchive iarch(is);

	cout << "reading map 1" << endl;
	iarch >> STEP1MAP;

	cout << "reading map 2" << endl;
	iarch >> STEP2MAP;

	is.close();
}

void Cube::buildEndMaps()
{
	readTurnTables();

	// ensure choose table is loaded
	assert(CHOOSE[5][2] == 10);

	cout << "building map 1";
	buildMap1();
	cout << endl;

	cout << "building map 2";
	buildMap2();
	cout << endl;

	cout << "archiving maps (DO NOT CLOSE)" << endl;
	archiveEndMaps();
}

void Cube::archiveEndMaps()
{
	// store the tables in turnTables.ser
	ofstream os("endTables.ser", ios::binary);
	boost::archive::binary_oarchive oarch(os);
	oarch << STEP1MAP << STEP2MAP;
	os.close();
}

void Cube::buildMap1()
{
	// ensure tables have been initialized
	assert(EDGE_COLORS_TABLE2[3000][3] == 13466);

	int MAP1SIZE = 5000000; // MUST BE LARGER THAN OK_TURNS1.size()

	
	Cube solvedCube;
	CubeNumsStep1 solvedCubeNums;

	//MAKE THIS AN EMPTY MOVE INSTRUCTION
	STEP1MAP[solvedCubeNums] = MoveInstruction::FRONT;

	queue<CubeNumsStep1> cubeQueue;
	cubeQueue.push(solvedCubeNums);

	while (STEP1MAP.size() < MAP1SIZE) {
		// Retrive the first cube nums and its corresponding moves
		CubeNumsStep1 cube = cubeQueue.front();
		
		cubeQueue.pop();
		
		// Loop through all the allowable turns in this step to use a breadth
		// first search to generate all possible cubes
		for (int j = 0; j < NUM_TURNS_STEP1; ++j) {
			CubeNumsStep1 turnedCube = cube.turn(j);
			
			// only add if this cube has never been seen before
			if (STEP1MAP.count(turnedCube) == 0) {
				
				cubeQueue.push(turnedCube);

				// Push back the inverse of the turn used to get there
				STEP1MAP[turnedCube] = OK_TURNS1[j].oppTurn;

				// To show the map loading...
				if (STEP1MAP.size() % (MAP1SIZE / 10) == 0) {
					cout << ".";
				}
			}
		}
	}
}

void Cube::buildMap2()
{
	// ensure tables have been initialized
	assert(EDGE_COLORS_TABLE2[3000][3] == 13466);

	int MAP2SIZE = 5000000;	// MUST BE GREATER THAN OK_TURNS2!!!

	Cube solvedCube;
	CubeNumsStep2 solvedCubeNums;
	STEP2MAP[solvedCubeNums] = MoveInstruction::FRONT;

	queue<CubeNumsStep2> cubeQueue;
	cubeQueue.push(solvedCubeNums);

	while (STEP2MAP.size() < MAP2SIZE) {
		// Retrive the first cube nums and its corresponding moves
		CubeNumsStep2 cube = cubeQueue.front();
		cubeQueue.pop();
		
		// Loop through all the allowable turns in this step to use a breadth
		// first search to generate all possible cubes
		for (int j = 0; j < OK_TURNS2.size(); ++j) {
			CubeNumsStep2 turnedCube = cube.turn(j);
			
			// only add if this cube has never been seen before
			if (STEP2MAP.count(turnedCube) == 0) {
				
				cubeQueue.push(turnedCube);
				
				// Add this list of moves to the hash tables
				STEP2MAP[turnedCube] = OK_TURNS2[j].oppTurn;
				
				// To show the map loading...
				if (STEP2MAP.size() % (MAP2SIZE / 10) == 0) {
					cout << ".";
				}
			}
		}
	}
}

deque<Cube::Turn> Cube::turnsFromEndMap1(CubeNumsStep1 start)
{
	deque<Turn> path;
	CubeNumsStep1 currNums = start;

	Cube solvedCube;
	while (currNums != CubeNumsStep1()) {
		
		MoveInstruction mi = STEP1MAP[currNums];
		path.push_back(Turn(mi));

		int i = getIndex1(mi);
		currNums = currNums.turn(i);
	}

	return path;
}

deque<Cube::Turn> Cube::turnsFromEndMap2(CubeNumsStep2 start)
{
	deque<Turn> path;
	CubeNumsStep2 currNums = start;

	Cube solvedCube;
	while (currNums != CubeNumsStep2()) {

		MoveInstruction mi = STEP2MAP[currNums];
		path.push_back(Turn(mi));

		int i = getIndex2(mi);
		currNums = currNums.turn(i);
	}

	return path;
}


