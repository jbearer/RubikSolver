#include "serialstream.h"
#include <sstream>
#include <string>

using std::stringstream;
using std::string;

namespace serialComm {

	serialstream::serialstream(char* port) : port_{ port } {
		// Nothing else to do.
	}


	serialstream::~serialstream() {
		// Nothing to do.
	}

	template <class T>
	serialstream& operator<<(serialstream& stream, const T& data) {
		stringstream sstream;
		sstream << data;
		string dataString = sstream.str();

		const char* buffer = dataString.c_str();
		unsigned numBytes = dataString.size();

		stream.port_.writeData(buffer, numBytes);

		return stream;
	}

	/// Template specialization for std::string.
	template <>
	serialstream& operator<<(serialstream& stream, const string& data) {
		stream.port_.writeData(data.c_str(), data.size() + 1);
		return stream;
	}

	/// Template specialization for null-terminated cstring
	template <>
	serialstream& operator<<(serialstream& stream, const cstring& data) {
		for (size_t i = 0; data[i] != '\0'; ++i) {
			stream << data[i];
		}
	}

	serialstream& operator<<(serialstream& stream, manipulator m) {
		return m(stream);
	}

	template <class T>
	T& operator>>(serialstream& stream, T& in) {
		char* buffer;
		unsigned numBytes = sizeof(T);
		stream.port_.readData(buffer, numBytes);

		// Convert to the right type.
		stringstream sstream;
		sstream << (string)buffer;
		sstream >> in;
		return in;
	}

	/// Template specialization for null-terminated cstring
	template <>
	cstring& operator>>(serialstream& stream, cstring& in) {
		size_t i = 0;
		do {
			stream.port_.readData(in + i, 1);
			++i;
		} while (in[i] != '\0');
		return in;
	}

	/// Template specialization for std::string
	template <>
	string& operator>>(serialstream& stream, string& in) {
		cstring buffer;
		stream >> buffer;
		in = buffer;
		return in;
	}

	serialstream& endl(serialstream& stream) {
		stream << '\n';
		return stream;
	}
}
