/** 
 * \file cube-test.cpp
 */
#include <gtest/gtest.h>
#include "cubeSolver.h"
#include <ctime>
#include <iostream>
#include <cstdlib>

using CommProtocol::MoveInstruction;
using CubeSolver::Cube;
using CubeSolver::Turn;
using std::cin;
using std::cout;
using std::endl;

using namespace CubeSolver;

const std::string ENDMAP_SMALL_PATH = "ser/endMap_small.ser";
const std::string ENDMAP_BIG_PATH = "ser/endMap_big.ser";
const std::string TURNTABLES_PATH = "ser/turnTables.ser";

std::vector<Turn> allTurns({
		Turn(MoveInstruction::FRONT), Turn(MoveInstruction::RIGHT),
		Turn(MoveInstruction::BACK), Turn(MoveInstruction::LEFT),
		Turn(MoveInstruction::UP), Turn(MoveInstruction::DOWN),
		Turn(MoveInstruction::FRONT_INVERTED), Turn(MoveInstruction::RIGHT_INVERTED),
		Turn(MoveInstruction::BACK_INVERTED), Turn(MoveInstruction::LEFT_INVERTED),
		Turn(MoveInstruction::UP_INVERTED), Turn(MoveInstruction::DOWN_INVERTED)
	});

int MAP_SIZE_SMALL = 5000;
int MAP_SIZE_BIG = 10000000;

Turn randomTurn1() {
	int i = rand() % NUM_TURNS_STEP1;
	return OK_TURNS1[i];		
}

Turn randomTurn2() {
	int i = rand() % NUM_TURNS_STEP2;
	return OK_TURNS2[i];		
}

Cube scramble(std::vector<MoveInstruction> turns)
{
	Cube cube;

	for (auto mi: turns)
		cube = cube.turnWith(Turn(mi));

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

	cube1 = Cube::right(cube1);
	ASSERT_FALSE(cube1 == cube2);

	cube2 = Cube::right(cube2);
	ASSERT_TRUE(cube1 == cube2);

	cube1 = Cube::front2(cube1);
	cube2 = Cube::frontI(cube2);
	cube2 = Cube::frontI(cube2);
	ASSERT_TRUE(cube1 == cube2);
}

TEST_F(CubeTest, turns)
{
	// Construct a solved cube
	Cube cube;
	size_t numCycles = 0;

	// should run R' D' R D exactly 6 times before solved
	do {
		cube = Cube::rightI(cube);
		cube = Cube::downI(cube);
		cube = Cube::right(cube);
		cube = Cube::down(cube);
		++numCycles;
	} while (!cube.isSolved());

	EXPECT_EQ(numCycles, 6);

}

TEST_F(CubeTest, cube_nums)
{
	Cube cube;

	CubeNumsStep1 cubeNums1 = CubeNumsStep1(cube);
	for (int i = 0; i < NUM_TURNS_STEP1; ++i) {
		Cube newCube = cube.turnWith(OK_TURNS1[i]);
		CubeNumsStep1 newCubeNums1 = CubeNumsStep1(newCube);
		
		EXPECT_TRUE(newCubeNums1 == cubeNums1.turn(i));
	}

	CubeNumsStep2 cubeNums2 = CubeNumsStep2(cube);
	for (int i = 0; i < NUM_TURNS_STEP2; ++i) {
		Cube newCube = cube.turnWith(OK_TURNS2[i]);
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
		Turn turn = randomTurn1();
		cube = cube.turnWith(turn);
		cubeNums = cubeNums.turn(getIndex1(turn.repr));
		ASSERT_EQ(CubeNumsStep1(cube), cubeNums) 
			<< "failed turn: " << turn.toString << endl;
	}
}

TEST_F(CubeTest, cube_nums_sequence2)
{
	size_t numTurns = 10000;
	Cube cube;
	CubeNumsStep2 cubeNums;

	for (size_t i = 0; i < numTurns; ++i) {
		Turn turn = randomTurn2();
		cube = cube.turnWith(turn);
		cubeNums = cubeNums.turn(getIndex2(turn.repr));
		ASSERT_EQ(CubeNumsStep2(cube), cubeNums) 
			<< "failed turn: " << turn.toString << endl;
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
	cube = Cube::front(cube);
	cube = Cube::right(cube);

	solve(cube, EndMap1_trivial(), EndMap2_trivial());
	ASSERT_TRUE(cube.isSolved());
}

TEST_F(CubeSolverTest, find_path_long)
{

	Cube cube;
	cube = Cube::front(cube);
	cube = Cube::right(cube);
	cube = Cube::back(cube);
	cube = Cube::upI(cube);
	cube = Cube::right2(cube);

	solve(cube, EndMap1_trivial(), EndMap2_trivial());
	ASSERT_TRUE(cube.isSolved());
}

TEST_F(CubeSolverTest, lookup_path_short)
{
	Cube cube;
	cube = Cube::front(cube);
	cube = Cube::back(cube);
	cube = Cube::up(cube);

	solve(cube, endMap1, endMap2);
	ASSERT_TRUE(cube.isSolved());
}

TEST_F(CubeSolverTest, find_and_lookup_path)
{
	Cube cube;

	cube = Cube::down(cube);
	cube = Cube::right(cube);
	cube = Cube::frontI(cube);
	cube = Cube::upI(cube);
	cube = Cube::frontI(cube);
	cube = Cube::back(cube);
	cube = Cube::left(cube);

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
			cube = cube.turnWith(randomTurn1());
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


