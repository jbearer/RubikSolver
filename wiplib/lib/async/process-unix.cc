#include "process.h"

class wiplib::ProcessUnix {

private:
    bool isRunning_;
    Pid pid_;

public:
    ProcessUnix() : isRunning_{false}
    {
        // Nothing else to do.
    }

    ProcessUnix(const char* filename, const char* argv[], const char* envp[])
    : isRunning_{false}, pid_{this->begin(filename, argv, envp)}
    {
        isRunning_ = true;
    }

    ProcessUnix(Pid pid)
    /// \todo We should check if the pid corresponds to a running process, and set isRunning_ accordingly.
    : isRunning_{true},
      pid_{pid}
    {
        // Nothing else to do
    }

    ProcessUnix(
        const wiplib::ProcessUnix&) = default;

    wiplib::ProcessUnix& operator=(
        const wiplib::ProcessUnix& other)
    {
        isRunning_ = other.isRunning_;
        pid_ = other.pid_;
        return *this;
    }

    ~ProcessUnix() {
        kill();
    }

    bool operator==(
        const wiplib::ProcessUnix& other) const
    {
        return pid_ == other.pid_;
    }

    bool operator!=(
        const wiplib::ProcessUnix& other) const
    {
        return !(*this == other);
    }

    Pid begin(const char* filename, const char* argv[], const char* envp[]) {
        /// \todo Implement ProcessUnix::begin.
        return 0;
    }

    void kill() {
        /// \todo Implement ProcessUnix::kill.
    }

    int reap() {
        /// \todo Implement ProcessUnix::reap.
        return 0;
    }

    bool isRunning() const {
        return isRunning_;
    }

    Pid getPid() const {
        return isRunning() ? pid_ : -1;
    }

    size_t std::hash<wiplib::ProcessUnix>::operator() (
        const wiplib::ProcessUnix& process) const noexcept
    {
        return process.getPid();
    }

};
