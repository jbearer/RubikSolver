/**
 * \headerfile color-stream.h "include/color-stream.h"
 * Determines the user's OS and includes the appropriate class, one of
 *	* ColorStreamWindows
 *	* ColorStreamUnix
 * Creates an alias ColorStream for the relevant class.
 */

#pragma once

#include "os-macros.h"

#ifdef OS

#if OS == WINDOWS

#include "color-stream-windows.h"
namespace wiplib {
	typedef ColorStreamWindows ColorStream;
}

#elif OS == UNIX

#include "color-stream-unix.h"
namespace wiplib {
	typedef wiplib::ColorStreamUnix ColorStream;
}

#else

#error Unable to determine OS.

#endif
#endif
