// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <stdexcept>

#include <nix/hdf5/DataTypeHDF5.hpp>

#include <iostream>
#include <cassert>

namespace nix {
namespace hdf5 {


H5::DataType data_type_to_h5_filetype(DataType dtype) {

    switch (dtype) {

        case DataType::Bool:   return H5::PredType::STD_B8LE;
        case DataType::Int8:   return H5::PredType::STD_I8LE;
        case DataType::Int16:  return H5::PredType::STD_I16LE;
        case DataType::Int32:  return H5::PredType::STD_I32LE;
        case DataType::Int64:  return H5::PredType::STD_I64LE;
        case DataType::UInt8:  return H5::PredType::STD_U8LE;
        case DataType::UInt16: return H5::PredType::STD_U16LE;
        case DataType::UInt32: return H5::PredType::STD_U32LE;
        case DataType::UInt64: return H5::PredType::STD_U64LE;
        case DataType::Float:  return H5::PredType::IEEE_F32LE;
        case DataType::Double: return H5::PredType::IEEE_F64LE;
        case DataType::String: return H5::StrType(H5::PredType::C_S1, H5T_VARIABLE);

        default:
            throw std::invalid_argument("Unkown DataType"); //FIXME
    }

    return H5::DataType();
}


template<size_t>
H5::DataType bool_to_predtype(void) { throw std::invalid_argument("sizeof(bool) > 8 not supported."); };

template<> H5::DataType bool_to_predtype<1>() { return H5::PredType::NATIVE_B8;  }
template<> H5::DataType bool_to_predtype<2>() { return H5::PredType::NATIVE_B16; }
template<> H5::DataType bool_to_predtype<4>() { return H5::PredType::NATIVE_B32; }
template<> H5::DataType bool_to_predtype<8>() { return H5::PredType::NATIVE_B64; }


H5::DataType data_type_to_h5_memtype(DataType dtype) {


    switch(dtype) {
        case DataType::Bool:   return bool_to_predtype<sizeof(bool)>();
        case DataType::Int8:   return H5::PredType::NATIVE_INT8;
        case DataType::Int16:  return H5::PredType::NATIVE_INT16;
        case DataType::Int32:  return H5::PredType::NATIVE_INT32;
        case DataType::Int64:  return H5::PredType::NATIVE_INT64;
        case DataType::UInt8:  return H5::PredType::NATIVE_UINT8;
        case DataType::UInt16: return H5::PredType::NATIVE_UINT16;
        case DataType::UInt32: return H5::PredType::NATIVE_UINT32;
        case DataType::UInt64: return H5::PredType::NATIVE_UINT64;
        case DataType::Float:  return H5::PredType::NATIVE_FLOAT;
        case DataType::Double: return H5::PredType::NATIVE_DOUBLE;
        case DataType::String: return H5::StrType(H5::PredType::C_S1, H5T_VARIABLE);

        default:
            throw std::invalid_argument("DataType not handled!"); //FIXME
    }

    return H5::DataType();
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
