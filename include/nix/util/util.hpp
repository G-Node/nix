// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

/**
 * @namespace nix::util
 * @brief Namespace for utility functions. This namespace is not part of the public API.
 */

#ifndef NIX_UTIL_H
#define NIX_UTIL_H

#include <string>
#include <sstream>
#include <vector>
#include <boost/optional.hpp>
#include <boost/lexical_cast.hpp>

#include <nix/Platform.hpp>
#include <nix/Exception.hpp>


namespace nix {
namespace util {

/**
 * Generates an ID-String.
 *
 * @param prefix    The prefix to append to the generated id.
 * @param length    The length of the ID.
 *
 * @return The generated id string.
 */
NIXAPI std::string createId(std::string prefix = "", int length = 16);

/**
 * Convert a time value into a string representation.
 * @todo Maybe its better to use C++11 std::chrono::time_point instead of time_t
 *
 * @param time    The time to convert.
 *
 * @return The sting representation of time.
 */
NIXAPI std::string timeToStr(time_t time);

/**
 * Convert a string representation of a date into a time value.
 *
 * @param time    The string representation of a date.
 *
 * @return The time value that is represented by the time parameter.
 */
NIXAPI time_t strToTime(const std::string &time);

/**
 * Convert a time value into a string representation.
 * @todo Maybe its better to use C++11 std::chrono::time_point instead of time_t
 *
 * @return The default time.
 */
NIXAPI time_t getTime();

/**
 * Checks if the passed string represents a valid SI unit.
 *
 * @param unit a string that is supposed to represent an SI unit.
 * @return bool true or false
 */
NIXAPI bool isSIUnit(const std::string &unit);

/**
 * Checks if the passed string is a valid combination of SI units.
 * For example mV^2*Hz^-1. Method accepts only the * notation.
 *
 * @param unit a string that should be tested
 * @return bool
 */
NIXAPI bool isCompoundSIUnit(const std::string &unit);

/**
 * Get the scaling between two SI units that are identified by the two strings.
 *
 * @param originUnit the original unit
 * @param destinationUnit the one into which a scaling should be done
 *
 * @return A double with the appropriate scaling
 * @throw Runtime Exception when units cannot be converted into each other by mere scaling
 */
NIXAPI double getSIScaling(const std::string &originUnit, const std::string &destinationUnit);

/**
 * Splits an SI unit into prefix, unit and the power components.
 * @param fullUnit
 * @param prefix
 * @param unit
 * @param power
 */
NIXAPI void splitUnit(const std::string &fullUnit, std::string &prefix, std::string &unit, std::string &power);

/**
 * Splits a SI unit compound into its atomic parts.
 *
 * @param compoundUnit string representing an SI unit that consists of many atomic units
 * @param atomicUnits vector<string> that takes the atomic units
 */
NIXAPI void splitCompoundUnit(const std::string &compoundUnit, std::vector<std::string> &atomicUnits);

/**
 * Convert a number into a string representation.
 *
 * @param number  The number to convert
 *
 * @return The string representation of number
 */
template<typename T> 
std::string numToStr(T number) {
    std::stringstream s;
    s << number;
    return s.str();
}

/**
 * Convert a string representing a number into a number.
 *
 * @param str   The string to convert.
 *
 * @return The number that was represented by the string.
 */
template<typename T> 
T strToNum(const std::string &str) {
    return boost::lexical_cast<T>(str);
}

/**
 * Check whether a given type is of type "boost::optional".
 * Usage: 
 * myBool = is_optional<decltype(myVar)>::value; 
 * myBool = is_optional<MY_TYPE>::value; 
 *
 * @param type   Template param: The type to check.
 *
 * @return bool (use '::value' to check result)
 */
template<typename>
struct is_optional : std::false_type {};
template<typename T>
struct is_optional<boost::optional<T>> : std::true_type {};

/**
 * Convert an unknown type to bool - only handles string explicitely
 * and attempts cast to bool an all other types. Useful when type
 * is either bool-convertible or string, but unknown which of the two.
 * 
 *
 * @param type   The variable to convert.
 *
 * @return bool
 */
template<typename T>
bool TtoBool(T var);

} // namespace util
} // namespace nix


#endif // NIX_UTIL_H
