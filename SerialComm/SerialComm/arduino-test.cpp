#include "serialstream.h"
#include "arduino-test/test-constants.h"

#define PORT "1"
#define NUM_BLINKS 10
#define INTERVAL 500

using namespace serialComm;
using namespace testConstants;

void waitForResponse(serialstream& stream) {
	SerialMessage response = WAIT;
	while (response != CONTINUE) {
		stream >> response;
	}
}

int main() {

	serialstream stream(PORT);

	// Test char transmission
	stream << TEST_CHAR;
	waitForResponse(stream);
	
	// Test float transmission
	stream << TEST_FLOAT;
	waitForResponse(stream);

	// Test string transmission
	stream << TEST_STRING;
	waitForResponse(stream);

	// Test endl
	stream << TEST_STRING << endl << TEST_CHAR;
	waitForResponse(stream);

	// Test blink
	for (size_t i = 0; i < NUM_BLINKS; ++i) {
		Sleep(INTERVAL / 2);
		stream << BLINK;
		Sleep(INTERVAL / 2);
		stream << CONTINUE;
	}

	return 0;
}