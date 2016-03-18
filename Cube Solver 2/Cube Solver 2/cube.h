#pragma once
/**
* \file cube.h
* \brief interface for Cube class
*/
#include "comm-protocol.h"

#include <utility>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <queue>
#include <tuple>
#include <boost/archive/binary_oarchive.hpp>



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
	struct CubeNums;

public:

	/// Memoized the necessary factorials and combinations
	static int FAC[12];
	static int CHOOSE[12][12];

	/// Define enums referring to specific edge and corner pieces
	enum Edge_t { YB, YR, YO, YG, RB, OB, RG, OG, WB, WR, WO, WG };
	enum Corner_t { YRB, YOB, YRG, YOG, WRB, WOB, WRG, WOG };

	/// Memoizes combinatorial values for calculating Cube codes
	static void initChoose();

	/// Initializes constants and loads the hash tables
	static void init();

	/// for two-face solve
	static void init2();

	/// Scramble and solve a test cube
	static void testDFS();
	static void turnTest();
	static void test();

	/// Compares normal turning and look-up turning
	static void timeTurnTables();

	static void timeDFS();

	////// Constructors ///////
	/// creates a default solved cube
	Cube() = default;

	/// Five-argument parameterized constructor.  Constructs a cube
	/// out of edgeColors, edgeOrients, cornerColors, and cornerOrients
	Cube(Edge_t eColors[12], char eOrients[12], char eOrbits[12],
		Corner_t cColors[8], char cOrients[8]);

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
	//static std::vector<CommProtocol::MoveInstruction> solve(Cube cube);

	/// for two-turn solve
	static void solve2(Cube cube);

	/// Prints the cube in the form edgeColors, edgeOrients, cornerColors, cornerOrients
	void print() const;

