#include "cube.h"

#include <assert.h>
#include <iostream>
#include <algorithm>
#include <queue>
#include <ctime>
#include <map>
#include <fstream>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <bitset>
#include <vector>
#include "comm-protocol.h"


using namespace std;
using CommProtocol::MoveInstruction;

unordered_map<int, vector<MoveInstruction>> Cube::TURNMAP2;

// Vector of allowable turns for each step (easy to iterate through)
const vector<Cube::Turn> Cube::OK_TURNS1({
	Cube::Turn(MoveInstruction::FRONT), Cube::Turn(MoveInstruction::RIGHT), Cube::Turn(MoveInstruction::BACK), Cube::Turn(MoveInstruction::LEFT),
	Cube::Turn(MoveInstruction::UP), Cube::Turn(MoveInstruction::DOWN), Cube::Turn(MoveInstruction::FRONT_INVERTED), Cube::Turn(MoveInstruction::RIGHT_INVERTED),
	Cube::Turn(MoveInstruction::BACK_INVERTED), Cube::Turn(MoveInstruction::LEFT_INVERTED), Cube::Turn(MoveInstruction::UP_INVERTED), Cube::Turn(MoveInstruction::DOWN_INVERTED) });

const vector<Cube::Turn> Cube::OK_TURNS2({
	Cube::Turn(MoveInstruction::FRONT_2), Cube::Turn(MoveInstruction::RIGHT), Cube::Turn(MoveInstruction::BACK_2), Cube::Turn(MoveInstruction::LEFT),
	Cube::Turn(MoveInstruction::UP_2), Cube::Turn(MoveInstruction::DOWN_2), Cube::Turn(MoveInstruction::RIGHT_INVERTED), Cube::Turn(MoveInstruction::LEFT_INVERTED) });

// ok turns for the two-turn solve
const vector<Cube::Turn> Cube::OK_TURNS_2FACE({
	Cube::Turn(MoveInstruction::FRONT), Cube::Turn(MoveInstruction::RIGHT), Cube::Turn(MoveInstruction::FRONT_INVERTED), Cube::Turn(MoveInstruction::RIGHT_INVERTED) });

int Cube::FAC[12];
int Cube::CHOOSE[12][12];

// Cube constructor
Cube::Cube(Edge_t edgeColors[12], char edgeOrients[12], char edgeOrbits[12],
	Corner_t cornerColors[8], char cornerOrients[8])
{
	for (int i = 0; i < NUM_EDGES; ++i) {
		edgeColors_[i] = edgeColors[i];
		edgeOrients_[i] = edgeOrients[i];
		edgeOrbits_[i] = edgeOrbits[i];
	}
	for (int i = 0; i < NUM_CORNERS; ++i) {
		cornerColors_[i] = cornerColors[i];
		cornerOrients_[i] = cornerOrients[i];
	}
}

//////////////////// INITIALIZING //////////////////////

int Cube::fac(int n)
{
	return n == 0 ? 1 : n*fac(n - 1);
}

void Cube::initChoose()
{
	// Memoize values for FAC and CHOOSE tables
	for (int n = 0; n < 12; ++n) {
		FAC[n] = fac(n);
	}

	for (int n = 0; n < 12; ++n) {
		for (int k = 0; k <= n; ++k) {
			CHOOSE[n][k] = FAC[n] / (FAC[k] * FAC[n - k]);
		}
	}
}

// Fills the maps for a normal solve
void Cube::init()
{
	initChoose();

	// Create a solved cube, with corresponding empty vector<Turn> queue.
	// These are the starting points for hashmaps in steps 1, 2, and 4
	Cube cube;
	queue<Cube> cubeQueue;
	cubeQueue.push(cube);

	queue<vector<MoveInstruction>> turnsQueue;
	turnsQueue.push(vector<MoveInstruction>());

	std::cout << "filling map 1/4";

	// MAX_SIZE: 2^11 = 2048
	buildMap(cubeQueue, turnsQueue, "cubeMap1.ser", &Cube::edgeOrientsCode, OK_TURNS1, 2048);
	std::cout << endl;

	std::cout << "filling map 2/4";

	// MAX_SIZE: 3^7 * (12 choose 4) = 1082565
	//buildMap(cubeQueue, turnsQueue, "cubeMaps.ser", &Cube::step2Code, OK_TURNS2, 50000);
	std::cout << endl;

	std::cout << "filling map 3/4";
	/*
	// Build the 96 unique starting points and corresponding queue for step 3
	//queue<Cube> endPoints = step4ValidCorners();
	queue<vector<MoveInstruction>> step3TurnsQueue;
	for (int i = 0; i < endPoints.size(); ++i) {
		step3TurnsQueue.push(vector<MoveInstruction>());
	}
	*/
	// MAX_SIZE: 8c4 * 8c4 * 6 * 96 = 2822400
	// (could be 8c4*8c4*6 = 29400 if we have only one starting point
	//buildMap(endPoints, step3TurnsQueue, "cubeMap3.ser", &Cube::step3Code, OK_TURNS3, 50000);
	std::cout << endl;

	std::cout << "filling map 4/4";

	// MAX_SIZE: (4!)^3 / 2 * 96 = 663552
	//buildMap(cubeQueue, turnsQueue, "cubeMap4.ser", &Cube::step4Code, OK_TURNS4, 50000);
	std::cout << endl;
}

