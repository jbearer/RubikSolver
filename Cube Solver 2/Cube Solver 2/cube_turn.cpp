#include "cube.h"
#include <algorithm>
#include <vector>
#include <iostream>

using namespace std;

// Vectors containing numbers that correspond to faces
const vector<Cube::Edge_t> Cube::FRONT_EDGES({ YG, OG, WG, RG });
const vector<Cube::Edge_t> Cube::RIGHT_EDGES({ YO, OB, WO, OG });
const vector<Cube::Edge_t> Cube::BACK_EDGES({ YB, RB, WB, OB });
const vector<Cube::Edge_t> Cube::LEFT_EDGES({ YR, RG, WR, RB });
const vector<Cube::Edge_t> Cube::UP_EDGES({ YB, YO, YG, YR });
const vector<Cube::Edge_t> Cube::DOWN_EDGES({ WG, WO, WB, WR });

const vector<Cube::Corner_t> Cube::FRONT_CORNERS({ YRG, YOG, WOG, WRG });
const vector<Cube::Corner_t> Cube::RIGHT_CORNERS({ YOG, YOB, WOB, WOG });
const vector<Cube::Corner_t> Cube::BACK_CORNERS({ YOB, YRB, WRB, WOB });
const vector<Cube::Corner_t> Cube::LEFT_CORNERS({ YRB, YRG, WRG, WRB });
const vector<Cube::Corner_t> Cube::UP_CORNERS({ YRB, YOB, YOG, YRG });
const vector<Cube::Corner_t> Cube::DOWN_CORNERS({ WRG, WOG, WOB, WRB });

// Corresponding edges in the slices
const vector<Cube::Edge_t> Cube::LR_SLICE({ YB, YG, WB, WG });
const vector<Cube::Edge_t> Cube::FB_SLICE({ YR, YO, WR, WO });
const vector<Cube::Edge_t> Cube::UD_SLICE({ RB, OB, RG, OG });


Cube::Turn::Turn(char c) : repr{ c }
{

	switch (c) {

	case 'F':
		turnFunc = front; toString = "F"; oppTurn = frontI;
		break;
	case 'R':
		turnFunc = right; toString = "R"; oppTurn = rightI;
		break;
	case 'B':
		turnFunc = back; toString = "B"; oppTurn = backI;
		break;
	case 'L':
		turnFunc = left; toString = "L"; oppTurn = leftI;
		break;
	case 'U':
		turnFunc = up; toString = "U"; oppTurn = upI;
		break;
	case 'D':
		turnFunc = down; toString = "D"; oppTurn = downI;
		break;

	case 'a':
		turnFunc = front2; toString = "F2"; oppTurn = front2;
		break;
	case 'c':
		turnFunc = right2; toString = "R2"; oppTurn = right2;
		break;
	case 'e':
		turnFunc = back2; toString = "B2"; oppTurn = back2;
		break;
	case 'g':
		turnFunc = left2; toString = "L2"; oppTurn = left2;
		break;
	case 'h':
		turnFunc = up2; toString = "U2"; oppTurn = up2;
		break;
	case 'i':
		turnFunc = down2; toString = "D2"; oppTurn = down2;
		break;
	case 'f':
		turnFunc = frontI; toString = "F'"; oppTurn = front;
		break;
	case 'r':
		turnFunc = rightI; toString = "R'"; oppTurn = right;
		break;
	case 'b':
		turnFunc = backI; toString = "B'"; oppTurn = back;
		break;
	case 'l':
		turnFunc = leftI; toString = "L'"; oppTurn = left;
		break;
	case 'u':
		turnFunc = upI; toString = "U'"; oppTurn = up;
		break;
	case 'd':
		turnFunc = downI; toString = "D'"; oppTurn = down;
		break;

	default: cout << "error: not a valid char" << endl;
	}
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

void Cube::turn(const vector<Edge_t>& faceEdges,
	const vector<Corner_t>& faceCorners)
{
	forwardCycle(faceEdges, edgeColors_);
	forwardCycle(faceEdges, edgeOrients_);
	forwardCycle(faceEdges, edgeOrbits_);

	forwardCycle(faceCorners, cornerColors_);
	forwardCycle(faceCorners, cornerOrients_);
}

template <typename Cubie>
void Cube::forwardCycle(const vector<Cubie>& positions, char* cubies)
{
	char temp = cubies[positions[3]];
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

void Cube::turnI(const vector<Edge_t>& faceEdges,
	const vector<Corner_t>& faceCorners)
{
	backwardCycle(faceEdges, edgeColors_);
	backwardCycle(faceEdges, edgeOrients_);
	backwardCycle(faceEdges, edgeOrbits_);

	backwardCycle(faceCorners, cornerColors_);
	backwardCycle(faceCorners, cornerOrients_);
}

template <typename Cubie>
void Cube::backwardCycle(const vector<Cubie>& positions, char* cubies)
{
	char temp = cubies[positions[0]];
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

void Cube::turn2(const vector<Edge_t>& faceEdges, const vector<Corner_t>& faceCorners)
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


void Cube::orientEdges(const vector<Edge_t>& edges)
{
	// edge orientations are changed identically (wrt the face) for U,D,U',D'
	for (char pos : edges) {
		edgeOrients_[pos] = !edgeOrients_[pos];
	}
}

void Cube::orientCorners(const vector<Corner_t>& pos)
{
	// corner orientations also change the same for U,D,U',D',L,R,L',R'
	cornerOrients_[pos[0]] = (cornerOrients_[pos[0]] + 1) % 3;
	cornerOrients_[pos[1]] = (cornerOrients_[pos[1]] + 2) % 3;
	cornerOrients_[pos[2]] = (cornerOrients_[pos[2]] + 1) % 3;
	cornerOrients_[pos[3]] = (cornerOrients_[pos[3]] + 2) % 3;
}
