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

namespace nix {
namespace hdf5 {
namespace h5x {

DataType DataType::copy(hid_t source) {
    DataType hi_copy = H5Tcopy(source);
    hi_copy.check("Could not copy type");
    return hi_copy;
}

DataType DataType::makeStrType(size_t size) {
    DataType str_type = H5Tcopy(H5T_C_S1);
    str_type.check("Could not create string type");
    str_type.size(size);
    return str_type;
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


bool DataType::isVariableString() const {
    HTri res = H5Tis_variable_str(hid);
    res.check("DataType::isVariableString(): H5Tis_variable_str failed");
    return res.result();
}
} // h5x

bool DataType::isCompound() const {
    return class_t() == H5T_COMPOUND;
}



h5x::DataType data_type_to_h5_filetype(DataType dtype) {

   /* The switch is structred in a way in order to get
      warnings from the compiler when not all cases are
      handled and throw an exception if one of the not
      handled cases actually appears (i.e., we have no
      default case, because that silences the compiler.)
   */

    switch (dtype) {

        case DataType::Bool:   return h5x::DataType::copy(H5T_STD_B8LE);
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
        //special case the bool
        //we treat them as bit fields for now, since hdf5 has no bool support
        //as of 1.8.12
        case DataType::Bool:   return h5x::DataType::copy(H5T_NATIVE_B8);
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
    }

    std::cerr << "FIXME: Not implemented " << vclass << " " << vsize << " " << vsign << " " << std::endl;
    assert(NOT_IMPLEMENTED);
    return DataType::Nothing;
}

} // namespace hdf4
} // namespace nix