// Fills the maps for a 2-face solve
void Cube::init2()
{
	Cube cube;
	queue<Cube> cubeQueue;
	cubeQueue.push(cube);

	queue<vector<MoveInstruction>> turnsQueue;
	turnsQueue.push(vector<MoveInstruction>());

	std::cout << "filling map 1/4";

	// MAX_SIZE: 2^11 = 2048
	buildMap(cubeQueue, turnsQueue, "CubeMap2Face.ser", &Cube::code2, OK_TURNS_2FACE, 500000);
	std::cout << endl;
}


void Cube::print() const
{
	std::cout << "//// EDGES ////" << endl;

	// cast to an int, since they're chars
	for (size_t i = 0; i < NUM_EDGES; ++i) {
		std::cout << (int)edgeColors_[i] << "   ";
		std::cout << (int)edgeOrients_[i] << "   ";
		std::cout << (int)edgeOrbits_[i] << endl;

	}
	std::cout << endl;

	std::cout << " /// CORNERS ////" << endl;
	for (size_t i = 0; i < NUM_CORNERS; ++i) {
		std::cout << (int)cornerColors_[i] << "   ";
		std::cout << (int)cornerOrients_[i] << endl;
	}

	std::cout << endl << endl;
}


int Cube::edgeOrientsCode() {

	// Two codes have the same code if the edge orientations are the same
	// Essentially creates a unique base 2 number
	int sum = 0;
	for (int i = 0; i < NUM_EDGES; ++i) {
		sum += edgeOrients_[i] * pow(2, i);
	}
	return sum;
}

int Cube::cornerOrientsCode()
{
	int sum = 0;
	// NUM_CORNERS - 1 because the last corner is defined by the previous 7
	for (int i = 0; i < NUM_CORNERS; ++i) {
		sum += cornerOrients_[i] * pow(3, i);
	}
	return sum;
}



int Cube::cornerColorsCode()
{
	initChoose();

	int sum = 0;
	for (int i = 0; i < NUM_CORNERS - 1; ++i) {

		int numGreater = 0;
		for (int j = i + 1; j < NUM_CORNERS; ++j) {

			if (cornerColors_[j] < cornerColors_[i]) {
				++numGreater;
			}
		}
		sum += (numGreater * FAC[8 - i - 1]);
	}
	return sum;
}

int Cube::edgeOrbitsCode()
{
	// Hashes {0,0,0,0,_,_...} to 0, {0,0,0,_,0,_...} to 1,
	// ... and {...,_,0,0,0,0} to 494 = (12 choose 4) - 1.
	int sum = 0;
	int offset = 3;
	for (int i = 0; offset >= 0; ++i) {
		Edge_t currEdge = edgeColors_[i];
		if (edgeColors_[i] == YB || edgeColors_[i] == YG ||
			edgeColors_[i] == WB || edgeColors_[i] == WG) {
			--offset;
		}
		else {
			sum += CHOOSE[12 - i - 1][offset];
		}
	}
	return sum;
}

int Cube::edgeColorsCode1()
{
	int sum = 0;
	int ORBIT_SIZE = 4;

	for (int i = 0; i < ORBIT_SIZE - 1; ++i) {

		int numGreater = 0;
		for (int j = i + 1; j < ORBIT_SIZE; ++j) {

			if (edgeColors_[LR_SLICE[j]] < edgeColors_[LR_SLICE[i]]) {
				++numGreater;
			}
		}
		sum += (numGreater * FAC[ORBIT_SIZE - i - 1]);
	}
	return sum;
}

