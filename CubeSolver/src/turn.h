#pragma once

/**
* \file turn.h
*/

#include <iostream>
//#include "../../MotorControl/src/arduino-incl/comm-protocol.h"

namespace CubeSolver {

	enum Turn {
	    LEFT = 0,
	    RIGHT = 1,
	    UP = 2,
	    DOWN = 3,
	    FRONT = 4,
	    BACK = 5,

	    LEFT_2 = 6,
	    RIGHT_2 = 7,
	    UP_2 = 8,
	    DOWN_2 = 9,
	    FRONT_2 = 10,
	    BACK_2 = 11,

	    LEFT_INVERTED = 12,
	    RIGHT_INVERTED = 13,
	    UP_INVERTED = 14,
	    DOWN_INVERTED = 15,
	    FRONT_INVERTED = 16,
	    BACK_INVERTED = 17
	};

	/**
	 * @brief      Return the inverse Turn
	 *
	 * @param[in]  For example, oppTurn(FRONT) = FRONT_INVERTED
	 *			   and oppTurn(FRONT_2) = FRONT_2
	 *
	 * @return     Returns a Turn
	 */
	Turn oppTurn(Turn turn);

	/// Prints on the Turn in standard notation
	std::ostream& operator<<(std::ostream& out, Turn turn);
}
