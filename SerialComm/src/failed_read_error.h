#pragma once

#include <stdexcept>

/**
 * \class failed_read_error
 * \brief Indicates an attempted serial returned with a read length of 0 bytes.
 */
class failed_read_error :
	public std::runtime_error
{
public:
	failed_read_error();
};

