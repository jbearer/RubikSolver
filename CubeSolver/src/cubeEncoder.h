#pragma once

#include "cube.h"

namespace CubeSolver {

	/**
	* @class 	CubeEncoder
	*
	* @brief 	Provides the methods for turning a Rubiks cube, as represented
	*			in the Cube class, into two different tuples of three shorts.
	*			This more compact representation uniquely represents a cube and
	*			is more efficient.
	*/
	class CubeEncoder {

	public:

		/// The size of an orbit, like the LR_SLICE
		static const size_t ORBIT_SIZE = 4;

		/// The four edges in the left-right slice (meridian of the cube)
		static const Cube::Edge_t LR_SLICE[ORBIT_SIZE];

		/////// USED FOR STEP 1 /////////////

		/**
		 * @brief      Returns an integer uniquely defined by the cube's
		 * 			   edge orientation
		 *
		 * @details    A cube has twelve edges, each with an orientation of
		 * 			   0 or 1.  Therefore, there are 2^12 = 4096 different
		 * 			   edge orientations possible.  edgeOrientsCode essentially
		 * 			   returns the binary number that the bit-vector
		 * 			   corresponds to.
		 *
		 * @param[in]  cube  The cube to encode
		 *
		 * @return     A short uniquely defined by cube.edgeOrients_
		 * @todo 	   There are actually only 2^11 = 2048 possible edge
		 * 			   orientations, since the last edge is determined by
		 * 			   the previous 11.  Modify method to generate code only
		 * 			   based on the first 11 edges.
		 */
		static ushort edgeOrientsCode(const Cube& cube);

		/**
		 * @brief      Returns an short defined by cube.cornerOrients_
		 *
		 * @details    Similar to edgeOrientsCode, but there are eight
		 * 			   corners each with three orientations, so it is
		 * 			   interperated as an 8-digit ternary number
		 * @param[in]  cube  The cube to encode
		 *
		 * @return     A code based on cube.cornerOrients_
		 * @todo 	   Like edgeOrients, the last corner is determined by
		 * 			   the first seven.
		 */
		static ushort cornerOrientsCode(const Cube& cube);

		/**
		 * @brief      Returns a short defined by positions of LR_SLICE edges
		 *
		 * @details    After step 1, the positions of the four edges in the
		 * 			   LR_SLICE (YG, YB, WB, WG) must be in some permutation
		 * 			   of those positions.  Called 'orbit' because we only care
		 * 			   that the four edges are in the proper slice, but not that
		 * 			   specific positions.  Each of those edges is encoded as a
		 * 			   0, and the other edges are a 1.  Then the function returns
		 * 			   the a unique number corresponding to the permutation
		 * 			   (see https://en.wikipedia.org/wiki/Combinatorial_number_system)
		 * 			   There are 12 nCr 4 = 495 possible results.  Encodes
		 * 			   {0,0,0,0,1,1...} as 0,
		 * 			   {0,0,0,1,0,1,1,...} as 1,
		 * 			   ...
		 * 			   {...,1,1,0,0,0,0} as 494 = (12 nCr 4) - 1
		 * 			   where 0 corresponds to an LR_SLICE edge and 1 doesn't
		 *
		 * @param[in]  cube  The cube to encode
		 *
		 * @return     A code based on information from edgeColors_
		 */
		static ushort edgeOrbitsCode(const Cube& cube);


		/////// USED FOR STEP 2 ////////

		/**
		 * @brief      Returns a unique code based on positions of edges
		 * 			   in the LR_SLICE
		 *
		 * @details    During step two, LR_SLICE edges remained closed.
		 * 			   Also uses combinatorial ordering to create a code
		 * 			   for the 4! = 24 possibilities
		 *
		 * @param[in]  cube  The cube to encode
		 *
		 * @return     A code for the positions of the LR_SLICE colors
		 */
		static ushort edgeColorsCode1(const Cube& cube);

		/**
		 * @brief      Same as edgeColorsCode1 but for non-LR_SLICE
		 * 			   edges.  There are 8! = 5040 possibilities.
		 *
		 * @param[in]  cube  The cube to encode
		 *
		 * @return     A code based on non-LR_SLICE colors
		 */
		static ushort edgeColorsCode2(const Cube& cube);

		/**
		 * @brief      Like edgeColorsCode2 but for the 8 corners. 8! ways
		 *
		 * @param[in]  cube  The cube to encode
		 *
		 * @return     A code based on the positions of the corners
		 */
		static ushort cornerColorsCode(const Cube& cube);


		/////////// ITERATION FUNCTIONS /////////////

		/**
		 * @brief      Increments the edgeOrients_ of a cube
		 *
		 * @details    Used for generating all the possible edgeOrients_ when
		 * 			   constructing turn tables.  edgeOrients_ is a bit-vector
		 * 			   of size 12.  Essentially increments this to the next
		 * 			   binary number.
		 *
		 * @param      cube  The cube to increment
		 *
		 * @return     false if edgeOrients_ becomes {0,0,...}, true otherwise
		 */
		static bool nextEdgeOrients(Cube& cube);

		/**
		 * @brief      Increments cornerOrients_.
		 *
		 * @details    Like nextEdgeOrients, but increments to the next ternary
		 * 			   number with size 8
		 *
		 * @param      cube  The cube
		 *
		 * @return     false if cornerOrients_ become {0,0,...}, true otherwise
		 */
		static bool nextCornerOrients(Cube& cube);

		/**
		 * @brief      Modifies edgeColors_ to correspond to the next edgeOrbits
		 *  		   code.
		 *
		 * @details    Assumes edgeColors_ only consists of 0's and 1's, since
		 * 			   we only care about whether the edge is in the LR_SLICE
		 * 			   or not.  Then, simply calls next_permutation.
		 *
		 * @param      cube  The cube to increment.  This could not represent a
		 * 			   realistic cube, since the edges colors are represnted as
		 * 			   either in the LR_SLICE or not, so they are either YB (on
		 * 			   the slice) or YO (not on it)
		 *
		 * @return     false if it returns the first ordering, consisting of
		 * 			   4 YB's then 8 YO's.  true otherwise
		 */
		static bool nextEdgeOrbits(Cube& cube);

		/**
		 * @brief      Sets edgeColors_ to the first edge orbits, as described
		 *  		   in nextEdgeOrbit, to make it easier to loop through
		 *  		   all the possible edge orbits.
		 *
		 * @param      cube  The cube whose edgeColors_ we are changing
		 */
		static void setFirstEdgeOrbits(Cube& cube);

		/**
		 * @brief      Increments the LR_SLICE edges in edgeColors_
		 *
		 * @details    Extracts the edges from the LR_SLICE, then uses
		 * 			   std::next_permutation to increment them.  Then
		 * 			   inserts the new permutation back into edgeColors_
		 *
		 * @param      cube  The cube
		 *
		 * @return     false if the result is the first ordering
		 */
		static bool nextEdgeColors1(Cube& cube);

		/**
		 * @brief      Increments the non-LR_SLICE edges
		 *
		 * @details    Same as nextEdgeColors1, but with the 8 edges
		 * 			   not in the LR_SLICE orbit
		 *
		 * @param      cube  The cube
		 *
		 * @return     false if the result is the first ordering
		 */
		static bool nextEdgeColors2(Cube& cube);

		/**
		 * @brief      Increments cornerColors_.  Uses next_permutation
		 *
		 * @param      cube  The cube
		 *
		 * @return     true if it the result is first ordering
		 */
		static bool nextCornerColors(Cube& cube);
	};
}