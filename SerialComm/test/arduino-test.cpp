#include "../src/serialstream.h"
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

void waitForResponse(serialstream& stream) {
	const int NUM_CHECKS = 1000;
	const int WAIT_TIME = 10000;

	SerialMessage response = WAIT;
	for (int i = 0; i < NUM_CHECKS; ++i) {
		try {
			stream >> response;
		} catch (failed_read_error) {
			response = WAIT;
		}
		if (response == CONTINUE) return;
		else if (response == ERR) {
			std::cout << "Received error from serial port." << std::endl;
			return;
		}

		//for debugging
		if (response != WAIT) std::cout << "READ BYTE: " << response << std::endl;
		//

		delay(WAIT_TIME / NUM_CHECKS);
	}

	throw std::runtime_error("Timed out.");
}

int main() {

	const char* PORT = "COM8";

	serialstream stream(PORT);

	std::cout << "Testing char transmission." << std::endl;
	stream << TEST_CHAR;
	waitForResponse(stream);
	std::cout << "Received response." << std::endl;

	std::cout << "Testing C string transmission." << std::endl;
	stream << TEST_STRING;
	waitForResponse(stream);
	std::cout << "Received response." << std::endl;

	std::cout << "Testing custom endl." << std::endl;
	stream << TEST_STRING << endl << TEST_CHAR;
	waitForResponse(stream);
	std::cout << "Received response." << std::endl;

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