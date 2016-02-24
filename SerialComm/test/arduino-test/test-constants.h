#pragma once

/**
 * \file test-constants.h
 * \note This file is used by arduino and serialstream. This means it
 *	must be compatible with both C and C++.
 */

/// Constants used by both arduino sketch and serialstream to coordinate
/// message passing
namespace testConstants {
	typedef char SerialMessage;

	const SerialMessage BLINK = 1;

	const SerialMessage CONTINUE = 2;

	const SerialMessage WAIT = 3;

  const SerialMessage ERR = 4;

	const char TEST_CHAR = 'a';

  const int TEST_STRING_LEN = 6;
 
	const char TEST_STRING[TEST_STRING_LEN] = { 'h', 'e', 'l', 'l', 'o', '\0' };

  const int NUM_BLINKS = 10;

  const int INTERVAL = 500;

}
