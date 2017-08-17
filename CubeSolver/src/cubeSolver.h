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

#include "comm-protocol.h"
#include "cube.h"
#include "cubeEncoder.h"
#include "turn.h"
#include "translate.h"

namespace CubeSolver {


/// Useful constants for generating Turn Tables
const int NUM_EDGE_ORIENTS = 4096; // 2^12
const int NUM_CORNER_ORIENTS = 6561; // 3^8
const int NUM_CORNER_COLORS = 40320; // 8!
const int NUM_EDGE_ORBITS = 495; // 12 nCr 4
const int NUM_EDGE_COLORS1 = 24; // 4!
const int NUM_EDGE_COLORS2 = 40320; // 8!

///////////////////////////////////
//// Cube nums/////////////////////
///////////////////////////////////

/**
 * @brief      Store a cube as a set of three shorts: edgeOrients,
 * 			   cornerOrients, and edgeOrbits.  These three numbers do not
 * 			   Uniquely identify a cube, but are sufficient to distinguish all
 * 			   cubes during step one of the algorithm.  They are used to compute
 * 			   turns more quickly (the results can be easily serialized) and
 * 			   because they are smaller in memory.
 */
struct CubeNumsStep1 {
	struct Hash;

	/// Constructors: Default initialize to a solved cube.
	CubeNumsStep1();
	CubeNumsStep1(const Cube& cube);
	CubeNumsStep1(ushort edgeOrients, ushort cornerOrients, ushort edgeOrbits);

	bool operator==(const CubeNumsStep1& rhs) const;
	bool operator!=(const CubeNumsStep1& rhs) const;

	void print();

	struct Hash : public std::unary_function<CubeNumsStep1, size_t> {
		size_t operator()(const CubeNumsStep1& cube) const;
	};

	/// Serialize cubenums.  Unused attribute gets rid of compiler warnings
	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version __attribute__((unused)))
	{
		ar & edgeOrients_ & cornerOrients_ & edgeOrbits_;
	}

	static CubeNumsStep1 turn(const CubeNumsStep1& cube, int i);

private:

	ushort edgeOrients_;
	ushort cornerOrients_;
	ushort edgeOrbits_;
};

/**
 * @brief      Like CubeNumsStep1, store the cube as a set of three integers:
 * 			   cornerColors, edgeColors1, and edgeColors2.  Uniquely determine a
 * 			   cube for step 2.
 */
struct CubeNumsStep2{
	struct Hash;

	/// Default construct to a solved cube.
	CubeNumsStep2();
	CubeNumsStep2(const Cube& cube);
	CubeNumsStep2(ushort cornerColors, ushort edgeColors1, ushort edgeColors2);

	bool operator==(const CubeNumsStep2& rhs) const;
	bool operator!=(const CubeNumsStep2& rhs) const;


	struct Hash : public std::unary_function<CubeNumsStep2, size_t> {
		size_t operator()(const CubeNumsStep2& cube) const;
	};

	/// serialize cubenums
	template<typename Archive>
	void serialize(Archive& ar, const uint version __attribute__((unused)))
	{
		ar & cornerColors_ & edgeColors1_ & edgeColors2_;
	}

	static CubeNumsStep2 turn(const CubeNumsStep2& cube, int i);

private:

	ushort cornerColors_;
	ushort edgeColors1_;
	ushort edgeColors2_;

};

typedef std::unordered_map<CubeNumsStep1, Turn, CubeNumsStep1::Hash> EndMap1;
typedef std::unordered_map<CubeNumsStep2, Turn, CubeNumsStep2::Hash> EndMap2;


std::vector<Turn> solve(Cube& cube, EndMap1* endMap1, EndMap2* endMap2);

std::vector<Turn> solveToCube(Cube& start, Cube end, EndMap1* endMap1, EndMap2* endMap2);


/// Hash maps from cube codes to the required turns to solve
static std::unordered_map<CubeNumsStep1, Turn, CubeNumsStep1::Hash> STEP1MAP;
static std::unordered_map<CubeNumsStep2, Turn, CubeNumsStep2::Hash> STEP2MAP;


void readEndMaps(std::string pathToFile, EndMap1*& endMap1, EndMap2*& endMap2);


std::deque<Turn> solveStep1DFS(Cube cube, EndMap1* endMap1);
bool solveStep1Helper(int depth, const CubeNumsStep1& curr, EndMap1* endMap1, std::deque<Turn>& result);

