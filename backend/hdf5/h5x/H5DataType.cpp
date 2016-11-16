// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.


#include "H5DataType.hpp"

#include <stdexcept>
#include <iostream>
#include <cassert>
#include <string>
#include <memory>
#include <cstdlib>

namespace nix {
namespace hdf5 {
namespace h5x {

DataType DataType::copy(hid_t source) {
    DataType hi_copy = H5Tcopy(source);
    hi_copy.check("Could not copy type");
    return hi_copy;
}

DataType DataType::make(H5T_class_t klass, size_t size) {
    DataType dt = H5Tcreate(klass, size);
    dt.check("Could not create datatype");
    return dt;
}

DataType DataType::makeStrType(size_t size) {
    DataType str_type = H5Tcopy(H5T_C_S1);
    str_type.check("Could not create string type");
    str_type.size(size);
    return str_type;
}

DataType DataType::makeCompound(size_t size) {
    DataType res = H5Tcreate(H5T_COMPOUND, size);
    res.check("Could not create compound type");
    return res;
}

DataType DataType::makeEnum(const DataType &base) {
    DataType res = H5Tenum_create(base.h5id());
    res.check("Could not create enum type");
    return res;
}

H5T_class_t DataType::class_t() const {
    return H5Tget_class(hid);
}

void DataType::size(size_t t) {
    HErr res = H5Tset_size(hid, t);
    res.check("DataType::size: Could not set size");
}

size_t DataType::size() const {
    return H5Tget_size(hid); //FIXME: throw on 0?
}

void DataType::sign(H5T_sign_t sign) {
    HErr res = H5Tset_sign(hid, sign);
    res.check("DataType::sign(): H5Tset_sign failed");
}

H5T_sign_t DataType::sign() const {
    H5T_sign_t res = H5Tget_sign(hid);
    return res;
}

bool DataType::isVariableString() const {
    HTri res = H5Tis_variable_str(hid);
    res.check("DataType::isVariableString(): H5Tis_variable_str failed");
    return res.result();
}

bool DataType::isCompound() const {
    return class_t() == H5T_COMPOUND;
}

unsigned int DataType::member_count() const {
    int res = H5Tget_nmembers(hid);
    if (res < 0) {
        throw H5Exception("DataType::member_count(): H5Tget_nmembers faild");
    }
    return static_cast<unsigned  int>(res);
}

H5T_class_t DataType::member_class(unsigned int index) const {
    return H5Tget_member_class(hid, index);
}

std::string DataType::member_name(unsigned int index) const {
    char *data = H5Tget_member_name(hid, index);
    std::string res(data);
    std::free(data);
    return res;
}

std::vector<std::string> DataType::member_names() const {
    unsigned int c = member_count();
    std::vector<std::string> names;

    for (unsigned int i = 0; i < c; i++) {
        names.emplace_back(member_name(i));
    }

    return names;
}

size_t DataType::member_offset(unsigned int index) const {
    return H5Tget_member_offset(hid, index);
}

DataType DataType::member_type(unsigned int index) const {
    h5x::DataType res = H5Tget_member_type(hid, index);
    res.check("DataType::member_type(): H5Tget_member_type failed");
    return res;
}


void DataType::insert(const std::string &name, size_t offset, const DataType &dtype) {
    HErr res = H5Tinsert(hid, name.c_str(), offset, dtype.hid);
    res.check("DataType::insert(): H5Tinsert failed.");
}

void DataType::insert(const std::string &name, void *value) {
    HErr res = H5Tenum_insert(hid, name.c_str(), value);
    res.check("DataType::insert(): H5Tenum_insert failed.");
}

void DataType::enum_valueof(const std::string &name, void *value) {
    HErr res = H5Tenum_valueof(hid, name.c_str(), value);
    res.check("DataType::enum_valueof(): H5Tenum_valueof failed");
}

bool DataType::enum_equal(const DataType &other) const {
    if (class_t() != H5T_ENUM || other.class_t() != H5T_ENUM) {
        return false;
    }

    std::vector<std::string> a_names = this->member_names();
    std::vector<std::string> b_names = other.member_names();

    if (a_names.size() != b_names.size()) {
        return false;
    }

    std::sort(std::begin(a_names), std::end(a_names));
    std::sort(std::begin(b_names), std::end(b_names));


    return std::equal(std::begin(a_names),
                      std::end(a_names),
                      std::begin(b_names));
}

} // h5x


h5x::DataType make_file_booltype() {
    h5x::DataType booltype = h5x::DataType::makeEnum(H5T_NATIVE_INT8);
    booltype.insert("FALSE", 0UL);
    booltype.insert("TRUE", 1UL);
    return booltype;
}

h5x::DataType make_mem_booltype() {
    h5x::DataType booltype = h5x::DataType::make(H5T_ENUM, sizeof(bool));
    booltype.insert("FALSE", false);
    booltype.insert("TRUE", true);
    return booltype;
}

static const h5x::DataType boolfiletype = make_file_booltype();
static const h5x::DataType boolmemtype = make_mem_booltype();

h5x::DataType data_type_to_h5_filetype(DataType dtype) {

   /* The switch is structured in a way in order to get
      warnings from the compiler when not all cases are
      handled and throw an exception if one of the not
      handled cases actually appears (i.e., we have no
      default case, because that silences the compiler.)
   */

    switch (dtype) {

        case DataType::Bool:   return boolfiletype;
        case DataType::Int8:   return h5x::DataType::copy(H5T_STD_I8LE);
        case DataType::Int16:  return h5x::DataType::copy(H5T_STD_I16LE);
        case DataType::Int32:  return h5x::DataType::copy(H5T_STD_I32LE);
        case DataType::Int64:  return h5x::DataType::copy(H5T_STD_I64LE);
        case DataType::UInt8:  return h5x::DataType::copy(H5T_STD_U8LE);
        case DataType::UInt16: return h5x::DataType::copy(H5T_STD_U16LE);
        case DataType::UInt32: return h5x::DataType::copy(H5T_STD_U32LE);
        case DataType::UInt64: return h5x::DataType::copy(H5T_STD_U64LE);
        case DataType::Float:  return h5x::DataType::copy(H5T_IEEE_F32LE);
        case DataType::Double: return h5x::DataType::copy(H5T_IEEE_F64LE);
        case DataType::String: return h5x::DataType::makeStrType();
        //shall we create our own OPAQUE type?
        case DataType::Opaque: return h5x::DataType::copy(H5T_NATIVE_OPAQUE);

        case DataType::Char: break; //FIXME
        case DataType::Nothing: break;
    }

    throw std::invalid_argument("Unkown DataType"); //FIXME
}

h5x::DataType data_type_to_h5_memtype(DataType dtype) {

    // See data_type_to_h5_filetype for the reason why the switch is structured
    // in the way it is.

    switch(dtype) {
        case DataType::Bool:   return boolmemtype;
        case DataType::Int8:   return h5x::DataType::copy(H5T_NATIVE_INT8);
        case DataType::Int16:  return h5x::DataType::copy(H5T_NATIVE_INT16);
        case DataType::Int32:  return h5x::DataType::copy(H5T_NATIVE_INT32);
        case DataType::Int64:  return h5x::DataType::copy(H5T_NATIVE_INT64);
        case DataType::UInt8:  return h5x::DataType::copy(H5T_NATIVE_UINT8);
        case DataType::UInt16: return h5x::DataType::copy(H5T_NATIVE_UINT16);
        case DataType::UInt32: return h5x::DataType::copy(H5T_NATIVE_UINT32);
        case DataType::UInt64: return h5x::DataType::copy(H5T_NATIVE_UINT64);
        case DataType::Float:  return h5x::DataType::copy(H5T_NATIVE_FLOAT);
        case DataType::Double: return h5x::DataType::copy(H5T_NATIVE_DOUBLE);
        case DataType::String: return h5x::DataType::makeStrType();
        case DataType::Opaque: return h5x::DataType::copy(H5T_NATIVE_OPAQUE);

        case DataType::Char: break; //FIXME
        case DataType::Nothing: break;
    }

    throw std::invalid_argument("DataType not handled!"); //FIXME
}


h5x::DataType data_type_to_h5(DataType dtype, bool for_memory) {
    if (for_memory) {
        return data_type_to_h5_memtype(dtype);
    } else {
        return data_type_to_h5_filetype(dtype);
    }
}

#define NOT_IMPLEMENTED false

DataType
data_type_from_h5(H5T_class_t vclass, size_t vsize, H5T_sign_t vsign)
{
    if (vclass == H5T_INTEGER) {
        switch (vsize) {
        case 1: return vsign == H5T_SGN_2 ? DataType::Int8  : DataType::UInt8;
        case 2: return vsign == H5T_SGN_2 ? DataType::Int16 : DataType::UInt16;
        case 4: return vsign == H5T_SGN_2 ? DataType::Int32 : DataType::UInt32;
        case 8: return vsign == H5T_SGN_2 ? DataType::Int64 : DataType::UInt64;
        }
    } else if (vclass == H5T_FLOAT) {
        return vsize == 8 ? DataType::Double : DataType::Float;
    } else if (vclass == H5T_STRING) {
        return DataType::String;
    } else if (vclass == H5T_BITFIELD) {
        switch (vsize) {
            case 1: return DataType::Bool;
        }
    } else if (vclass == H5T_ENUM) {
        switch (vsize) {
            case 1: return DataType::Bool;
        }
    }

    std::cerr << "FIXME: Not implemented " << vclass << " " << vsize << " " << vsign << " " << std::endl;
    assert(NOT_IMPLEMENTED);
    return DataType::Nothing;
}


DataType data_type_from_h5(const h5x::DataType &dtype) {

    H5T_class_t ftclass = dtype.class_t();

    size_t     size;
    H5T_sign_t sign;

    if (ftclass == H5T_COMPOUND) {
        //if it is a compound data type then it must be a
        //a property dataset, we can handle that
        int nmems = dtype.member_count();
        assert(nmems == 6);
        h5x::DataType vtype = dtype.member_type(0);

        ftclass = vtype.class_t();
        size = vtype.size();
        sign = vtype.sign();

        if (ftclass == H5T_ENUM) {
            if (!boolfiletype.enum_equal(vtype)) {
                return DataType::Nothing;
            }
            return DataType::Bool;
        }
    } else if (ftclass == H5T_ENUM) {
        if (!boolfiletype.enum_equal(dtype)) {
            return DataType::Nothing;
        }
        return DataType::Bool;
    } else if (ftclass == H5T_OPAQUE) {
        return DataType::Opaque;
    } else {
        size = dtype.size();
        sign = dtype.sign();
    }

    return data_type_from_h5(ftclass, size, sign);
}

} // namespace hdf5
} // namespace nix
