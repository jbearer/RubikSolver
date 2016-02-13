/**
* \file serial.cpp
* \author http://playground.arduino.cc/Interfacing/CPPWindows, 2016
*	Modified by Jeb Bearer, 2016
*/

#include "serial.h"
#include <stdexcept>
#include <string>

Serial::Serial(const char *portName)
	: connected_{ false },
	hSerial_{ CreateFile(portName, GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL) } {

	// Check if the connection was unsuccessful.
	if (this->hSerial_ == INVALID_HANDLE_VALUE)
	{
		// Unsuccessful connection.

		if (GetLastError() == ERROR_FILE_NOT_FOUND) {
			throw std::runtime_error(
				"ERROR: Handle was not attached. Reason: " + (std::string)portName + " not available.");
		}
		else
		{
			throw std::runtime_error("ERROR: Handle was not attached. Reason unknown.");
		}
	}

	// Successful connection, set comms parameters.

	DCB dcbSerialParams = { 0 };

	// Try to get the current serial parameters.
	if (!GetCommState(this->hSerial_, &dcbSerialParams))
	{
		throw std::runtime_error("ERROR: Failed to get current serial parameters.");
	}

	// Define serial connection parameters for the arduino board.
	dcbSerialParams.BaudRate = CBR_9600;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;

	// Setting the DTR to Control_Enable ensures that the Arduino is properly
	// reset upon establishing a connection.
	dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;

	// Set the parameters and check for their proper application.
	if (!SetCommState(hSerial_, &dcbSerialParams))
	{
		throw std::runtime_error("ERROR: Could not set Serial Port parameters.");
	}

	// If everything went fine we're connected.
	this->connected_ = true;

	// Flush any remaining characters in the buffers.
	PurgeComm(this->hSerial_, PURGE_RXCLEAR | PURGE_TXCLEAR);

	// We wait 2s as the arduino board will be reseting.
	Sleep(ARDUINO_WAIT_TIME);

}

Serial::~Serial() {
	// Check if we are connected before trying to disconnect.
	if (this->connected_)
	{
		this->connected_ = false;

		// Close the serial handler.
		CloseHandle(this->hSerial_);
	}
}

int Serial::readData(char *buffer, unsigned int numBytes) {

	// Number of bytes we'll try to read.
	unsigned int toRead;

	// Number of bytes that are actually read.
	DWORD bytesRead;

	// Use the ClearCommError function to get status info on the Serial port.
	ClearCommError(this->hSerial_, &this->errors_, &this->status_);

	// Check if there is something to read.
	if (this->status_.cbInQue > 0) {
		// Check if there is enough data to read the required number of bytes.
		if (this->status_.cbInQue > numBytes) {
			toRead = numBytes;
		} else {
			// Read only the available characters to prevent.
			toRead = this->status_.cbInQue;
		}

		//Try to read the require number of chars, and return the number of read bytes on success
		if (ReadFile(this->hSerial_, buffer, toRead, &bytesRead, NULL)) {
			return bytesRead;
		}

	}

	// Nothing has been read or an error was detected
	return 0;

}


bool Serial::writeData(const char *buffer, unsigned int numBytes)
{
	DWORD bytesSend;

	// Try to write the buffer to the Serial port.
	if (!WriteFile(this->hSerial_, (void *)buffer, numBytes, &bytesSend, 0))
	{
		// Write failed, get comm error and return false.
		ClearCommError(this->hSerial_, &this->errors_, &this->status_);

		return false;
	}
	else
		return true;
}

bool Serial::isConnected() const
{
	return this->connected_;
}

