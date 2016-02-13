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

}