#include "cubeSolver.h"

using namespace std;

int main()
{
	readTurnTables();
	EndMap1* endMap1;
	EndMap2* endMap2;

	readEndMaps("ser/endMap_big.ser", endMap1, endMap2);
	
	vector<Turn> allTurns({
		Turn(MoveInstruction::FRONT), Turn(MoveInstruction::RIGHT),
		Turn(MoveInstruction::BACK), Turn(MoveInstruction::LEFT),
		Turn(MoveInstruction::UP), Turn(MoveInstruction::DOWN),
		Turn(MoveInstruction::FRONT_INVERTED), Turn(MoveInstruction::RIGHT_INVERTED),
		Turn(MoveInstruction::BACK_INVERTED), Turn(MoveInstruction::LEFT_INVERTED),
		Turn(MoveInstruction::UP_INVERTED), Turn(MoveInstruction::DOWN_INVERTED)
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
			vector<Turn>::iterator randIt = allTurns.begin();
			advance(randIt, rand() % allTurns.size());
			cube = cube.turnWith(*randIt);
			cout << randIt->toString << " ";
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