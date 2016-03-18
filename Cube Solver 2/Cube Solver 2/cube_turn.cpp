#include "cube.h"
#include "comm-protocol.h"
#include <algorithm>
#include <vector>
#include <iostream>

using namespace std;
using CommProtocol::MoveInstruction;

// Vectors containing numbers that correspond to faces
const Cube::Edge_t Cube::FRONT_EDGES[4] = { YG, OG, WG, RG };
const Cube::Edge_t Cube::RIGHT_EDGES[4] = { YO, OB, WO, OG };
const Cube::Edge_t Cube::BACK_EDGES[4] = { YB, RB, WB, OB };
const Cube::Edge_t Cube::LEFT_EDGES[4] = { YR, RG, WR, RB };
const Cube::Edge_t Cube::UP_EDGES[4] = { YB, YO, YG, YR };
const Cube::Edge_t Cube::DOWN_EDGES[4] = { WG, WO, WB, WR };

const Cube::Corner_t Cube::FRONT_CORNERS[4] = { YRG, YOG, WOG, WRG };
const Cube::Corner_t Cube::RIGHT_CORNERS[4] = { YOG, YOB, WOB, WOG };
const Cube::Corner_t Cube::BACK_CORNERS[4] = { YOB, YRB, WRB, WOB };
const Cube::Corner_t Cube::LEFT_CORNERS[4] = { YRB, YRG, WRG, WRB };
const Cube::Corner_t Cube::UP_CORNERS[4] = { YRB, YOB, YOG, YRG };
const Cube::Corner_t Cube::DOWN_CORNERS[4] = { WRG, WOG, WOB, WRB };

// Corresponding edges in the slices
const Cube::Edge_t Cube::LR_SLICE[4] = { YB, YG, WB, WG };
const Cube::Edge_t Cube::FB_SLICE[4] = { YR, YO, WR, WO };
const Cube::Edge_t Cube::UD_SLICE[4] = { RB, OB, RG, OG };



Cube::Turn::Turn(MoveInstruction c) : repr{ c }
{

	switch (c) {

	case MoveInstruction::FRONT:
		turnFunc = front; toString = "F"; oppTurn = MoveInstruction::FRONT_INVERTED;
		break;
	case MoveInstruction::RIGHT:
		turnFunc = right; toString = "R"; oppTurn = MoveInstruction::RIGHT_INVERTED;
		break;
	case MoveInstruction::BACK:
		turnFunc = back; toString = "B"; oppTurn = MoveInstruction::BACK_INVERTED;
		break;
	case MoveInstruction::LEFT:
		turnFunc = left; toString = "L"; oppTurn = MoveInstruction::LEFT_INVERTED;
		break;
	case MoveInstruction::UP:
		turnFunc = up; toString = "U"; oppTurn = MoveInstruction::UP_INVERTED;
		break;
	case MoveInstruction::DOWN:
		turnFunc = down; toString = "D"; oppTurn = MoveInstruction::DOWN_INVERTED;
		break;

	case MoveInstruction::FRONT_2:
		turnFunc = front2; toString = "F2"; oppTurn = MoveInstruction::FRONT_2;
		break;
	case MoveInstruction::RIGHT_2:
		turnFunc = right2; toString = "R2"; oppTurn = MoveInstruction::RIGHT_2;
		break;
	case MoveInstruction::BACK_2:
		turnFunc = back2; toString = "B2"; oppTurn = MoveInstruction::BACK_2;
		break;
	case MoveInstruction::LEFT_2:
		turnFunc = left2; toString = "L2"; oppTurn = MoveInstruction::LEFT_2;
		break;
	case MoveInstruction::UP_2:
		turnFunc = up2; toString = "U2"; oppTurn = MoveInstruction::UP_2;
		break;
	case MoveInstruction::DOWN_2:
		turnFunc = down2; toString = "D2"; oppTurn = MoveInstruction::DOWN_2;
		break;

	case MoveInstruction::FRONT_INVERTED:
		turnFunc = frontI; toString = "F'"; oppTurn = MoveInstruction::FRONT;
		break;
	case MoveInstruction::RIGHT_INVERTED:
		turnFunc = rightI; toString = "R'"; oppTurn = MoveInstruction::RIGHT;
		break;
	case MoveInstruction::BACK_INVERTED:
		turnFunc = backI; toString = "B'"; oppTurn = MoveInstruction::BACK;
		break;
	case MoveInstruction::LEFT_INVERTED:
		turnFunc = leftI; toString = "L'"; oppTurn = MoveInstruction::LEFT;
		break;
	case MoveInstruction::UP_INVERTED:
		turnFunc = upI; toString = "U'"; oppTurn = MoveInstruction::UP;
		break;
	case MoveInstruction::DOWN_INVERTED:
		turnFunc = downI; toString = "D'"; oppTurn = MoveInstruction::DOWN;
		break;

	default: cout << "error: not a valid char" << endl;
	}
}

