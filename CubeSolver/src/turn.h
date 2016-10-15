#pragma once

/**
* \file turn.h
*/

#include <iostream>
#include "../../MotorControl/src/arduino-incl/comm-protocol.h"

namespace CubeSolver {

	typedef CommProtocol::MoveInstruction Turn;

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
