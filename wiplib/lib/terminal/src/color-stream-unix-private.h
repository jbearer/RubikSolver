/**
 * \file color-stream-unix-private.h
 * Implementation of templated class ColorStreamUnix.
 * See color-stream-unix.h for details.
 */

#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>

wiplib::ColorStreamUnix::ColorStreamUnix() {
	supportedColors_["black"] = 30;
	supportedColors_["red"] = 31;
	supportedColors_["green"] = 32;
	supportedColors_["yellow"] = 33;
	supportedColors_["blue"] = 34;
	supportedColors_["magenta"] = 35;
	supportedColors_["cyan"] = 36;
	supportedColors_["white"] = 37;
}

wiplib::ColorStreamUnix::~ColorStreamUnix() {
    resetColor();
}

bool wiplib::ColorStreamUnix::isSupported(std::string color) const {
	/// \todo Make sure terminal supports (enough) colors
	return supportedColors_.find(color) != supportedColors_.end();
}

std::vector<std::string> wiplib::ColorStreamUnix::supportedColors() const {
	std::vector<std::string> colors;
	for (auto kvPair : supportedColors_) {
		colors.push_back(kvPair.first);
	}
	return colors;
}

bool wiplib::ColorStreamUnix::setColor(std::string color) {
    if (!isSupported(color)) return false;

    setColor(supportedColors_[color]);
    return true;
}

void wiplib::ColorStreamUnix::resetColor() {
    setColor(0);
}

template <class T>
wiplib::ColorStreamUnix& 
wiplib::ColorStreamUnix::operator<<(const T& output) {
    std::cout << output;
    return *this;
}

wiplib::ColorStreamUnix& 
wiplib::ColorStreamUnix::operator<<(std::ostream&(*manipulator)(std::ostream&)) {
    manipulator(std::cout);
    return *this;
}

void wiplib::ColorStreamUnix::setColor(char color) {
    std::string escapeSequence = "\033[";
    escapeSequence.push_back(color);
    escapeSequence.push_back('m');
    std::cout << escapeSequence;
}
