#include "process.h"
#include <string>

wiplib::Process::Process() : impl_{new ProcessImpl} {}

wiplib::Process::Process(
            const std::string& filename,
            const std::string& argv = "")
: impl_{new ProcessImpl(filename, argv)} {}

wiplib::Process::Process(Pid pid) : impl_{new ProcessImpl(pid)} {}

wiplib::Process::Process(const wiplib::Process& other)
: impl_{new ProcessImpl(*(other.impl_))} {}

wiplib::Process& wiplib::Process::operator=(
    const wiplib::Process& other)
{
    delete impl_;
    impl_ = new ProcessImpl(*(other.impl_));
}

wiplib::Process::~Process() {
    delete impl_;
}

bool wiplib::Process::operator==(const wiplib::Process& other) {
    return *impl_ == *(other.impl_);
}

bool wiplib::Process::operator!=(const wiplib::Process& other) {
    return *impl_ != *(other.impl_);
}

Pid wiplib::Process::begin(
    const std::string& filename,
    const std::string& argv = "")
{
    return impl_->begin(filename, argv);
}

void wiplib::Process::kill(int exitCode) {
    impl_->kill(exitCode);
}

int wiplib::Process::reap() {
    return impl_->reap();
}

bool wiplib::Process::isRunning() const {
    return impl_->isRunning();
}

Pid wiplib::Process::getPid() const {
    return impl_->getPid();
}

size_t std::hash<wiplib::Process>::operator() (
    const wiplib::Process& process) const noexcept
{
    std::hash<Pid> pidHash;
    return pidHash(process.getPid());
}