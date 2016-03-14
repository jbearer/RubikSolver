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

#define DEBUG true

namespace CommProtocol {

  const int LED_INDICATOR = 10;

	typedef char SerialMessage;

	/**
	 * \enum MoveInstruction
	 * \brief Constants representing a single cube action.
	 */
	enum MoveInstruction : SerialMessage {
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

  void blinkIndicator();
  
  void indicateError();

}