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
#include <boost/archive/binary_oarchive.hpp>
#include <boost/unordered_map.hpp>



/**
* \class Cube
* \brief Contains the implementation for a Rubik's Cube
* \details A rubik's cube is identified by it's edge colors,
*		   edge orientations, corner colors, and corner orientations
*		   the ith index of an array corresponds to the cubie
*		   in the space that is home to cubie i.
*/
class Cube {

	/// forward declare Turn, CubeNums struct
	struct Turn;
	struct CubeNumsStep1;
	struct CubeNumsStep2;


public:

	/// Memoized the necessary factorials and combinations
	static int FAC[12];
	static int CHOOSE[12][12];

	/// Define enums referring to specific edge and corner pieces
	enum Edge_t { YB, YR, YO, YG, RB, OB, RG, OG, WB, WR, WO, WG };
	enum Corner_t { YRB, YOB, YRG, YOG, WRB, WOB, WRG, WOG };

	/// Memoizes combinatorial values for calculating Cube codes
	static void initChoose();
	static int fac(int n);

	/////////// TESTING ///////////
	
	/// ensure all the turn methods work for cubes
	static void turnTest();

	static void test();
	static void test1();

	/// Compares normal turning and look-up turning
	static void timeTurnTables();

	/// solve a cube using DFS given a set of moves
	static void testDFS();

	/// generate 100 random cubes, finds the average and max solve
	static void timeDFS();

	static void turnTableTest();

	///////// Constructors //////////

	/// creates a default solved cube
	Cube() = default;

	/// Four-argument parameterized constructor.  Constructs a cube
	/// out of edgeColors, edgeOrients, cornerColors, and cornerOrients
	Cube(Edge_t eColors[12], char eOrients[12],
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

	// Solving:
	std::vector<CommProtocol::MoveInstruction> solve();

	/// Prints the cube in the form edgeColors, edgeOrients, cornerColors, cornerOrients
	void print() const;

private:


	typedef unsigned short ushort;

	/*
	* \class CubeNums
	* \brief	Represent the Cube's position with three unsigned shorts
	*/
	/*
	struct CubeNums {

		struct Hash1;
		struct Hash2;
		friend class boost::serialization::access;

		// Constructors:

		CubeNums(); ///< default constructor: creates invalid cubenums

		/// parameterized constructor: assigns the three cubeNums data members
		CubeNums(ushort in1, ushort in2, ushort in3);

		/// hash function for cubenums in step 1
		struct Hash1 : public std::unary_function<CubeNums, size_t> {
			size_t operator()(const CubeNums& cube) const;
		};
		/// hash function for cubenums in step 2
		// NOTE: I think overflow is okay in this case
		struct Hash2 : public std::unary_function<CubeNums, size_t> {
			size_t operator()(const CubeNums& cube) const;
		};

		/// serialize cubenums
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar & first & second & third;
		}

		CubeNums turn(int i) const;

		/// turn1 gets the ith element of the turn tables for step 1
		CubeNums turn1(int i) const;
		/// turn2 gets the ith element of turn tables for step 2
		CubeNums turn2(int i) const;

		bool operator==(const CubeNums& rhs) const;
		bool operator!=(const CubeNums& rhs) const;

		ushort first;	// edgeOrients or cornerColors
		ushort second;	// cornerOrients or edgeColors1
		ushort third;	// edgeOrbits or edgeColors2
	};
	*/
	
	struct CubeNumsStep1{
		struct Hash;
		
		// Constructors
		CubeNumsStep1();
		CubeNumsStep1(const Cube& cube);
		CubeNumsStep1(ushort edgeOrients, ushort cornerOrients, ushort edgeOrbits);

		bool operator==(const CubeNumsStep1& rhs) const;
		bool operator!=(const CubeNumsStep1& rhs) const;

		struct Hash : public std::unary_function<CubeNumsStep1, size_t> {
			size_t operator()(const CubeNumsStep1& cube) const;
		};

		/// serialize cubenums
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar & edgeOrients_ & cornerOrients_ & edgeOrbits_;
		}

		CubeNumsStep1 turn(int i) const;

	private:
		friend class Cube;
		ushort edgeOrients_;
		ushort cornerOrients_;
		ushort edgeOrbits_;
	};

	struct CubeNumsStep2{
		struct Hash;

		CubeNumsStep2();
		CubeNumsStep2(const Cube& cube);
		CubeNumsStep2(ushort cornerColors, ushort edgeColors1, ushort edgeColors2);

		bool operator==(const CubeNumsStep2& rhs) const;
		bool operator!=(const CubeNumsStep2& rhs) const;


		struct Hash : public std::unary_function<CubeNumsStep2, size_t> {
			size_t operator()(const CubeNumsStep2& cube) const;
		};

		/// serialize cubenums
		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar & cornerColors_ & edgeColors1_ & edgeColors2_;
		}

		CubeNumsStep2 turn(int i) const;

	private:
		
		ushort cornerColors_;
		ushort edgeColors1_;
		ushort edgeColors2_;

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
	static std::unordered_map<CubeNumsStep1, CommProtocol::MoveInstruction, CubeNumsStep1::Hash> STEP1MAP;
	static std::unordered_map<CubeNumsStep2, CommProtocol::MoveInstruction, CubeNumsStep2::Hash> STEP2MAP;

	static const int NUM_TURNS_STEP1 = 12;
	static const int NUM_TURNS_STEP2 = 8;

	/// Vectors with function pointers to allowable turns
	static const Turn OK_TURNS1[NUM_TURNS_STEP1];
	static const Turn OK_TURNS2[NUM_TURNS_STEP2];


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
	
	//CubeNums cubeNums1();
	//CubeNums cubeNums2();

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

	///////// SOLVING ////////////


	std::deque<Turn> solveStep1DFS();
	bool solveStep1Helper(int depth, const CubeNumsStep1& curr, std::deque<Turn>& result);

	std::deque<Turn> solveStep2DFS();
	bool solveStep2Helper(int depth, const CubeNumsStep2& curr, std::deque<Turn>& result);

	////////// END MAPS ////////////

	static void buildEndMaps();
	static void buildMap1();
	static void buildMap2();

	static void archiveEndMaps();
	static void readEndMaps();

	static std::deque<Turn> turnsFromEndMap1(CubeNumsStep1 start);
	static std::deque<Turn> turnsFromEndMap2(CubeNumsStep2 start);

	static int getIndex1(CommProtocol::MoveInstruction);
	static int getIndex2(CommProtocol::MoveInstruction);


	/// data members set to a solved cube.
	Edge_t edgeColors_[12] = {YB, YR, YO, YG, RB, OB, RG, OG, WB, WR, WO, WG };	///< Numbers represent colors (see table of consts above)
	char edgeOrients_[12] = { 0,0,0,0,0,0,0,0,0,0,0,0 };  ///< 0 is oriented, 1 is unoriented
	Corner_t cornerColors_[8] = { YRB, YOB, YRG, YOG, WRB, WOB, WRG, WOG };	///< Numbers represent colors (see table of consts above)
	char cornerOrients_[8] = { 0,0,0,0,0,0,0,0 };	///< 0 is oriented, 1 is clockwise, 2 is counterclockwise

};


