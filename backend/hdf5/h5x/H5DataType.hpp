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
#include "H5Object.hpp"

#include <nix/Platform.hpp>

#include <type_traits>
#include <vector>

namespace nix {
namespace hdf5 {

namespace h5x {

class NIXAPI DataType : public hdf5::H5Object {

public:
    DataType() : H5Object() { }
    DataType(hid_t hid) : H5Object(hid, false) { }
    DataType(hid_t hid, bool is_copy) : H5Object(hid, is_copy) { }
    DataType(const DataType &other) : H5Object(other) { }

    bool equal(const DataType &other) const;
    bool operator ==(const DataType &other) const {
        return equal(other);
    }

    //Basically the same as DataType(hid_t, true)
    // but more explicit, so it is easier in the code
    // to read
    static DataType copy(hid_t);

    static DataType make(H5T_class_t klass, size_t size);
    static DataType makeStrType(size_t size = H5T_VARIABLE);
    static DataType makeCompound(size_t size);
    static DataType makeEnum(const DataType &base);

    H5T_class_t class_t() const;

    void size(size_t);
    size_t size() const;

    void sign(H5T_sign_t sign);
    H5T_sign_t sign() const;

    bool isVariableString() const;

    // Compound type related
    bool isCompound() const;
    unsigned int member_count() const;
    DataType member_type(unsigned int index) const;
    DataType member_type(const std::string &name) const;

    H5T_class_t member_class(unsigned int index) const;
    std::string member_name(unsigned int index) const;
    std::vector<std::string> member_names() const;
    size_t member_offset(unsigned int index) const;
    unsigned int member_index(const std::string &name) const;

    void insert(const std::string &name, size_t offset, const DataType &dtype);
    void insert(const std::string &name, void *value);

    template<typename T, typename = std::enable_if_t<!std::is_pointer<T>::value>>
    void insert(const std::string &name, T value) {
        this->insert(name, &value);
    }

    void enum_valueof(const std::string &name, void *value);
    bool enum_equal(const DataType &other) const;
};

}


NIXAPI h5x::DataType data_type_to_h5_filetype(DataType dtype);
NIXAPI h5x::DataType data_type_to_h5_memtype(DataType dtype);
NIXAPI h5x::DataType data_type_to_h5(DataType dtype, bool for_memory);


NIXAPI DataType data_type_from_h5(H5T_class_t vclass, size_t vsize, H5T_sign_t vsign);
NIXAPI DataType data_type_from_h5(const h5x::DataType &dtype);

} // namespace hdf5
} // namespace nix

#endif //NIX_DATA_TYPE_HDF5_H
