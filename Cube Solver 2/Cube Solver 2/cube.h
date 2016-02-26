#pragma once
/**
* \file cube.h
* \brief interface for Cube class
*/
#include "comm-protocol.h"

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <queue>

/// Memoized the necessary factorials and combinations
static int FAC[12];
static int CHOOSE[12][12];

/**
* \class Cube
* \brief Contains the implementation for a Rubik's Cube
* \details A rubik's cube is identified by it's edge colors,
*		   edge orientations, corner colors, and corner orientations
*		   the ith index of an array corresponds to the cubie
*		   in the space that is home to cubie i.
*/
class Cube {

	/// forward declare Turn struct
	struct Turn;
public:

	/// Memoizes combinatorial values for calculating Cube codes
	static void initChoose();

	/// Initializes constants and loads the hash tables
	static void init();

	/// for two-face solve
	static void init2();

	/// Scramble and solve a test cube
	static void test();

	////// Constructors ///////
	Cube() = delete;

	/// Five-argument parameterized constructor.  Constructs a cube
	/// out of edgeColors, edgeOrients, cornerColors, and cornerOrients
	Cube(char eColors[12], char eOrients[12], char eOrbits[12],
		char cColors[8], char cOrients[8]);

	Cube(const Cube& rhs) = default; ///< Default copy constructor


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


	/**
	* \fn		solve
	* \brief	Solves the cube by running steps 1 through 4.
	* \details	Uses Thistlethwaite's method.  First uses F,R,B,L,U,D to orient
	*			the edges.  Then uses F,R,B,L,U2,D2 to finish step 2 (and this
	*			combination of moves will prevent the edges from becoming
	*			unoriented.  Then uses F2,R,B2,L,U2,D2 to finish step 3.
	*			Finally, uses F2,R2,B2,L2,U2,D2 to solve the rest.
	*			In the end, it prints out the steps taken to solve the cube
	*/
	static void solve(Cube cube);

	/// for two-turn solve
	static void solve2(Cube cube);

	/// Creates an instance of a solved Cube
	static Cube solvedCube();

	/// Prints the cube in the form edgeColors, edgeOrients, cornerColors, cornerOrients
	void print() const;

private:

	/// A turn struct, to easily convert between representations
	struct Turn {
		/// Constructors
		Turn() = delete;
		Turn(MotorControl::MoveInstruction c);

		/// Data members
		Cube(*turnFunc)(Cube);
		std::string toString;
		MotorControl::MoveInstruction repr;
		MotorControl::MoveInstruction oppTurn;

		/// Construct a vector of Turns from a vector of MoveInstructions
		static std::vector<Turn> movesToTurns(std::vector<MotorControl::MoveInstruction> moves);
	};

	/// Some useful constants
	static const char NUM_EDGES = 12;
	static const char NUM_CORNERS = 8;
	
	/// Define enums referring to specific edge and corner pieces
	enum Edge_t { YB, YR, YO, YG, RB, OB, RG, OG, WB, WR, WO, WG};
	enum Corner_t { YRB, YOB, YRG, YOG, WRB, WOB, WRG, WOG };

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

	/// Hash maps from cube codes to the required turns to solve
	static std::unordered_map<int, std::vector<MotorControl::MoveInstruction>> STEP1MAP;
	static std::unordered_map<int, std::vector<MotorControl::MoveInstruction>> STEP2MAP;
	static std::unordered_map<int, std::vector<MotorControl::MoveInstruction>> STEP3MAP;
	static std::unordered_map<int, std::vector<MotorControl::MoveInstruction>> STEP4MAP;

	static std::unordered_map < int, std::vector<MotorControl::MoveInstruction>> TURNMAP2;

	/// Vectors with function pointers to allowable turns
	static const std::vector<Turn> OK_TURNS1;
	static const std::vector<Turn> OK_TURNS2;
	static const std::vector<Turn> OK_TURNS3;
	static const std::vector<Turn> OK_TURNS4;

	static const std::vector<Turn> OK_TURNS_2FACE;


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
	template <typename Cubie>
	void forwardCycle(const Cubie positions[4], char* cubies);

	/**
	* \fn		forwardCycle
	* \brief	Cycles the numbers at the given indices backward (i.e. CC on the cube)
	* \details	For example, then numbers at indices {1,3,5,6} will now be at {3,5,6,1}
	*/
	template <typename Cubie>
	void backwardCycle(const Cubie positions[4], char* cubies);


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


	//////////////////////// CUBE CODES /////////////////////

