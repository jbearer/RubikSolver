#include "process.h"
#include "async.h"
#include "parse.h"
#include "containers.h"
#include "windows-util.h"
#include <Windows.h>
#include <string>
#include <algorithm>
#include <stdexcept>

#include <iostream>

/**
 * \class ProcessWindows
 * Windows-targeted implementation of the Process interface defined in
 * process.h.
 */
class wiplib::ProcessWindows {
private:

    /**
     * \brief Start executing the given executable in a new process.
     * \param filename The name of the executable file to run.
     * \param argv An array of strings containing the command line arguments
     *  to the new process.
     * \param envp  An array of strings of the form "key=value" describing
     *  the environment variables for the new process.
     * \return The Pid of the process.
     * \throws std::runtime_error {The process could not be started.}
     */
    static HANDLE exec(
        const std::string& filename,
        const std::string& argv);

    /// A handle to the currently running process.
    HANDLE handle_;

public:

    /// \todo Check result of every windows call to make sure nothing that should
    /// be succeeding is failing.

    ProcessWindows() : handle_{NULL}
    {
        // Nothing else to do.
    }

    ProcessWindows(
        const std::string& filename,
        const std::string& argv = "")
    : handle_{exec(filename, argv)}
    {}

    ProcessWindows(Pid pid)
    : handle_{OpenProcess(PROCESS_QUERY_INFORMATION, TRUE, pid)}
    {}

    ProcessWindows(
        const wiplib::ProcessWindows&) = default;

    wiplib::ProcessWindows& operator=(
        const wiplib::ProcessWindows& other)
    {
        handle_ = other.handle_;
        return *this;
    }

    ~ProcessWindows() {
        kill();
    }

    bool operator==(
        const wiplib::ProcessWindows& other) const
    {
        return getPid() == other.getPid();
    }

    bool operator!=(
        const wiplib::ProcessWindows& other) const
    {
        return !(*this == other);
    }

    Pid begin(
        const std::string& filename,
        const std::string& argv = "")
    {
        handle_ = exec(filename, argv);
        return getPid();
    }

    void kill(int exitCode = 0) {
        TerminateProcess(handle_, exitCode);

        // Don't return until the process is actually dead.
        reap();
    }

    int reap() {
        WaitForSingleObject(handle_, INFINITE);

        DWORD exitCode;
        GetExitCodeProcess(handle_, &exitCode);

        return exitCode;
    }

    bool isRunning() const {
        DWORD exitCode;
        GetExitCodeProcess(handle_, &exitCode);
        return exitCode == STILL_ACTIVE;
    }

    Pid getPid() const {
        return isRunning() ? GetProcessId(handle_) : -1;
    }

    HANDLE exec(
        const std::string& filename,
        const std::string& argv)
    {
        PROCESS_INFORMATION processInfo;
        STARTUPINFO startupInfo;

        ZeroMemory( &startupInfo, sizeof(startupInfo) );
        startupInfo.cb = sizeof(startupInfo);
        ZeroMemory( &processInfo, sizeof(processInfo) );

        LPTSTR lpCommandLine = NULL;

        // Make a deep copy of filename to get a mutable c_string
        char* filenameCstr = new char[filename.size() + 1];
        std::copy(filename.begin(), filename.end(), filenameCstr);
        filenameCstr[filename.size()] = '\0';

        if (!argv.empty()) {
            // Make a deep copy of argv to get a mutable C array
            std::string commandLine = filename + std::string(" ") + argv;
            lpCommandLine = new char[commandLine.size() + 1];
            std::copy(commandLine.begin(), commandLine.end(), lpCommandLine);
            lpCommandLine[commandLine.size()] = '\0';
        }

        if (!CreateProcess(filenameCstr, lpCommandLine,
            NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS, NULL, NULL, 
            &startupInfo, &processInfo)) 
        {
            delete[] filenameCstr;
            delete[] lpCommandLine;
            throw std::runtime_error(std::string("Unable to begin process \'") +
                filename + std::string("\':\n\t") + getLastErrorMessage());
        }

        else {
            delete[] filenameCstr;
            delete[] lpCommandLine;
            return processInfo.hProcess;
        }
    }
};
