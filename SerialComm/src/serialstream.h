#pragma once

#include <sstream>
#include <string>
#include "serial.h"
#include "failed_read_error.h"
#include <iostream>

namespace SerialComm {

	class serialstream;

	typedef char* cstring;
	typedef serialstream&(*manipulator)(serialstream&);

	/// Custom stream manipulator, used like std::endl
	serialstream& endl(serialstream&);

	/**
	 * \class serialstream
	 * \brief A convenient interface for communicating with a serial port.
	 *	Usage is the same as std::ostream objects.
	 */
	class serialstream
	{
	public:

		/**
		* \brief Initialize Serial communication with the given COM port.
		*/
		serialstream(const char* port);

		/**
		 * \brief Close the communication.
		 */
		~serialstream();

		// Deleted members.
		serialstream() = delete;
		serialstream(const serialstream&) = delete;

	private:

		typedef Serial port_t;

		/// The port we want to communicate with.
		port_t port_;

		/**
		 * \brief Write data to the serial port.
		 * \note Type T must define an << operator with left operand of type
		 *	std::ostream or compatible
		 */
		template <class T>
		friend serialstream& operator<<(serialstream& stream, const T& data);
		friend serialstream& operator<<(serialstream& stream, manipulator m);

		/**
		 * \brief Read data from the serial port.
		 * \note Type T must define a >> operator with left operand of type
		 *	std::istream or compatible.
		 */
		template <class T>
		friend T& operator>>(serialstream& stream, T& in);

	};

	///////////////////////////////////////////////////////////////////////////
	// Member definitions
	///////////////////////////////////////////////////////////////////////////

	serialstream::serialstream(const char* port) : port_{ port } {
		// Nothing else to do.
	}


	serialstream::~serialstream() {
		// Nothing to do
	}

	///////////////////////////////////////////////////////////////////////////
	// Template definitions
	///////////////////////////////////////////////////////////////////////////

	template <class T>
	serialstream& operator<<(serialstream& stream, const T& data) {
		std::cout << "Outputting: " << data << std::endl;
		std::stringstream sstream;
		sstream << data;
		std::string dataString = sstream.str();

		const char* buffer = dataString.c_str();
		unsigned numBytes = dataString.size();

		stream.port_.writeData(buffer, numBytes);

		return stream;
	}

	/// Template specialization for std::string.
	template <>
	serialstream& operator<<(serialstream& stream, const std::string& data) {
		stream.port_.writeData(data.c_str(), data.size() + 1);
		return stream;
	}

	/// Template specialization for null-terminated cstring
	template <>
	serialstream& operator<<(serialstream& stream, const cstring& data) {
		for (size_t i = 0; data[i] != '\0'; ++i) {
			stream << data[i];
		}
		return stream;
	}

	serialstream& operator<<(serialstream& stream, manipulator m) {
		return m(stream);
	}

	template <class T>
	T& operator>>(serialstream& stream, T& in) {
		char* buffer;
		unsigned numBytes = sizeof(T);
		int bytesRead = stream.port_.readData(buffer, numBytes);

		if (!bytesRead) throw failed_read_error();

		// Convert to the right type.
		std::stringstream sstream;
		std::string strBuffer(buffer);
		sstream << strBuffer;
		sstream >> in;
		return in;
	}

	/// Template specialization for null-terminated cstring
	template <>
	cstring& operator>>(serialstream& stream, cstring& in) {
		size_t i = 0;
		do {
			if (!stream.port_.readData(in + i, 1)) {
				in[i] = '\0';
			}
		} while (in[i++] != '\0');
		return in;
	}

	/// Template specialization for std::string
	template <>
	std::string& operator>>(serialstream& stream, std::string& in) {
		cstring buffer;
		stream >> buffer;
		if (buffer[0] == '\0') {
			in = "";
		} else {
			in = buffer;
		}
		return in;
	}

	serialstream& endl(serialstream& stream) {
		stream << '\n';
		return stream;
	}
}
