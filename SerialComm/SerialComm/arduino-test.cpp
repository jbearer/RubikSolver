#include "serialstream.h"
#include "test-constants.h"
#include <iostream>
#include <stdexcept>

using namespace serialComm;
using namespace testConstants;


void waitForResponse(serialstream& stream) {
	const int INTERVAL = 100;
	const int TIME_OUT = 100000000;

	SerialMessage response = WAIT;
	for (int i = 0; i < TIME_OUT / INTERVAL; ++i) {
		try {
			stream >> response;
		} catch (failed_read_error) {
			response = WAIT;
			continue;
		}
		if (response == CONTINUE) return;

		Sleep(INTERVAL);
	}

	throw std::runtime_error("Timed out.");
}

int main() {

	const int NUM_BLINKS = 10;
	const int INTERVAL = 500;
	const char* PORT = "COM8";

	serialstream stream(PORT);

	std::cout << "Testing char transmission." << std::endl;
	stream << TEST_CHAR;
	waitForResponse(stream);
	std::cout << "Received response." << std::endl;
	
	std::cout << "Testing float transmission." << std::endl;
	stream << TEST_FLOAT;
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
		Sleep(INTERVAL / 2);
		std::cout << "BLINK" << std::endl;
		stream << BLINK;
		Sleep(INTERVAL / 2);
		stream << CONTINUE;
	}

	std::cout << "Finished testing." << std::endl;

	return 0;
}