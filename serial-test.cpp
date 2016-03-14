#include "serialstream.h"
#include "arduino-test/test-constants.h"
#include <iostream>
#include <stdexcept>
#include <time.h>

using namespace SerialComm;
using namespace testConstants;

void delay(int milliseconds)
{
	clock_t endwait;
	endwait = clock() + milliseconds * (CLOCKS_PER_SEC / 1000);
	while (clock() < endwait) {}
}

int main() {

	const char* PORT = "\\\\.\\COM10";

	serialstream stream(PORT);

	for(size_t i = 0; i < NUM_CHARS; ++i) {

		std::cout << "Testing char " << i << " transmission." << std::endl;
		stream << TEST_CHAR;

	}

	std::cout << "Testing blink." << std::endl;
	for (size_t i = 0; i < NUM_BLINKS; ++i) {
		delay(INTERVAL / 2);
		std::cout << "BLINK" << std::endl;
		stream << BLINK;
		delay(INTERVAL / 2);
		stream << CONTINUE;
	}

	std::cout << "Finished testing." << std::endl;

	return 0;
}