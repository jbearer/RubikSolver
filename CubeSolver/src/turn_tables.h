#pragma once

#include "cube.h"
#include "turn.h"

namespace CubeSolver {


/// Useful constants for generating Turn Tables
const int NUM_EDGE_ORIENTS = 4096; // 2^12
const int NUM_CORNER_ORIENTS = 6561; // 3^8
const int NUM_CORNER_COLORS = 40320; // 8!
const int NUM_EDGE_ORBITS = 495; // 12 nCr 4
const int NUM_EDGE_COLORS1 = 24; // 4!
const int NUM_EDGE_COLORS2 = 40320; // 8!

/// Turn tables
namespace TurnTables{
    extern int EDGE_ORIENTS_TABLE[NUM_EDGE_ORIENTS][NUM_TURNS_STEP1];
    extern int CORNER_ORIENTS_TABLE[NUM_CORNER_ORIENTS][NUM_TURNS_STEP1];
    extern int CORNER_COLORS_TABLE[NUM_CORNER_COLORS][NUM_TURNS_STEP2];
    extern int EDGE_ORBITS_TABLE[NUM_EDGE_ORBITS][NUM_TURNS_STEP1];
    extern int EDGE_COLORS_TABLE1[NUM_EDGE_COLORS1][NUM_TURNS_STEP2];
    extern int EDGE_COLORS_TABLE2[NUM_EDGE_COLORS2][NUM_TURNS_STEP2];
}
/// Reads turn tables from turnTables.ser.  Stores them in proper arrays
void readTurnTables();


///////////////////////// TURN TABLES ///////////////////////////
/*
* Turn tables hold the results of applying each turn to a cube's state.
* They should increase increase the speed of the cube solver because the
* solver no longer has to construct a new cube and apply the turn.  Instead
* it has memoized the results of each turns, and can look it up in a table.
*/

/// Calls the build methods for each turn table, then calls archiveTurnTables
void buildTurnTables();

/// Stores the turn tables in "turnTables.ser"
void archiveTurnTables();

/// Reads turn tables from turnTables.ser.  Stores them in proper arrays
void readTurnTables();

/**
* \fn       buildEdgeOrientsTable
* \brief    Builds the turn table of edge orientations to speed up step 1
* Creates a cube, then iterates through all the possible edgeOrientations
* using the method nextEdgeOrients().  For each orientation, it loops
* through all the allowed turns in step 1.  For each turn, it generates
* a new cube, applies the turn, and calculates the new edge orientations code.
* It then stores that code in the proper place of the turn table.
*/
void buildEdgeOrientsTable();

/**
* \fn       buildCornerOrientsTable
* \brief    Builds the turn table of corner orientations
* \details  See buildEdgeOrients
*/
void buildCornerOrientsTable();

/**
* \fn       buildEdgeOrbitsTable
* \brief    Builds the turn table of edge orbits for step 1
* \details  Similar to buildEdgeOrients.  Creates a cube with edge colors
* that will have code 0.  Generates all possible 12 nCr 4 positions for the
* edges in the LR slice.  Uses the stl method next_permutation to change
* the colors and increment the cube code.  In this method, the colors
* used for calculating the codes are impossible, but represent the
* simplifications used when calculating the edgeOrbits code.
*/
void buildEdgeOrbitsTable();

/**
* \fn       buildCornersColorsTable
* \brief    Builds the turn table of corner colors for step 2
* \details  Generates all possible 8! positions of the corner colors.
*           Then, memoizes the outcome of each of the possible 12 turns.
*/
void buildCornerColorsTable();

/**
* \fn       buildEdgeColorsTable1
* \brief    Builds the turn table for the LR slice edge colors for step 2
* \details  In step2, the LR slice is invariant, so this step constructs
*           all the possibilities for the LR slice (4! = 24 possibilities)
*/
void buildEdgeColorsTable1();

/**
* \fn       buildEdgeColorsTable1
* \brief    Builds the turn table for the non-LR edge colors for step 2
* \details  In step2, the LR slice is invariant.  This this step constructs
*           all the possibilities for the NON-LR slice edges (8! total)
*/
void buildEdgeColorsTable2();

}
