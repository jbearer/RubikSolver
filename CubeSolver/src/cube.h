#pragma once

#include "turn.h"

namespace CubeSolver {

	const int NUM_TURNS_STEP1 = 12;
	const int NUM_TURNS_STEP2 = 8;

	// Lists the allowable turns for each step
	extern Turn TURNS_STEP1[NUM_TURNS_STEP1];
	extern Turn TURNS_STEP2[NUM_TURNS_STEP2];

	/**
	* \class Cube
	* \brief Stores a cube, composed of 12 Edge_t's and 8 Corner_t's
	* \detail 	A cube is uniquely defined by the position of its 12 edges,
	*			its 8 corners, and their orientations.  Each edge has two
	* 			possible orientations {0,1} and each corner has three
	*			possibilities {0,1,2}. Also contains methods to turn the cube
	*/
	class Cube 
	{

	public:

		/**
		* \enum Edge_t
		* \brief 	Each enum refers to a particular edge of the Rubiks cube
		* \detail 	Enums are shorthand for their color:
		*			Y(ellow) B(lue) R(ed) O(range) G(reen) W(hite).
		*			Each pair represents the edge at the intersection of faces
		*/
		enum Edge_t { YB, YR, YO, YG, RB, OB, RG, OG, WB, WR, WO, WG };
		
		/**
		* \enum Corner_t
		* \brief	Each enum represents one of eight corners of the cube
		* \detail 	Similar to Edge_t, each is the intersection of three faces
		*/
		enum Corner_t { YRB, YOB, YRG, YOG, WRB, WOB, WRG, WOG };

		/// Some useful constants
		static const int NUM_EDGES = 12;
		static const int NUM_CORNERS = 8;
		static const int EDGES_PER_FACE = 4;
		static const int CORNERS_PER_FACE = 4;
			

		/// arrays containing enums that correspond to faces
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


		///////// Constructors //////////

		/// creates a default solved cube
		Cube() = default;

		/// Four-argument parameterized constructor.
		Cube(Edge_t eColors[NUM_EDGES], int eOrients[NUM_EDGES],
			Corner_t cColors[NUM_CORNERS], int cOrients[NUM_CORNERS]);

		Cube(const Cube& rhs) = default;

		/// Two cubes are equal if the edges and corners have the same
		/// positions and orientations (i.e. all the arrays are equal)
		bool operator==(const Cube& rhs);
		bool operator!=(const Cube& rhs);

		/// Returns true if this is a solved cube
		bool isSolved();


		/**
		* \fn 		turn
		* \detail 	turn takes in a copy of a cube and a Turn.  It then applies
		* the turn by calling one of turnRightOrLeft, turnFrontOrBack, or
		* turnUpOrDown depending on which face it is turning.  Also, F2 is
		* encoded as 2 F turns, and F' is encoded as three F turns.
		*/
		static Cube turn(Cube cube, Turn turn);
		


	private:

		friend class CubeEncoder;
		friend std::ostream& operator<<(std::ostream& out, const Cube& cube);

		/**
		* \fn 		turnRightOrLeft
		* \detail 	Given a list of edges and corners that correspond to either
		* 			the left of the right face, calls turnCubies to cycle the
		*			cubies.  No reorientation is necessary for right or left
		*/
		void turnRightOrLeft(const Edge_t edges[EDGES_PER_FACE],
							 const Corner_t corners[CORNERS_PER_FACE]);
		
		/**
		* \fn 		turnFrontOrBack
		* \detail 	Given a list of edges and corners that correspond to either
		* 			the front of the back face, calls turnCubies to cycle the
		*			cubies.  Must reorient the edges and corners
		*/
		void turnFrontOrBack(const Edge_t edges[EDGES_PER_FACE],
						     const Corner_t corners[CORNERS_PER_FACE]);
		
		/**
		* \fn 		turnUpOrDown
		* \detail 	Same as turnFrontOrBack, but only needs to reorient edges
		*/
		void turnUpOrDown(const Edge_t edges[EDGES_PER_FACE],
						  const Corner_t corners[CORNERS_PER_FACE]);

		/// Applies a turn twice
		void turn2(Turn turn);

		/// Applies a turn three times, which is the inverse of turn
		void turnI(Turn turn);

		/**
		* \fn		turnCubies
		* \brief	Calls forwardCycle on the data members of Cube.
		*/
		void turnCubies(const Edge_t edges[EDGES_PER_FACE],
					    const Corner_t corners[CORNERS_PER_FACE]);

		/**
		* \fn		forwardCycle
		* \brief	Cycles the numbers at the given indices forward (i.e. clockwise on the cube)
		* \details	For example, then numbers at indices {1,3,5,6} will now be at {6,1,3,5}
		* \params	Positions will be an array of edges or corners corresponding to the indices
		* 			to cycle.  cubies will be either edgeOrients_, edgeColors_, cornerOrients_,
		*			or cornerColors_
		*/
		template <typename Index_t, typename Cubie_t>
		void forwardCycle(const Index_t positions[4], Cubie_t* cubies);

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

		/// data members set to a solved cube.

		Edge_t edgeColors_[NUM_EDGES] = {YB, YR, YO, YG, RB, OB, RG, OG, WB, WR, WO, WG };
		int edgeOrients_[NUM_EDGES] = { 0,0,0,0,0,0,0,0,0,0,0,0 };  ///< 0 is oriented, 1 is unoriented
		Corner_t cornerColors_[NUM_CORNERS] = { YRB, YOB, YRG, YOG, WRB, WOB, WRG, WOG };
		int cornerOrients_[NUM_CORNERS] = { 0,0,0,0,0,0,0,0 };	///< 0 is oriented, 1 is clockwise, 2 is counterclockwise

	}; // end class Cube

	std::ostream& operator<<(std::ostream& out, const Cube& cube);

} // end namespace CubeSolver