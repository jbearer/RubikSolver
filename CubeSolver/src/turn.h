#pragma once

/**
* \file turn.h
*/

#include <iostream>

namespace CubeSolver {

	/**
	 * @brief      Represents a turn of a rubik's cube
	 * @detail     Follows standard labelling procedure.
	 * 			   L = Left, R = Right, U = Up, D = Down,
	 * 			   F = Front, B = Back,
	 * 			   *2 means turn twice, *i means counterclockwise
	 */
	enum Turn {
	    L = 0,
	    R = 1,
	    U = 2,
	    D = 3,
	    F = 4,
	    B = 5,

	    L2 = 6,
	    R2 = 7,
	    U2 = 8,
	    D2 = 9,
	    F2 = 10,
	    B2 = 11,

	    Li = 12,
	    Ri = 13,
	    Ui = 14,
	    Di = 15,
	    Fi = 16,
	    Bi = 17
	};

	/**
	 * @brief      Return the inverse Turn
	 *
	 * @param[in]  For example, oppTurn(F) = Fi
	 *			   and oppTurn(F2) = F2
	 *
	 * @return     Returns a Turn
	 */
	Turn oppTurn(Turn turn);

	/// Prints on the Turn in standard notation
	std::ostream& operator<<(std::ostream& out, Turn turn);
}
