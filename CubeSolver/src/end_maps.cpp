#include "cubeSolver.h"

#include <unordered_map>
#include <iostream>
#include <fstream>
#include <boost/serialization/unordered_map.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <queue>

using namespace CubeSolver;

using std::cout;
using std::endl;

const std::string END_TABLES_PATH = "ser/end_maps.ser";

void CubeSolver::readEndMaps(std::string pathToFile, std::unique_ptr<EndMap1>& endMap1, std::unique_ptr<EndMap2>& endMap2)
{
	std::ifstream is(pathToFile, std::ios::binary);
	boost::archive::binary_iarchive iarch(is);

	cout << "reading map 1" << endl;
	iarch >> *endMap1;
	cout << endMap1->size() << endl;

	cout << "reading map 2" << endl;
	iarch >> *endMap2;
	cout << endMap2->size() << endl;

	is.close();
}

void CubeSolver::buildEndMaps(std::string pathToFile, size_t map1Size, size_t map2Size)
{
	readTurnTables();

	cout << "building map 1";
	std::unique_ptr<EndMap1> e1 = std::move(std::unique_ptr<EndMap1>(new EndMap1));
	buildMap1(e1, map1Size);
	cout << endl;

	cout << "building map 2";
	std::unique_ptr<EndMap2> e2 = std::move(std::unique_ptr<EndMap2>(new EndMap2));
	buildMap2(e2, map2Size);
	cout << endl;

	cout << "archiving maps (DO NOT CLOSE)" << endl;
	archiveEndMaps(e1, e2, pathToFile);
}

void CubeSolver::archiveEndMaps(std::unique_ptr<EndMap1>& e1, std::unique_ptr<EndMap2>& e2, std::string pathToFile)
{
	// store the tables in turnTables.ser
	std::ofstream os(pathToFile, std::ios::binary);
	boost::archive::binary_oarchive oarch(os);

	oarch << *e1 << *e2;

	os.close();
}

void CubeSolver::buildMap1(std::unique_ptr<EndMap1>& end_map, size_t mapSize)
{
	// map_size must be greater than OK_TURNS size to work
	//assert(mapSize > NUM_TURNS_STEP1);

	// ensure tables have been initialized
	//assert(Cube::EDGE_COLORS_TABLE2[3000][3] == 13466);

	Cube solvedCube;
	CubeNumsStep1 solvedCubeNums;

	(*end_map)[solvedCubeNums] = F;

	std::queue<CubeNumsStep1> cubeQueue;
	cubeQueue.push(solvedCubeNums);

	while ((*end_map).size() < mapSize) {
		// Retrive the first cube nums and its corresponding moves
		CubeNumsStep1 cube = cubeQueue.front();

		cubeQueue.pop();

		// Loop through all the allowable turns in this step to use a breadth
		// first search to generate all possible cubes
		for (int j = 0; j < NUM_TURNS_STEP1; ++j) {
			CubeNumsStep1 turnedCube = CubeNumsStep1::turn(cube, j);

			// only add if this cube has never been seen before
			if ((*end_map).count(turnedCube) == 0) {

				cubeQueue.push(turnedCube);

				// Push back the inverse of the turn used to get there
				(*end_map)[turnedCube] = oppTurn(TURNS_STEP1[j]);

				// To show the map loading...
				if ((*end_map).size() % (mapSize / 10) == 0) {
					cout << ".";
				}
			}
		}
	}
}

void CubeSolver::buildMap2(std::unique_ptr<EndMap2>& end_map, size_t mapSize)
{
	// ensure tables have been initialized
	//assert(EDGE_COLORS_TABLE2[3000][3] == 13466);

	//size_t MAP2SIZE = 5000;	// MUST BE GREATER THAN TURNS_STEP2!!!

	Cube solvedCube;
	CubeNumsStep2 solvedCubeNums;
	(*end_map)[solvedCubeNums] = F;

	std::queue<CubeNumsStep2> cubeQueue;
	cubeQueue.push(solvedCubeNums);

	while ((*end_map).size() < mapSize) {
		// Retrive the first cube nums and its corresponding moves
		CubeNumsStep2 cube = cubeQueue.front();
		cubeQueue.pop();

		// Loop through all the allowable turns in this step to use a breadth
		// first search to generate all possible cubes
		for (int j = 0; j < NUM_TURNS_STEP2; ++j) {
			CubeNumsStep2 turnedCube = CubeNumsStep2::turn(cube, j);

			// only add if this cube has never been seen before
			if ((*end_map).count(turnedCube) == 0) {

				cubeQueue.push(turnedCube);

				// Add this list of moves to the hash tables
				(*end_map)[turnedCube] = oppTurn(TURNS_STEP2[j]);

				// To show the map loading...
				if ((*end_map).size() % (mapSize / 10) == 0) {
					cout << ".";
				}
			}
		}
	}
}


