// Copyright © 2014, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>

#ifndef NIX_DATATYPE_H
#define NIX_DATATYPE_H

#include <nix/Platform.hpp>

#include <cstdint>
#include <ostream>

namespace std {
#if !defined(WIN32) && __cplusplus < 201300L
template<bool B, class T = void>
using enable_if_t = typename std::enable_if<B, T>::type;
#endif
}

namespace nix {

/**
 * @brief Enumeration providing constants for all valid data types.
 *
 * Those data types are used by {@link nix::DataArray} and {@link nix::Property}
 * in order to indicate of what type the stored data of value is.
 */
NIXAPI enum class DataType {
    Bool,
    Char,
    Float,
    Double,
    Int8,
    Int16,
    Int32,
    Int64,
    UInt8,
    UInt16,
    UInt32,
    UInt64,
    String,
    Opaque,

    Nothing = -1
};

// helper functions
template<typename T>
using is_true_numeric = std::integral_constant<bool,
        std::is_integral<T>::value &&
        !(std::is_same<T, bool>::value ||
          std::is_same<T, char>::value)
>;

template<typename T, bool SIGN, size_t SIZE>
using is_integer_with_sign_and_size = std::integral_constant<bool,
        is_true_numeric<T>::value &&
        std::is_signed<T>::value == SIGN &&
        sizeof(T) == SIZE
>;



/**
 * @brief Determine if a type is a valid data type.
 */
template<typename T, class Enable = void>
struct to_data_type {
    static const bool is_valid = false;
};

template<typename T>
struct to_data_type<T, typename std::enable_if_t<std::is_same<T, bool>::value>> {
    static const bool is_valid = true;
    static const DataType value = DataType::Bool;
};

template<typename T>
struct to_data_type<T, typename std::enable_if_t<std::is_same<T, char>::value>> {
    static const bool is_valid = true;
    static const DataType value = DataType::Char;
};

template<typename T>
struct to_data_type<T, typename std::enable_if_t<
        std::is_floating_point<T>::value && sizeof(T) == 4>
> {
    static const bool is_valid = true;
    static const DataType value = DataType::Float;
};

template<typename T>
struct to_data_type<T, typename std::enable_if_t<
        std::is_floating_point<T>::value && sizeof(T) == 8>
> {
    static const bool is_valid = true;
    static const DataType value = DataType::Double;
};

template<typename T>
struct to_data_type<T, typename std::enable_if_t<
        is_integer_with_sign_and_size<T, true, 1>::value
> > {
    static const bool is_valid = true;
    static const DataType value = DataType::Int8;
};

template<typename T>
struct to_data_type<T, typename std::enable_if_t<
        is_integer_with_sign_and_size<T, false, 1>::value
> > {
    static const bool is_valid = true;
    static const DataType value = DataType::UInt8;
};

template<typename T>
struct to_data_type<T, typename std::enable_if_t<
        is_integer_with_sign_and_size<T, true, 2>::value
>> {
    static const bool is_valid = true;
    static const DataType value = DataType::Int16;
};

template<typename T>
struct to_data_type<T, typename std::enable_if_t<
        is_integer_with_sign_and_size<T, false, 2>::value
>> {
    static const bool is_valid = true;
    static const DataType value = DataType::UInt16;
};

template<typename T>
struct to_data_type<T, typename std::enable_if_t<
        is_integer_with_sign_and_size<T, true, 4>::value
>> {
    static const bool is_valid = true;
    static const DataType value = DataType::Int32;
};

template<typename T>
struct to_data_type<T, typename std::enable_if_t<
        is_integer_with_sign_and_size<T, false, 4>::value
>> {
    static const bool is_valid = true;
    static const DataType value = DataType::UInt32;
};

template<typename T>
struct to_data_type<T, typename std::enable_if_t<
        is_integer_with_sign_and_size<T, true, 8>::value
>> {
    static const bool is_valid = true;
    static const DataType value = DataType::Int64;
};

template<typename T>
struct to_data_type<T, typename std::enable_if_t<
        is_integer_with_sign_and_size<T, false, 8>::value
>> {
    static const bool is_valid = true;
    static const DataType value = DataType::UInt64;
};

template<typename T>
struct to_data_type<T, typename std::enable_if_t<
        std::is_same<T, std::string>::value
>> {
    static const bool is_valid = true;
    static const DataType value = DataType::String;
};

/**
 * @brief Determine the size of a data type.
 *
 * @param dtype         The data type.
 *
 * @return The size of the type.
 */
NIXAPI size_t data_type_to_size(DataType dtype);

/**
 * @brief Convert a data type into string representation.
 *
 * @param dtype         The data type.
 *
 * @return A human readable name for the given type.
 */
NIXAPI std::string data_type_to_string(DataType dtype);

/**
* @brief Yield a data type from a string representation.
*
* @param dtype         The string-like data type.
*
* @return The DataType matching the provided string.
*/
NIXAPI DataType string_to_data_type(const std::string& dtype);

/**
* @brief Determines whether a DataType is numeric.
*
* @param dtype      The data type
*
* @return True when dtype represents a numeric data type, false otherwise
*/
NIXAPI bool data_type_is_numeric(DataType dtype);

/**
 * @brief Output operator for data type.
 *
 * Prints a human readable string representation of the
 * data type to an output stream.
 *
 * @param out           The output stream.
 * @param dtype         The data type to print.
 *
 * @return The output stream.
 */
NIXAPI std::ostream& operator<<(std::ostream &out, const DataType dtype);


} // nix::

#endif
