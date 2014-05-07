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

#include <cstdint>
#include <ostream>

#include <nix/Platform.hpp>

namespace nix {

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

    Nothing = -1
};

template<typename T>
struct to_data_type {
    static const bool is_valid = false;
};


template<>
struct to_data_type<bool> {
    static const bool is_valid = true;
    static const DataType value = DataType::Bool;
};

template<>
struct to_data_type<char> {
    static const bool is_valid = true;
    static const DataType value = DataType::Char;
};

template<>
struct to_data_type<float> {
    static const bool is_valid = true;
    static const DataType value = DataType::Float;
};

template<>
struct to_data_type<double> {
    static const bool is_valid = true;
    static const DataType value = DataType::Double;
};

template<>
struct to_data_type<int8_t> {
    static const bool is_valid = true;
    static const DataType value = DataType::Int8;
};

template<>
struct to_data_type<uint8_t> {
    static const bool is_valid = true;
    static const DataType value = DataType::UInt8;
};

template<>
struct to_data_type<int16_t> {
    static const bool is_valid = true;
    static const DataType value = DataType::Int16;
};

template<>
struct to_data_type<uint16_t> {
    static const bool is_valid = true;
    static const DataType value = DataType::UInt16;
};

template<>
struct to_data_type<int32_t> {
    static const bool is_valid = true;
    static const DataType value = DataType::Int32;
};

template<>
struct to_data_type<uint32_t> {
    static const bool is_valid = true;
    static const DataType value = DataType::UInt32;
};

template<>
struct to_data_type<int64_t> {
    static const bool is_valid = true;
    static const DataType value = DataType::Int64;
};

template<>
struct to_data_type<uint64_t> {
    static const bool is_valid = true;
    static const DataType value = DataType::UInt64;
};

template<>
struct to_data_type<std::string> {
    static const bool is_valid = true;
    static const DataType value = DataType::String;
};

NIXAPI size_t data_type_to_size(DataType dtype);
NIXAPI std::string data_type_to_string(DataType dtype);

NIXAPI std::ostream& operator<<(std::ostream &out, const DataType dtype);


} // nix::

#endif