std::deque<Turn> solveStep2DFS(Cube cube, EndMap2* endMap2);
bool solveStep2Helper(int depth, const CubeNumsStep2& curr, EndMap2* endMap2, std::deque<Turn>& result);

std::deque<Turn> turnsFromEndMap1(CubeNumsStep1 start, EndMap1* endMap1);
std::deque<Turn> turnsFromEndMap2(CubeNumsStep2 start, EndMap2* endMap2);


int getIndex1(Turn m);
int getIndex2(Turn m);

/// Turn tables
namespace TurnTables{
	extern int EDGE_ORIENTS_TABLE[NUM_EDGE_ORIENTS][NUM_TURNS_STEP1];
	extern int CORNER_ORIENTS_TABLE[NUM_CORNER_ORIENTS][NUM_TURNS_STEP1];
	extern int CORNER_COLORS_TABLE[NUM_CORNER_COLORS][NUM_TURNS_STEP2];
	extern int EDGE_ORBITS_TABLE[NUM_EDGE_ORBITS][NUM_TURNS_STEP1];
	extern int EDGE_COLORS_TABLE1[NUM_EDGE_COLORS1][NUM_TURNS_STEP2];
	extern int EDGE_COLORS_TABLE2[NUM_EDGE_COLORS2][NUM_TURNS_STEP2];
}
/// Reads turn tables from turnTables.ser.  Stores them in proper arrays
void readTurnTables();


///////////////////////// TURN TABLES ///////////////////////////
/*
* Turn tables hold the results of applying each turn to a cube's state.
* They should increase increase the speed of the cube solver because the
* solver no longer has to construct a new cube and apply the turn.  Instead
* it has memoized the results of each turns, and can look it up in a table.
*/

/// Calls the build methods for each turn table, then calls archiveTurnTables
void buildTurnTables();

/// Stores the turn tables in "turnTables.ser"
void archiveTurnTables();

/// Reads turn tables from turnTables.ser.  Stores them in proper arrays
void readTurnTables();

/**
* \fn		buildEdgeOrientsTable
* \brief	Builds the turn table of edge orientations to speed up step 1
* Creates a cube, then iterates through all the possible edgeOrientations
* using the method nextEdgeOrients().  For each orientation, it loops
* through all the allowed turns in step 1.  For each turn, it generates
* a new cube, applies the turn, and calculates the new edge orientations code.
* It then stores that code in the proper place of the turn table.
*/
void buildEdgeOrientsTable();

/**
* \fn		buildCornerOrientsTable
* \brief	Builds the turn table of corner orientations
* \details	See buildEdgeOrients
*/
void buildCornerOrientsTable();

/**
* \fn		buildEdgeOrbitsTable
* \brief	Builds the turn table of edge orbits for step 1
* \details	Similar to buildEdgeOrients.  Creates a cube with edge colors
* that will have code 0.  Generates all possible 12 nCr 4 positions for the
* edges in the LR slice.  Uses the stl method next_permutation to change
* the colors and increment the cube code.  In this method, the colors
* used for calculating the codes are impossible, but represent the
* simplifications used when calculating the edgeOrbits code.
*/
void buildEdgeOrbitsTable();

/**
* \fn		buildCornersColorsTable
* \brief	Builds the turn table of corner colors for step 2
* \details	Generates all possible 8! positions of the corner colors.
*			Then, memoizes the outcome of each of the possible 12 turns.
*/
void buildCornerColorsTable();

/**
* \fn		buildEdgeColorsTable1
* \brief	Builds the turn table for the LR slice edge colors for step 2
* \details	In step2, the LR slice is invariant, so this step constructs
*			all the possibilities for the LR slice (4! = 24 possibilities)
*/
void buildEdgeColorsTable1();

/**
* \fn		buildEdgeColorsTable1
* \brief	Builds the turn table for the non-LR edge colors for step 2
* \details	In step2, the LR slice is invariant.  This this step constructs
*			all the possibilities for the NON-LR slice edges (8! total)
*/
void buildEdgeColorsTable2();




////////////////////////////////
//// End Maps //////////////////
////////////////////////////////

void buildEndMaps( std::string pathToFile, size_t map1Size = 5000, size_t map2Size = 5000);
std::queue<CubeNumsStep1> buildMap1(size_t mapSize);
void buildMap2(size_t mapSize);

void archiveEndMaps(std::string pathToFile);


} // end namespace CubeSolver


