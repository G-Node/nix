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
#include <stdexcept>

namespace nix {

std::string data_type_to_string(DataType dtype) {

    std::string str;

    switch(dtype) {

    case DataType::Bool:    str = "Bool";    break;
    case DataType::Char:    str = "Char";    break;
    case DataType::Float:   str = "Float";   break;
    case DataType::Double:  str = "Double";  break;
    case DataType::Int8:    str = "Int8";    break;
    case DataType::Int16:   str = "Int16";   break;
    case DataType::Int32:   str = "Int32";   break;
    case DataType::Int64:   str = "Int64";   break;
    case DataType::UInt8:   str = "UInt8";   break;
    case DataType::UInt16:  str = "UInt16";  break;
    case DataType::UInt32:  str = "UInt32";  break;
    case DataType::UInt64:  str = "UInt64";  break;
    case DataType::String:  str = "String";  break;
    case DataType::Nothing: str = "Nothing"; break;
    default:
        str = "FIXME";
    }

    return str;
}


std::ostream &operator<<(std::ostream &out, const DataType dtype) {
    out << "DataType::" << data_type_to_string(dtype);
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
