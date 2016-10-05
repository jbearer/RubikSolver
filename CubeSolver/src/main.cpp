#include "cubeSolver.h"

#include <iostream>

using std::cout;
using std::endl;

using namespace CubeSolver;
using namespace CommProtocol;

int main()
{
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
}
