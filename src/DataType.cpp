// Copyright © 2014 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>

#include <nix/DataType.hpp>

#include <string>
#include <stdexcept>
#include <map>
#include <algorithm>

namespace nix {

std::string data_type_to_string(DataType dtype) {

    std::string str;

    switch(dtype) {
    case DataType::Bool:     str = "Bool";     break;
    case DataType::Char:     str = "Char";     break;
    case DataType::Float:    str = "Float";    break;
    case DataType::Double:   str = "Double";   break;
    case DataType::Int8:     str = "Int8";     break;
    case DataType::Int16:    str = "Int16";    break;
    case DataType::Int32:    str = "Int32";    break;
    case DataType::Int64:    str = "Int64";    break;
    case DataType::UInt8:    str = "UInt8";    break;
    case DataType::UInt16:   str = "UInt16";   break;
    case DataType::UInt32:   str = "UInt32";   break;
    case DataType::UInt64:   str = "UInt64";   break;
    case DataType::String:   str = "String";   break;
    case DataType::Nothing:  str = "Nothing";  break;
    case DataType::Date:     str = "Date";     break;
    case DataType::DateTime: str = "Datetime"; break;
    case DataType::Opaque:   str = "Opaque";   break;
    }

    return str;
}

DataType string_to_data_type(const std::string& dtype) {

    static std::map<std::string, DataType> type_map = {
        {"bool", DataType::Bool},
        {"char", DataType::Char},
        {"float", DataType::Float},
        {"single", DataType::Float},
        {"double", DataType::Double},
        {"int8", DataType::Int8},
        {"int16", DataType::Int16},
        {"int32", DataType::Int32},
        {"int64", DataType::Int64},
        {"uint8", DataType::UInt8},
        {"uint16", DataType::UInt16},
        {"uint32", DataType::UInt32},
        {"uint64", DataType::UInt64},
        {"string", DataType::String},
        {"date", DataType::Date},
        {"datetime", DataType::DateTime},
        {"opaque", DataType::Opaque},
        {"nothing", DataType::Nothing}
    };

    std::string dtype_l = dtype;
    std::transform(dtype_l.begin(), dtype_l.end(), dtype_l.begin(), ::tolower);

    if (type_map.find(dtype_l) == type_map.end()) {
        throw std::invalid_argument("Unkown DataType");
    }

    return type_map[dtype_l];
}


bool data_type_is_numeric(DataType dtype) {
    return (dtype == DataType::UInt8 ||
            dtype == DataType::UInt16 ||
            dtype == DataType::UInt32 ||
            dtype == DataType::UInt64 ||
            dtype == DataType::Int8 ||
            dtype == DataType::Int16 ||
            dtype == DataType::Int32 ||
            dtype == DataType::Int64 ||
            dtype == DataType::Float ||
            dtype == DataType::Double);
}


std::ostream &operator<<(std::ostream &out, const DataType dtype) {
    out << data_type_to_string(dtype);
    return out;
}


size_t data_type_to_size(DataType dtype) {
    switch(dtype) {

        case DataType::Bool:
            return sizeof(bool);

        case DataType::Int8:
        case DataType::UInt8:
            return 1;

        case DataType::Int16:
        case DataType::UInt16:
            return 2;

        case DataType::Int32:
        case DataType::UInt32:
        case DataType::Float:
            return 4;

        case DataType::Int64:
        case DataType::UInt64:
        case DataType::Double:
            return 8;

        //strings are char *, but not sure that is the correct thing to do here
        case DataType::String:
            return sizeof(char *);

        default:
            throw std::invalid_argument("Unkown DataType");
    }
}

}
