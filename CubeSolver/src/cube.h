#pragma once

#ifndef CUBE_H_INCLUDED
#define CUBE_H_INCLUDED 1

/**
* \class Cube
* \brief Stores a cube, composed of 12 Edge_t's and 8 Corner_t's
* Contains methods to turn the cube
*/

//#include "../util/combo_util.hpp"
#include "turn.h"
#include "../../MotorControl/src/arduino-incl/comm-protocol.h"

// Bad to use using?
using CommProtocol::MoveInstruction;

static Turn OK_TURNS1[NUM_TURNS_STEP1] = {
	Turn(MoveInstruction::FRONT), Turn(MoveInstruction::RIGHT), Turn(MoveInstruction::BACK), Turn(MoveInstruction::LEFT),
	Turn(MoveInstruction::UP), Turn(MoveInstruction::DOWN), Turn(MoveInstruction::FRONT_INVERTED), Turn(MoveInstruction::RIGHT_INVERTED),
	Turn(MoveInstruction::BACK_INVERTED), Turn(MoveInstruction::LEFT_INVERTED), Turn(MoveInstruction::UP_INVERTED), Turn(MoveInstruction::DOWN_INVERTED) };

static Turn OK_TURNS2[NUM_TURNS_STEP2] = {
	Turn(MoveInstruction::FRONT_2), Turn(MoveInstruction::RIGHT), Turn(MoveInstruction::BACK_2), Turn(MoveInstruction::LEFT),
	Turn(MoveInstruction::UP_2), Turn(MoveInstruction::DOWN_2), Turn(MoveInstruction::RIGHT_INVERTED), Turn(MoveInstruction::LEFT_INVERTED) };


class Cube
{
public:

	/// Define enums referring to specific edge and corner pieces
	enum Edge_t { YB, YR, YO, YG, RB, OB, RG, OG, WB, WR, WO, WG };
	enum Corner_t { YRB, YOB, YRG, YOG, WRB, WOB, WRG, WOG };

	/// Some useful constants
	static const int NUM_EDGES = 12;
	static const int NUM_CORNERS = 8;	

	/// vectors containing numbers that correspond to faces
	static const Edge_t FRONT_EDGES[4];
	static const Edge_t RIGHT_EDGES[4];
	static const Edge_t LEFT_EDGES[4];
	static const Edge_t BACK_EDGES[4];
	static const Edge_t UP_EDGES[4];
	static const Edge_t DOWN_EDGES[4];

	static const Corner_t FRONT_CORNERS[4];
	static const Corner_t RIGHT_CORNERS[4];
	static const Corner_t LEFT_CORNERS[4];
	static const Corner_t BACK_CORNERS[4];
	static const Corner_t UP_CORNERS[4];
	static const Corner_t DOWN_CORNERS[4];

	/// Vectors corresponding to specific edge orbits
	static const Edge_t UD_SLICE[4];
	static const Edge_t FB_SLICE[4];
	static const Edge_t LR_SLICE[4];

	///////// Constructors //////////

	/// creates a default solved cube
	Cube() = default;

	/// Four-argument parameterized constructor.  Constructs a cube
	/// out of edgeColors, edgeOrients, cornerColors, and cornerOrients
	Cube(Edge_t eColors[12], char eOrients[12],
		Corner_t cColors[8], char cOrients[8]);

	Cube(const Cube& rhs) = default;

	bool operator==(const Cube& rhs);
	bool operator!=(const Cube& rhs);

	// TODO: make operator<<
	/// Prints the cube in the form edgeColors, edgeOrients, cornerColors, cornerOrients
	void print() const;


	bool isSolved();

	static Cube turn(Cube cube, MoveInstruction mi);
	void orientEdges(MoveInstruction);
	void cycle(MoveInstruction mi);

	/**
	* \brief Turning methods.  Each of the following static methods takes in a
	* copy of a cube.  It then applies the appropriate turn by cycling the edges and corners
	* and fixing orients.  The first six methods represent clockwise turns, the following
	* are CC (inverse) turns, then double turns.  All the methods require cycling the
	* arrays.  up and down require edge reorientation, and up down front and back
	* require corner orientation.
	*
	*/
	static Cube front(Cube cube);
	static Cube right(Cube cube);
	static Cube back(Cube cube);
	static Cube left(Cube cube);
	static Cube up(Cube cube);
	static Cube down(Cube cube);

