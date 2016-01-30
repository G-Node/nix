// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_DATA_TYPE_HDF5_H
#define NIX_DATA_TYPE_HDF5_H

#include <nix/DataType.hpp>
#include "BaseHDF5.hpp"

#include <nix/Platform.hpp>

namespace nix {
namespace hdf5 {

namespace h5x {

class NIXAPI DataType : public hdf5::BaseHDF5 {

public:
    DataType() : BaseHDF5() { }
    DataType(hid_t hid) : BaseHDF5(hid, false) { }
    DataType(hid_t hid, bool is_copy) : BaseHDF5(hid, is_copy) { }
    DataType(const DataType &other) : BaseHDF5(other) { }


    //Basically the same as DataType(hid_t, true)
    // but more explicit, so it is easier in the code
    // to read
    static DataType copy(hid_t);
    static DataType makeStrType(size_t size = H5T_VARIABLE);


    void size(size_t);
    size_t size() const;

    bool isVariableString() const;
};

}


NIXAPI h5x::DataType data_type_to_h5_filetype(DataType dtype);


NIXAPI h5x::DataType data_type_to_h5_memtype(DataType dtype);

NIXAPI DataType data_type_from_h5(H5T_class_t vclass, size_t vsize, H5T_sign_t vsign);

} // namespace hdf5
} // namespace nix

#endif //NIX_DATA_TYPE_HDF5_H
