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
std::string createId(std::string prefix = "", int length = 16);

/**
 * Convert a time value into a string representation.
 * @todo Maybe its better to use C++11 std::chrono::time_point instead of time_t
 *
 * @param time    The time to convert.
 *
 * @return The sting representation of time.
 */
std::string timeToStr(time_t time);

/**
 * Get the scaling between two SI units that are identified by the two strings.
 *
 * @param origin_unit the original unit
 * @param destination_unit the one into which a scaling should be done
 *
 * @return A double with the appropriate scaling
 * @throw Runtime Exception when units cannot be converted into each other by mere scaling
 */
double getScaling(const std::string &origin_unit, const std::string &destination_unit);

/**
 * Convert a string representation of a date into a time value.
 *
 * @param time    The string representation of a date.
 *
 * @return The time value that is represented by the time parameter.
 */
time_t strToTime(const std::string &time);

/**
 * Convert a number into a string representation.
 *
 * @param number  The number to convert
 *
 * @return The string representation of number
 */
template<typename T> std::string numToStr(T number) {
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
template<typename T> T strToNum(const std::string &str) {
    std::stringstream s(str);
    T number;
    return s >> number ? number : 0;
}



} // namespace util
} // namespace nix


#endif // NIX_UTIL_H
