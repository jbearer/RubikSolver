/**
 * \file color-stream-windows-private.h
 * Implementation of templated class ColorStreamWindows.
 * See color-stream-windows.h for details.
 */

#include <windows.h>
#include <Wincon.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <iostream>

wiplib::ColorStreamWindows::ColorStreamWindows()
:   console_{GetStdHandle(STD_OUTPUT_HANDLE)}
{
	supportedColors_["red"] = FOREGROUND_RED;
	supportedColors_["green"] = FOREGROUND_GREEN;
	supportedColors_["blue"] = FOREGROUND_BLUE;
	supportedColors_["white"] = 
		FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN;
	supportedColors_["black"] = 0;
	supportedColors_["yellow"] = FOREGROUND_RED | FOREGROUND_GREEN;
	supportedColors_["magenta"] = FOREGROUND_RED | FOREGROUND_BLUE;
	supportedColors_["cyan"] = FOREGROUND_BLUE | FOREGROUND_GREEN;
    GetConsoleScreenBufferInfo( console_, &defaults_ );
}

wiplib::ColorStreamWindows::~ColorStreamWindows() {
    resetColor();
}

bool wiplib::ColorStreamWindows::isSupported(std::string color) const {
	return supportedColors_.find(color) != supportedColors_.end();
}

std::vector<std::string> wiplib::ColorStreamWindows::supportedColors() const {
	std::vector<std::string> colors;
	for (auto kvPair : supportedColors_) {
		colors.push_back(kvPair.first);
	}
	return colors;
}

bool wiplib::ColorStreamWindows::setColor(std::string color) {
    if (!isSupported(color)) return false;

    return setColor(supportedColors_[color]);
}

void wiplib::ColorStreamWindows::resetColor() {
    SetConsoleTextAttribute(console_, defaults_.wAttributes);
}

template <class T>
wiplib::ColorStreamWindows& 
wiplib::ColorStreamWindows::operator<<(const T& output) {
    std::cout << output;
    return *this;
}

wiplib::ColorStreamWindows& 
wiplib::ColorStreamWindows::operator<<(std::ostream&(*manipulator)(std::ostream&)) {
    manipulator(std::cout);
    return *this;
}

bool wiplib::ColorStreamWindows::setColor(char color) {
    // Get the first 4 bits (background color code) of the user's settings.
    char bgColor = defaults_.wAttributes & 0xf0;

    if (bgColor == (color & 0x0f)) {
    	// The text is the same as the background color.
    	// Do not set the color.
    	return false;
    }

    // Set the new text color, preserving user's background color.
    SetConsoleTextAttribute(console_, color | bgColor);
    return true;
}
