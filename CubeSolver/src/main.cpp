#include "cubeSolver.h"

#include <iostream>
#include <vector>

using std::cout;
using std::endl;

using namespace CubeSolver;
using namespace CommProtocol;

void solve1()
{
	readTurnTables();

	EndMap1* endMap1_big;
	EndMap2* endMap2_big;

	readEndMaps("ser/endMap_big.ser", endMap1_big, endMap2_big);

	int edgeOrients[Cube::NUM_EDGES] = {0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0};
	int cornerOrients[Cube::NUM_CORNERS] = {1, 0, 2, 2, 1, 1, 0, 2};
	Cube::Edge_t edgeColors[Cube::NUM_EDGES] =
		{Cube::RB, Cube::RG, Cube::WR, Cube::YG,
		 Cube::WG, Cube::YR, Cube::WO, Cube::YO,
		 Cube::WB, Cube::OB, Cube::YB, Cube::OG};

	Cube::Corner_t cornerColors[Cube::NUM_CORNERS] =
		{Cube::WRG, Cube::WRB, Cube::YOB, Cube::WOG,
		Cube::YRB, Cube::WOB, Cube::YRG, Cube::YOG};

	Cube cube(edgeColors, edgeOrients, cornerColors, cornerOrients);
/*
	std::vector<Turn> maneuver =
	{
		FRONT,
		RIGHT,
		UP_INVERTED,
		LEFT,
		DOWN_INVERTED,
		BACK,
		DOWN_2,
		LEFT,
		RIGHT_2,
		UP,
		FRONT_2,
		BACK_2,
		RIGHT,
		FRONT_INVERTED,
		RIGHT_INVERTED,
		UP,
		LEFT_INVERTED,
		UP,
		FRONT_2,
		DOWN_2
	};

	cout << "scrambling maneuver: ";
	for (auto t: maneuver) {
		cout << t << " ";
		cube = Cube::turn(cube, t);
	}
	cout << endl;
*/
	// time each individual solve
	clock_t cubeTime;
	cubeTime = clock();

	std::vector<Turn> turns = solve(cube, endMap1_big, endMap2_big);

	for (auto t : turns)
		cout << t << " ";
	cout << endl;

	cubeTime = clock() - cubeTime;

	cout << "solve took " << (float) cubeTime / CLOCKS_PER_SEC;
	cout << " seconds" << endl;

	delete endMap1_big;
	delete endMap2_big;
}


/** Find how many cycles it takes to make DRFBLU*/
size_t cycleLength()
{
	// create a solved cube
	Cube cube, solvedCube;

	std::vector<Turn> turns =
	{
		RIGHT_INVERTED,
		DOWN_INVERTED,
		RIGHT,
		DOWN
	};

	size_t num_cycles = 0;

	do {
		for (auto inputTurn : turns) {
			cube = Cube::turn(cube, inputTurn);
		}
		++num_cycles;
	} while (cube != solvedCube);

	return num_cycles;
}

int main()
{
	solve1();
	//cycleLength();
	//std::cout << cycleLength() << std::endl;
	/*
	readTurnTables();
	EndMap1* endMap1;
	EndMap2* endMap2;

	readEndMaps("ser/endMap_big.ser", endMap1, endMap2);

	std::vector<Move> allTurns({
		FRONT, RIGHT, BACK, LEFT, UP, DOWN,
		FRONT_INVERTED, RIGHT_INVERTED, BACK_INVERTED,
		LEFT_INVERTED, UP_INVERTED, DOWN_INVERTED
	});

	cout << "a" << endl;

	int NUM_TRIALS = 100;
	int MANEUVER_SIZE = 10;

	clock_t t;
	t = clock();

	float maxTime = 0;

	cout << "b" << endl;

	for (int i = 0; i < NUM_TRIALS; ++i) {

		Cube cube;

		cout << "scramble: ";

		for (int j = 0; j < MANEUVER_SIZE; ++j) {
			std::vector<Move>::iterator randIt = allTurns.begin();
			advance(randIt, rand() % allTurns.size());
			cube = Cube::turn(cube, *randIt);
			cout << *randIt << " ";
		}
		cout << endl;

		cout << "c" << i << endl;

		// time each individual solve
		clock_t cubeTime;
		cubeTime = clock();

		cout << "d" << i << endl;

		solve(cube, endMap1, endMap2);

		cout << "e" << i << endl;

		cubeTime = clock() - cubeTime;
		if (float(cubeTime) / CLOCKS_PER_SEC > maxTime) {
			maxTime = float(cubeTime) / CLOCKS_PER_SEC;
		}

		cout << "f" << i << endl;
	}

	cout << "g" << endl;
	t = clock() - t;

	cout << "max time: " << maxTime << endl;
	cout << "average time: " << ((float)t / CLOCKS_PER_SEC) / NUM_TRIALS << endl;

	delete endMap1;
	delete endMap2;
	*/
}
