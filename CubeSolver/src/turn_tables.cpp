#include "cubeSolver.h"
#include "assert.h"
#include <iostream>
#include <boost/archive/binary_oarchive.hpp>
#include <fstream>

using namespace std;

int MAKE_EDGE_ORIENTS_TABLE[NUM_EDGE_ORIENTS][NUM_TURNS_STEP1];
int MAKE_CORNER_ORIENTS_TABLE[NUM_CORNER_ORIENTS][NUM_TURNS_STEP1];
int MAKE_CORNER_COLORS_TABLE[NUM_CORNER_COLORS][NUM_TURNS_STEP2];
int MAKE_EDGE_ORBITS_TABLE[NUM_EDGE_ORBITS][NUM_TURNS_STEP1];
int MAKE_EDGE_COLORS_TABLE1[NUM_EDGE_COLORS1][NUM_TURNS_STEP2];
int MAKE_EDGE_COLORS_TABLE2[NUM_EDGE_COLORS2][NUM_TURNS_STEP2];

const string TURN_TABLES_PATH = "ser/turn_tables.ser";

void buildTurnTables()
{
	// fill all the static turn tables
	cout << "building EDGE_ORIENTS_TABLE (1/6)" << endl;
	buildEdgeOrientsTable();

	cout << "building CORNER_ORIENTS_TABLE (2/6)" << endl;
	buildCornerOrientsTable();
	
	cout << "building EDGE_ORBITS_TABLE (3/6)" << endl;
	buildEdgeOrbitsTable();

	cout << "building CORNER_COLORS_TABLE (4/6)" << endl;
	buildCornerColorsTable();

	cout << "building EDGE_COLORS1_TABLE (5/6)" << endl;
	buildEdgeColorsTable1();

	cout << "building EDGE_COLORS2_TABLE (6/6)" << endl;
	buildEdgeColorsTable2();

	cout << "archiving tables (DO NOT CLOSE)" << endl;
	archiveTurnTables();

}

void archiveTurnTables()
{
	// store the tables in turnTables.ser
	ofstream os(TURN_TABLES_PATH, ios::binary);
	boost::archive::binary_oarchive oarch(os);

	// tables for step 1
	oarch << MAKE_EDGE_ORIENTS_TABLE << MAKE_CORNER_ORIENTS_TABLE << MAKE_EDGE_ORBITS_TABLE;

	// tables for step 2
	oarch << MAKE_CORNER_COLORS_TABLE << MAKE_EDGE_COLORS_TABLE1 << MAKE_EDGE_COLORS_TABLE2;
	os.close();
}

void buildEdgeOrientsTable()
{
	// default initialize an orientation with eleven 0's
	Cube cube;
	do {
		int currCode = cube.edgeOrientsCode();

		// iterate through all the turns in step1, add results to table
		for (size_t j = 0; j < NUM_TURNS_STEP1; ++j) {
			Turn currTurn = OK_TURNS1[j];
			Cube newCube = cube.turnWith(currTurn);

			int newCode = newCube.edgeOrientsCode();
			assert(newCode < NUM_EDGE_ORIENTS);
			MAKE_EDGE_ORIENTS_TABLE[currCode][j] = newCode;
		}
	} while (cube.nextEdgeOrients());

	// debugging to make sure it iterated through edges correctly
	//for (int i = 0; i < Cube::NUM_EDGES; ++i) {
	//	assert(cube.edgeOrients_[i] == 0);
	//}
}

void buildCornerOrientsTable()
{
	// default initialize a cube with cornerOrients_ as 0
	Cube cube;

	do {
		int currCode = cube.cornerOrientsCode();

		for (size_t j = 0; j < NUM_TURNS_STEP1; ++j) {
			Turn currTurn = OK_TURNS1[j];
			Cube newCube = cube.turnWith(currTurn);

			int newCode = newCube.cornerOrientsCode();
			assert(newCode < NUM_CORNER_ORIENTS);
			MAKE_CORNER_ORIENTS_TABLE[currCode][j] = newCube.cornerOrientsCode();
		}
	} while (cube.nextCornerOrients());

	// debugging to make sure it iterated through edges correctly
	//for (int i = 0; i < NUM_CORNERS; ++i) {
	//	assert(cube.cornerOrients_[i] == 0);
	//}
}

