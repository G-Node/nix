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
#include <iostream>
#include <vector>
#include <cmath>
#include <type_traits>

#include <boost/optional.hpp>
#include <boost/none_t.hpp>

#include <nix/Exception.hpp>
#include <nix/Platform.hpp>


namespace nix {
    
enum class DimensionType : unsigned int;

namespace util {

/**
 * @brief Remove blank spaces from the entire string
 *
 * @param str   The string to trim (will be modified in-place)
 */
NIXAPI void deblankString(std::string &str);

/**
 * @brief Remove blank spaces from the entire string
 *
 * @param str   The string to trim
 *
 * @return The trimmed string
 */
NIXAPI std::string deblankString(const std::string &str);

/**
 * @brief Generates an ID-String.
 *
 * @return The generated id string.
 */
NIXAPI std::string createId();

/**
 * @brief Convert a time value into a string representation.
 *
 * @param time    The time to convert.
 *
 * @return The sting representation of time.
 */
NIXAPI std::string timeToStr(time_t time);

/**
 * @brief Convert a string representation of a date into a time value.
 *
 * @param time    The string representation of a date.
 *
 * @return The time value that is represented by the time parameter.
 */
NIXAPI time_t strToTime(const std::string &time);

/**
 * @brief Convert a time value into a string representation.
 *
 * @return The default time.
 */
NIXAPI time_t getTime();

/**
 * @brief Extract id from given entity. Does not work for dimensions
 *
 * @return The entity id.
 */
template<typename T>
std::string toId(const T &entity) {
    return entity.id();
}

/**
 * @brief Extract name from given entity. Does not work for dimensions
 *
 * @return The entity name.
 */
template<typename T>
std::string toName(const T &entity) {
    return entity.name();
}

/**
 * @brief Sanitizer function that deblanks units and replaces mu and µ
 * with the "u" replacement.
 *
 * @param unit The old unit.
 *
 * @return The sanitized unit.
 */
NIXAPI std::string unitSanitizer(const std::string &unit);

/**
 * @brief Checks if the passed string represents a valid SI unit.
 *
 * @param unit  A string that is supposed to represent an SI unit.
 *
 * @return True if a valid SI unit, false otherwise.
 */
NIXAPI bool isSIUnit(const std::string &unit);

/**
 * @brief Checks if the passed string is a valid combination of SI units.
 *
 * For example mV^2*Hz^-1. Method accepts only the * notation.
 *
 * @param unit  A string that should be tested
 *
 * @return True if a valid compound si unti, false otherwise.
 */
NIXAPI bool isCompoundSIUnit(const std::string &unit);

/**
 * @brief Returns whether or not two units represent scalable versions
 * of the same SI unit.
 *
 * @param unitA A string representing the first unit.
 * 
 * @param unitB A string representing the second unit.
 *
 * @return True if the units are scalable version of the same unit.
 **/
NIXAPI bool isScalable(const std::string &unitA, const std::string &unitB);

/**
 * @brief Returns whether or not in all cases the units at the same
 * index in the two given unit vectors are scalable versions of the same
 * SI unit.
 *
 * @param unitsA A vector of unit strings.
 * 
 * @param unitsB A vector of unit strings.
 *
 * @return True if the units are scalable version of the same unit.
 */
NIXAPI bool isScalable(const std::vector<std::string> &unitsA, const std::vector<std::string> &unitsB);

/**
 * @brief Returns whether or not in all cases the strings at the same
 * index in the two given string vectors are either both set or both
 * not set (empty).
 *
 * @param stringsA A vector of unit strings.
 * 
 * @param stringsB A vector of unit strings.
 *
 * @return True if the units are scalable version of the same unit.
 */
NIXAPI bool isSetAtSamePos(const std::vector<std::string> &stringsA, const std::vector<std::string> &stringsB);

/**
 * @brief Get the scaling between two SI units that are identified by the two strings.
 *
 * @param originUnit            The original unit
 * @param destinationUnit       The one into which a scaling should be done
 *
 * @return A double with the appropriate scaling
 *
 * @throw std::runtime_error Exception when units cannot be converted into each other by mere scaling
 */
NIXAPI double getSIScaling(const std::string &originUnit, const std::string &destinationUnit);

/**
 * Splits an SI unit into prefix, unit and the power components.
 *
 * @param fullUnit
 * @param prefix
 * @param unit
 * @param power
 */
NIXAPI void splitUnit(const std::string &fullUnit, std::string &prefix, std::string &unit, std::string &power);

/**
 * Splits a SI unit compound into its atomic parts.
 *
 * @param compoundUnit  An SI unit that consists of many atomic units
 * @param atomicUnits   A vector that takes the atomic units
 */
NIXAPI void splitCompoundUnit(const std::string &compoundUnit, std::vector<std::string> &atomicUnits);

/**
 * @brief Converts minutes and hours to seconds.
 *
 * @param unit  The original unit (i.e. h for hour, or min for minutes)
 * @param value The original value
 *
 * @return The value in converted to seconds
*/
template<typename T>
T convertToSeconds(const std::string &unit, T value) {
    T seconds;
    if (unit == "min") {
        seconds = value * 60;
    } else if (unit == "h") {
        std::string new_unit = "min";
        seconds = convertToSeconds(new_unit, value * 60);
    } else if (unit == "s" || unit == "sec") {
        seconds = value;
    } else if (isScalable(unit, "s")) {
        seconds = value * getSIScaling(unit, "s");
    } else {
        std::cerr <<  "[nix::util::convertToSeconds] Warning: given unit is not supported!" << std::endl;
        seconds = value;
    }
    return seconds;
}

/**
 * @brief Converts temperatures given in degrees Celsius of Fahrenheit to Kelvin.
 *
 * @param unit  The original unit {"F", "°F", "C", "°C"}
 * @param value The original value
 *
 * @return The temperature in Kelvin
 */
template<typename T>
T convertToKelvin(const std::string &unit, T value) {
   T temperature;
   if (unit == "°C" || unit == "C") {
       temperature = value + 273.15;
   } else if (unit == "°F" || unit == "F") {
       double temp = (value - 32) * 5.0/9 + 273.15;
       temperature = std::is_integral<T>::value ? std::round(temp) : temp;
   } else if (unit == "°K" || unit == "K") {
       temperature = value;
   } else if (isScalable(unit, "K")) {
       temperature = value * getSIScaling(unit, "K");
   } else {
       std::cerr << "[nix::util::convertToKelvin] Warning: given unit is not supported" << std::endl;
       temperature = value;
   }
   return temperature;
}

/**
 * @brief Convert a number into a string representation.
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
 * @brief Convert a DimensionType into a string representation.
 *
 * @param number  The DimensionType to convert
 *
 * @return The string representation of the DimensionType
 */
NIXAPI std::string dimTypeToStr(const DimensionType &dtype);

/**
 * @brief Convert string to number
 * 
 * Convert a string representing a number into a number.
 *
 * @param str   The string to convert.
 *
 * @return The number that was represented by the string.
 */
template<typename T>
T strToNum(const std::string &str) {
    std::stringstream s(str);
    T number;
    return s >> number ? number : 0;
}

/*
 * Check whether a given type is of type "boost::optional"
 */
template<typename>
struct is_optional : std::false_type {};
template<typename T>
struct is_optional<boost::optional<T>> : std::true_type {};

/*
 * Optional de-referencing:
 * De-reference boost optional type if such given, returned var
 * unchanged otherwise.
 */
template<typename T>
T deRef(T var) {
    return var;
}
template<typename R>
R deRef(boost::optional<R> var) {
    if(var) return *var;
    else return R();
}

} // namespace util
} // namespace nix


#endif // NIX_UTIL_H
