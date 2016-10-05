/** 
 * \file cube-test.cpp
 */
#include <gtest/gtest.h>
#include "cubeSolver.h"
#include <ctime>
#include <iostream>
#include <cstdlib>

using CubeSolver::Cube;
using std::cout;
using std::endl;

using namespace CubeSolver;
using namespace CommProtocol;

const std::string ENDMAP_SMALL_PATH = "ser/endMap_small.ser";
const std::string ENDMAP_BIG_PATH = "ser/endMap_big.ser";
const std::string TURNTABLES_PATH = "ser/turnTables.ser";

std::vector<Move> allTurns({
		FRONT, RIGHT, BACK, LEFT, UP, DOWN,
		FRONT_INVERTED, RIGHT_INVERTED, BACK_INVERTED,
		LEFT_INVERTED, UP_INVERTED, DOWN_INVERTED
	});

int MAP_SIZE_SMALL = 5000;
int MAP_SIZE_BIG = 10000000;

Move randomTurn1() {
	int i = rand() % NUM_TURNS_STEP1;
	return OK_TURNS1[i];		
}

Move randomTurn2() {
	int i = rand() % NUM_TURNS_STEP2;
	return OK_TURNS2[i];		
}

Cube scramble(std::vector<MoveInstruction> turns)
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
		Cube newCube = Cube::turn(cube, OK_TURNS1[i]);
		CubeNumsStep1 newCubeNums1 = CubeNumsStep1(newCube);
		
		EXPECT_TRUE(newCubeNums1 == cubeNums1.turn(i));
	}

	CubeNumsStep2 cubeNums2 = CubeNumsStep2(cube);
	for (int i = 0; i < NUM_TURNS_STEP2; ++i) {
		Cube newCube = Cube::turn(cube, OK_TURNS2[i]);
		CubeNumsStep2 newCubeNums2 = CubeNumsStep2(newCube);
		
		EXPECT_TRUE(newCubeNums2 == cubeNums2.turn(i));

	}
}

// Compare a cube with cube_nums using the same sequence of turns
TEST_F(CubeTest, cube_nums_sequence1)
{
	size_t numTurns = 10000;
	Cube cube;
	CubeNumsStep1 cubeNums;

	for (size_t i = 0; i < numTurns; ++i) {
		Move turn = randomTurn1();
		cube = Cube::turn(cube, turn);
		cubeNums = cubeNums.turn(getIndex1(turn));
		ASSERT_EQ(CubeNumsStep1(cube), cubeNums) 
			<< "failed turn: " << turn << endl;
	}
}

TEST_F(CubeTest, cube_nums_sequence2)
{
	size_t numTurns = 10000;
	Cube cube;
	CubeNumsStep2 cubeNums;

	for (size_t i = 0; i < numTurns; ++i) {
		Move turn = randomTurn2();
		cube = Cube::turn(cube, turn);
		cubeNums = cubeNums.turn(getIndex2(turn));
		ASSERT_EQ(CubeNumsStep2(cube), cubeNums) 
			<< "failed turn: " << turn << endl;
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

TEST(CubeSolverPerf, performance)
{
	readTurnTables();

	int NUM_TRIALS = 500;
	int MANEUVER_SIZE = 100;

	EndMap1* endMap1_big;
	EndMap2* endMap2_big;

	readEndMaps(ENDMAP_BIG_PATH, endMap1_big, endMap2_big);

	clock_t t;
	t = clock();

	float maxTime = 0;

	for (int i = 0; i < NUM_TRIALS; ++i) {

		Cube cube;

		for (int j = 0; j < MANEUVER_SIZE; ++j) {
			cube = Cube::turn(cube, randomTurn1());
		}


		// time each individual solve
		clock_t cubeTime;
		cubeTime = clock();
		
		solve(cube, endMap1_big, endMap2_big);
		
		EXPECT_TRUE(cube.isSolved());

		cubeTime = clock() - cubeTime;
		if (float(cubeTime) / CLOCKS_PER_SEC > maxTime) {
			maxTime = float(cubeTime) / CLOCKS_PER_SEC;
		}
		cout << ".";

	}

	t = clock() - t;

	cout << endl << "max time: " << maxTime << endl;
	cout << "average time: " << ((float)t / CLOCKS_PER_SEC) / NUM_TRIALS << endl;

	delete endMap1_big;
	delete endMap2_big;
}


