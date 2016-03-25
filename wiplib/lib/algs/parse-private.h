#pragma once
#include <string>
#include <sstream>
#include <stdexcept>

template<typename T>
std::string wiplib::toString(const T& thing) {
    std::stringstream stream;
    stream << thing;
    return stream.str();
}
