/**
 * \headerfile os-macros.h "include/os-macros.h"
 * Defines macros which can be used to determine the OS at compile time.
 */

#pragma once

#define WINDOWS 0
#define UNIX 1

#if defined(_WIN16) || defined(_WIN32) || defined(_WIN64)

#define OS WINDOWS

#elif defined(__unix__) || defined(__unix)

#define OS UNIX

#else

#error Unable to determine OS.

#endif
