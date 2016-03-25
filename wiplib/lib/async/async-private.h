#include <time.h>
#include "thread.h"
#include "process.h"
#include <unordered_set>

void wiplib::delay(size_t milliseconds) {
    clock_t endTime = clock() + milliseconds * (CLOCKS_PER_SEC / 1000);
    while (clock() < endTime) {}
}

bool wiplib::timedMonitor(bool(*condition)(), size_t millis) {
	clock_t endTime = clock() + millis * (CLOCKS_PER_SEC / 1000);
	while (clock() < endTime) {
		if (condition()) return true;
	}
	return false;
}

// Need this because gcc doesn't expand variadic templates in anonymous lambdas.
// grrr....
template<typename ReturnType, typename... ArgList>
void scheduleHelper(
    size_t millis, ReturnType(*function)(ArgList...), ArgList... args) 
{
	wiplib::delay(millis);
	function(args...);
}

template<typename ReturnType, typename... ArgList>
void wiplib::schedule(
    size_t millis, ReturnType(*function)(ArgList...), ArgList... args)
{
    Thread myThread( scheduleHelper );
}

// Need this because gcc doesn't expand variadic templates in anonymous lambdas.
// grrr....
template<typename ReturnType, typename... ArgList>
void scheduleCallbackHelper(
    size_t millis, ReturnType(*function)(ArgList...), ArgList... args,
    void(*callback)(ReturnType))
{
	wiplib::delay(millis);
	callback(function(args...));
}

template<typename ReturnType, typename... ArgList>
void wiplib::schedule(
    size_t millis, ReturnType(*function)(ArgList...), ArgList... args,
    void(*callback)(ReturnType))
{
    Thread(scheduleCallbackHelper);
}
