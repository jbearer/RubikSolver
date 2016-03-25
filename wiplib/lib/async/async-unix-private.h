#include <sys/types.h>
#include <signal.h>

std::unordered_set<wiplib::Process> wiplib::runningProcesses() {
	/// \todo implement runningProcesses
	return std::unordered_set<Process>();
}

Pid wiplib::getPid() {
	return ::getPid();
}
