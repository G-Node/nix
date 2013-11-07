// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

/**
 * @file pandora/Util.hpp
 * @brief Declaration of utility functions.
 *
 * @namespace pandora::util
 * @brief Namespace for utility functions. This namespace is not part of the public API.
 */

#ifndef PAN_UTIL_H_INCLUDED
#define PAN_UTIL_H_INCLUDED

#include <string>
#include <sstream>

namespace pandora {
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
} // namespace pandora


#endif /* PAN_UTIL_H_INCLUDED */
