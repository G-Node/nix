// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_DATASET_H
#define NIX_DATASET_H

#include <nix/hdf5/hdf5include.hpp>
#include <nix/hdf5/Selection.hpp>
#include <nix/hdf5/DataSpace.hpp>
#include <nix/hdf5/DataTypeHDF5.hpp>
#include <nix/Hydra.hpp>
#include <nix/Value.hpp>

#include <nix/Platform.hpp>

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


class NIXAPI DataSet {

private:

    H5::DataSet h5dset;

public:
    DataSet() { }
    explicit DataSet(H5::DataSet dset);

    DataSet& operator=(const DataSet &other) {h5dset = other.h5dset; return *this;}

    void read(DataType dtype, const NDSize &size, void *data) const;
    void write(DataType dtype, const NDSize &size, const void *data);

    void read(DataType dtype, void *data, const Selection &fileSel, const Selection &memSel) const;
    void write(DataType dtype, const void *data, const Selection &fileSel, const Selection &memSel);

    void read(std::vector<Value> &values) const;
    void write(const std::vector<Value> &values);

    template<typename T> void read(T &value, bool resize = false) const;
    template<typename T> void read(T &value, const Selection &fileSel, bool resize = false) const;
    template<typename T> void read(T &value, const Selection &fileSel, const Selection &memSel) const;

    template<typename T> void write(const T &value);
    template<typename T> void write(const T &value, const Selection &fileSel);
    template<typename T> void write(const T &value, const Selection &fileSel, const Selection &memSel);


    static DataSet create(const H5::CommonFG &parent, const std::string &name, DataType dtype, const NDSize &size);

    template<typename T>
    static DataSet create(const H5::CommonFG &parent, const std::string &name, const T &value);

    static DataSet create(const H5::CommonFG &parent, const std::string &name, const H5::DataType &fileType,
                          const NDSize &size, const NDSize &maxsize = {}, const NDSize &chunks = {},
                          bool maxSizeUnlimited = true, bool guessChunks = true);

    static NDSize guessChunking(NDSize dims, DataType dtype);

    static NDSize guessChunking(NDSize dims, size_t element_size);

    void setExtent(const NDSize &dims);
    Selection createSelection() const;
    NDSize size() const;

    void vlenReclaim(DataType mem_type, void *data, H5::DataSpace *dspace = nullptr) const;

    static H5::DataType fileTypeForValue(DataType dtype);
    static H5::DataType memTypeForValue(DataType dtype);

    DataType dataType(void) const;

    bool hasAttr(const std::string &name) const;
    void removeAttr(const std::string &name) const;

    template <typename T>
    void setAttr(const std::string &name, const T &value) const;

    template <typename T>
    bool getAttr(const std::string &name, T &value) const;


private:
    static void readAttr(const H5::Attribute &attr, H5::DataType mem_type, const NDSize &size, void *data);
    static void readAttr(const H5::Attribute &attr, H5::DataType mem_type, const NDSize &size, std::string *data);

    static void writeAttr(const H5::Attribute &attr, H5::DataType mem_type, const NDSize &size, const void *data);
    static void writeAttr(const H5::Attribute &attr, H5::DataType mem_type, const NDSize &size, const std::string *data);

};

//template functions

template<typename T>
void DataSet::setAttr(const std::string &name, const T &value) const
{
    typedef Hydra<const T> hydra_t;

    const hydra_t hydra(value);
    DataType dtype = hydra.element_data_type();
    NDSize shape = hydra.shape();

    H5::Attribute attr;

    if (hasAttr(name)) {
        attr = h5dset.openAttribute(name);
    } else {
        H5::DataType fileType = data_type_to_h5_filetype(dtype);
        H5::DataSpace fileSpace = DataSpace::create(shape, false);
        attr = h5dset.createAttribute(name, fileType, fileSpace);
    }

    writeAttr(attr, data_type_to_h5_memtype(dtype), shape, hydra.data());
}


