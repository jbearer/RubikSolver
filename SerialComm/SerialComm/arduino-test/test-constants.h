#pragma once

/**
 * \file test-constants.h
 * \note This file is used by arduino and serialstream. This means it
 *	must be compatible with both C and C++.
 */

/// Constants used by both arduino sketch and serialstream to coordinate
/// message passing
namespace testConstants {
	typedef int SerialMessage;

	const SerialMessage BLINK = 1;

	const SerialMessage CONTINUE = 2;

	const SerialMessage WAIT = 3;

	const char TEST_CHAR = 'a';

	const float TEST_FLOAT = 3.141592653;

	const char TEST_STRING[] = { 'h', 'e', 'l', 'l', 'o', '\0' };

}