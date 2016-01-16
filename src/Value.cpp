// Copyright © 2014, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>

#include <nix/Value.hpp>

#include <cassert>
#include <cstdlib>
#include <cstring>

namespace nix {

/* swap and swap helpers */



void Value::swap(Value &other) {
    using std::swap;

    Variant::swap(other);

    swap(uncertainty, other.uncertainty);
    swap(reference, other.reference);
    swap(filename, other.filename);
    swap(encoder, other.encoder);
    swap(checksum, other.checksum);
}

/* operators and functions */

std::ostream& operator<<(std::ostream &out, const Value &value)
{
    out << "Value{[" << value.type() << "] ";

    switch(value.type()) {
    case DataType::Bool:
        out << value.get<bool>();
        break;

    case DataType::String:
        out << value.get<std::string>();
        break;

    case DataType::Int32:
        out << value.get<int32_t>();
        break;

    case DataType::UInt32:
        out << value.get<uint32_t>();
        break;

    case DataType::Int64:
        out << value.get<int64_t>();
        break;

    case DataType::UInt64:
        out << value.get<uint64_t>();
        break;

    case DataType::Double:
        out << value.get<double>();
        break;

    case DataType::Nothing:
        break;

    default:
        out << "IMPLEMENT ME";
        break;
    }


    out << "}";
    return out;
}

bool operator==(const Value &a, const Value &b)
{

    bool match = static_cast<const Variant &>(a) == static_cast<const Variant &>(b);
    if (!match)
        return match;

    match = match & (a.reference == b.reference);
    match = match & (a.checksum == b.checksum);
    match = match & (a.uncertainty == b.uncertainty);
    match = match & (a.filename == b.filename);
    match = match & (a.encoder == b.encoder);

    return match;
}

void swap(Value &a, Value &b)
{
    a.swap(b);
}

}
