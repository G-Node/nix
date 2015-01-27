// Copyright © 2015 German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <kellner@bio.lmu.de>

#ifndef NIX_LOCATION_H5_H
#define NIX_LOCATION_H5_H

#include <nix/hdf5/BaseHDF5.hpp>

namespace nix {
namespace hdf5 {

class NIXAPI LocID : public BaseHDF5 {
public:
    LocID();

    LocID(hid_t hid);

    LocID(const LocID &other);

    LocID(const H5::Group &h5group);


    bool hasAttr(const std::string &name) const;
    void removeAttr(const std::string &name) const;

    template <typename T>
    void setAttr(const std::string &name, const T &value) const;

    template <typename T>
    bool getAttr(const std::string &name, T &value) const;

private:

    H5::Attribute openAttr(const std::string &name) const;
    H5::Attribute createAttr(const std::string &name, H5::DataType fileType, H5::DataSpace fileSpace) const;

    static void readAttr(const H5::Attribute &attr, H5::DataType mem_type, const NDSize &size, void *data);
    static void readAttr(const H5::Attribute &attr, H5::DataType mem_type, const NDSize &size, std::string *data);

    static void writeAttr(const H5::Attribute &attr, H5::DataType mem_type, const NDSize &size, const void *data);
    static void writeAttr(const H5::Attribute &attr, H5::DataType mem_type, const NDSize &size, const std::string *data);
};


template<typename T> void LocID::setAttr(const std::string &name, const T &value) const
{
    typedef Hydra<const T> hydra_t;

    const hydra_t hydra(value);
    DataType dtype = hydra.element_data_type();
    NDSize shape = hydra.shape();

    H5::Attribute attr;

    if (hasAttr(name)) {
        attr = openAttr(name);
    } else {
        H5::DataType fileType = data_type_to_h5_filetype(dtype);
        H5::DataSpace fileSpace = DataSpace::create(shape, false);
        attr = createAttr(name, fileType, fileSpace);
    }

    writeAttr(attr, data_type_to_h5_memtype(dtype), shape, hydra.data());
}



template<typename T> bool LocID::getAttr(const std::string &name, T &value) const
{
    if (!hasAttr(name)) {
        return false;
    }

    Hydra<T> hydra(value);

    //determine attr's size and resize value accordingly
    H5::Attribute attr = openAttr(name);
    H5::DataSpace space = attr.getSpace();
    int rank = space.getSimpleExtentNdims();
    NDSize dims(static_cast<size_t>(rank));
    space.getSimpleExtentDims (dims.data(), nullptr);
    hydra.resize(dims);

    DataType dtype = hydra.element_data_type();
    H5::DataType mem_type = data_type_to_h5_memtype(dtype);

    readAttr(attr, mem_type, dims, hydra.data());

    return true;
}



} //nix::hdf5

} //nix

#endif