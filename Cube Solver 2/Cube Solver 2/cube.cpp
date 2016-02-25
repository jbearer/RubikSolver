#include "cube.h"


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


using namespace std;
using MotorControl::MoveInstruction;

// Hashmaps for each step
unordered_map<int, vector<MoveInstruction>> Cube::STEP1MAP;
unordered_map<int, vector<MoveInstruction>> Cube::STEP2MAP;
unordered_map<int, vector<MoveInstruction>> Cube::STEP3MAP;
unordered_map<int, vector<MoveInstruction>> Cube::STEP4MAP;

unordered_map<int, vector<MoveInstruction>> Cube::TURNMAP2;

// Vector of allowable turns for each step (easy to iterate through)
const vector<Cube::Turn> Cube::OK_TURNS1({
	Cube::Turn(MoveInstruction::FRONT), Cube::Turn(MoveInstruction::RIGHT), Cube::Turn(MoveInstruction::BACK), Cube::Turn(MoveInstruction::LEFT),
	Cube::Turn(MoveInstruction::UP), Cube::Turn(MoveInstruction::DOWN), Cube::Turn(MoveInstruction::FRONT_INVERTED), Cube::Turn(MoveInstruction::RIGHT_INVERTED),
	Cube::Turn(MoveInstruction::BACK_INVERTED), Cube::Turn(MoveInstruction::LEFT_INVERTED), Cube::Turn(MoveInstruction::UP_INVERTED), Cube::Turn(MoveInstruction::DOWN_INVERTED) });

const vector<Cube::Turn> Cube::OK_TURNS2({
	Cube::Turn(MoveInstruction::FRONT), Cube::Turn(MoveInstruction::RIGHT), Cube::Turn(MoveInstruction::BACK), Cube::Turn(MoveInstruction::LEFT),
	Cube::Turn(MoveInstruction::UP_2), Cube::Turn(MoveInstruction::DOWN_2),
	Cube::Turn(MoveInstruction::FRONT_INVERTED), Cube::Turn(MoveInstruction::RIGHT_INVERTED), Cube::Turn(MoveInstruction::BACK_INVERTED), Cube::Turn(MoveInstruction::LEFT_INVERTED) });

const vector<Cube::Turn> Cube::OK_TURNS3({
	Cube::Turn(MoveInstruction::FRONT_2), Cube::Turn(MoveInstruction::RIGHT), Cube::Turn(MoveInstruction::BACK_2), Cube::Turn(MoveInstruction::LEFT),
	Cube::Turn(MoveInstruction::UP_2), Cube::Turn(MoveInstruction::DOWN_2), Cube::Turn(MoveInstruction::RIGHT_INVERTED), Cube::Turn(MoveInstruction::LEFT_INVERTED) });

const vector<Cube::Turn> Cube::OK_TURNS4({
	Cube::Turn(MoveInstruction::FRONT_2), Cube::Turn(MoveInstruction::RIGHT_2), Cube::Turn(MoveInstruction::BACK_2), Cube::Turn(MoveInstruction::LEFT_2),
	Cube::Turn(MoveInstruction::UP_2), Cube::Turn(MoveInstruction::DOWN_2) });

// ok turns for the two-turn solve
const vector<Cube::Turn> Cube::OK_TURNS_2FACE({
	Cube::Turn(MoveInstruction::FRONT), Cube::Turn(MoveInstruction::RIGHT), Cube::Turn(MoveInstruction::FRONT_INVERTED), Cube::Turn(MoveInstruction::RIGHT_INVERTED) });


