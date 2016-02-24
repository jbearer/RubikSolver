#include "test-constants.h"

using namespace testConstants;

const char RECEIVE_INDICATOR = 8;
const char SEND_INDICATOR = 9;
const char OUTPUT_PIN = 10;

const int INDICATOR_DELAY = 500;

// Indicate that an operation entered an error state by rapidly blinking a pin
// and writing an error code to the serial port
// Pre: pin is in HIGH position
// Post: pin is in HIGH position
void indicateError(char pin) {
  const int NUM_BLINKS = 5;
  const int BLINK_TIME = INDICATOR_DELAY / 5;

  Serial.println(ERR, DEC);

  for (int i = 0; i < NUM_BLINKS; ++i) {
    digitalWrite(pin, LOW);
    delay(BLINK_TIME);
    digitalWrite(pin, HIGH);
    delay(BLINK_TIME);
  }
}

// Indicate a successful operation by blinking a pin
// Pre: pin is in HIGH position
// Post: pin is in HIGH position
void indicateSuccess(char pin) {
   const int NUM_BLINKS = 2;
  const int BLINK_TIME = 1000;

  for (int i = 0; i < NUM_BLINKS; ++i) {
    digitalWrite(pin, LOW);
    delay(BLINK_TIME);
    digitalWrite(pin, HIGH);
    delay(BLINK_TIME);
  }
}

// Send a transmission to the serial port, and indicate transmission by illuminating LED
// (cstring overload)
void sendTransmission(const char* message) {
  digitalWrite(SEND_INDICATOR, HIGH);
  while (message && *message != '\0') {
    Serial.print(*message);
    ++message;
  }
  Serial.println();
  delay(INDICATOR_DELAY);
  digitalWrite(SEND_INDICATOR, LOW);
}

// char overload
void sendTransmission(char message) {
  digitalWrite(SEND_INDICATOR, HIGH);
  Serial.write(message);
  delay(INDICATOR_DELAY);
  digitalWrite(SEND_INDICATOR, LOW);
}

// Read a transmission from the serial port, and indicate progress by illuminating LED
// Return number of bytes read, or -numBytesRead if the input terminated without a null character
// cstring overload
int getTransmission(char* buff) {

  int bufferSize = 256;
  int stringLength = 0;
  buff = new char[bufferSize];

  if (!Serial.available()) {
    delete buff;
    buff = 0;
    return 0;
  }

  digitalWrite(SEND_INDICATOR, HIGH);
  while (Serial.available()) {

    // Read another character and add it to the string
    char nextChar = Serial.read();
    buff[stringLength] = nextChar;
    ++stringLength;

    if (nextChar == '\0') {
      // End of string
      delay(INDICATOR_DELAY);
      digitalWrite(RECEIVE_INDICATOR, LOW);
      return stringLength;
    }

    if (stringLength >= bufferSize) {
      // Move to a new buffer with double the capacity
      char* newBuffer = new char[bufferSize*2];
      for (int i = 0; i < bufferSize; ++i) {
        newBuffer[i] = buff[i];
      }

      // Free the old buffer
      delete[] buff;
      buff = newBuffer;
    }
  }

  // We reached the end of the input without encountering a null character
  delay(INDICATOR_DELAY);
  indicateError(RECEIVE_INDICATOR);
  digitalWrite(RECEIVE_INDICATOR, LOW);
  return -stringLength;
}

// char overload
// return 1 if successful read or 0 if unsuccessful
int getTransmission(char& data) {
  
  if (Serial.available()) {
    digitalWrite(RECEIVE_INDICATOR, HIGH);
    data = Serial.read();
    delay(INDICATOR_DELAY);
    digitalWrite(RECEIVE_INDICATOR, LOW);
    return 1;
  }
  return 0;
}

// return true if we were able to read a byte
// return false only if we failed to read a byte
// indicate error only if we read a byte and it did not match the expected byte
bool testGetChar(const char expected) {
  char transmission;
  if (!getTransmission(transmission)){
    return false;
  }

  if (transmission != expected) indicateError(OUTPUT_PIN);
  return true;
}

bool strEqual(const char* actual, const char* expected) {
  if (!actual || !expected) return false;
  while(*actual == *expected) {
    if (*actual == '\0') return true;
    ++actual;
    ++expected;
  }
  return false;
}

// return true if we were able to read a string
// return false only if we failed to read any bytes
// indicate error only if we read a string and it did not match the expected byte,
// or if the string was not null-terminated
bool testGetString(const char* expected, const int expectedLength) {
  char* transmission;
  int bytesRead = getTransmission(transmission);
  if (!getTransmission(transmission)) {
    return false;
  }
  else if (bytesRead != expectedLength || !strEqual(transmission, expected)) {
    indicateError(OUTPUT_PIN);
  }
  return true;
}

bool testBlink() {
   char instruction;
   if (!getTransmission(instruction)) return false;
   if (instruction == BLINK) digitalWrite(OUTPUT_PIN, HIGH);
   else if (instruction == CONTINUE) digitalWrite(OUTPUT_PIN, LOW);
   else return false;
   return true;
}

void setup() {
  pinMode(RECEIVE_INDICATOR, OUTPUT);
  pinMode(SEND_INDICATOR, OUTPUT);
  pinMode(OUTPUT_PIN, OUTPUT);
  Serial.begin(9600);
}

void passedTest() {
  indicateSuccess(OUTPUT_PIN);
  digitalWrite(OUTPUT_PIN, LOW);
  sendTransmission(CONTINUE);
  indicateSuccess(OUTPUT_PIN);
  digitalWrite(OUTPUT_PIN, LOW);
}

void loop() {
  static int testNum = 0;

  switch(testNum) {
    case 0:
     if (testGetChar(TEST_CHAR)) {
      passedTest();
      //++testNum; COMMENTED OUT TO ONLY TEST CHARS
     }
     break;
    case 1:
      if (testGetString(TEST_STRING, TEST_STRING_LEN)) {
        passedTest();
        ++testNum;
      }
      break;
    default:
      testBlink();
  }
}