int Cube::edgeColorsCode2()
{
	int sum = 0;
	int ORBIT_SIZE = 8;

	// combined LR_SLICE and FB_SLICE
	Edge_t permArray[8] = { YR, YO, RB, OB, RG, OG, WR, WO };

	for (int i = 0; i < ORBIT_SIZE - 1; ++i) {

		int numGreater = 0;
		for (int j = i + 1; j < ORBIT_SIZE; ++j) {

			if (edgeColors_[permArray[j]] < edgeColors_[permArray[i]]) {
				++numGreater;
			}
		}
		sum += (numGreater * FAC[ORBIT_SIZE - i - 1]);
	}
	return sum;
}

int Cube::step1Code()
{
	return step1Code(edgeOrientsCode(), cornerOrientsCode(), edgeOrbitsCode());

}

int Cube::step1Code(int edgeOrients, int cornerOrients, int edgeOrbits)
{
	return NUM_EDGE_ORIENTS * NUM_CORNER_ORIENTS * edgeOrbits +
		NUM_EDGE_ORIENTS * cornerOrients +
		edgeOrients;
}

long Cube::step2Code()
{
	return step2Code(cornerColorsCode(), edgeColorsCode1(), edgeColorsCode2());
}

long Cube::step2Code(int cornerColors, int edgeColors1, int edgeColors2)
{
	return NUM_CORNER_COLORS * NUM_EDGE_COLORS2 * edgeColors1 +
		NUM_CORNER_COLORS * edgeColors2 +
		cornerColors;
}

// Code for a 2-face solve (for debugging only)
int Cube::code2()
{
	const int NUM_EDGES_2 = 7;		//only 7 relevant edges in 2-face
	const int NUM_CORNERS_2 = 6;		//only 6 relevant corners in 2-face

	char edges[NUM_EDGES_2] = { YO, YG, RG, OG, OB, WG, WO };
	char corners[NUM_CORNERS_2] = { YOB, YRG, YOG, WOB, WRG, WOG };

	// edge orients don't change because only F and R for 2-face

	int sum1 = 0;
	for (int i = 0; i < NUM_CORNERS_2 - 1; ++i) {
		sum1 += cornerOrients_[corners[i]] * pow(3, i);
	}

	//cout << "sum1: " << sum1 << endl;

	int sum2 = 0;
	for (int i = 0; i < NUM_CORNERS_2 - 1; ++i) {
		int numGreater = 0;
		for (int j = i + 1; j < NUM_CORNERS_2; ++j) {

			if (cornerColors_[corners[j]] < cornerColors_[corners[i]]) {
				++numGreater;
			}
		}
		sum2 += (numGreater * FAC[NUM_CORNERS_2 - i - 1]);
	}

	int sum3 = 0;
	for (int i = 0; i < NUM_EDGES_2 - 1; ++i) {
		int numGreater = 0;
		for (int j = i + 1; j < NUM_EDGES_2; ++j) {

			if (edgeColors_[edges[j]] < edgeColors_[edges[i]]) {
				++numGreater;
			}
		}
		sum3 += (numGreater * FAC[NUM_EDGES_2 - i - 1]);
	}

	return sum1 + 243*sum2 + 243000*sum3;
}
/*
std::vector<CommProtocol::MoveInstruction> Cube::solve(Cube cube)
{
	// Retrieve the Turns to solve each step

	//vector<Turn> step1 = doStep(cube, STEP1MAP, &Cube::edgeOrientsCode, OK_TURNS1);
	std::cout << endl;

	//vector<Turn> step2 = doStep(cube, STEP2MAP, &Cube::step2Code, OK_TURNS2);
	std::cout << endl;

	//vector<Turn> step3 = doStep(cube, STEP3MAP, &Cube::step3Code, OK_TURNS3);
	std::cout << endl;

	//vector<Turn> step4 = doStep(cube, STEP4MAP, &Cube::step4Code, OK_TURNS4);
	std::cout << endl;

	std::cout << "SUCCESS!!!!" << endl;

	// Print out all the steps

	//printTurns(step1);
	//printTurns(step2);
	//printTurns(step3);
	//printTurns(step4);
}
*/
// Solve for 2-face (debugging only)
void Cube::solve2(Cube cube)
{

	vector<Turn> step2 = doStep(cube, TURNMAP2, &Cube::code2, OK_TURNS_2FACE);
	std::cout << endl;

	std::cout << "SUCCESS!!!!" << endl;

	printTurns(step2);

}


