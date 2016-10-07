#include "cubeSolver.h"

#include <unordered_map>
#include <iostream>
#include <fstream>
#include <boost/serialization/unordered_map.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <queue>

using namespace CubeSolver;

using CommProtocol::MoveInstruction;
using std::cout;
using std::endl;

const std::string END_TABLES_PATH = "ser/end_maps.ser";

std::unordered_map<CubeNumsStep1,
	MoveInstruction,
	CubeNumsStep1::Hash> MAKE_STEP1MAP;
	
std::unordered_map<CubeNumsStep2,
	MoveInstruction,
	CubeNumsStep2::Hash> MAKE_STEP2MAP;

void CubeSolver::readEndMaps(std::string pathToFile, EndMap1*& endMap1, EndMap2*& endMap2)
{
	std::ifstream is(pathToFile, std::ios::binary);
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

void CubeSolver::buildEndMaps(std::string pathToFile, size_t map1Size, size_t map2Size)
{
	readTurnTables();

	// TODO: assert turn tables properly loaded

	cout << "building map 1";
	buildMap1(map1Size);
	cout << endl;

	cout << "building map 2";
	buildMap2(map2Size);
	cout << endl;

	cout << "archiving maps (DO NOT CLOSE)" << endl;
	archiveEndMaps(pathToFile);
}

void CubeSolver::archiveEndMaps(std::string pathToFile)
{
	// store the tables in turnTables.ser
	std::ofstream os(pathToFile, std::ios::binary);
	boost::archive::binary_oarchive oarch(os);

	oarch << MAKE_STEP1MAP << MAKE_STEP2MAP;
	
	os.close();
}

std::queue<CubeNumsStep1> CubeSolver::buildMap1(size_t mapSize)
{
	// map_size must be greater than OK_TURNS size to work
	//assert(mapSize > NUM_TURNS_STEP1);

	// ensure tables have been initialized
	//assert(Cube::EDGE_COLORS_TABLE2[3000][3] == 13466);
	
	Cube solvedCube;
	CubeNumsStep1 solvedCubeNums;

	MAKE_STEP1MAP[solvedCubeNums] = MoveInstruction::FRONT;

	std::queue<CubeNumsStep1> cubeQueue;
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
				MAKE_STEP1MAP[turnedCube] = oppTurn(TURNS_STEP1[j]);

				// To show the map loading...
				if (MAKE_STEP1MAP.size() % (mapSize / 10) == 0) {
					cout << ".";
				}
			}
		}
	}

	return cubeQueue;
}

void CubeSolver::buildMap2(size_t mapSize)
{
	// ensure tables have been initialized
	//assert(EDGE_COLORS_TABLE2[3000][3] == 13466);

	//size_t MAP2SIZE = 5000;	// MUST BE GREATER THAN TURNS_STEP2!!!

	Cube solvedCube;
	CubeNumsStep2 solvedCubeNums;
	MAKE_STEP2MAP[solvedCubeNums] = MoveInstruction::FRONT;

	std::queue<CubeNumsStep2> cubeQueue;
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
				MAKE_STEP2MAP[turnedCube] = oppTurn(TURNS_STEP2[j]);
				
				// To show the map loading...
				if (MAKE_STEP2MAP.size() % (mapSize / 10) == 0) {
					cout << ".";
				}
			}
		}
	}
}


