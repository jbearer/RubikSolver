#pragma once
#include <string>
#include <functional>
#include "os-macros.h"
#include "async.h"

namespace wiplib {

    class ProcessWindows;
    class ProcessUnix;

    #ifdef OS

    #if OS == WINDOWS

    typedef ProcessWindows ProcessImpl;

    #elif OS == UNIX

    typedef ProcessUnix ProcessImpl;

    #else

    #error Unable to determine OS.

    #endif
    #endif

    /**
     * \class Process
     * Can be used to create and manage processes in a multi-processing
     * environment.
     */
    class Process {

    public:

        /**
         * \brief Construct a process with no specified executable.
         * \post isRunning() returns false.
         */
        Process();

        /**
         * \brief Construct a process to execute the given function, and begin
         *  executing the function.
         * \throws std::runtime_error {The process could not be started.}
         * \post If successful, isRunning() returns true and getPid() returns
         *  the correct ID of the process.
         */
        Process(
            const std::string& filename,
            const std::string& argv = "");

        /**
         * \brief Construct a process which is a handle to a currently running
         *  process.
         * \param pid The ID of the desired process.
         */
        Process(Pid pid);

        Process(const Process& other);

        /// \todo Process move constructor.

        /**
         * \brief Kill the process.
         */
        ~Process();

        Process& operator=(const Process& other);

        /// \todo Process move assignment operator.

        /// \todo Process swap.

        /**
         * \brief Determine if two Process objects refer to the same process.
         */
        bool operator==(const Process& other) const;

        /**
         * \brief Determine if two Process objects do not refer to the same
         *  process.
         */
        bool operator!=(const Process& other) const;

        /**
         * \brief Start executing the given executable in a new process.
         * \param filename The name of the executable file to run.
         * \param argv (optional) A strings representing the command line
         *  arguments to the new process. If not present, no arguments are
         *  given to the command line.
         * \return The Pid of the process.
         * \post The state of this object reflects the state of the
         *  corresponding process.
         * \throws std::runtime_error {The process could not be started.}
         */
        Pid begin(
            const std::string& filename,
            const std::string& argv = "");

        /**
         * \brief Force the process to stop running.
         * \param exitCode (optional) The exit code with which the proess should
         *  terminate. Defaults to 0.
         * \post isRunning() returns false.
         */
        void kill(int exitCode = 0);

        /**
         * \brief Blocks execution until the process terminates.
         * \return The exit code of the process.
         * \post isRunning() returns false.
         */
        int reap();

        /**
         * \return True if and only if this object corresponds to a currently
         *  running process.
         */
        bool isRunning() const;

        /**
         * \return The ID of the process, or -1 if no process is running.
         */
        Pid getPid() const;
        
    private:

        ProcessImpl *impl_;

    };
}

namespace std {
template<>
struct hash<wiplib::Process> {
    size_t operator() (const wiplib::Process&) const noexcept;
};
}
