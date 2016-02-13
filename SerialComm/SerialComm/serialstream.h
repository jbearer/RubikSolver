#pragma once

#include "serial.h"

namespace serialComm {

	class serialstream;
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
		serialstream(char* port);

		/**
		 * \brief Close the communication.
		 */
		~serialstream();

		// Deleted members.
		serialstream() = delete;
		serialstream(const serialstream&) = delete;

	private:

		/// The port we want to communicate with.
		Serial port_;

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
}
