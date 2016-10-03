#include "cubeSolver.h"

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
#include <queue>

using namespace std;
//using CommProtocol::MoveInstruction;
using CommProtocol::MoveInstruction::FRONT;

const string END_TABLES_PATH = "ser/end_maps.ser";

unordered_map<CubeNumsStep1,
	MoveInstruction,
	CubeNumsStep1::Hash> MAKE_STEP1MAP;
	
unordered_map<CubeNumsStep2,
	MoveInstruction,
	CubeNumsStep2::Hash> MAKE_STEP2MAP;

void buildEndMaps(string pathToFile, size_t map1Size, size_t map2Size)
{
	readTurnTables();

	// ensure choose table is loaded
	//assert(CHOOSE[5][2] == 10);

	cout << "building map 1";
	buildMap1(map1Size);
	cout << endl;

	cout << "building map 2";
	buildMap2(map2Size);
	cout << endl;

	cout << "archiving maps (DO NOT CLOSE)" << endl;
	archiveEndMaps(pathToFile);
}

void archiveEndMaps(string pathToFile)
{
	// store the tables in turnTables.ser
	ofstream os(pathToFile, ios::binary);
	boost::archive::binary_oarchive oarch(os);

	oarch << MAKE_STEP1MAP << MAKE_STEP2MAP;
	
	os.close();
}

queue<CubeNumsStep1> buildMap1(size_t mapSize)
{
	// map_size must be greater than OK_TURNS size to work
	//assert(mapSize > NUM_TURNS_STEP1);

	// ensure tables have been initialized
	//assert(Cube::EDGE_COLORS_TABLE2[3000][3] == 13466);
	
	Cube solvedCube;
	CubeNumsStep1 solvedCubeNums;

	MAKE_STEP1MAP[solvedCubeNums] = MoveInstruction::FRONT;

	queue<CubeNumsStep1> cubeQueue;
	cubeQueue.push(solvedCubeNums);

	while (MAKE_STEP1MAP.size() < mapSize) {
		// Retrive the first cube nums and its corresponding moves
		CubeNumsStep1 cube = cubeQueue.front();
		
		cubeQueue.pop();
		
		// Loop through all the allowable turns in this step to use a breadth
		// first search to generate all possible cubes
		for (int j = 0; j < NUM_TURNS_STEP1; ++j) {
			CubeNumsStep1 turnedCube = cube.turn(j);
			
			// only add if this cube has never been seen before
			if (MAKE_STEP1MAP.count(turnedCube) == 0) {
				
				cubeQueue.push(turnedCube);

				// Push back the inverse of the turn used to get there
				MAKE_STEP1MAP[turnedCube] = OK_TURNS1[j].oppTurn;

				// To show the map loading...
				if (MAKE_STEP1MAP.size() % (mapSize / 10) == 0) {
					cout << ".";
				}
			}
		}
	}

	return cubeQueue;
}

void buildMap2(size_t mapSize)
{
	// ensure tables have been initialized
	//assert(EDGE_COLORS_TABLE2[3000][3] == 13466);

	//size_t MAP2SIZE = 5000;	// MUST BE GREATER THAN OK_TURNS2!!!

	Cube solvedCube;
	CubeNumsStep2 solvedCubeNums;
	MAKE_STEP2MAP[solvedCubeNums] = MoveInstruction::FRONT;

	queue<CubeNumsStep2> cubeQueue;
	cubeQueue.push(solvedCubeNums);

	while (MAKE_STEP2MAP.size() < mapSize) {
		// Retrive the first cube nums and its corresponding moves
		CubeNumsStep2 cube = cubeQueue.front();
		cubeQueue.pop();
		
		// Loop through all the allowable turns in this step to use a breadth
		// first search to generate all possible cubes
		for (int j = 0; j < NUM_TURNS_STEP2; ++j) {
			CubeNumsStep2 turnedCube = cube.turn(j);
			
			// only add if this cube has never been seen before
			if (MAKE_STEP2MAP.count(turnedCube) == 0) {
				
				cubeQueue.push(turnedCube);
				
				// Add this list of moves to the hash tables
				MAKE_STEP2MAP[turnedCube] = OK_TURNS2[j].oppTurn;
				
				// To show the map loading...
				if (MAKE_STEP2MAP.size() % (mapSize / 10) == 0) {
					cout << ".";
				}
			}
		}
	}
}


