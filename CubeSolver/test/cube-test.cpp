/**
 * \file cube-test.cpp
 */
#include <gtest/gtest.h>
#include "cubeSolver.h"
#include <ctime>
#include <iostream>
#include <cstdlib>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>

using CubeSolver::Cube;
using std::cout;
using std::endl;

using namespace CubeSolver;
using namespace CommProtocol;
using namespace boost::accumulators;

const std::string ENDMAP_SMALL_PATH = "ser/endMap_small.ser";
const std::string ENDMAP_BIG_PATH = "ser/endMap_big.ser";
const std::string TURNTABLES_PATH = "ser/turnTables.ser";

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

Cube scramble(std::vector<Turn> turns)
{
	Cube cube;

	for (auto mi: turns)
		cube = Cube::turn(cube, mi);

	return cube;
}

TEST(Build, buildEndMaps)
{
	//buildEndMaps(ENDMAP_SMALL_PATH, MAP_SIZE_SMALL, MAP_SIZE_SMALL);
	buildEndMaps(ENDMAP_BIG_PATH, MAP_SIZE_BIG / 50, MAP_SIZE_BIG * 2);
}

TEST(Build, buildTurnTables)
{
	buildTurnTables();
}


/////////// cube tests ////////////////

class CubeTest : public ::testing::Test {
 protected:

  static void SetUpTestCase() {

  	readTurnTables();
  }

};

TEST_F(CubeTest, operator_equals)
{
	Cube cube1, cube2;

	ASSERT_TRUE(cube1 == cube2);

	cube1 = Cube::turn(cube1, RIGHT);
	ASSERT_FALSE(cube1 == cube2);

	cube2 = Cube::turn(cube2, RIGHT);
	ASSERT_TRUE(cube1 == cube2);

	cube1 = Cube::turn(cube1, FRONT_2);
	cube2 = Cube::turn(cube2, FRONT_INVERTED);
	cube2 = Cube::turn(cube2, FRONT_INVERTED);
	ASSERT_TRUE(cube1 == cube2);
}

TEST_F(CubeTest, turns)
{
	// Construct a solved cube
	Cube cube;
	size_t numCycles = 0;

	// should run R' D' R D exactly 6 times before solved
	do {
		cube = Cube::turn(cube, RIGHT_INVERTED);
		cube = Cube::turn(cube, DOWN_INVERTED);
		cube = Cube::turn(cube, RIGHT);
		cube = Cube::turn(cube, DOWN);
		++numCycles;
	} while (!cube.isSolved());

	EXPECT_EQ(numCycles, 6);

}

TEST_F(CubeTest, cube_nums)
{
	Cube cube;

	CubeNumsStep1 cubeNums1 = CubeNumsStep1(cube);
	for (int i = 0; i < NUM_TURNS_STEP1; ++i) {
		Cube newCube = Cube::turn(cube, TURNS_STEP1[i]);
		CubeNumsStep1 newCubeNums1 = CubeNumsStep1(newCube);

		EXPECT_TRUE(newCubeNums1 ==
			CubeNumsStep1::turn(cubeNums1, i));
	}

	CubeNumsStep2 cubeNums2 = CubeNumsStep2(cube);
	for (int i = 0; i < NUM_TURNS_STEP2; ++i) {
		Cube newCube = Cube::turn(cube, TURNS_STEP2[i]);
		CubeNumsStep2 newCubeNums2 = CubeNumsStep2(newCube);

		EXPECT_TRUE(newCubeNums2 ==
			CubeNumsStep2::turn(cubeNums2, i));

	}
}

// Compare a cube with cube_nums using the same sequence of turns
TEST_F(CubeTest, cube_nums_sequence1)
{
	size_t numTurns = 10000;
	Cube cube;
	CubeNumsStep1 cubeNums;

	for (size_t i = 0; i < numTurns; ++i) {
		Turn t = randomTurn1();
		cube = Cube::turn(cube, t);
		cubeNums = CubeNumsStep1::turn(cubeNums, getIndex1(t));
		ASSERT_EQ(CubeNumsStep1(cube), cubeNums)
			<< "failed turn: " << t << endl;
	}
}

TEST_F(CubeTest, cube_nums_sequence2)
{
	size_t numTurns = 10000;
	Cube cube;
	CubeNumsStep2 cubeNums;

	for (size_t i = 0; i < numTurns; ++i) {
		Turn t = randomTurn2();
		cube = Cube::turn(cube, t);
		cubeNums = CubeNumsStep2::turn(cubeNums, getIndex2(t));
		ASSERT_EQ(CubeNumsStep2(cube), cubeNums)
			<< "failed turn: " << t << endl;
	}
}


class CubeSolverTest : public ::testing::Test {
 protected:
  // Per-test-case set-up.
  // Called before the first test in this test case.
  // Can be omitted if not needed.
  static void SetUpTestCase() {
  	readTurnTables();
  	buildEndMaps(ENDMAP_SMALL_PATH, MAP_SIZE_SMALL, MAP_SIZE_SMALL);
    readEndMaps(ENDMAP_SMALL_PATH, endMap1, endMap2);
    }

  // Per-test-case tear-down.
  // Called after the last test in this test case.
  // Can be omitted if not needed.
  static void TearDownTestCase() {
    delete endMap1;
    delete endMap2;
  }

  // Some expensive resource shared by all tests.
  	static EndMap1* endMap1;
	static EndMap2* endMap2;

};

