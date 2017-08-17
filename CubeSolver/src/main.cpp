#include "cubeSolver.h"

#include <iostream>
#include <algorithm>

using std::cout;
using std::endl;

using namespace CubeSolver;
using namespace CommProtocol;

/** Find how many cycles it takes to make DRFBLU*/
size_t cycleLength()
{
	// create a solved cube
	Cube cube, solvedCube;

	std::vector<Turn> turns = {FRONT_INVERTED, RIGHT_INVERTED, DOWN_INVERTED};
	size_t num_cycles = 0;

	do {
		for (auto inputTurn : turns) {
			cube = Cube::turn(cube, inputTurn);
		}
		++num_cycles;
	} while (cube != solvedCube);

	return num_cycles;
}

void translate_test(EndMap1* e1, EndMap2* e2)
{
	std::vector<Color> colors{Red, Orange, Yellow, Green, Blue, White};
	size_t good = 0, bad = 0;

	do {
		EasyCube c(
			std::vector<Color>(8, colors[0]),
			std::vector<Color>(8, colors[1]),
			std::vector<Color>(8, colors[2]),
			std::vector<Color>(8, colors[3]),
			std::vector<Color>(8, colors[4]),
			std::vector<Color>(8, colors[5])
		);
		try {
			Cube cube = translate(c);

			solve(cube, e1, e2);
			for (auto c : colors)
				cout << c << " ";
			cout << endl;
			++good;
		}
		catch( const std::exception& e) {
			++bad;
		}
	}
	while (std::next_permutation(colors.begin(), colors.end()));

	cout << "num good " << good << endl;
	cout << "num bad " << bad << endl;

	// EasyCube c(
	// 	std::vector<Color>(8, Color::Blue),
	// 	std::vector<Color>(8, Color::Orange),
	// 	std::vector<Color>(8, Color::White),
	// 	std::vector<Color>(8, Color::Yellow),
	// 	std::vector<Color>(8, Color::Red),
	// 	std::vector<Color>(8, Color::Green)
	// );

	// Cube cube = translate(c);

	// std::vector<Turn> turns = solve(cube, e1, e2);
	// for (auto t : turns) {
	// 	cout << t << " ";
	// }
	// cout << endl;

}

void solveToCubeTest(EndMap1* e1, EndMap2* e2)
{
	Cube start;
	start = Cube::turn(start, FRONT_INVERTED);

	Cube end;
	end = Cube::turn(end, RIGHT);
	end = Cube::turn(end, UP);

	cout << "about to solve" << endl;
	std::vector<Turn> turns = solveToCube(start, end, e1, e2);
	for (auto t : turns) {
		cout << t << " ";
	} cout << endl;
}

Cube makeCube(std::vector<Color> colors) {
	EasyCube c(
		std::vector<Color>(8, colors[0]),
		std::vector<Color>(8, colors[1]),
		std::vector<Color>(8, colors[2]),
		std::vector<Color>(8, colors[3]),
		std::vector<Color>(8, colors[4]),
		std::vector<Color>(8, colors[5])
		);
	return translate(c);
}

void allCycles(EndMap1* e1, EndMap2* e2)
{
	/* Result: steps to go through a tour, such that every color has been on
	 * each facelet at least once
	 * L'  R'  D'  U'  B'  F'  F2  R  L  B2  U2  L'  L'  D2  F2  U2  R'  R'
	 * F2  B2  D2  B2  F2  U2  L'  L'  B2  F2  R'  R'
	 * L'  R'  B'  F'  D'  U'  R  L  F2  D2  F2  L  R  U2  F2  L  R  F2
	 * L'  R'  B'  F'  D'  U'  R  R  D2  F2  D2  R'  R'  D2  F2  L  R  F2
	 * F2  B2  D2  B2  F2  U2  L'  L'  B2  F2  R'  R'
	 * L'  R'  D'  U'  B'  F'  B2  R  L  F2  U2  L'  L'  U2  F2  D2  R'  R'
	 * L'  R'  D'  U'  B'  F'  F2  R  L  B2  U2  L'  L'  D2  F2  U2  R'  R'
	 * L'  R'  D'  U'  B'  F'  F2  R  L  F2  U2  R'  R'  U2  F2  U2  R'  R'
	 * L'  R'  D'  U'  B'  F'  F2  R  L  F2  U2  R'  R'  U2  F2  U2  R'  R'
	 *
	 */
	std::vector<Cube> cubes {
		makeCube(std::vector<Color>{Red, Yellow, Blue, Green, White, Orange}),
		makeCube(std::vector<Color>{Orange, Yellow, Green, Blue, White, Red}),
		makeCube(std::vector<Color>{Yellow, Green, Orange, Red, Blue, White}),
		makeCube(std::vector<Color>{Green, Red, White, Yellow, Orange, Blue}),
		makeCube(std::vector<Color>{Blue, Red, Yellow, White, Orange, Green}),
		makeCube(std::vector<Color>{White, Green, Red, Orange, Blue, Yellow}),
		makeCube(std::vector<Color>{Red, White, Green, Blue, Yellow, Orange}),
		makeCube(std::vector<Color>{Green, Orange, Yellow, White, Red, Blue}),
		Cube()
	};

	for (auto& c : cubes){
		cout << c << endl;
	}

	Cube start;

	for (auto c : cubes) {
		std::vector<Turn> result = solveToCube(start, c, e1, e2);
		for (auto t : result)
			cout << t << " ";
		cout << endl;
		start = c;
	}
}

int main()
{
	//std::cout << cycleLength() << std::endl;

	readTurnTables();
	EndMap1* e1;
	EndMap2* e2;

	readEndMaps("ser/endMap_big.ser", e1, e2);

	//translate_test(e1, e2);
	allCycles(e1, e2);


	/*
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
	*/
	delete e1;
	delete e2;

}