	/**
	* \fn		step1Code
	* \brief	Hashes the cube into an integer
	* \details	Cubes had unique codes based only on edge orientation.
	* \return	int
	*/
	int step1Code();

	/**
	* \fn		step2Code
	* \brief	Hashes the cube into a relevant integer for step 2
	* \details	Cubes have unique codes based only on corner orientation
	*			and the position of the UD_SLICE edges
	* \return	int
	*/
	int step2Code();


	/**
	* \fn		step3Code
	* \brief	Hashes the cube into a relevant integer for step 2
	* \details	Cubes have unique codes based only on edge orbits
	*			and corner colors (corners must be in one of 96 possibilities)
	* \return	int
	*/
	int step3Code();

	/**
	* \fn		step4Code
	* \brief	Hashes the cube into a relevant integer for step 4
	* \details	Cube have unique codes based only on edge colors
	*/
	int step4Code();

	/// Helper method to hash the corners to a unique number.  Used to determine
	/// The only 96 possible corner positions using double turns
	int step4CornerCode();


	/// Code for the 2-face solver
	int code2();

	/// Helper method to determine the code for cubies in a specific orbit
	int step4Help(const Edge_t orbit[4], char cubies[12]);

	/**
	* \fn		doStep
	* \brief	Maneuvers the cube to complete a given step
	* \details	Calls doStep to determine how to most quickly finish the step
	*			Then turns the cube with the given function pointers.  Also prints
	*			The steps and the cube.  Returns the turn vector.
	* \param	Cube& cube					a reference to the cube
	* \param	bool(Cube::*solved)()		the method to see if it's solved
	* \param	std::vector<Turn> okSteps				the vector of allowable turns for this step
	*/
	static std::vector<Turn> doStep(Cube& cube, const std::unordered_map<int, std::vector<MotorControl::MoveInstruction>>& stepTable,
		int (Cube::*hashFunction)(), std::vector<Turn> okSteps);

	/**
	* \fn		findTurns
	* \brief	Uses a breadth-first search to find the fastest way to finish a step
	* \details	Given a method to see if the cube is solved and a vector of allowable
	*			turns, findTurns creates a queue of Cubes and a queue of std::vector<Turn>.
	*			Using a bfs, it continues to try every possible turn, check if it
	*			is "solved", and if not, adds it to the queue.  If it is "solved,"
	*			it returns the associated std::vector<Turn>.
	* \param	bool (Cube::*solved)()		Method to check if the cube is done with step
	* \param	std::vector<Turn> okSteps				A vector of allowable turns for this step
	*/
	std::vector<Turn> findTurns(const std::unordered_map<int, std::vector<MotorControl::MoveInstruction>>& stepTable, int (Cube::*code)(), std::vector<Turn> okSteps);

	/**
	* \fn		printSteps
	* \brief	Given a vector of function pointers to turns, prints them in readable form
	*/
	static void printTurns(std::vector<Turn> steps);

	/// Inverts the step in a std::vector<Turn>.  For example, F R U2 becomes U2 R' F'
	static std::vector<Turn> invert(std::vector<Turn> steps);
	//static std::vector<char> invert(std::vector<char> turns);
	static Cube doTurns(Cube cube, std::vector<Turn> turns);

	//static MoveInstruction turnToChar(Cube(*turn)(Cube));


	/// Builds the hashmaps holding the cubes
	static void buildMap(std::queue<Cube> cubeQueue, std::queue<std::vector<MotorControl::MoveInstruction>> turnsQueue,
		std::string fname, int (Cube::*hashFunction)(), std::vector<Turn> okTurns, int tableSize);



	/// Generates a queue with all 96 possible corner positions before step 4.
	/// Used as a starting point to build the step 3 tables
	static std::queue<Cube> step4ValidCorners();

	/// Loads a map from a filename to a hash map
	static void loadMap(std::unordered_map<int, std::vector<MotorControl::MoveInstruction>>& stepMap, std::string fname);

	static int fac(int n);

	/// builds a turn table for the edge orientations
	//static void buildTurnTable1();

	/// data members that define a cube.  For the following, indices represent positions
	char edgeColors_[12];	///< Numbers represent colors (see table of consts above)
	char edgeOrients_[12];  ///< 0 is oriented, 1 is unoriented
	char cornerColors_[8];	///< Numbers represent colors (see table of consts above)
	char cornerOrients_[8];	///< 0 is oriented, 1 is clockwise, 2 is counterclockwise

	char edgeOrbits_[12];  ///< 0, 1, or 2 depending on whether the edge is at home



};