vector<Cube::Turn> Cube::doStep(Cube& cube, const unordered_map<int, vector<MoveInstruction>>& stepTable, int (Cube::*code)(),
	vector<Turn> okSteps)
{
	// Time how long it takes to do the step
	clock_t t2;
	t2 = clock();

	// Find turns to complete the step
	vector<Turn> turns = cube.findTurns(stepTable, code, okSteps);
	
	t2 = clock() - t2;
	std::cout << "time: " << (float)t2 / CLOCKS_PER_SEC << endl;
	
	// Apply the turns to the cube
	cube = doTurns(cube, turns);

	return turns;
}

vector<Cube::Turn> Cube::findTurns(const unordered_map<int, vector<MoveInstruction>>& stepTable, int (Cube::*code)(), vector<Turn> okSteps)
{
	// keep a set of the cubes seen so far, so no duplicates
	unordered_set<int> seenCubes;

	// check to see if the first cube matches
	int firstHash = (this->*code)();
	seenCubes.insert(firstHash);

	// if the cube can already has a value in the table
	if (stepTable.count(firstHash) > 0) {
		vector<MoveInstruction> lastMoves = (stepTable.find(firstHash))->second;
		vector<Turn> lastTurns = Turn::movesToTurns(lastMoves);
		
		// invert the last moves
		vector<Turn> flipped = invert(lastTurns);

		return flipped;
	}

	// create a cubeQueue and turnsQueue
	queue<Cube> cubeQueue;
	cubeQueue.push(*this);

	queue<vector<Turn>> turnsQueue;
	turnsQueue.push(vector<Turn>());

	int i = 0;
	while (true) {
		
		// to keep track of how long it is taking...
		if (i % 100 == 0) {
			std::cout << ".";
		}

		// pop the first cube and turn vector
		Cube currCube = cubeQueue.front();
		cubeQueue.pop();

		vector<Turn> currTurns = turnsQueue.front();
		turnsQueue.pop();

		// push back a dummy turn that will immediately be popped
		// (to simplify the loop)
		currTurns.push_back(Turn(MoveInstruction::FRONT));

		for (auto turn : okSteps) {

			// apply a turn to the cube.
			Cube turnedCube = turn.turnFunc(currCube);

			// Calculate the code, and check if it matches
			int cubeCode = (turnedCube.*code)();

			if (seenCubes.count(cubeCode) == 0) {

				// If we've never seen the cube before, insert it.  Otherwise, ignore it
				seenCubes.insert(cubeCode);

				currTurns.pop_back();
				currTurns.push_back(turn.repr);

				// if this cube is in the step table, then we have found a shortest path
				// from the solved cube to the current state.
				if (stepTable.count(cubeCode) > 0) {

					vector<MoveInstruction> lastMoves = stepTable.find(cubeCode)->second;
					vector<Turn> lastTurns = Turn::movesToTurns(lastMoves);

					// invert the path from the solved cube to the current cube
					vector<Turn> flipped = invert(lastTurns);

					// concatenate the two paths
					currTurns.insert(currTurns.end(), flipped.begin(), flipped.end());

					return currTurns;
				}

				// otherwise, push onto the queue and keep searching
				else {
					cubeQueue.push(turnedCube);
					turnsQueue.push(currTurns);
				}
			}
		}
		++i;
	}
}