EndMap1* CubeSolverTest::endMap1 = NULL;
EndMap2* CubeSolverTest::endMap2 = NULL;

EndMap1* EndMap1_trivial()
{
	EndMap1* endMap1 = new EndMap1();

	// load with a dummy move instruction
	(*endMap1)[CubeNumsStep1()] = MoveInstruction::FRONT;
	return endMap1;
}

EndMap2* EndMap2_trivial()
{
	EndMap2* endMap2 = new EndMap2();

	// load with a dummy move instruction
	(*endMap2)[CubeNumsStep2()] = MoveInstruction::FRONT;
	return endMap2;
}

TEST_F(CubeSolverTest, endMap)
{
	std::queue<CubeNumsStep1> cubeQueue = buildMap1(MAP_SIZE_SMALL);

	int size = cubeQueue.size();

	for (int i = 0; i < size; ++i) {
		CubeNumsStep1 cubeNums = cubeQueue.front();
		turnsFromEndMap1(cubeNums, endMap1);
		cubeQueue.pop();
	}
}

TEST_F(CubeSolverTest, turnsFromEndMap_trivial)
{
	Cube cube;
	CubeNumsStep1 cubeNums(cube);
	turnsFromEndMap1(cubeNums, endMap1);
}

TEST_F(CubeSolverTest, trivial)
{
	Cube cube;
	solve(cube, EndMap1_trivial(), EndMap2_trivial());
	ASSERT_TRUE(cube.isSolved());
}

TEST_F(CubeSolverTest, find_path_short)
{
	Cube cube;
	cube = Cube::turn(cube, FRONT);
	cube = Cube::turn(cube, RIGHT);

	solve(cube, EndMap1_trivial(), EndMap2_trivial());
	ASSERT_TRUE(cube.isSolved());
}

TEST_F(CubeSolverTest, find_path_long)
{

	Cube cube;
	cube = Cube::turn(cube, FRONT);
	cube = Cube::turn(cube, RIGHT);
	cube = Cube::turn(cube, BACK);
	cube = Cube::turn(cube, UP_INVERTED);
	cube = Cube::turn(cube, RIGHT_2);

	solve(cube, EndMap1_trivial(), EndMap2_trivial());
	ASSERT_TRUE(cube.isSolved());
}

TEST_F(CubeSolverTest, lookup_path_short)
{
	Cube cube;
	cube = Cube::turn(cube, FRONT);
	cube = Cube::turn(cube, BACK);
	cube = Cube::turn(cube, UP);

	solve(cube, endMap1, endMap2);
	ASSERT_TRUE(cube.isSolved());
}

TEST_F(CubeSolverTest, find_and_lookup_path)
{
	Cube cube;

	cube = Cube::turn(cube, DOWN);
	cube = Cube::turn(cube, RIGHT);
	cube = Cube::turn(cube, FRONT_INVERTED);
	cube = Cube::turn(cube, UP_INVERTED);
	cube = Cube::turn(cube, FRONT_INVERTED);
	cube = Cube::turn(cube, BACK);
	cube = Cube::turn(cube, LEFT);

	solve(cube, endMap1, endMap2);

	// assert that the cube is now solved
	ASSERT_TRUE(cube.isSolved());
}

void calculateStats(std::vector<double> vec, double& max, double& avg, double& stdev)
{
	accumulator_set<double, stats<tag::variance>> acc;
  	for_each(vec.begin(), vec.end(), std::ref(acc));

  	max = *std::max_element(vec.begin(), vec.end());
  	avg = mean(acc);
  	stdev = std::sqrt(variance(acc));
}

TEST(CubeSolverPerf, performance)
{
	readTurnTables();

	int NUM_TRIALS = 500;
	int MANEUVER_SIZE = 300;

	std::vector<double> timeVec;
	std::vector<double> numTurnsVec;

	EndMap1* endMap1_big;
	EndMap2* endMap2_big;

	readEndMaps(ENDMAP_BIG_PATH, endMap1_big, endMap2_big);

	for (int i = 0; i < NUM_TRIALS; ++i) {

		Cube cube;

		for (int j = 0; j < MANEUVER_SIZE; ++j) {
			cube = Cube::turn(cube, randomTurn1());
		}

		// time each individual solve
		clock_t cubeTime;
		cubeTime = clock();

		std::vector<Turn> turns = solve(cube, endMap1_big, endMap2_big);
		for (auto t : turns)
			cout << t << " ";
		cout << endl;

		cubeTime = clock() - cubeTime;

		timeVec.push_back((float) cubeTime / CLOCKS_PER_SEC);
		numTurnsVec.push_back((float) turns.size());

		EXPECT_TRUE(cube.isSolved());

	}

	cout << endl;

	cout << "testing translate" << endl;
	EasyCube c(
		std::vector<Color>(8, Color::Blue),
		std::vector<Color>(8, Color::Orange),
		std::vector<Color>(8, Color::White),
		std::vector<Color>(8, Color::Yellow),
		std::vector<Color>(8, Color::Red),
		std::vector<Color>(8, Color::Green)
	);
	Cube cube = translate(c);

	cout << cube << endl;

	std::vector<Turn> turns = solve(cube, endMap1_big, endMap2_big);
	for (auto t : turns)
		cout << t << " ";
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


	delete endMap1_big;
	delete endMap2_big;
}


