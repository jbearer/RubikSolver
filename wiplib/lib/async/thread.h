/**
 * \headerfile thread.h
 * Interface for a thread object.
 *
 * \todo Implement this interface.
 */

#pragma once

namespace wiplib {
    class Thread {
    public:

        /**
         * \brief Construct a thread with no specified function.
         */
        Thread();

        /**
         * \brief Construct a thread to execute the given function, and begin
         *  executing the function.
         */
        template<typename ReturnType, typename... Args>
        Thread(ReturnType(*function)(Args...));

        void join();

        void detach();
        
    private:

    };
}

#ifdef OS

#if OS == WINDOWS

#include "thread-windows-private.h"

#elif OS == UNIX

#include "thread-unix-private.h"
#else

#error Unable to determine OS.

#endif
#endif
