#pragma once
#include <cstdlib>
#include "os-macros.h"

#ifdef OS

#if OS == WINDOWS

#include <windows.h>
typedef DWORD Pid;

#elif OS == UNIX

#include <sys/types.h>
typedef pid_t Pid;

#else

#error Unable to determine OS.

#endif
#endif

#include "process.h"
#include <unordered_set>

namespace wiplib {

    /**
     * \brief Block execution for a specified duration.
     * \delay Only returns after the duration has elapsed.
     * \param millis THe duration in milliseconds to delay.
     */
    void delay(size_t millis);

    /**
     * \brief Monitor a condition for a specified period of time.
     *	The function returns when either the condition becomes true, or the
     *	time limit expires.
     * \param condition A function returning bool that evaluates to true if
     *	and only if the condition is true.
     * \param millis The time in milliseconds for which to monitor the condition.
     * \return True if the function exited with a true condition, false if the
     *	function exited with a time limit.
     */
    bool timedMonitor(bool(*condition)(), size_t millis);

    /** 
     * \brief Schedule a function to be executed at some time in the future.
     * \details schedule returns immediately. After a specified delay,
     *  the given function is executed and passed the given arguments.
     *
     * \param millis The delay in milliseconds before executing the function.
     * \param function A pointer to the function to execute.
     * \param args Arguments which will be passed to function.
     *
     * \todo Should return (via a reference argument) some kind of struct containing
 	 *	information about the return value of the scheduled function, including:
 	 *	 * A bool indicating whether or not the rest of the data is meaningful (ie
 	 *		whether the function has run yet)
 	 *	 * The return value of the function
     */
    template<typename ReturnType, typename... ArgList>
    void schedule(
        size_t millis, ReturnType(*function)(ArgList...), ArgList... args);

    /**
     * \brief Same as above, but when function is executed, a callback function
     *  is called and passed the result of function as an argument.
     */
    template<typename ReturnType, typename... ArgList>
    void schedule(
        size_t millis, ReturnType(*function)(ArgList...), ArgList... args,
        void(*callback)(ReturnType));

    /**
     * \brief Get handles to all processes currently running on the system.
     */
    std::unordered_set<Process> runningProcesses();

    /**
     * \brief Get the ID of the current process.
     */
    Pid getPid();
}

#include "async-private.h"

#ifdef OS

#if OS == WINDOWS

#include "async-windows-private.h"

#elif OS == UNIX

#include "async-unix-private.h"
#else

#error Unable to determine OS.

#endif
#endif