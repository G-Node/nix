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

namespace nix {

void Value::maybe_deallocte_string() {
    if (dtype == DataType::String) {
        std::free(v_string);
        dtype = DataType::Nothing;
    }
}

/**************/
/* setters */

void Value::set(none_t) {
    maybe_deallocte_string();

    dtype = DataType::Nothing;
    v_bool = false;
}

void Value::set(bool value) {
    maybe_deallocte_string();

    dtype = DataType::Bool;
    v_bool = value;
}

void Value::set(int32_t value) {
    maybe_deallocte_string();

    dtype = DataType::Int32;
    v_int32 = value;
}

void Value::set(uint32_t value) {
    maybe_deallocte_string();

    dtype = DataType::UInt32;
    v_uint32 = value;
}

void Value::set(int64_t value) {
    maybe_deallocte_string();

    dtype = DataType::Int64;
    v_int64 = value;
}

void Value::set(uint64_t value) {
    maybe_deallocte_string();

    dtype = DataType::UInt64;
    v_uint64 = value;
}

void Value::set(double value) {
    maybe_deallocte_string();

    dtype = DataType::Double;
    v_double = value;
}

void Value::set(const std::string &value) {
    set(value.c_str(), value.length());
}

void Value::set(const char *value, const size_t len) {

    const size_t len_plus_null = len + 1;
    void *data;

    if (dtype != DataType::String) {
        dtype = DataType::String;
        data = std::malloc(len_plus_null);
    } else {
        data = std::realloc(v_string, len_plus_null);
    }

    std::memcpy(data, value, len);
    v_string = static_cast<char *>(data);
    v_string[len] = '\0';
}

/**************/
/* getters */

void Value::get(none_t &tag) const {
    check_argument_type(DataType::Nothing);
}

void Value::get(bool &value) const {
    check_argument_type(DataType::Bool);
    value = v_bool;
}

void Value::get(int32_t &value) const {
    check_argument_type(DataType::Int32);
    value = v_int32;
}

void Value::get(uint32_t &value) const {
    check_argument_type(DataType::UInt32);
    value = v_uint32;
}

void Value::get(int64_t &value) const {
    check_argument_type(DataType::Int64);
    value = v_int64;
}

void Value::get(uint64_t &value) const {
    check_argument_type(DataType::UInt64);
    value = v_uint64;
}


void Value::get(double &value) const {
    check_argument_type(DataType::Double);
    value = v_double;
}

void Value::get(std::string &value) const {
    check_argument_type(DataType::String);
    value = v_string;
}

/* swap and swap helpers */


#if 0 //set to one to check that all supported DataTypes are handled
#define CHECK_SUPOORTED_VALUES
#endif

#define DATATYPE_SUPPORT_NOT_IMPLEMENTED false

void Value::swap(Value &other) {
    using std::swap;
    swap(uncertainty, other.uncertainty);
    swap(reference, other.reference);
    swap(filename, other.filename);
    swap(encoder, other.encoder);
    swap(checksum, other.checksum);

    //now for the variant members
    if (dtype == other.dtype) {
        switch(dtype) {
        case DataType::Nothing: /* nothing to do, literally */ break;
        case DataType::Bool:   swap(v_bool, other.v_bool);     break;
        case DataType::Int32:  swap(v_int32, other.v_int32);   break;
        case DataType::UInt32: swap(v_uint32, other.v_uint32); break;
        case DataType::Int64:  swap(v_int64, other.v_int64);   break;
        case DataType::UInt64: swap(v_uint64, other.v_uint64); break;
        case DataType::Double: swap(v_double, other.v_double); break;
        case DataType::String: swap(v_string, other.v_string); break;
#ifndef CHECK_SUPPORTED_VALUES
        default: assert(DATATYPE_SUPPORT_NOT_IMPLEMENTED);
#endif
        }

        swap(dtype, other.dtype);

    } else {

        switch(dtype) {
        case DataType::Nothing: swap_helper<none_t>(other);     break;
        case DataType::Bool:    swap_helper<bool>(other);        break;
        case DataType::Int32:   swap_helper<int32_t>(other);     break;
        case DataType::UInt32:  swap_helper<uint32_t>(other);    break;
        case DataType::Int64:   swap_helper<int64_t>(other);     break;
        case DataType::UInt64:  swap_helper<uint64_t>(other);    break;
        case DataType::Double:  swap_helper<double>(other);      break;
        case DataType::String:  swap_helper<std::string>(other); break;
#ifndef CHECK_SUPPORTED_VALUES
        default: assert(DATATYPE_SUPPORT_NOT_IMPLEMENTED);
#endif
        }
    }
}

void Value::assign_variant_from(const Value &other) {
    switch (other.dtype) {
    case DataType::Bool:    set(other.v_bool);   break;
    case DataType::Int32:   set(other.v_int32);  break;
    case DataType::UInt32:  set(other.v_uint32); break;
    case DataType::Int64:   set(other.v_int64);  break;
    case DataType::UInt64:  set(other.v_uint64); break;
    case DataType::Double:  set(other.v_double); break;
    case DataType::String:  set(std::string(other.v_string)); break;
    case DataType::Nothing: set(none);           break;

#ifndef CHECK_SUPPORTED_VALUES
    default: assert(DATATYPE_SUPPORT_NOT_IMPLEMENTED);
#endif
    }
}


bool Value::supports_type(DataType dtype) {
    switch (dtype) {
        case DataType::Bool:    // we fall through here ...
        case DataType::Int32:   // .oO ( what's happening ...
        case DataType::UInt32:  // ... who am I ...
        case DataType::Int64:   // ... why am I here ...
        case DataType::UInt64:  // ... wind, is that a good name ..
        case DataType::Double:  // ... lets call it tail ...
        case DataType::String:  // ... round, ... ground, ...
        case DataType::Nothing: // ... hello ground ...
            return true;

        default:
            return false;
    }
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
    if (a.type() != b.type()) {
        return false;
    }
    bool match = true;
    match = match & (a.reference == b.reference);
    match = match & (a.checksum == b.checksum);
    match = match & (a.uncertainty == b.uncertainty);
    match = match & (a.filename == b.filename);
    match = match & (a.encoder == b.encoder);
    if (!match)
        return match;

    switch(a.type()) {
    case DataType::Nothing: return true;
    case DataType::Bool:   return a.get<bool>() == b.get<bool>();
    case DataType::Int32:  return a.get<int32_t>() == b.get<int32_t>();
    case DataType::UInt32: return a.get<uint32_t>() == b.get<uint32_t>();
    case DataType::Int64:  return a.get<int64_t>() == b.get<int64_t>();
    case DataType::UInt64: return a.get<uint64_t>() == b.get<uint64_t>();
    case DataType::Double: return a.get<double>() == b.get<double>();
    case DataType::String: return a.get<std::string>() == b.get<std::string>();
#ifndef CHECK_SUPPORTED_VALUES
    default: assert(DATATYPE_SUPPORT_NOT_IMPLEMENTED); return false;
#endif
    }
}

void swap(Value &a, Value &b)
{
    a.swap(b);
}

}
