#pragma once
#include <windows.h>
#include <string>

namespace wiplib {

    /**
     * \brief Retrieve a message describing the current error code, as obtained
     *  from getLastError().
     */
    std::string getLastErrorMessage();
}