/**
 * \file comm-protocol.h
 * \brief Defines a protocol for serial communication between a PC program and
 *	and Arduino sketch which turns the motors attached the Rubik's cube.
 * \details The constants defined here encode the 18 valid cube actions (turn
 *	each of six faces once or twice clockwise, or once counterclockwise). The
 *	solve algorithm should include this file, and use this protocol to transmit
 *	the solution over the serial port.
 *
 *	The arduino sketch should also include the file, and should use the protocol
 *	to read incoming bytes and decode them to perform the correct sequence of
 *	operations on the cube.
 */

#pragma once

namespace CommProtocol {

	typedef char SerialMessage;

	/**
	 * \enum MoveInstruction
	 * \brief Constants representing a single cube action.
	 */
	enum MoveInstruction : SerialMessage {
	    LEFT,
	    RIGHT,
	    UP,
	    DOWN,
	    FRONT,
	    BACK,

	    LEFT_2,
	    RIGHT_2,
	    UP_2,
	    DOWN_2,
	    FRONT_2,
	    BACK_2,

	    LEFT_INVERTED,
	    RIGHT_INVERTED,
	    UP_INVERTED,
	    DOWN_INVERTED,
	    FRONT_INVERTED,
	    BACK_INVERTED
	};

}
