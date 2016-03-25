#include "parse.h"
#include <string>
#include <sstream>
#include <stdexcept>
#include <typeinfo>
#include <algorithm>
#include <utility>

using std::string;
using std::stringstream;

bool wiplib::isInt(string s) {
    auto i = s.begin();
    if (*i == '-') ++i;
    while (i != s.end()) {
        if (!isdigit(*i)) return false;
        ++i;
    }
    return true;
}

bool wiplib::isFloat(string s) {
    bool seenDecimal = false;

    auto i = s.begin();
    if (*i == '-') ++i;
    while (i != s.end()) {
        if (!seenDecimal && *i == '.') {
            // We are only allowed one decimal point.
            seenDecimal = true;
        }
        else if (*i == 'f') {
            // f is only allowed as the last character.
            return ++i == s.end();
        }
        else if (!isdigit(*i)) {
            return false;
        }
        ++i;
    }
    return true;
}

int wiplib::toInt(string s) {
    stringstream stream(s);
    int i;
    stream >> i;
    return i;
}

float wiplib::toFloat(string s) {
    stringstream stream(s);
    float f;
    stream >> f;
    return f;
}

bool wiplib::floatEqual(float x, float y, float tolerance) {
    if (x == 0) return y < tolerance;
    if (y == 0) return x < tolerance;

    float percentDiff1 = (x - y) / y;
    float percentDiff2 = (y - x) / x;
    return percentDiff1 < tolerance && percentDiff2 < tolerance;
}
