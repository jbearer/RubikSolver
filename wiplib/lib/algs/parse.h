/**
 * \headerfile parse.h
 * Declares several methods useful for parseing strings.
 */

#include <string>
#include <sstream>
#include <stdexcept>
#include <typeinfo>
#include <vector>

namespace wiplib {

/**
 * \brief Determine if the string s is a valid representation of an integer.
 */
bool isInt(std::string s);

/**
 * \brief Determine if the string s is a valid representation of a floating
 *	point number.
 */
bool isFloat(std::string s);

/**
 * \brief Convert a string to an integer.
 * \param s A string to convert.
 * \return The integer value of s.
 */
int toInt(std::string s);

/**
 * \brief Convert a string to a float.
 * \param s A string to convert.
 * \return The floating point value of s.
 */
float toFloat(std::string s);

/**
 * \brief Convert some object to a string.
 */
template<typename T>
std::string toString(const T& thing);

/**
 * \brief Check if two floating point numbers.
 * \details Evaluates to true if and only if the percentage difference between
 *	the numbers is less than tolerance.
 * \param tolerance The percentage difference less than which two numbers are
 *	considered equal. Defaults to 0.1%.
 */
bool floatEqual(float x, float y, float tolerance = 0.001);

}

#include "parse-private.h"
