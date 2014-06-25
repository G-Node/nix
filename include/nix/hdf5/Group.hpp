// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_GROUP_H
#define NIX_GROUP_H

#include <string>

#include <nix/hdf5/hdf5include.hpp>
#include <nix/hdf5/DataSet.hpp>

#include <nix/Hydra.hpp>
#include <nix/hdf5/DataSpace.hpp>

#include <nix/Platform.hpp>

namespace nix {
namespace hdf5 {


/**
 * TODO documentation
 */
class NIXAPI Group {

private:

    H5::Group h5group;

public:

    Group();

    Group(H5::Group h5group);

    Group(const Group &group);

    Group& operator=(const Group &group);

    bool hasAttr(const std::string &name) const;
    void removeAttr(const std::string &name) const;

    template <typename T>
    void setAttr(const std::string &name, const T &value) const;

    template <typename T>
    bool getAttr(const std::string &name, T &value) const;

    bool hasObject(const std::string &path) const;
    size_t objectCount() const;
    std::string objectName(size_t index) const;

    bool hasData(const std::string &name) const;
    DataSet openData(const std::string &name) const;
    void removeData(const std::string &name);

    template<typename T>
    void setData(const std::string &name, const T &value);
    template<typename T>
    bool getData(const std::string &name, T &value) const;

    bool hasGroup(const std::string &name) const;
    Group openGroup(const std::string &name, bool create = true) const;
    void removeGroup(const std::string &name);
    void renameGroup(const std::string &old_name, const std::string &new_name);

    bool operator==(const Group &group) const;
    bool operator!=(const Group &group) const;

    H5::Group h5Group() const;
    virtual ~Group();

private:
    static void readAttr(const H5::Attribute &attr, H5::DataType mem_type, const NDSize &size, void *data);
    static void readAttr(const H5::Attribute &attr, H5::DataType mem_type, const NDSize &size, std::string *data);

    static void writeAttr(const H5::Attribute &attr, H5::DataType mem_type, const NDSize &size, const void *data);
    static void writeAttr(const H5::Attribute &attr, H5::DataType mem_type, const NDSize &size, const std::string *data);
}; // group Group

//template functions

template<typename T> void Group::setAttr(const std::string &name, const T &value) const
{
    typedef Hydra<const T> hydra_t;

    const hydra_t hydra(value);
    DataType dtype = hydra.element_data_type();
    NDSize shape = hydra.shape();

    H5::Attribute attr;

    if (hasAttr(name)) {
        attr = h5group.openAttribute(name);
    } else {
        H5::DataType fileType = data_type_to_h5_filetype(dtype);
        H5::DataSpace fileSpace = DataSpace::create(shape, false);
        attr = h5group.createAttribute(name, fileType, fileSpace);
    }

    writeAttr(attr, data_type_to_h5_memtype(dtype), shape, hydra.data());
}



template<typename T> bool Group::getAttr(const std::string &name, T &value) const
{
    if (!hasAttr(name)) {
        return false;
    }

    Hydra<T> hydra(value);

    //determine attr's size and resize value accordingly
    H5::Attribute attr = h5group.openAttribute(name);
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

template<typename T>
void Group::setData(const std::string &name, const T &value)
{
    const Hydra<const T> hydra(value);
    DataType dtype = hydra.element_data_type();
    NDSize shape = hydra.shape();

    DataSet ds;
    if (!hasData(name)) {
        ds = DataSet::create(h5group, name, dtype, shape);
    } else {
        ds = openData(name);
        ds.setExtent(shape);
    }

    ds.write(dtype, shape, hydra.data());
}

template<typename T>
bool Group::getData(const std::string &name, T &value) const
{
    if (!hasData(name)) {
        return false;
    }

    Hydra<T> hydra(value);
    DataSet ds = openData(name);

    DataType dtype = hydra.element_data_type();
    NDSize shape = ds.size();

    hydra.resize(shape);
    ds.read(dtype, shape, hydra.data());

    return true;
}

} // namespace hdf5
} // namespace nix


#endif /* NIX_GROUP_H */
