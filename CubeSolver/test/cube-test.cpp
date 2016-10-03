/** 
 * \file cube-test.cpp
 */
#include <gtest/gtest.h>
#include "cubeSolver.h"
//#include "end_maps.h"
//#include "turn_tables.h"
#include <ctime>
#include <iostream>
#include <cstdlib>

using namespace std;
using CommProtocol::MoveInstruction;

const std::string ENDMAP_SMALL_PATH = "ser/endMap_small.ser";
const std::string ENDMAP_BIG_PATH = "ser/endMap_big.ser";
const std::string TURNTABLES_PATH = "ser/turnTables.ser";

vector<Turn> allTurns({
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

Cube scramble(vector<MoveInstruction> turns)
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
  // Per-test-case set-up.
  // Called before the first test in this test case.
  // Can be omitted if not needed.
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
	queue<CubeNumsStep1> cubeQueue = buildMap1(MAP_SIZE_SMALL);

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



/*
void Cube::turnTest()
{
	Cube cube;
	Cube cube2;

	cube = front(cube);
	cube = frontI(cube);

	assert(CubeNumsStep1(cube) == CubeNumsStep1(cube2));
	assert(CubeNumsStep2(cube) == CubeNumsStep2(cube2));

	cube = right(cube);
	cube = rightI(cube);

	assert(CubeNumsStep1(cube) == CubeNumsStep1(cube2));
	assert(CubeNumsStep2(cube) == CubeNumsStep2(cube2));

	cube = back(cube);
	cube = backI(cube);

	assert(CubeNumsStep1(cube) == CubeNumsStep1(cube2));
	assert(CubeNumsStep2(cube) == CubeNumsStep2(cube2));

	cube = left(cube);
	cube = leftI(cube);

	assert(CubeNumsStep1(cube) == CubeNumsStep1(cube2));
	assert(CubeNumsStep2(cube) == CubeNumsStep2(cube2));

	cube = up(cube);
	cube = upI(cube);

	assert(CubeNumsStep1(cube) == CubeNumsStep1(cube2));
	assert(CubeNumsStep2(cube) == CubeNumsStep2(cube2));

	cube = down(cube);
	cube = downI(cube);

	assert(CubeNumsStep1(cube) == CubeNumsStep1(cube2));
	assert(CubeNumsStep2(cube) == CubeNumsStep2(cube2));

}
*/
/*
void Cube::timeDFS()
{
	readTurnTables();
	readEndMaps();

	vector<Turn> allTurns({
		Turn(MoveInstruction::FRONT), Turn(MoveInstruction::RIGHT),
		Turn(MoveInstruction::BACK), Turn(MoveInstruction::LEFT),
		Turn(MoveInstruction::UP), Turn(MoveInstruction::DOWN),
		Turn(MoveInstruction::FRONT_INVERTED), Turn(MoveInstruction::RIGHT_INVERTED),
		Turn(MoveInstruction::BACK_INVERTED), Turn(MoveInstruction::LEFT_INVERTED),
		Turn(MoveInstruction::UP_INVERTED), Turn(MoveInstruction::DOWN_INVERTED)
	});


	int NUM_TRIALS = 100;
	int MANEUVER_SIZE = 100;

	clock_t t;
	t = clock();

	float maxTime = 0;

	for (int i = 0; i < NUM_TRIALS; ++i) {

		Cube cube;

		for (int j = 0; j < MANEUVER_SIZE; ++j) {
			vector<Turn>::iterator randIt = allTurns.begin();
			advance(randIt, rand() % allTurns.size());
			cube = cube.turnWith(*randIt);
		}

		// time each individual solve
		clock_t cubeTime;
		cubeTime = clock();
		
		cube.solve();
		
		cubeTime = clock() - cubeTime;
		if (float(cubeTime) / CLOCKS_PER_SEC > maxTime) {
			maxTime = float(cubeTime) / CLOCKS_PER_SEC;
		}
	}
	t = clock() - t;

	cout << "max time: " << maxTime << endl;
	cout << "average time: " << ((float)t / CLOCKS_PER_SEC) / NUM_TRIALS << endl;

}

void Cube::turnTableTest()
{
	Cube cube;
	cube = front(cube);
	cube = up(cube);
	cube = right(cube);
	CubeNumsStep1 cubeNums(cube);

	for (size_t i = 0; i < NUM_TURNS_STEP1; ++i) {
		Cube turnedCube = OK_TURNS1[i].turnFunc(cube);
		CubeNumsStep1 expected(turnedCube);

		CubeNumsStep1 turnedCubeNums = cubeNums.turn(i);
		
		cout << "Expected: " << expected.first << ", "
		<< expected.second << ", " << expected.third << endl;
		cout << "Actual: " << turnedCubeNums.first << ", "
		<< turnedCubeNums.second << ", " << turnedCubeNums.third << endl;
		

		if (!(expected == turnedCubeNums)) {
		cout << "doesn't match for i = " << i;
		}
		
		assert(CubeNumsStep1(cube) == cubeNums);
	}
	cout << endl << endl;

	Cube cube2;
	CubeNumsStep2 cubeNums2 (cube2);


	for (size_t i = 0; i < NUM_TURNS_STEP2; ++i) {
		Cube turnedCube = cube2.turnWith(OK_TURNS2[i]);
		CubeNumsStep2 expected (turnedCube);

		CubeNumsStep2 turnedCubeNums = cubeNums2.turn(i);
		
		cout << "Expected: " << expected.first << ", "
		<< expected.second << ", " << expected.third << endl;
		cout << "Actual: " << turnedCubeNums.first << ", "
		<< turnedCubeNums.second << ", " << turnedCubeNums.third << endl;
		
		if (!(expected == turnedCubeNums)) {
		cout << "doesn't match for i = " << i << endl;;
		}
		
		assert(CubeNumsStep2(cube2) == cubeNums2);
	}
}




void Cube::timeTurnTables()
{
	readTurnTables();
	readEndMaps();

	size_t NUM_TURNS = 20000000;
	Cube cube1;
	cout << "timing with normal turning" << endl;
	
	clock_t t1;
	t1 = clock();
	
	for (size_t i = 0; i < NUM_TURNS; ++i) {
		cube1 = front(cube1);
		cube1 = right(cube1);
		cube1 = back(cube1);
		cube1 = left(cube1);
	}

	t1 = clock() - t1;
	std::cout << "time for normal turning: " << (float)t1 / CLOCKS_PER_SEC << std::endl;

	Cube cube2;

	short edgeOrients = cube2.edgeOrientsCode();
	short cornerOrients = cube2.cornerOrientsCode();
	short edgeOrbits = cube2.edgeOrbitsCode();


	cout << "timing with look-up turning" << endl;
	// look-up turning about 50-times faster
	clock_t t2;
	t2 = clock();

	for (size_t i = 0; i < NUM_TURNS; ++i) {
		//front
		edgeOrients = EDGE_ORIENTS_TABLE[edgeOrients][0];
		cornerOrients = CORNER_ORIENTS_TABLE[cornerOrients][0];
		edgeOrbits = EDGE_ORBITS_TABLE[edgeOrbits][0];

		//right
		edgeOrients = EDGE_ORIENTS_TABLE[edgeOrients][1];
		cornerOrients = CORNER_ORIENTS_TABLE[cornerOrients][1];
		edgeOrbits = EDGE_ORBITS_TABLE[edgeOrbits][1];

		//back
		edgeOrients = EDGE_ORIENTS_TABLE[edgeOrients][2];
		cornerOrients = CORNER_ORIENTS_TABLE[cornerOrients][2];
		edgeOrbits = EDGE_ORBITS_TABLE[edgeOrbits][2];

		//left
		edgeOrients = EDGE_ORIENTS_TABLE[edgeOrients][3];
		cornerOrients = CORNER_ORIENTS_TABLE[cornerOrients][3];
		edgeOrbits = EDGE_ORBITS_TABLE[edgeOrbits][3];
	}

	t2 = clock() - t2;
	std::cout << "time for look-up turning: " << (float)t2 / CLOCKS_PER_SEC << std::endl;

}

void Cube::test1()
{
	readTurnTables();
	vector<Turn> maneuver = {
		Turn(MoveInstruction::FRONT),
		Turn(MoveInstruction::RIGHT),
		Turn(MoveInstruction::UP),
		Turn(MoveInstruction::LEFT),
		Turn(MoveInstruction::BACK),
		Turn(MoveInstruction::DOWN)
	};

	Cube cube;
	CubeNumsStep1 solvedNums;
	STEP1MAP[solvedNums] = MoveInstruction::FRONT;

	for (auto turn : maneuver) {
		cube = cube.turnWith(turn);
	}

	clock_t t;
	t = clock();

	cube.solveStep1DFS();

	t = clock() - t;		//11.555 seconds
	cout << "time: " << (float)t / CLOCKS_PER_SEC << endl;
}

void Cube::test()
{
	Cube cube;

	cout << cube.edgeOrientsCode() << endl;
	cout << cube.cornerOrientsCode() << endl;
	cout << cube.edgeOrbitsCode() << endl;

	cout << cube.cornerColorsCode() << endl;
	cout << cube.edgeColorsCode1() << endl;
	cout << cube.edgeColorsCode2() << endl;
	buildTurnTables();
	readTurnTables();
	buildEndMaps();
	readEndMaps();

}
*/
/*
int main()
{
	clock_t t;
	t = clock();

	Cube::testDFS();

	cout << endl << endl;
	t = clock() - t;
	cout << "time: " << (float)t / CLOCKS_PER_SEC << endl;
	
	std::system("pause");
	return 0;
}
*/

/*
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
*/

