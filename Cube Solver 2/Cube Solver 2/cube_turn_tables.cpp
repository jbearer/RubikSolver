#include "cube.h"
#include "assert.h"
#include <iostream>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <fstream>

using namespace std;

int Cube::EDGE_ORIENTS_TABLE[Cube::NUM_EDGE_ORIENTS][Cube::NUM_TURNS_STEP1];
int Cube::CORNER_ORIENTS_TABLE[Cube::NUM_CORNER_ORIENTS][Cube::NUM_TURNS_STEP1];
int Cube::CORNER_COLORS_TABLE[Cube::NUM_CORNER_COLORS][Cube::NUM_TURNS_STEP2];
int Cube::EDGE_ORBITS_TABLE[Cube::NUM_EDGE_ORBITS][Cube::NUM_TURNS_STEP1];
int Cube::EDGE_COLORS_TABLE1[Cube::NUM_EDGE_COLORS1][Cube::NUM_TURNS_STEP2];
int Cube::EDGE_COLORS_TABLE2[Cube::NUM_EDGE_COLORS2][Cube::NUM_TURNS_STEP2];

void Cube::readTurnTables()
{
	ifstream is("turnTables.ser",ios::binary);
	boost::archive::binary_iarchive iarch(is);

	// turn tables for step 1
	iarch >> EDGE_ORIENTS_TABLE >> CORNER_ORIENTS_TABLE >> EDGE_ORBITS_TABLE;
	
	// turn tables for step 2
	iarch >> CORNER_COLORS_TABLE >> EDGE_COLORS_TABLE1 >> EDGE_COLORS_TABLE2;
	is.close();

}

void Cube::buildTurnTables()
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

void Cube::archiveTurnTables()
{
	// store the tables in turnTables.ser
	ofstream os("turnTables.ser", ios::binary);
	boost::archive::binary_oarchive oarch(os);

	// tables for step 1
	oarch << EDGE_ORIENTS_TABLE << CORNER_ORIENTS_TABLE << EDGE_ORBITS_TABLE;

	// tables for step 2
	oarch << CORNER_COLORS_TABLE << EDGE_COLORS_TABLE1 << EDGE_COLORS_TABLE2;
	os.close();
}

void Cube::buildEdgeOrientsTable()
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
			EDGE_ORIENTS_TABLE[currCode][j] = newCode;
		}
	} while (cube.nextEdgeOrients());

	// debugging to make sure it iterated through edges correctly
	for (int i = 0; i < NUM_EDGES; ++i) {
		assert(cube.edgeOrients_[i] == 0);
	}
}

void Cube::buildCornerOrientsTable()
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
			CORNER_ORIENTS_TABLE[currCode][j] = newCube.cornerOrientsCode();
		}
	} while (cube.nextCornerOrients());

	// debugging to make sure it iterated through edges correctly
	for (int i = 0; i < NUM_CORNERS; ++i) {
		assert(cube.cornerOrients_[i] == 0);
	}
}

void Cube::buildEdgeOrbitsTable()
{
	Cube cube;

	// set edge colors to first edge colors (have code 0)
	// represents a cube with LR slice edges at YB locations
	// and other edges in the YO spaces
	Edge_t newEdgeColors[NUM_EDGES] = { YB,YB,YB,YB,YO,YO,YO,YO,YO,YO,YO,YO };
	for (size_t i = 0; i < NUM_EDGES; ++i) {
		cube.edgeColors_[i] = newEdgeColors[i];
	}

	do {
		int currCode = cube.edgeOrbitsCode();

		for (size_t j = 0; j < NUM_TURNS_STEP1; ++j) {
			Turn currTurn = OK_TURNS1[j];
			Cube newCube = cube.turnWith(currTurn);

			EDGE_ORBITS_TABLE[currCode][j] = newCube.edgeOrbitsCode();
		}
	} while (next_permutation(cube.edgeColors_, cube.edgeColors_ + NUM_EDGES));

	// debugging to make sure it iterated through edges correctly
	for (int i = 0; i < NUM_EDGES; ++i) {
		assert(cube.edgeColors_[i] == newEdgeColors[i]);
	}
}

void Cube::buildCornerColorsTable()
{
	Cube cube;
	do {
		int currCode = cube.cornerColorsCode();

		for (size_t j = 0; j < NUM_TURNS_STEP2; ++j) {
			Turn currTurn = OK_TURNS2[j];
			Cube newCube = cube.turnWith(currTurn);

			CORNER_COLORS_TABLE[currCode][j] = newCube.cornerColorsCode();
		}
	} while (next_permutation(cube.cornerColors_, cube.cornerColors_ + NUM_CORNERS));

}



void Cube::buildEdgeColorsTable1()
{
	Cube cube;
	const int ORBIT_SIZE = 4;

	// Make lrSliceColors equal to LR_SLICE
	Edge_t lrSliceColors[ORBIT_SIZE] = { YB, YG, WB, WG };

	do {
		// set the edges in the LR_SLICE equal to the current permutation
		for (int i = 0; i < ORBIT_SIZE; ++i) {
			cube.edgeColors_[LR_SLICE[i]] = lrSliceColors[i];
		}

		int currCode = cube.edgeColorsCode1();

		for (size_t j = 0; j < NUM_TURNS_STEP2; ++j) {
			Turn currTurn = OK_TURNS2[j];
			Cube newCube = cube.turnWith(currTurn);

			EDGE_COLORS_TABLE1[currCode][j] = newCube.edgeColorsCode1();
		}
	} while (next_permutation(lrSliceColors, lrSliceColors + 4));
}

void Cube::buildEdgeColorsTable2()
{
	Cube cube;

	const int ORBIT_SIZE2 = 8;

	// Make lrSliceColors equal to LR_SLICE
	Edge_t fb_ud_colors[ORBIT_SIZE2] = { YR, YO, RB, OB, RG, OG, WR, WO };
	Edge_t currPermutation[ORBIT_SIZE2] = { YR, YO, RB, OB, RG, OG, WR, WO };
	
	do {
		// set the edges in the LR_SLICE equal to the current permutation
		for (int i = 0; i < ORBIT_SIZE2; ++i) {
			cube.edgeColors_[fb_ud_colors[i]] = currPermutation[i];
		}

		int currCode = cube.edgeColorsCode2();

		for (size_t j = 0; j < NUM_TURNS_STEP2; ++j) {
			Turn currTurn = OK_TURNS2[j];
			Cube newCube = cube.turnWith(currTurn);

			EDGE_COLORS_TABLE2[currCode][j] = newCube.edgeColorsCode2();
		}
	} while (next_permutation(currPermutation, currPermutation + 8));
}


bool Cube::nextEdgeOrients()
{
	for (int i = 0; i < NUM_EDGES; ++i) {
		
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

bool Cube::nextCornerOrients()
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



