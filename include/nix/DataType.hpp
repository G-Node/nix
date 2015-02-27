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

#include <nix/Platform.hpp>
#include <map>

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
    Date,
    DateTime,
    Opaque,

    Nothing = -1
};

/**
 * @brief Determine if a type is a valid data type.
 */
template<typename T>
struct to_data_type {
    static const bool is_valid = false;
};

/**
 * @brief Determine if a type is a valid data type.
 */
template<>
struct to_data_type<bool> {
    static const bool is_valid = true;
    static const DataType value = DataType::Bool;
};

/**
 * @brief Determine if a type is a valid data type.
 */
template<>
struct to_data_type<char> {
    static const bool is_valid = true;
    static const DataType value = DataType::Char;
};

/**
 * @brief Determine if a type is a valid data type.
 */
template<>
struct to_data_type<float> {
    static const bool is_valid = true;
    static const DataType value = DataType::Float;
};

/**
 * @brief Determine if a type is a valid data type.
 *
 * @internal
 */
template<>
struct to_data_type<double> {
    static const bool is_valid = true;
    static const DataType value = DataType::Double;
};

/**
 * @brief Determine if a type is a valid data type.
 */
template<>
struct to_data_type<int8_t> {
    static const bool is_valid = true;
    static const DataType value = DataType::Int8;
};

/**
 * @brief Determine if a type is a valid data type.
 */
template<>
struct to_data_type<uint8_t> {
    static const bool is_valid = true;
    static const DataType value = DataType::UInt8;
};

/**
 * @brief Determine if a type is a valid data type.
 */
template<>
struct to_data_type<int16_t> {
    static const bool is_valid = true;
    static const DataType value = DataType::Int16;
};

/**
 * @brief Determine if a type is a valid data type.
 */
template<>
struct to_data_type<uint16_t> {
    static const bool is_valid = true;
    static const DataType value = DataType::UInt16;
};

/**
 * @brief Determine if a type is a valid data type.
 */
template<>
struct to_data_type<int32_t> {
    static const bool is_valid = true;
    static const DataType value = DataType::Int32;
};

/**
 * @brief Determine if a type is a valid data type.
 */
template<>
struct to_data_type<uint32_t> {
    static const bool is_valid = true;
    static const DataType value = DataType::UInt32;
};

/**
 * @brief Determine if a type is a valid data type.
 */
template<>
struct to_data_type<int64_t> {
    static const bool is_valid = true;
    static const DataType value = DataType::Int64;
};

/**
 * @brief Determine if a type is a valid data type.
 */
template<>
struct to_data_type<uint64_t> {
    static const bool is_valid = true;
    static const DataType value = DataType::UInt64;
};

/**
 * @brief Determine if a type is a valid data type.
 */
template<>
struct to_data_type<std::string> {
    static const bool is_valid = true;
    static const DataType value = DataType::String;
};

/**
* @brief Map of the DataType to its string representation.
*/
struct data_type_maps {
    static std::map<std::string, DataType> str_to_dtype;
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
* @return A human readable name for the given type.
*/
NIXAPI DataType string_to_data_type(std::string dtype);

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
