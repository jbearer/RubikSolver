/** 
 * \file cube-test.cpp
 */

#include "cube.h"
#include <ctime>
#include <iostream>
#include <cstdlib>

using namespace std;
using CommProtocol::MoveInstruction;


void Cube::testDFS()
{
	readTurnTables();
	readEndMaps();
	
	Cube cube;

	vector<Turn> maneuver = {
		Turn(MoveInstruction::DOWN),
		Turn(MoveInstruction::RIGHT),
		Turn(MoveInstruction::FRONT_INVERTED),
		Turn(MoveInstruction::UP_INVERTED),
		Turn(MoveInstruction::FRONT_INVERTED),
		Turn(MoveInstruction::BACK),
		Turn(MoveInstruction::LEFT)
		};
		
	for (auto turn : maneuver) {
		cube = (turn.turnFunc)(cube);
	}
	
	cout << "Starting..." << endl;

	clock_t t;
	t = clock();

	cube.solve();

	t = clock() - t;
	cout << "time for searching: " << (float)t / CLOCKS_PER_SEC << endl;
	
	}

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
/*
void Cube::turnTableTest()
{
	Cube cube;
	cube = front(cube);
	cube = up(cube);
	cube = right(cube);
	CubeNums cubeNums = cube.cubeNums1();

	for (int i = 0; i < NUM_TURNS_STEP1; ++i) {
		Cube turnedCube = OK_TURNS1[i].turnFunc(cube);
		CubeNums expected = turnedCube.cubeNums1();

		CubeNums turnedCubeNums = cubeNums.turn1(i);
		/*
		cout << "Expected: " << expected.first << ", "
		<< expected.second << ", " << expected.third << endl;
		cout << "Actual: " << turnedCubeNums.first << ", "
		<< turnedCubeNums.second << ", " << turnedCubeNums.third << endl;


		if (!(expected == turnedCubeNums)) {
		cout << "doesn't match for i = " << i;
		}
		
		assert(cube.cubeNums1() == cubeNums);
	}
	cout << endl << endl;

	Cube cube2;
	CubeNums cubeNums2 = cube2.cubeNums2();


	for (int i = 0; i < NUM_TURNS_STEP2; ++i) {
		Cube turnedCube = cube2.turnWith(OK_TURNS2[i]);
		CubeNums expected = turnedCube.cubeNums2();

		CubeNums turnedCubeNums = cubeNums2.turn2(i);
		/*
		cout << "Expected: " << expected.first << ", "
		<< expected.second << ", " << expected.third << endl;
		cout << "Actual: " << turnedCubeNums.first << ", "
		<< turnedCubeNums.second << ", " << turnedCubeNums.third << endl;

		if (!(expected == turnedCubeNums)) {
		cout << "doesn't match for i = " << i << endl;;
		}
		
		assert(cube2.cubeNums2() == cubeNums2);
	}
}

void Cube::turnTest()
{
	Cube cube;
	Cube cube2;

	cube = front(cube);
	cube = frontI(cube);

	assert(CubeNumsStep1(cube) == CubeNumsStep1(cube2));
	assert(cube.cubeNums2() == cube2.cubeNums2());

	cube = right(cube);
	cube = rightI(cube);

	assert(cube.cubeNums1() == cube2.cubeNums1());
	assert(cube.cubeNums2() == cube2.cubeNums2());

	cube = back(cube);
	cube = backI(cube);

	assert(cube.cubeNums1() == cube2.cubeNums1());
	assert(cube.cubeNums2() == cube2.cubeNums2());

	cube = left(cube);
	cube = leftI(cube);

	assert(cube.cubeNums1() == cube2.cubeNums1());
	assert(cube.cubeNums2() == cube2.cubeNums2());

	cube = up(cube);
	cube = upI(cube);

	assert(cube.cubeNums1() == cube2.cubeNums1());
	assert(cube.cubeNums2() == cube2.cubeNums2());

	cube = down(cube);
	cube = downI(cube);

	assert(cube.cubeNums1() == cube2.cubeNums1());
	assert(cube.cubeNums2() == cube2.cubeNums2());

}
*/

void Cube::timeTurnTables()
{
	initChoose();
	readTurnTables();
	readEndMaps();

	size_t NUM_TURNS = 20000000;
	Cube cube1;
	cout << "timing with normal turning" << endl;
	
	clock_t t1;
	t1 = clock();
	
	for (int i = 0; i < NUM_TURNS; ++i) {
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

	for (int i = 0; i < NUM_TURNS; ++i) {
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
	//14.457 seconds without subtyping

	readTurnTables();
	vector<Turn> maneuver = { Turn(MoveInstruction::FRONT),
	Turn(MoveInstruction::RIGHT),
	Turn(MoveInstruction::UP),
	Turn(MoveInstruction::LEFT),
	Turn(MoveInstruction::BACK),
	Turn(MoveInstruction::DOWN) };

	Cube cube;
	CubeNumsStep1 solvedNums;
	STEP1MAP[solvedNums] = MoveInstruction::FRONT;

	for (auto turn : maneuver) {
		cube = cube.turnWith(turn);
	}

	clock_t t;
	t = clock();

	cube.solveStep1DFS();

	t = clock() - t;
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
	//buildTurnTables();
	//readTurnTables();
	//buildEndMaps();
	//readTurnTables();

}

int main()
{
	clock_t t;
	t = clock();

	Cube::initChoose();

	Cube::test1();

	cout << endl << endl;
	t = clock() - t;
	cout << "time: " << (float)t / CLOCKS_PER_SEC << endl;
	
	std::system("pause");
	return 0;
}