Cube Cube::turnWith(Turn inputTurn)
{
	return (inputTurn.turnFunc)(*this);
}

vector<Cube::Turn> Cube::Turn::movesToTurns(std::vector<MoveInstruction> moves)
{
	vector<Turn> turns;
	for (MoveInstruction m : moves) {
		turns.push_back(Turn(m));
	}
	return turns;
}


Cube Cube::front(Cube cube)
{
	cube.turn(FRONT_EDGES, FRONT_CORNERS);
	cube.orientCorners(FRONT_CORNERS);
	return cube;
}

Cube Cube::right(Cube cube)
{
	cube.turn(RIGHT_EDGES, RIGHT_CORNERS);
	return cube;
}

Cube Cube::back(Cube cube)
{
	cube.turn(BACK_EDGES, BACK_CORNERS);
	cube.orientCorners(BACK_CORNERS);
	return cube;
}

Cube Cube::left(Cube cube)
{
	cube.turn(LEFT_EDGES, LEFT_CORNERS);
	return cube;
}

Cube Cube::up(Cube cube)
{
	cube.turn(UP_EDGES, UP_CORNERS);
	cube.orientEdges(UP_EDGES);
	cube.orientCorners(UP_CORNERS);
	return cube;
}

Cube Cube::down(Cube cube)
{
	cube.turn(DOWN_EDGES, DOWN_CORNERS);
	cube.orientEdges(DOWN_EDGES);
	cube.orientCorners(DOWN_CORNERS);
	return cube;
}

void Cube::turn(const Edge_t faceEdges[4], const Corner_t faceCorners[4])
{
	forwardCycle(faceEdges, edgeColors_);
	forwardCycle(faceEdges, edgeOrients_);
	forwardCycle(faceEdges, edgeOrbits_);

	forwardCycle(faceCorners, cornerColors_);
	forwardCycle(faceCorners, cornerOrients_);
}

template <typename Index_t, typename Cubie_t>
void Cube::forwardCycle(const Index_t positions[4], Cubie_t* cubies)
{
	Cubie_t temp = cubies[positions[3]];
	for (size_t i = 3; i > 0; --i) {
		cubies[positions[i]] = cubies[positions[i - 1]];
	}
	cubies[positions[0]] = temp;
}

Cube Cube::frontI(Cube cube)
{
	cube.turnI(FRONT_EDGES, FRONT_CORNERS);
	cube.orientCorners(FRONT_CORNERS);
	return cube;
}

Cube Cube::rightI(Cube cube)
{
	cube.turnI(RIGHT_EDGES, RIGHT_CORNERS);
	return cube;
}

Cube Cube::backI(Cube cube)
{
	cube.turnI(BACK_EDGES, BACK_CORNERS);
	cube.orientCorners(BACK_CORNERS);
	return cube;
}

Cube Cube::leftI(Cube cube)
{
	cube.turnI(LEFT_EDGES, LEFT_CORNERS);
	return cube;
}

