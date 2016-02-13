/**
 * \file serial.h
 * \author http://playground.arduino.cc/Interfacing/CPPWindows, 2016
 *	Modified by Jeb Bearer, 2016
 */

#pragma once
#define ARDUINO_WAIT_TIME 2000

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

class Serial
{

public:

	/**
	 * \brief Initialize Serial communication with the given COM port.
	 */
	Serial(char *portName);

	/**
	 * \brief Close the connection.
	 */
	~Serial();

	/**
	 * \brief Read data from the port into a buffer. 
	 * \note if nbChar is greater than the maximum number of bytes available,
	 *	only the maximum numbr of bytes will be read. 
	 * \return The number of bytes read.
	 * \param buffer A pointer to a char buffer which will be loaded with the
	 *	incoming data.
	 * \param numBytes The number of bytes to read.
	 */
	int readData(char *buffer, unsigned int numBytes);

	/** 
	 * \breif Write data from a buffer to the Serial connection.
	 * \return True on success.
	 * \param buffer A pointer to a char buffer containing the data to write.
	 * \param numBytes The number of bytes to write.
	 */
	bool writeData(const char *buffer, unsigned int numBytes);

	/**
	 * \brief Check if connected to the serial port.
	 */
	bool isConnected() const;

private:

	/// Connection status.
	bool connected_;

	/// Serial comm handler.
	HANDLE hSerial_;

	/// Get various information about the connection.
	COMSTAT status_;

	/// Keep track of last error.
	DWORD errors_;

};
