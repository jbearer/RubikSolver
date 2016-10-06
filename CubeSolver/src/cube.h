#pragma once

/**
* \class Cube
* \brief Stores a cube, composed of 12 Edge_t's and 8 Corner_t's
* Contains methods to turn the cube
*/

#include "turn.h"

using CommProtocol::MoveInstruction;

namespace CubeSolver {

// Lists the allowable turns for each step
extern MoveInstruction OK_TURNS1[NUM_TURNS_STEP1];
extern MoveInstruction OK_TURNS2[NUM_TURNS_STEP2];

class Cube
{
public:

	/// Define enums referring to specific edge and corner pieces
	enum Edge_t { YB, YR, YO, YG, RB, OB, RG, OG, WB, WR, WO, WG };
	enum Corner_t { YRB, YOB, YRG, YOG, WRB, WOB, WRG, WOG };

	/// Some useful constants
	static const int NUM_EDGES = 12;
	static const int NUM_CORNERS = 8;
	static const int EDGES_PER_FACE = 4;
	static const int CORNERS_PER_FACE = 4;
		

	/// arrays containing numbers that correspond to faces
	static const Edge_t FRONT_EDGES[EDGES_PER_FACE];
	static const Edge_t RIGHT_EDGES[EDGES_PER_FACE];
	static const Edge_t LEFT_EDGES[EDGES_PER_FACE];
	static const Edge_t BACK_EDGES[EDGES_PER_FACE];
	static const Edge_t UP_EDGES[EDGES_PER_FACE];
	static const Edge_t DOWN_EDGES[EDGES_PER_FACE];

	static const Corner_t FRONT_CORNERS[CORNERS_PER_FACE];
	static const Corner_t RIGHT_CORNERS[CORNERS_PER_FACE];
	static const Corner_t LEFT_CORNERS[CORNERS_PER_FACE];
	static const Corner_t BACK_CORNERS[CORNERS_PER_FACE];
	static const Corner_t UP_CORNERS[CORNERS_PER_FACE];
	static const Corner_t DOWN_CORNERS[CORNERS_PER_FACE];

	/// The four edges in the left-right slice (meridian of the cube)
	static const Edge_t LR_SLICE[EDGES_PER_FACE];

	///////// Constructors //////////

	/// creates a default solved cube
	Cube() = default;

	/// Four-argument parameterized constructor.  Constructs a cube
	/// out of edgeColors, edgeOrients, cornerColors, and cornerOrients
	Cube(Edge_t eColors[NUM_EDGES], int eOrients[NUM_EDGES],
		Corner_t cColors[NUM_CORNERS], int cOrients[NUM_CORNERS]);

	Cube(const Cube& rhs) = default;

	bool operator==(const Cube& rhs);
	bool operator!=(const Cube& rhs);

	// TODO: make operator<<
	/// Prints the cube in the form edgeColors, edgeOrients, cornerColors, cornerOrients
	void print() const;

	/// Returns true if this is a solved cube
	bool isSolved();

	
	//void orientEdges(MoveInstruction);
	//void cycle(MoveInstruction mi);

	/**
	* \brief Turning methods.  Each of the following static methods takes in a
	* copy of a cube.  It then applies the appropriate turn by cycling the edges and corners
	* and fixing orients.  The first six methods represent clockwise turns, the following
	* are CC (inverse) turns, then double turns.  All the methods require cycling the
	* arrays.  up and down require edge reorientation, and up down front and back
	* require corner orientation.
	*
	*/

	static Cube turn(Cube cube, MoveInstruction mi);
	void turnRightOrLeft(const Edge_t edges[EDGES_PER_FACE], const Corner_t corners[CORNERS_PER_FACE]);
	void turnFrontOrBack(const Edge_t edges[EDGES_PER_FACE], const Corner_t corners[CORNERS_PER_FACE]);
	void turnUpOrDown(const Edge_t edges[EDGES_PER_FACE], const Corner_t corners[CORNERS_PER_FACE]);

	void turn2(MoveInstruction mi);
	void turnI(MoveInstruction mi);


	


private:

	friend class CubeEncoder;

	/////////////////// TURN METHODS ////////////////

	/**
	* \fn		turn
	* \brief	Calls forwardCycle on the data members of Cube.
	*/
	void turnCubies(const Edge_t edges[EDGES_PER_FACE], const Corner_t corners[CORNERS_PER_FACE]);

	

	/**
	* \fn		turnI
	* \brief	Calls backwardCycle on the data members of Cube.
	*/
	//void turnI(const Edge_t edges[4], const Corner_t corners[4]);

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
	//template <typename Index_t, typename Cubie_t>
	//void backwardCycle(const Index_t positions[4], Cubie_t* cubies);


	/**
	* \fn		orientEdges
	* \brief	Inverts the orientation of the edges at a given face.
	* \details	Used in the turning methods up and down.
	*/
	void orientEdges(const Edge_t edges[EDGES_PER_FACE]);

	/**
	* \fn		orientCorners
	* \brief	Changes the orientations of the corners at given face to represent a turn.
	* \details	Used in the turning methods up, down, left, and right.  Conveniently,
	*			the orientations for respective pieces changes in the same way for
	*			each of these turns.
	*/
	void orientCorners(const Corner_t corners[CORNERS_PER_FACE]);

	/**
	* \fn		turn2
	* \brief	Swaps different indices of the cube data members to represent a double
	*			turn on a Cube.
	* \details	Helper method for front2, right2, etc.
	*/
	//void turn2(const Edge_t edges[4], const Corner_t corners[4]);



	/// data members set to a solved cube.
	Edge_t edgeColors_[NUM_EDGES] = {YB, YR, YO, YG, RB, OB, RG, OG, WB, WR, WO, WG };	///< Numbers represent colors (see table of consts above)
	int edgeOrients_[NUM_EDGES] = { 0,0,0,0,0,0,0,0,0,0,0,0 };  ///< 0 is oriented, 1 is unoriented
	Corner_t cornerColors_[NUM_CORNERS] = { YRB, YOB, YRG, YOG, WRB, WOB, WRG, WOG };	///< Numbers represent colors (see table of consts above)
	int cornerOrients_[NUM_CORNERS] = { 0,0,0,0,0,0,0,0 };	///< 0 is oriented, 1 is clockwise, 2 is counterclockwise

}; // end class Cube

} // end namespace CubeSolver