std::deque<Cube::Turn> Cube::solveStep1()
{
	// Make sure end tables have been built
	Cube solvedCube;
	assert(STEP1MAP.count(solvedCube.cubeNums1()) == 1);
	assert(STEP2MAP.count(solvedCube.cubeNums2()) == 1);
	assert(EDGE_COLORS_TABLE2[3000][3] == 13466);



	// keep track of cubes seen so far, so no duplicates
	//unordered_set<CubeNums, CubeNums::CubeNumsHash1, CubeNums::key_equal> seenCubes;
	unordered_set<CubeNums, CubeNums::Hash1> seenCubes;


	// check to see if the first cube matches
	CubeNums firstCube = cubeNums1();
	seenCubes.insert(firstCube);

	// if the cube already has a value in the table
	if (STEP1MAP.count(firstCube) > 0) {

		deque<Turn> moves = turnsFromEndMap1(firstCube);

		// FOR DEBUGGING: print the turns
		for (auto turn : moves) {
			cout << turn.toString << " ";
		} cout << endl;

		return moves;
	}
	// create cube queue and turns queue
	queue<CubeNums> cubeQueue;
	cubeQueue.push(firstCube);

	queue<deque<Turn>> turnsQueue;
	turnsQueue.push(deque<Turn>());

	int i = 0;
	while (true) {

		// to keep track of how long it is taking...
		if (i % 1000 == 0) {
			cout << i << endl;
		}

		// pop the first cube and turn vector
		CubeNums currCube = cubeQueue.front();
		cubeQueue.pop();

		deque<Turn> currTurns = turnsQueue.front();
		turnsQueue.pop();

		// push back a dummy turn that will immediately be popped
		// (to simplify the loop)
		currTurns.push_back(Turn(MoveInstruction::FRONT));

		for (int j = 0; j < NUM_TURNS_STEP1; ++j) {
			CubeNums turnedCube = currCube.turn1(j);
			
			// Calculate the code, and check if it matches
			if (seenCubes.count(turnedCube) == 0) {

				// If we've never seen the cube before, insert it
				seenCubes.insert(turnedCube);

				currTurns.pop_back();
				currTurns.push_back(OK_TURNS1[j].repr);

				// if the cube is in the step table, then we have found a match
				if (STEP1MAP.count(turnedCube) > 0) {

					deque<Turn> lastMoves = turnsFromEndMap1(turnedCube);
					
					// concatenate the two paths
					currTurns.insert(currTurns.end(), lastMoves.begin(), lastMoves.end());
					
					// FOR DEBUGGING: print out the current turns
					for (auto turn : currTurns) {
						cout << turn.toString << " ";
					} cout << endl;
					
					return currTurns;
				}
				// otherwise, push onto the queue and keep searching
				else {
					cubeQueue.push(turnedCube);
					turnsQueue.push(currTurns);
				}
			}
		}
		++i;
	}
}

deque<Cube::Turn> Cube::solveStep1DFS()
{
	
	Cube solvedCube;

	assert(STEP1MAP.count(solvedCube.cubeNums1()) == 1);
	assert(STEP2MAP.count(solvedCube.cubeNums2()) == 1);
	assert(EDGE_COLORS_TABLE2[3000][3] == 13466);

	CubeNums currCube = cubeNums1();


	int MIN_DEPTH = 4;
	int MAX_DEPTH = 8;

	deque<Turn> result;

	for (int depth = MIN_DEPTH; depth < MAX_DEPTH; ++depth) {
		if (solveStep1Helper(depth, currCube, result)) {
			return result;
		}
	}
	cout << "path not found :(" << endl;
	return deque<Turn>();
}
int SOLVE_STEP_1_COUNTER = 0;
bool Cube::solveStep1Helper(int depth, const CubeNums& curr, deque<Turn>& result)
{
	//if (SOLVE_STEP_1_COUNTER % 100 == 0) cout << SOLVE_STEP_1_COUNTER << endl;
	//++SOLVE_STEP_1_COUNTER;
	if (depth == 0) {
		if (STEP1MAP.count(curr) > 0) {

			deque<Turn> lastTurns = turnsFromEndMap1(curr);

			result = lastTurns;
			return true;
		}
		else {
			// not found, so return false
			return false;
		}
	}
	else {
		// iterate through all the ok turns
		for (int i = 0; i < NUM_TURNS_STEP1; ++i) {

			CubeNums turned = curr.turn1(i);

			// cube found: push back current move and return true
			if (solveStep1Helper(depth - 1, turned, result)) {
				Turn currTurn = OK_TURNS1[i];

				result.push_front(currTurn);
				return true;
			}
		}
		return false;
	}
}


deque<Cube::Turn> Cube::solveStep2DFS()
{
	Cube solvedCube;
	assert(STEP1MAP.count(solvedCube.cubeNums1()) == 1);
	assert(STEP2MAP.count(solvedCube.cubeNums2()) == 1);
	assert(EDGE_COLORS_TABLE2[3000][3] == 13466);

	CubeNums currCube = cubeNums2();

	int MIN_DEPTH = 4;
	int MAX_DEPTH = 10;

	deque<Turn> result;

	for (int depth = MIN_DEPTH; depth < MAX_DEPTH; ++depth) {
		if (solveStep2Helper(depth, currCube, result)) {
			return result;
		}
	}

	cout << "path not found :(" << endl;
	return deque<Turn>();
}

