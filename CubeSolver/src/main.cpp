#include "cubeSolver.h"

#include <iostream>
#include <algorithm>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>

using std::cout;
using std::endl;

using namespace CubeSolver;
using namespace CommProtocol;
using namespace boost::accumulators;

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

void translate_test()
{
	std::vector<Color> colors{Red, Orange, Yellow, Green, Blue, White};
	size_t good = 0, bad = 0;

	Solver solver("ser/endMap_big.ser");

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

			solver.solve(cube);
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

void solveToCubeTest()
{
	Cube start;
	start = Cube::turn(start, FRONT_INVERTED);

	Cube end;
	end = Cube::turn(end, RIGHT);
	end = Cube::turn(end, UP);

	Solver solver("ser/endMap_big.ser");

	cout << "about to solve" << endl;
	std::vector<Turn> turns = solver.solveToCube(start, end);
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

void allCycles()
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

	Solver solver("ser/endMap_big.ser");

	for (auto c : cubes) {
		std::vector<Turn> result = solver.solveToCube(start, c);
		for (auto t : result)
			cout << t << " ";
		cout << endl;
		start = c;
	}
}

std::vector<Turn> allTurns({
		FRONT, RIGHT, BACK, LEFT, UP, DOWN,
		FRONT_INVERTED, RIGHT_INVERTED, BACK_INVERTED,
		LEFT_INVERTED, UP_INVERTED, DOWN_INVERTED
	});

int MAP_SIZE_SMALL = 5000;
int MAP_SIZE_BIG = 10000000;

Turn randomTurn1() {
	int i = rand() % NUM_TURNS_STEP1;
	return TURNS_STEP1[i];
}

Turn randomTurn2() {
	int i = rand() % NUM_TURNS_STEP2;
	return TURNS_STEP2[i];
}

void calculateStats(std::vector<double> vec, double& max, double& avg, double& stdev)
{
	accumulator_set<double, stats<tag::variance>> acc;
  	for_each(vec.begin(), vec.end(), std::ref(acc));

  	max = *std::max_element(vec.begin(), vec.end());
  	avg = mean(acc);
  	stdev = std::sqrt(variance(acc));
}

int main()
{
	//buildTurnTables();
	//buildEndMaps("ser/endMap_big", 1e7/20, 1e7*2);
	//std::cout << cycleLength() << std::endl;

	//translate_test(e1, e2);
	//allCycles();

	Solver solver;
	Cube c;
	c = Cube::turn(c, FRONT);
	c = Cube::turn(c, RIGHT);

	std::vector<Turn> result = solver.solve(c);
	for (auto t : result) {
		cout << t << " ";
	} cout << endl;



	{
	readTurnTables();

	int NUM_TRIALS = 500;
	int MANEUVER_SIZE = 300;

	std::vector<double> timeVec;
	std::vector<double> numTurnsVec;

	Solver solver("ser/endMap_big");

	for (int i = 0; i < NUM_TRIALS; ++i) {

		Cube cube;

		for (int j = 0; j < MANEUVER_SIZE; ++j) {
			cube = Cube::turn(cube, randomTurn1());
		}

		// time each individual solve
		clock_t cubeTime;
		cubeTime = clock();

		std::vector<Turn> turns = solver.solve(cube);
		for (auto t : turns)
			cout << t << " ";
		cout << endl;

		cubeTime = clock() - cubeTime;

		timeVec.push_back((float) cubeTime / CLOCKS_PER_SEC);
		numTurnsVec.push_back((float) turns.size());

		//EXPECT_TRUE(cube.isSolved());

	}

	cout << endl;

	double max = 0, mean = 0, stdev = 0;

	// Calculate stats for how long the solve takes
	calculateStats(timeVec, max, mean, stdev);

	cout << "max time: " << max << endl;
	cout << "average time: " << mean << endl;
	cout << "stdev time: " << stdev << endl;

	// Calculate stats for the number of turns
	calculateStats(numTurnsVec, max, mean, stdev);

	cout << "max turns: " << max << endl;
	cout << "average turns: " << mean << endl;
	cout << "stdev turns: " << stdev << endl;

	}

}
