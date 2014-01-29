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

namespace nix {

std::ostream &operator<<(std::ostream &out, const DataType dtype) {
    out << "DataType::";

    switch(dtype) {

    case DataType::Bool:
        out << "Bool";
        break;

    case DataType::Char:
        out << "Char";
        break;

    case DataType::Float:
        out << "Float";
        break;

    case DataType::Double:
        out << "Double";
        break;

    case DataType::Int8:
        out << "Int8";
        break;

    case DataType::Int16:
        out << "Int16";
        break;

    case DataType::Int32:
        out << "Int32";
        break;

    case DataType::Int64:
        out << "Int64";
        break;

    case DataType::UInt8:
        out << "UInt8";
        break;

    case DataType::UInt16:
        out << "UInt16";
        break;

    case DataType::UInt32:
        out << "UInt32";
        break;

    case DataType::UInt64:
        out << "UInt64";
        break;

    case DataType::String:
        out << "String";
        break;

    case DataType::Nothing:
        out << "Nothing";
        break;

    default:
        out << "FIXME";
    }

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