	static Cube frontI(Cube cube);
	static Cube rightI(Cube cube);
	static Cube backI(Cube cube);
	static Cube leftI(Cube cube);
	static Cube upI(Cube cube);
	static Cube downI(Cube cube);

	static Cube front2(Cube cube);
	static Cube right2(Cube cube);
	static Cube back2(Cube cube);
	static Cube left2(Cube cube);
	static Cube up2(Cube cube);
	static Cube down2(Cube cube);

	//////////////////////// CUBE CODES /////////////////////

	/**
	* \fn		step1Code
	* \brief	Hashes the cube into an integer
	* \details	Cubes had unique codes based only on edge orientation.
	* \return	int
	*/
	ushort edgeOrientsCode() const;

	/// same as edgeOrientsCode, but for corners
	ushort cornerOrientsCode() const;

	ushort cornerColorsCode() const;

	ushort edgeOrbitsCode() const;

	ushort edgeColorsCode1() const;
	ushort edgeColorsCode2() const;

	/// increments edge orientation to easily loop through them.  Returns false
	/// if the edgeOrients was {1,1,1...}, i.e. the "last" orientation.
	bool nextEdgeOrients();

	/// increments corner orientation to easily loop through them.  Returns false
	/// if the cornerOrients was {2,2,2...}, i.e. the "last" orientation.
	bool nextCornerOrients();

	bool nextEdgeOrbits();
	bool nextCornerColors();

	bool nextEdgeColors1();
	bool nextEdgeColors2();

	Cube turnWith(Turn inputTurn);

	// todo: get rid of
	void setFirstEdgeOrbits();


private:

	/////////////////// TURN METHODS ////////////////

	/**
	* \fn		turn
	* \brief	Calls forwardCycle on the data members of Cube.
	*/
	void turn(const Edge_t edges[4], const Corner_t corners[4]);

	

	/**
	* \fn		turnI
	* \brief	Calls backwardCycle on the data members of Cube.
	*/
	void turnI(const Edge_t edges[4], const Corner_t corners[4]);

	/**
	* \fn		forwardCycle
	* \brief	Cycles the numbers at the given indices forward (i.e. clockwise on the cube)
	* \details	For example, then numbers at indices {1,3,5,6} will now be at {6,1,3,5}
	*/
	template <typename Index_t, typename Cubie_t>
	void forwardCycle(const Index_t positions[4], Cubie_t* cubies);

	/**
	* \fn		forwardCycle
	* \brief	Cycles the numbers at the given indices backward (i.e. CC on the cube)
	* \details	For example, then numbers at indices {1,3,5,6} will now be at {3,5,6,1}
	*/
	template <typename Index_t, typename Cubie_t>
	void backwardCycle(const Index_t positions[4], Cubie_t* cubies);


	/**
	* \fn		orientEdges
	* \brief	Inverts the orientation of the edges at a given face.
	* \details	Used in the turning methods up and down.
	*/
	void orientEdges(const Edge_t edges[4]);

	/**
	* \fn		orientCorners
	* \brief	Changes the orientations of the corners at given face to represent a turn.
	* \details	Used in the turning methods up, down, left, and right.  Conveniently,
	*			the orientations for respective pieces changes in the same way for
	*			each of these turns.
	*/
	void orientCorners(const Corner_t corners[4]);

	/**
	* \fn		turn2
	* \brief	Swaps different indices of the cube data members to represent a double
	*			turn on a Cube.
	* \details	Helper method for front2, right2, etc.
	*/
	void turn2(const Edge_t edges[4], const Corner_t corners[4]);



	/// data members set to a solved cube.
	Edge_t edgeColors_[12] = {YB, YR, YO, YG, RB, OB, RG, OG, WB, WR, WO, WG };	///< Numbers represent colors (see table of consts above)
	char edgeOrients_[12] = { 0,0,0,0,0,0,0,0,0,0,0,0 };  ///< 0 is oriented, 1 is unoriented
	Corner_t cornerColors_[8] = { YRB, YOB, YRG, YOG, WRB, WOB, WRG, WOG };	///< Numbers represent colors (see table of consts above)
	char cornerOrients_[8] = { 0,0,0,0,0,0,0,0 };	///< 0 is oriented, 1 is clockwise, 2 is counterclockwise

};

#endif //CUBE_NUMS_H_INCLUDED