template<typename T> bool DataSet::getAttr(const std::string &name, T &value) const
{
    if (!hasAttr(name)) {
        return false;
    }

    Hydra<T> hydra(value);

    //determine attr's size and resize value accordingly
    H5::Attribute attr = h5dset.openAttribute(name);
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
DataSet DataSet::create(const H5::CommonFG &parent, const std::string &name, const T &data)
{
    const Hydra<const T> hydra(data);

    DataType dtype = hydra.element_data_type();
    H5::DataType fileType = data_type_to_h5_filetype(dtype);

    NDSize shape = hydra.shape();
    NDSize maxsize(shape.size(), H5S_UNLIMITED);

    NDSize chunks = guessChunking(shape, dtype);
    H5::DSetCreatPropList plcreate;
    int rank = static_cast<int>(chunks.size());
    plcreate.setChunk(rank, chunks.data());

    H5::DataSpace space = DataSpace::create(shape, maxsize);
    H5::DataSet dset = parent.createDataSet(name, fileType, space, plcreate);
    return DataSet(dset);
}



/**
 * Read *all* the data from a DataSet into memory
 *
 * @param value    Reference to a variable to store the data in
 * @param resize   Resize variable to fit the size of the DataSet
 */
template<typename T> void DataSet::read(T &value, bool resize) const
{
    Hydra<T> hydra(value);

    if (resize) {
        NDSize dims = this->size();
        hydra.resize(dims);
    }

    DataType dtype = hydra.element_data_type();
    NDSize size = hydra.shape();
    read(dtype, size, hydra.data());
}

/**
 * Read *selected* data from a DataSet into memory
 *
 * NB: Since this function assumes that whole variable is being read into,
 * the number of selected elements in fileSel and the size of the variable value
 * must be the same
 *
 * @param value    Reference to a variable to store the data in
 * @param fileSel  Selection to indicate which subspace of the DataSet to read
 * @param resize   Resize variable to fit the size of the Selection
 */
template<typename T> void DataSet::read(T &value, const Selection &fileSel, bool resize) const
{
    if (resize) {
        Hydra<T> hydra(value);
        NDSize fsize = fileSel.size();
        hydra.resize(fsize);
    }

    read(value, fileSel, Selection(value));
}

/**
 * Read *selected* data from a DataSet into *selected part* of memory
 *
 * @param value    Reference to a variable to store the data in
 * @param fileSel  Selection to indicate which subspace of the DataSet to read
 * @param memSel   Selection to indicate which subspace of the memory to read into
 */
template<typename T> void DataSet::read(T &value, const Selection &fileSel, const Selection &memSel) const
{
    Hydra<T> hydra(value);

    DataType dtype = hydra.element_data_type();
    this->read(dtype, hydra.data(), fileSel, memSel);
}

/* ************************************************************************* */

/**
 * Write all memory stored in the variable value into all of the DataSet
 *
 * NB: Size of the DataSet and the variable must be the same
 * @param value    Reference to a variable to read the data from
 */
template<typename T> void DataSet::write(const T &value)
{
    const Hydra<const T> hydra(value);

    DataType dtype = hydra.element_data_type();
    NDSize size = hydra.shape();
    write(dtype, size, hydra.data());
}

/**
 * Write all memory stored in the variable value into a *selected* subspace of the DataSet
 *
 * NB: Size of the DataSet and the variable must be the same
 * @param value    Reference to a variable to read the data from
 * @param fileSel    Selection to indicate into which subspace of value to read data from
 */
template<typename T> void DataSet::write(const T &value, const Selection &fileSel)
{
    write(value, fileSel, Selection(value));
}

/**
 * Write a *selected part* of memory stored in the variable value into a *selected* subspace of the DataSet
 *
 * NB: Size of the DataSet and the variable must be the same
 * @param value    Reference to a variable to read the data from
 * @param fileSel  Selection to indicate into which subspace of value to read data from
 * @param memSel   Selection to indicate into which subspace of the DataSpace to write to
 */
template<typename T> void DataSet::write(const T &value, const Selection &fileSel, const Selection &memSel)
{
    const Hydra<const T> hydra(value);
    DataType dtype = hydra.element_data_type();

    this->write(dtype, hydra.data(), fileSel, memSel);
}


} // namespace hdf5
} // namespace nix

#endif // NIX_DATASET_H