bool Cube::solveStep2Helper(int depth, const CubeNums& curr, deque<Turn>& result)
{
	if (depth == 0) {
		if (STEP2MAP.count(curr) > 0) {

			deque<Turn> lastTurns = turnsFromEndMap2(curr);

			result = lastTurns;
			return true;
		}
		else {
			return false;
		}
	}
	else {
		// iterate through all ok turns
		for (int i = 0; i < NUM_TURNS_STEP2; ++i) {
			CubeNums turnedCube = curr.turn2(i);

			// cube found: push back current move and return true
			if (solveStep2Helper(depth - 1, turnedCube, result)) {
				Turn currTurn = OK_TURNS2[i];

				result.push_front(currTurn);
				return true;
			}
		}
		return false;
	}
}


void Cube::buildMap(queue<Cube> cubeQueue, queue<vector<MoveInstruction>> turnsQueue, string fname,
	int (Cube::*code)(), vector<Turn> okTurns, int tableSize)
{
	// Initialize stepList, which is the hashmap to be serialized
	unordered_map<int, vector<MoveInstruction>> stepList;

	// Seed the first value of the stepList
	stepList[(cubeQueue.front().*code)()] = turnsQueue.front();

	while (stepList.size() < tableSize) {
	
		// Retrieve the first cube and its corresponding moves
		Cube currCube = cubeQueue.front();
		vector<MoveInstruction> currMoves = turnsQueue.front();

		cubeQueue.pop();
		turnsQueue.pop();

		// push back a dummy turn that will immediately be popped (to simplify the loop)
		currMoves.push_back(MoveInstruction::FRONT);

		// Loop through all the allowable turns in this step to use a breadth
		// first search to generate all possible cubes
		for (auto turn : okTurns) {
			
			Cube turnedCube = turn.turnFunc(currCube);
			int cubeCode = (turnedCube.*code)();

			// Only add to the queue if this cube is not on the stepList already
			if (stepList.count(cubeCode) == 0) {
				
				currMoves.pop_back();
				currMoves.push_back(turn.repr);
				
				cubeQueue.push(turnedCube);
				turnsQueue.push(currMoves);

				// Add this list of moves to the hash maps
				stepList[cubeCode] = currMoves;

				if (stepList.size() % (tableSize / 10) == 0) {
					std::cout << ".";
				}
			}
			
		}
	}

	// Serealize the hash table
	ofstream os(fname, ios::binary);
	boost::archive::binary_oarchive oarch(os);
	oarch << stepList;
	os.close();

	std::cout << "map serialized" << endl;
}

std::vector<MoveInstruction> Cube::solve()
{
	deque<Turn> firstTurns = solveStep1DFS();

	// apply turns found in first turns
	
	for (auto turn : firstTurns) {
		*this = (turn.turnFunc)(*this);
	}
	deque<Turn> lastTurns = solveStep2DFS();
	
	vector<MoveInstruction> instructions;
	
	// print and fill vector with move instructions to return
	for (auto turn : firstTurns) {
		cout << turn.toString << " ";
		instructions.push_back(turn.repr);
	} cout << endl;
	
	for (auto turn : lastTurns) {
		cout << turn.toString << " ";
		instructions.push_back(turn.repr);
	} cout << endl;
	

	return instructions;
}

void Cube::printTurns(vector<Turn> turns) {
	for (auto turn : turns) {
		cout << turn.toString << " ";
	}
	cout << endl;
}


vector<Cube::Turn> Cube::invert(vector<Turn> turns)
{
	reverse(turns.begin(), turns.end());

	vector<Turn> flipped;

	for (auto turn : turns) {
		Turn opposite = Turn(turn.oppTurn);
		flipped.push_back(opposite);
	}

	return flipped;
}

Cube Cube::doTurns(Cube cube, vector<Turn> turns)
{
	for (Turn turn : turns) {
		Cube(*currTurn)(Cube) = turn.turnFunc;
		cube = currTurn(cube);
	}

	return cube;
}

void Cube::loadMap(unordered_map<int, vector<MoveInstruction>>& stepTable, string fname)
{
	std::cout << "loading map from " << fname << endl;

	ifstream is(fname, ios::binary);

	boost::archive::binary_iarchive iarch(is);
	iarch >> stepTable;

	std::cout << "done reading" << endl;

}




/*
To do:

1) Build move tables
2) Shorten to two-phase
3) Serialize 6-face solve hash tables
4) Create different executables for building maps and solving cube
5) Account for symmetries



Notes:

1) 3/17: average solve: 1.78 sec.  Longest: 24 sec
2) First step: average solve: .017, Longest: .187
*/