private:

	struct CubeNums {
		struct Hash1;
		struct Hash2;
		typedef unsigned short ushort;
		friend class boost::serialization::access;

		CubeNums();

		CubeNums(ushort in1, ushort in2, ushort in3);

		struct Hash1 : public std::unary_function<CubeNums, size_t> {
			size_t operator()(const CubeNums& cube) const;
		};
		// NOTE: I think overflow is okay in this case
		struct Hash2 : public std::unary_function<CubeNums, size_t> {
			size_t operator()(const CubeNums& cube) const;
		};

		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar & first & second & third;
		}


		CubeNums turn1(int i) const;
		CubeNums turn2(int i) const;

		bool operator==(const CubeNums& rhs) const
		{
			return first == rhs.first &&
				second == rhs.second &&
				third == rhs.third;
		}

		ushort first;	// edgeOrients_ or cornerColors_
		ushort second;	// cornerOrients_ or edgeColors1_
		ushort third;	// edgeOrbits_ or edgeColors2_
	};


	/// A turn struct, to easily convert between representations
	struct Turn {
		/// Constructors
		Turn() = delete;
		Turn(CommProtocol::MoveInstruction c);

		/// Data members
		Cube(*turnFunc)(Cube);
		std::string toString;
		CommProtocol::MoveInstruction repr;
		CommProtocol::MoveInstruction oppTurn;

		/// Construct a vector of Turns from a vector of MoveInstructions
		static std::vector<Turn> movesToTurns(std::vector<CommProtocol::MoveInstruction> moves);
	};

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

	/// Hash maps from cube codes to the required turns to solve
	static std::unordered_map<CubeNums, CommProtocol::MoveInstruction, CubeNums::Hash1> STEP1MAP;
	static std::unordered_map<CubeNums, CommProtocol::MoveInstruction, CubeNums::Hash2> STEP2MAP;

	static std::unordered_map < int, std::vector<CommProtocol::MoveInstruction>> TURNMAP2;

	/// Vectors with function pointers to allowable turns
	static const std::vector<Turn> OK_TURNS1;
	static const std::vector<Turn> OK_TURNS2;

	static const std::vector<Turn> OK_TURNS_2FACE;

	static const int NUM_TURNS_STEP1 = 12;
	static const int NUM_TURNS_STEP2 = 8;

	static const int NUM_EDGE_ORIENTS = 4096; // 2^12
	static const int NUM_CORNER_ORIENTS = 6561; // 3^8
	static const int NUM_CORNER_COLORS = 40320; // 8!
	static const int NUM_EDGE_ORBITS = 495; // 12 nCr 4
	static const int NUM_EDGE_COLORS1 = 24; // 4!
	static const int NUM_EDGE_COLORS2 = 40320; // 8!

	/// Turn tables
	static int EDGE_ORIENTS_TABLE[NUM_EDGE_ORIENTS][NUM_TURNS_STEP1];
	static int CORNER_ORIENTS_TABLE[NUM_CORNER_ORIENTS][NUM_TURNS_STEP1];
	static int CORNER_COLORS_TABLE[NUM_CORNER_COLORS][NUM_TURNS_STEP2];
	static int EDGE_ORBITS_TABLE[NUM_EDGE_ORBITS][NUM_TURNS_STEP1];
	static int EDGE_COLORS_TABLE1[NUM_EDGE_COLORS1][NUM_TURNS_STEP2];
	static int EDGE_COLORS_TABLE2[NUM_EDGE_COLORS2][NUM_TURNS_STEP2];

	///////////////////////// TURN TABLES ///////////////////////////
	/*
	* Turn tables hold the results of applying each turn to a cube's state.
	* They should increase increase the speed of the cube solver because the
	* solver no longer has to construct a new cube and apply the turn.  Instead
	* it has memoized the results of each turns, and can look it up in a table.
	*/

	/// Calls the build methods for each turn table, then calls archiveTurnTables 
	static void buildTurnTables();

	/// Stores the turn tables in "turnTables.ser"
	static void archiveTurnTables();
	
	/// Reads turn tables from turnTables.ser.  Stores them in proper arrays
	static void readTurnTables();

	static void turnTableTest();


	/**
	* \fn		buildEdgeOrientsTable
	* \brief	Builds the turn table of edge orientations to speed up step 1
	* Creates a cube, then iterates through all the possible edgeOrientations
	* using the method nextEdgeOrients().  For each orientation, it loops
	* through all the allowed turns in step 1.  For each turn, it generates
	* a new cube, applies the turn, and calculates the new edge orientations code.
	* It then stores that code in the proper place of the turn table.
	*/
	static void buildEdgeOrientsTable();

	/** 
	* \fn		buildCornerOrientsTable
	* \brief	Builds the turn table of corner orientations
	* \details	See buildEdgeOrients
	*/
	static void buildCornerOrientsTable();

	/**
	* \fn		buildEdgeOrbitsTable
	* \brief	Builds the turn table of edge orbits for step 1
	* \details	Similar to buildEdgeOrients.  Creates a cube with edge colors
	* that will have code 0.  Generates all possible 12 nCr 4 positions for the 
	* edges in the LR slice.  Uses the stl method next_permutation to change
	* the colors and increment the cube code.  In this method, the colors
	* used for calculating the codes are impossible, but represent the
	* simplifications used when calculating the edgeOrbits code.
	*/
	static void buildEdgeOrbitsTable();

	/**
	* \fn		buildCornersColorsTable
	* \brief	Builds the turn table of corner colors for step 2
	* \details	Generates all possible 8! positions of the corner colors.
	*			Then, memoizes the outcome of each of the possible 12 turns.
	*/
	static void buildCornerColorsTable();

	/** 
	* \fn		buildEdgeColorsTable1
	* \brief	Builds the turn table for the LR slice edge colors for step 2
	* \details	In step2, the LR slice is invariant, so this step constructs
	*			all the possibilities for the LR slice (4! = 24 possibilities)
	*/
	static void buildEdgeColorsTable1();

	/**
	* \fn		buildEdgeColorsTable1
	* \brief	Builds the turn table for the non-LR edge colors for step 2
	* \details	In step2, the LR slice is invariant.  This this step constructs
	*			all the possibilities for the NON-LR slice edges (8! total)
	*/
	static void buildEdgeColorsTable2();


	/// increments edge orientation to easily loop through them.  Returns false
	/// if the edgeOrients was {1,1,1...}, i.e. the "last" orientation.
	bool nextEdgeOrients();

	/// increments corner orientation to easily loop through them.  Returns false
	/// if the cornerOrients was {2,2,2...}, i.e. the "last" orientation.
	bool nextCornerOrients();
	
	CubeNums cubeNums1();
	CubeNums cubeNums2();

	/////////////////// TURN METHODS ////////////////

	/**
	* \fn		turn
	* \brief	Calls forwardCycle on the data members of Cube.
	*/
	void turn(const Edge_t edges[4], const Corner_t corners[4]);

	Cube turnWith(Turn inputTurn);

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
	template <typename Index_t, typename Cubies_t>
	void forwardCycle(const Index_t positions[4], Cubies_t* cubies);

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


	//////////////////////// CUBE CODES /////////////////////

	/**
	* \fn		step1Code
	* \brief	Hashes the cube into an integer
	* \details	Cubes had unique codes based only on edge orientation.
	* \return	int
	*/
	int edgeOrientsCode();

	/// same as edgeOrientsCode, but for corners
	int cornerOrientsCode();

	int cornerColorsCode();

	int edgeOrbitsCode();

	int edgeColorsCode1();
	int edgeColorsCode2();

	static int step1Code(int edgeOrients, int cornerOrients, int edgeOrbits);
	int step1Code();
	/**
	* \fn		step2Code
	* \brief	Hashes the cube into a relevant integer for step 2
	* \details	Cubes have unique codes based only on corner orientation
	*			and the position of the UD_SLICE edges
	* \return	int
	*/
	long step2Code();
	static long step2Code(int cornerColors, int edgeColors1, int edgeColors2);


	/**
	* \fn		step3Code
	* \brief	Hashes the cube into a relevant integer for step 2
	* \details	Cubes have unique codes based only on edge orbits
	*			and corner colors (corners must be in one of 96 possibilities)
	* \return	int
	*/
	//int step3Code();

	/**
	* \fn		step4Code
	* \brief	Hashes the cube into a relevant integer for step 4
	* \details	Cube have unique codes based only on edge colors
	*/
	//int step4Code();

	/// Helper method to hash the corners to a unique number.  Used to determine
	/// The only 96 possible corner positions using double turns
	//int step4CornerCode();
	

	/// Code for the 2-face solver
	int code2();

	/// Helper method to determine the code for cubies in a specific orbit
	//int step4Help(const Edge_t orbit[4], Edge_t cubies[12]);

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
	static std::vector<Turn> doStep(Cube& cube, const std::unordered_map<int, std::vector<CommProtocol::MoveInstruction>>& stepTable,
		int (Cube::*hashFunction)(), std::vector<Turn> okSteps);

	std::deque<Turn> solveStep1();

	std::deque<Turn> solveStep1DFS();

	std::vector<CommProtocol::MoveInstruction> solve();

	bool solveStep1Helper(int depth, const CubeNums& curr, std::deque<Turn>& result);

	std::deque<Turn> solveStep2DFS();
	bool solveStep2Helper(int depth, const CubeNums& curr, std::deque<Turn>& result);

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
	std::vector<Turn> findTurns(const std::unordered_map<int, std::vector<CommProtocol::MoveInstruction>>& stepTable, int (Cube::*code)(), std::vector<Turn> okSteps);

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
	static void buildMap(std::queue<Cube> cubeQueue, std::queue<std::vector<CommProtocol::MoveInstruction>> turnsQueue,
		std::string fname, int (Cube::*hashFunction)(), std::vector<Turn> okTurns, int tableSize);

	static void buildEndMaps();
	static void buildMap1();
	static void buildMap2();

	static void archiveEndMaps();
	static void readEndMaps();

	static std::deque<Turn> turnsFromEndMap1(CubeNums start);
	static std::deque<Turn> turnsFromEndMap2(CubeNums start);

	static int getIndex1(CommProtocol::MoveInstruction);
	static int getIndex2(CommProtocol::MoveInstruction);

	/// Generates a queue with all 96 possible corner positions before step 4.
	/// Used as a starting point to build the step 3 tables
	//static std::queue<Cube> step4ValidCorners();

	/// Loads a map from a filename to a hash map
	static void loadMap(std::unordered_map<int, std::vector<CommProtocol::MoveInstruction>>& stepMap, std::string fname);

	static int fac(int n);

	/// data members set to a solved cube.
	Edge_t edgeColors_[12] = {YB, YR, YO, YG, RB, OB, RG, OG, WB, WR, WO, WG };	///< Numbers represent colors (see table of consts above)
	char edgeOrients_[12] = { 0,0,0,0,0,0,0,0,0,0,0,0 };  ///< 0 is oriented, 1 is unoriented
	Corner_t cornerColors_[8] = { YRB, YOB, YRG, YOG, WRB, WOB, WRG, WOG };	///< Numbers represent colors (see table of consts above)
	char cornerOrients_[8] = { 0,0,0,0,0,0,0,0 };	///< 0 is oriented, 1 is clockwise, 2 is counterclockwise

	char edgeOrbits_[12] = { 0,1,1,0,2,2,2,2,0,1,1,0 };  ///< 0, 1, or 2 depending on whether the edge is at home



};