void buildEdgeOrbitsTable()
{
	Cube cube;
/*
	// set edge colors to first edge colors (have code 0)
	// represents a cube with LR slice edges at YB locations
	// and other edges in the YO spaces
	Edge_t newEdgeColors[Cube::NUM_EDGES] = { YB,YB,YB,YB,YO,YO,YO,YO,YO,YO,YO,YO };
	for (size_t i = 0; i < Cube::NUM_EDGES; ++i) {
		cube.edgeColors_[i] = newEdgeColors[i];
	}
*/
	cube.setFirstEdgeOrbits();

	do {
		int currCode = cube.edgeOrbitsCode();

		for (size_t j = 0; j < NUM_TURNS_STEP1; ++j) {
			Turn currTurn = OK_TURNS1[j];
			Cube newCube = cube.turnWith(currTurn);

			MAKE_EDGE_ORBITS_TABLE[currCode][j] = newCube.edgeOrbitsCode();
		}
	} while (cube.nextEdgeOrbits());

	// debugging to make sure it iterated through edges correctly
	//for (int i = 0; i < Cube::NUM_EDGES; ++i) {
	//	assert(cube.edgeColors_[i] == newEdgeColors[i]);
	//}
}

void buildCornerColorsTable()
{
	Cube cube;
	do {
		int currCode = cube.cornerColorsCode();

		for (size_t j = 0; j < NUM_TURNS_STEP2; ++j) {
			Turn currTurn = OK_TURNS2[j];
			Cube newCube = cube.turnWith(currTurn);

			MAKE_CORNER_COLORS_TABLE[currCode][j] = newCube.cornerColorsCode();
		}
	} while (cube.nextCornerColors());

}



void buildEdgeColorsTable1()
{
	Cube cube;
	//const int ORBIT_SIZE = 4;

	// Make lrSliceColors equal to LR_SLICE
	//Edge_t lrSliceColors[ORBIT_SIZE] = { YB, YG, WB, WG };

	do {
		// set the edges in the LR_SLICE equal to the current permutation
		//for (int i = 0; i < ORBIT_SIZE; ++i) {
		//	cube.edgeColors_[LR_SLICE[i]] = lrSliceColors[i];
		//}

		int currCode = cube.edgeColorsCode1();

		for (size_t j = 0; j < NUM_TURNS_STEP2; ++j) {
			Turn currTurn = OK_TURNS2[j];
			Cube newCube = cube.turnWith(currTurn);

			MAKE_EDGE_COLORS_TABLE1[currCode][j] = newCube.edgeColorsCode1();
		}
	} while (cube.nextEdgeColors1());
}

void buildEdgeColorsTable2()
{
	Cube cube;

	//const int ORBIT_SIZE2 = 8;

	// Make lrSliceColors equal to LR_SLICE
	//Edge_t fb_ud_colors[ORBIT_SIZE2] = { YR, YO, RB, OB, RG, OG, WR, WO };
	//Edge_t currPermutation[ORBIT_SIZE2] = { YR, YO, RB, OB, RG, OG, WR, WO };
	
	do {
		// set the edges in the LR_SLICE equal to the current permutation
		//for (int i = 0; i < ORBIT_SIZE2; ++i) {
		//	cube.edgeColors_[fb_ud_colors[i]] = currPermutation[i];
		//}

		int currCode = cube.edgeColorsCode2();

		for (size_t j = 0; j < NUM_TURNS_STEP2; ++j) {
			Turn currTurn = OK_TURNS2[j];
			Cube newCube = cube.turnWith(currTurn);

			MAKE_EDGE_COLORS_TABLE2[currCode][j] = newCube.edgeColorsCode2();
		}
	} while (cube.nextEdgeColors2());
}

/*
bool nextEdgeOrients()
{
	for (int i = 0; i < Cube::NUM_EDGES; ++i) {
		
		// if you see a 1, change it to 0 and carry it over
		if (edgeOrients_[i] == 1) {
			edgeOrients_[i] = 0;
		}

		// If you see a 0, increment and return
		else {
			++edgeOrients_[i];
			return true;
		}
	}
	// the permutation was {1,1,1...}
	return false;
}

bool nextCornerOrients()
{
	for (size_t i = 0; i < NUM_CORNERS; ++i) {
		// if you reach a 2, change it to a 0 and carry over
		if (cornerOrients_[i] == 2) {
			cornerOrients_[i] = 0;
		}
		// if you reach a 0 or 1, increment it and return
		else {
			++cornerOrients_[i];
			return true;
		}
	}

	// the permutation was {2,2,2...}
	return false;
}
*/