// Cube constructor
// TODO: figure out how to get rid of edgeOrbits.  They're unnecessary
Cube::Cube(char eColors[12], char eOrients[12], char eOrbits[12],
	char cColors[8], char cOrients[8])
{
	for (int i = 0; i < NUM_EDGES; ++i) {
		edgeColors_[i] = eColors[i];
		edgeOrients_[i] = eOrients[i];
		edgeOrbits_[i] = eOrbits[i];
	}
	for (int i = 0; i < NUM_CORNERS; ++i) {
		cornerColors_[i] = cColors[i];
		cornerOrients_[i] = cOrients[i];
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
	Cube cube = solvedCube();
	queue<Cube> cubeQueue;
	cubeQueue.push(cube);

	queue<vector<MoveInstruction>> turnsQueue;
	turnsQueue.push(vector<MoveInstruction>());

	std::cout << "filling map 1/4";

	// MAX_SIZE: 2^11 = 2048
	buildMap(cubeQueue, turnsQueue, "cubeMap1.ser", &step1Code, OK_TURNS1, 2048);
	std::cout << endl;

	std::cout << "filling map 2/4";

	// MAX_SIZE: 3^7 * (12 choose 4) = 1082565
	buildMap(cubeQueue, turnsQueue, "cubeMaps.ser", &step2Code, OK_TURNS2, 1082565);
	std::cout << endl;

	std::cout << "filling map 3/4";

	// Build the 96 unique starting points and corresponding queue for step 3
	queue<Cube> endPoints = step4ValidCorners();
	queue<vector<MoveInstruction>> step3TurnsQueue;
	for (int i = 0; i < endPoints.size(); ++i) {
		step3TurnsQueue.push(vector<MoveInstruction>());
	}

	// MAX_SIZE: 8c4 * 8c4 * 6 * 96 = 2822400
	// (could be 8c4*8c4*6 = 29400 if we have only one starting point
	buildMap(endPoints, step3TurnsQueue, "cubeMap3.ser", &step3Code, OK_TURNS3, 2822400);
	std::cout << endl;

	std::cout << "filling map 4/4";

	// MAX_SIZE: (4!)^3 / 2 * 96 = 663552
	buildMap(cubeQueue, turnsQueue, "cubeMap4.ser", &step4Code, OK_TURNS4, 663552);
	std::cout << endl;
}

// Fills the maps for a 2-face solve
void Cube::init2()
{
	Cube cube = solvedCube();
	queue<Cube> cubeQueue;
	cubeQueue.push(cube);

	queue<vector<MoveInstruction>> turnsQueue;
	turnsQueue.push(vector<MoveInstruction>());

	std::cout << "filling map 1/4";

	// MAX_SIZE: 2^11 = 2048
	buildMap(cubeQueue, turnsQueue, "CubeMap2Face.ser", &code2, OK_TURNS_2FACE, 500000);
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


int Cube::step1Code() {

	// Two codes have the same code if the edge orientations are the same
	// Essentially creates a unique base 2 number
	int sum = 0;
	for (int i = 0; i < NUM_EDGES - 1; ++i) {
		sum += edgeOrients_[i] * pow(2, i);
	}
	return sum;
}

int Cube::step2Code() {

	// Two codes have the same code if the corner orientations are the same
	// and if the UD_SLICE edges are in the same place

	// Essentially creates a unique base 3 number for corner orients
	int sum1 = 0;
	for (int i = 0; i < NUM_CORNERS - 1; ++i) {
		sum1 += cornerOrients_[i] * pow(3, i);
	}

	// Hashes {0,0,0,0,_,_...} to 0, {0,0,0,_,0,_...} to 1,
	// ... and {...,_,0,0,0,0} to 494 = (12 choose 4) - 1.
	int sum2 = 0;
	int offset = 3;
	for (int i = 0; offset >= 0; ++i) {
		if (edgeOrbits_[i] == 0) {
			--offset;
		}
		else {
			sum2 += CHOOSE[12 - i - 1][offset];
		}
	}
	return sum1 * 495 + sum2;
}

int Cube::step3Code()
{
	// TODO: simplify code for step 3

	// certain edge orbit spots are known to be filled with 0's
	int openSpots[8] = { 1,2,4,5,6,7,9,10 };

	// creates a code for the edges of the FB_ORBIT
	// similar to second part of step2Code
	int sum1 = 0;
	int offset = 3;
	for (int i = 0; offset >= 0; ++i) {

		if (edgeOrbits_[openSpots[i]] == 1) {
			--offset;
		}
		else {
			sum1 += CHOOSE[8 - i - 1][offset];
		}
	}

	// creates a code for all the corners, similar to step4Help
	int sum2 = 0;
	for (int i = 0; i < 7; ++i) {

		int numGreater = 0;
		for (int j = i + 1; j < 8; ++j) {

			if (cornerColors_[j] < cornerColors_[i]) {
				++numGreater;
			}
		}
		sum2 += (numGreater * FAC[8 - i - 1]);
	}

	// some multiplication to ensure unique codes for different cubes
	return sum1 * 40320 + sum2;
}

int Cube::step4Help(const Edge_t orbit[4], char cubies[12])
{
	// Creates a unique code for the cubies of an orbit
	// essentially, 0,1,2,3 => 0, 0,1,3,2 => 1, ... 3,2,1,0 => 23
	int sum = 0;
	for (int i = 0; i < 3; ++i) {

		int numGreater = 0;
		for (int j = i + 1; j < 4; ++j) {

			if (cubies[orbit[j]] < cubies[orbit[i]]) {
				++numGreater;
			}
		}
		sum += (numGreater * FAC[4 - i - 1]);
	}
	return sum;
}

int Cube::step4Code()
{
	// TODO: simplify code for step 4

	// creates a unique code based on the positions of the edges within
	// their orbits
	int sum = 0;
	sum += step4Help(UD_SLICE, edgeColors_);
	sum += step4Help(LR_SLICE, edgeColors_) * 24;
	sum += step4Help(FB_SLICE, edgeColors_)*pow(24, 2);

	// takes into the account the overall position of the corners
	int sum2 = 0;
	for (int i = 0; i < 7; ++i) {

		int numGreater = 0;
		for (int j = i + 1; j < 8; ++j) {

			if (cornerColors_[j] < cornerColors_[i]) {
				++numGreater;
			}
		}
		sum2 += (numGreater * FAC[8 - i - 1]);
	}
	sum += sum2*pow(24, 3);

	return sum;
}

// Code for a 2-face solve (for debugging only)
int Cube::code2()
{
	char edges[7] = { YO, YG, RG, OG, OB, WG, WO };
	char corners[6] = { YOB, YRG, YOG, WOB, WRG, WOG };

	int sum1 = 0;
	for (int i = 0; i < 5; ++i) {
		sum1 += cornerOrients_[corners[i]] * pow(3, i);
	}

	int sum2 = 0;
	for (int i = 0; i < 5; ++i) {
		int numGreater = 0;
		for (int j = i + 1; j < 6; ++j) {

			if (cornerColors_[corners[j]] < cornerColors_[corners[i]]) {
				++numGreater;
			}
		}
		sum2 += (numGreater * FAC[6 - i - 1]);
	}

	int sum3 = 0;
	for (int i = 0; i < 6; ++i) {
		int numGreater = 0;
		for (int j = i + 1; j < 7; ++j) {

			if (edgeColors_[edges[j]] < edgeColors_[edges[i]]) {
				++numGreater;
			}
		}
		sum3 += (numGreater * FAC[7 - i - 1]);
	}
	return sum1 + 243*sum2 + 243000*sum3;
}

void Cube::solve(Cube cube)
{
	// Retrieve the Turns to solve each step

	vector<Turn> step1 = doStep(cube, STEP1MAP, &step1Code, OK_TURNS1);
	std::cout << endl;

	vector<Turn> step2 = doStep(cube, STEP2MAP, &step2Code, OK_TURNS2);
	std::cout << endl;

	vector<Turn> step3 = doStep(cube, STEP3MAP, &step3Code, OK_TURNS3);
	std::cout << endl;

	vector<Turn> step4 = doStep(cube, STEP4MAP, &step4Code, OK_TURNS4);
	std::cout << endl;

	std::cout << "SUCCESS!!!!" << endl;

	// Print out all the steps

	printTurns(step1);
	printTurns(step2);
	printTurns(step3);
	printTurns(step4);
}

// Solve for 2-face (debugging only)
void Cube::solve2(Cube cube)
{

	vector<Turn> step2 = doStep(cube, TURNMAP2, &code2, OK_TURNS_2FACE);
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


queue<Cube> Cube::step4ValidCorners()
{
	// add a solved Cube to the queue
	Cube cube = solvedCube();
	vector<Cube> cubeQueue;
	cubeQueue.push_back(cube);

	unordered_set<int> cubeCodes; // for easy access to the cubeCodes
	cubeCodes.insert(cube.step4CornerCode());

	// generate the 96 unique corner orientations
	for (int i = 0; cubeCodes.size() < 96; ++i) {

		Cube currCube = cubeQueue[i];

		for (auto turn : OK_TURNS4) {

			Cube turnedCube = turn.turnFunc(currCube);
			int code = turnedCube.step4CornerCode();

			if (cubeCodes.count(code) == 0) {
				cubeQueue.push_back(turnedCube);
				cubeCodes.insert(code);
			}
		}

	}

	queue<Cube> queueToReturn;

	for (auto c : cubeQueue) {
		queueToReturn.push(c);
	}

	return queueToReturn;
}

int Cube::step4CornerCode()
{
	int sum = 0;

	for (int i = 0; i < 7; ++i) {

		int numGreater = 0;
		for (int j = i + 1; j < 8; ++j) {

			if (cornerColors_[j] < cornerColors_[i]) {
				++numGreater;
			}
		}
		sum += (numGreater * FAC[8 - i - 1]);
	}
	return sum;
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

Cube Cube::solvedCube()
{
	// creates a solved cube
	char eColors[12] = { 0,1,2,3,4,5,6,7,8,9,10,11 };
	char eOrients[12] = { 0,0,0,0,0,0,0,0,0,0,0,0 };
	char eOrbits[12] = { 0,1,1,0,2,2,2,2,0,1,1,0 };

	char cColors[8] = { 0,1,2,3,4,5,6,7 };
	char cOrients[8] = { 0,0,0,0,0,0,0,0 };

	Cube cube(eColors, eOrients, eOrbits, cColors, cOrients);

	return cube;
}


void Cube::printTurns(vector<Turn> turns) {
	for (auto turn : turns) {
		cout << turn.toString << " ";
	}
	cout << endl;
}

/*
void Cube::printTurns(vector<char> turns)
{
	for (auto turn : turns) {
		switch (turn) {

		case FRONT_2: std::cout << "F2"; break;
		case RIGHT_2: std::cout << "R2"; break;
		case BACK_2: std::cout << "B2"; break;
		case LEFT_2: std::cout << "L2"; break;
		case UP_2: std::cout << "U2"; break;
		case DOWN_2: std::cout << "D2"; break;

		case FRONT_INVERTED: std::cout << "F'"; break;
		case RIGHT_INVERTED: std::cout << "R'"; break;
		case BACK_INVERTED: std::cout << "B'"; break;
		case LEFT_INVERTED: std::cout << "L'"; break;
		case UP_INVERTED: std::cout << "U'"; break;
		case DOWN_INVERTED: std::cout << "D'"; break;

		default: std::cout << turn;
		}
		std::cout << " ";
	}
	std::cout << endl;
}
*/

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
/*
vector<MoveInstruction> Cube::invert(vector<MoveInstruction> turns)
{
	reverse(turns.begin(), turns.end());
	vector<char> flipped;

	for (auto step : turns) {
		switch (step) {
		case FRONT: step = FRONT_INVERTED; break;
		case RIGHT: step = RIGHT_INVERTED; break;
		case BACK: step = BACK_INVERTED; break;
		case LEFT: step = LEFT_INVERTED; break;
		case UP: step = UP_INVERTED; break;
		case DOWN: step = DOWN_INVERTED; break;

		case FRONT_INVERTED: step = FRONT; break;
		case RIGHT_INVERTED: step = RIGHT; break;
		case BACK_INVERTED: step = BACK; break;
		case LEFT_INVERTED: step = LEFT; break;
		case UP_INVERTED: step = UP; break;
		case DOWN_INVERTED: step = DOWN; break;
		}

		flipped.push_back(step);
	}

	return flipped;
}
*/
/*
MoveInstruction Cube::turnToChar(Cube(*turn)(Cube))
{
	if (turn == front) return FRONT;
	else if (turn == right) return RIGHT;
	else if (turn == back) return BACK;
	else if (turn == left) return LEFT;
	else if (turn == up) return UP;
	else if (turn == down) return DOWN;

	// return random numbers MoveInstructions
	else if (turn == front2) return FRONT_2;
	else if (turn == right2) return RIGHT_2;
	else if (turn == back2) return BACK_2;
	else if (turn == left2) return LEFT_2;
	else if (turn == up2) return UP_2;
	else if (turn == down2) return DOWN_2;

	else if (turn == frontI) return FRONT_INVERTED;
	else if (turn == rightI) return RIGHT_INVERTED;
	else if (turn == backI) return BACK_INVERTED;
	else if (turn == leftI) return LEFT_INVERTED;
	else if (turn == upI) return UP_INVERTED;
	else if (turn == downI) return DOWN_INVERTED;
}
*/
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

/////// building turn tables //////////
/*
void Cube::buildTurnTable1()
{
	int TURN_TABLE_1[2048][12];

	bitset<11> bits;

	Cube cube = solvedCube();
	for (int i = 0; i < 2048; ++i) {

		bits = bitset<11>(i);
		for (int bit = 0, int n = bits.size(); bit < n; ++bit) {
			cube.edgeOrients_[i] = bit;
		}
		
		int code = cube.step1Code();

		for (auto turn : OK_TURNS1) {

		}
	}

}
*/
void Cube::test()
{
	loadMap(TURNMAP2, "CubeMap2Face.ser");
	char response;

	do {
		std::cout << "enter maneuver (followed by a \".\")" << endl;
		
		char move;
		vector<Turn> maneuver;

		cin >> move;

		while (move != '.') {
			maneuver.push_back(Turn((MoveInstruction) move));
			cin >> move;
		}
		std::cout << "done entering" << endl;

		Cube cube = solvedCube();
		
		cube = doTurns(cube, maneuver);

		std::cout << "STARTING" << endl;

		solve2(cube);

		std::cout << endl << endl;
		std::cout << "would you like to continue? (y/n)" << endl;
		cin >> response;
	} while (response == 'y');
}


int main()
{
	Cube::initChoose();
	//Cube::init2();

	
	clock_t t;
	t = clock();

	Cube::test();

	std::cout << endl << endl;

	t = clock() - t;
	std::cout << "time: " << (float)t / CLOCKS_PER_SEC << endl;

	std::cout << endl << endl;
	
	system("pause");
	
	return 0;
}

/*
To do:

1) Build move tables
2) Shorten to two-phase
3) Serialize 6-face solve hash tables
4) Create different executables for building maps and solving cube
5) Account for symmetries



Notes:

1) Binary archive is about 5 times faster than text archive

2) Having the turn methods return a new cube is faster than
	having them take a reference to the cube, modify it, insert it into
	the queue, and then unturn it.

3) using ints was bout 4 times larger than chars (makes sense)
4) using enums for turns was also 3 times larger than chars
*/