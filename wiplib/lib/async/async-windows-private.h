#include <windows.h>
#include <Psapi.h>
#include "process.h"

std::unordered_set<wiplib::Process> wiplib::runningProcesses() {

    Pid *pids = nullptr;
    unsigned numProcesses;
    size_t bufSize = 64;

    do {
        // Enumerate processes with an increasing buffer size until the buffer
        // is big enough for all the processes.
        delete pids;
        pids = new Pid[bufSize];

        DWORD numBytes;

        EnumProcesses(pids, sizeof(pids), &numBytes);
        numProcesses = numBytes / sizeof(Pid);

        // Increase the buffer size to prepare for another attempt.
        bufSize *= 2;
    } while (numProcesses >= bufSize);

    std::unordered_set<wiplib::Process> processes;
    for (size_t i = 0; i < numProcesses; ++i) {
        processes.emplace(pids[i]);
    }

    return processes;
}

Pid wiplib::getPid() {
    return GetCurrentProcessId();
}