Cube Cube::upI(Cube cube)
{
	cube.turnI(UP_EDGES, UP_CORNERS);
	cube.orientEdges(UP_EDGES);
	cube.orientCorners(UP_CORNERS);
	return cube;
}

Cube Cube::downI(Cube cube)
{
	cube.turnI(DOWN_EDGES, DOWN_CORNERS);
	cube.orientEdges(DOWN_EDGES);
	cube.orientCorners(DOWN_CORNERS);
	return cube;
}


void Cube::turnI(const Edge_t faceEdges[4],
	const Corner_t faceCorners[4])
{
	backwardCycle(faceEdges, edgeColors_);
	backwardCycle(faceEdges, edgeOrients_);
	backwardCycle(faceEdges, edgeOrbits_);

	backwardCycle(faceCorners, cornerColors_);
	backwardCycle(faceCorners, cornerOrients_);
}

template <typename Index_t, typename Cubie_t>
void Cube::backwardCycle(const Index_t positions[4], Cubie_t* cubies)
{
	Cubie_t temp = cubies[positions[0]];
	for (size_t i = 0; i < 3; ++i) {
		cubies[positions[i]] = cubies[positions[i + 1]];
	}
	cubies[positions[3]] = temp;
}



Cube Cube::front2(Cube cube)
{
	cube.turn2(FRONT_EDGES, FRONT_CORNERS);
	return cube;
}

Cube Cube::right2(Cube cube)
{
	cube.turn2(RIGHT_EDGES, RIGHT_CORNERS);
	return cube;
}

Cube Cube::back2(Cube cube)
{
	cube.turn2(BACK_EDGES, BACK_CORNERS);
	return cube;
}

Cube Cube::left2(Cube cube)
{
	cube.turn2(LEFT_EDGES, LEFT_CORNERS);
	return cube;
}

Cube Cube::up2(Cube cube)
{
	cube.turn2(UP_EDGES, UP_CORNERS);
	return cube;
}

Cube Cube::down2(Cube cube)
{
	cube.turn2(DOWN_EDGES, DOWN_CORNERS);
	return cube;
}

void Cube::turn2(const Edge_t faceEdges[4], const Corner_t faceCorners[4])
{
	// do double turn by swapping pairs of cubies.  No change in orientation.
	swap(edgeColors_[faceEdges[0]], edgeColors_[faceEdges[2]]);
	swap(edgeColors_[faceEdges[1]], edgeColors_[faceEdges[3]]);
	swap(edgeOrients_[faceEdges[0]], edgeOrients_[faceEdges[2]]);
	swap(edgeOrients_[faceEdges[1]], edgeOrients_[faceEdges[3]]);
	swap(edgeOrbits_[faceEdges[0]], edgeOrbits_[faceEdges[2]]);
	swap(edgeOrbits_[faceEdges[1]], edgeOrbits_[faceEdges[3]]);

	swap(cornerColors_[faceCorners[0]], cornerColors_[faceCorners[2]]);
	swap(cornerColors_[faceCorners[1]], cornerColors_[faceCorners[3]]);
	swap(cornerOrients_[faceCorners[0]], cornerOrients_[faceCorners[2]]);
	swap(cornerOrients_[faceCorners[1]], cornerOrients_[faceCorners[3]]);
}


void Cube::orientEdges(const Edge_t edges[4])
{
	// edge orientations are changed identically (wrt the face) for U,D,U',D'
	for (int i = 0; i < 4; ++i) {
		edgeOrients_[edges[i]] = !edgeOrients_[edges[i]];
	}
}

void Cube::orientCorners(const Corner_t pos[4])
{
	// corner orientations also change the same for U,D,U',D',L,R,L',R'
	cornerOrients_[pos[0]] = (cornerOrients_[pos[0]] + 1) % 3;
	cornerOrients_[pos[1]] = (cornerOrients_[pos[1]] + 2) % 3;
	cornerOrients_[pos[2]] = (cornerOrients_[pos[2]] + 1) % 3;
	cornerOrients_[pos[3]] = (cornerOrients_[pos[3]] + 2) % 3;
}
