// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_WRAP_ID_H
#define NIX_WRAP_ID_H

#include <string>

#include <boost/optional.hpp>

#include <nix/Platform.hpp>
#include <nix/Hydra.hpp>
#include <nix/hdf5/hdf5include.hpp>
#include <nix/hdf5/DataSpace.hpp>
#include <nix/hdf5/DataTypeHDF5.hpp>


namespace nix {
namespace hdf5 {


class StringWriter {
public:
    typedef std::string  value_type;
    typedef value_type  *pointer;
    typedef char        *data_type;
    typedef data_type   *data_ptr;


    StringWriter(const NDSize &size, pointer stringdata)
            : nelms(size.nelms()), data(stringdata) {
        buffer = new data_type[nelms];
    }

    data_ptr operator*() {
        return buffer;
    }

    void finish() {
        for (size_t i = 0; i < nelms; i++) {
            data[i] = buffer[i];
        }
    }

    ~StringWriter() {
        delete[] buffer;
    }

private:
    size_t   nelms;
    pointer  data;
    data_ptr buffer;
};

class StringReader {
public:
    typedef const std::string   value_type;
    typedef value_type         *pointer;
    typedef const char         *data_type;
    typedef data_type          *data_ptr;


    StringReader(const NDSize &size, pointer stringdata)
            : nelms(size.nelms()), data(stringdata) {
        buffer = new data_type[nelms];
        for (size_t i = 0; i < nelms; i++) {
            buffer[i] = data[i].c_str();
        }
    }

    data_ptr operator*() {
        return buffer;
    }

    ~StringReader() {
        delete[] buffer;
    }

private:
    size_t   nelms;
    pointer  data;
    data_ptr buffer;
};


class NIXAPI BaseHDF5 {

protected:

    hid_t hid;

public:

    BaseHDF5();

    BaseHDF5(hid_t hid);

    BaseHDF5(const BaseHDF5 &other);

    BaseHDF5(BaseHDF5 &&other);

    BaseHDF5& operator=(const BaseHDF5 &other);

    BaseHDF5& operator=(BaseHDF5 &&other);

    bool hasAttr(const std::string &name) const;
    void removeAttr(const std::string &name) const;

    template <typename T>
    void setAttr(const std::string &name, const T &value) const;

    template <typename T>
    bool getAttr(const std::string &name, T &value) const;

    bool operator==(const BaseHDF5 &other) const;

    bool operator!=(const BaseHDF5 &other) const;

    //NB: use the following functions with caution
    hid_t h5id() const; //no refcount increase

    int refCount() const;

    virtual void close();

    virtual ~BaseHDF5();

protected:

    void inc() const;

    void dec() const;

    void invalidate();

private:

    H5::Attribute openAttr(const std::string &name) const;
    H5::Attribute createAttr(const std::string &name, H5::DataType fileType, H5::DataSpace fileSpace) const;

    static void readAttr(const H5::Attribute &attr, H5::DataType mem_type, const NDSize &size, void *data);
    static void readAttr(const H5::Attribute &attr, H5::DataType mem_type, const NDSize &size, std::string *data);

    static void writeAttr(const H5::Attribute &attr, H5::DataType mem_type, const NDSize &size, const void *data);
    static void writeAttr(const H5::Attribute &attr, H5::DataType mem_type, const NDSize &size, const std::string *data);
};


template<typename T> void BaseHDF5::setAttr(const std::string &name, const T &value) const
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



template<typename T> bool BaseHDF5::getAttr(const std::string &name, T &value) const
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


} // namespace hdf5
} // namespace nix

#endif /* NIX_WRAP_ID_H */
