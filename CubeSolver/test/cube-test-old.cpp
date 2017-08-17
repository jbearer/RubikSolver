/* Old tests that I used before switching to google tests
This file exists in case I want to convert these existing
tests into google tests.  However, many of the methods no longer exist,
so it would take some time to convert.  This file should NOT be tracked
in the RubikSolver repo.
*/


void Cube::turnTableTest()
{
	Cube cube;
	cube = front(cube);
	cube = up(cube);
	cube = right(cube);
	CubeNumsStep1 cubeNums(cube);

	for (size_t i = 0; i < NUM_TURNS_STEP1; ++i) {
		Cube turnedCube = TURNS_STEP1[i].turnFunc(cube);
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
		Cube turnedCube = cube2.turnWith(TURNS_STEP2[i]);
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

