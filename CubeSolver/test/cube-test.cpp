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
using namespace boost::accumulators;

const std::string ENDMAP_SMALL_PATH = "ser/endMap_small.ser";
const std::string ENDMAP_BIG_PATH = "ser/endMap_big.ser";
const std::string TURNTABLES_PATH = "ser/turnTables.ser";

std::vector<Turn> allTurns({F, R, B, L, U, D, Fi, Ri, Bi, Li, Ui, Di});

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

	cube1 = Cube::turn(cube1, R);
	ASSERT_FALSE(cube1 == cube2);

	cube2 = Cube::turn(cube2, R);
	ASSERT_TRUE(cube1 == cube2);

	cube1 = Cube::turn(cube1, F2);
	cube2 = Cube::turn(cube2, Fi);
	cube2 = Cube::turn(cube2, Fi);
	ASSERT_TRUE(cube1 == cube2);
}

TEST_F(CubeTest, turns)
{
	// Construct a solved cube
	Cube cube;
	size_t numCycles = 0;

	// should run R' D' R D exactly 6 times before solved
	do {
		cube = Cube::turn(cube, Ri);
		cube = Cube::turn(cube, Di);
		cube = Cube::turn(cube, R);
		cube = Cube::turn(cube, D);
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

TEST(EndMapTest, turnsFromEndMap_trivial)
{
	std::unique_ptr<EndMap1> endMap1(new EndMap1);
	buildMap1(endMap1, MAP_SIZE_SMALL);

	CubeNumsStep1 cubeNums;		// initialize to solved cube
	turnsFromEndMap1(cubeNums, endMap1);
}

TEST(EndMapTest, endMap)
{
	std::unique_ptr<EndMap1> endMap1(new EndMap1);
	buildMap1(endMap1, MAP_SIZE_SMALL);

	// confirm that the end maps have all cubes
	// two turns away from a solved cube
	for (auto t1 : allTurns) {
		for (auto t2 : allTurns) {
			Cube cube;
			cube = Cube::turn(cube, t1);
			cube = Cube::turn(cube, t2);
			CubeNumsStep1 nums(cube);
			turnsFromEndMap1(nums, endMap1);
		}
	}
}

TEST(SolverTest, trivial)
{
	Cube cube;
	Solver small_solver;
	small_solver.solve(cube);
	ASSERT_TRUE(cube.isSolved());
}

TEST(SolverTest, find_path_short)
{
	Cube cube;
	cube = Cube::turn(cube, F);
	cube = Cube::turn(cube, R);

	Solver small_solver;
	small_solver.solve(cube);

	ASSERT_TRUE(cube.isSolved());
}

TEST(SolverTest, find_path_long)
{

	Cube cube;
	cube = Cube::turn(cube, F);
	cube = Cube::turn(cube, R);
	cube = Cube::turn(cube, B);
	cube = Cube::turn(cube, Ui);
	cube = Cube::turn(cube, R2);

	Solver small_solver;
	small_solver.solve(cube);

	ASSERT_TRUE(cube.isSolved());
}

TEST(SolverTest, lookup_path_short)
{
	Cube cube;
	cube = Cube::turn(cube, F);
	cube = Cube::turn(cube, B);
	cube = Cube::turn(cube, U);

	Solver solver(MAP_SIZE_SMALL, MAP_SIZE_SMALL);

	solver.solve(cube);
	ASSERT_TRUE(cube.isSolved());
}

TEST(SolverTest, find_and_lookup_path)
{
	Cube cube;

	cube = Cube::turn(cube, D);
	cube = Cube::turn(cube, R);
	cube = Cube::turn(cube, Fi);
	cube = Cube::turn(cube, Ui);
	cube = Cube::turn(cube, Fi);
	cube = Cube::turn(cube, B);
	cube = Cube::turn(cube, L);

	Solver solver(MAP_SIZE_SMALL, MAP_SIZE_SMALL);
	solver.solve(cube);

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

	Solver solver(ENDMAP_BIG_PATH);

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

		EXPECT_TRUE(cube.isSolved());

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


