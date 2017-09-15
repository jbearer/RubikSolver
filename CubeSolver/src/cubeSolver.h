#pragma once

/**
* @brief	Contains methods to solve a Rubiks Cube
*
* @details	Solves the cube in two steps.  First, does a iterative deepening DFS
*			to "orient" the edges and corners.  Then uses a subset of the possible moves
*			which preserve the orientations to place the pieces in the proper places,
*			another iterative deepening DFS.
*/

#include <vector>
#include <unordered_map>
#include <queue>
#include <memory>

#include "cube.h"
#include "cube_nums.h"
#include "cubeEncoder.h"
#include "turn.h"
#include "turn_tables.h"
#include "translate.h"

namespace CubeSolver {

typedef std::unordered_map<CubeNumsStep1, Turn, CubeNumsStep1::Hash> EndMap1;
typedef std::unordered_map<CubeNumsStep2, Turn, CubeNumsStep2::Hash> EndMap2;

/* reads end maps and turn tables, then solves the cube */
class Solver {
public:
	// Constructors
	// Default constructor has no end maps
	Solver();
	// Read in end maps from a file
	Solver(std::string file);
	// Construct end maps of a specified size
	Solver(size_t map1, size_t map2);

	std::vector<Turn> solve(Cube& cube);

	std::vector<Turn> solveToCube(Cube& start, Cube end);

private:
	std::deque<Turn> solveStep1DFS(Cube cube);
	bool solveStep1Helper(int depth, const CubeNumsStep1& curr, std::deque<Turn>& result);

	std::deque<Turn> solveStep2DFS(Cube cube);
	bool solveStep2Helper(int depth, const CubeNumsStep2& curr, std::deque<Turn>& result);

	std::unique_ptr<EndMap1> e1_;
	std::unique_ptr<EndMap2> e2_;

};

std::deque<Turn> turnsFromEndMap1(CubeNumsStep1 start, std::unique_ptr<EndMap1>& endMap1);
std::deque<Turn> turnsFromEndMap2(CubeNumsStep2 start, std::unique_ptr<EndMap2>& endMap2);

int getIndex1(Turn m);
int getIndex2(Turn m);

void readEndMaps(std::string pathToFile, std::unique_ptr<EndMap1>& endMap1, std::unique_ptr<EndMap2>& endMap2);

////////////////////////////////
//// End Maps //////////////////
////////////////////////////////

void buildEndMaps( std::string pathToFile, size_t map1Size = 5000, size_t map2Size = 5000);
void buildMap1(std::unique_ptr<EndMap1>& map, size_t mapSize);
void buildMap2(std::unique_ptr<EndMap2>& map, size_t mapSize);

void archiveEndMaps(std::unique_ptr<EndMap1>& e1, std::unique_ptr<EndMap2>& e2, std::string pathToFile);


} // end namespace CubeSolver


