#include "windows-util.h"

std::string wiplib::getLastErrorMessage() {
	LPVOID errorMessage;
    DWORD errorCode = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        errorCode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &errorMessage,
        0, NULL );

    return std::string((LPTSTR)errorMessage);
}
