// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_DATASET_H
#define NIX_DATASET_H

#include "Selection.hpp"
#include "DataSpace.hpp"
#include "H5DataType.hpp"
#include "LocID.hpp"
#include <nix/Hydra.hpp>
#include <nix/Value.hpp>

#include <nix/Platform.hpp>

namespace nix {
namespace hdf5 {

class NIXAPI DataSet : public LocID {

public:

    DataSet() : LocID() { };

    DataSet(hid_t hid);

    DataSet(hid_t hid, bool is_copy) : LocID(hid, is_copy) { }

    DataSet(const DataSet &other);

    void read(void *data, const h5x::DataType &memType, const DataSpace &memSpace, const DataSpace &fileSpace) const;
    void write(const void *data, const h5x::DataType &memType, const DataSpace &memSpace, const DataSpace &fileSpace);

    void read(h5x::DataType memType, const NDSize &size, void *data) const;
    void write(DataType dtype, const NDSize &size, const void *data);

    void read(h5x::DataType memType, void *data, const Selection &fileSel, const Selection &memSel) const;
    void write(DataType dtype, const void *data, const Selection &fileSel, const Selection &memSel);

    template<typename T> void read(T &value, bool resize = false) const;
    template<typename T> void read(T &value, const Selection &fileSel, bool resize = false) const;
    template<typename T> void read(T &value, const Selection &fileSel, const Selection &memSel) const;

    template<typename T> void write(const T &value);
    template<typename T> void write(const T &value, const Selection &fileSel);
    template<typename T> void write(const T &value, const Selection &fileSel, const Selection &memSel);

    static NDSize guessChunking(NDSize dims, const h5x::DataType &dtype);

    static NDSize guessChunking(NDSize dims, size_t element_size);

    void setExtent(const NDSize &dims);
    Selection createSelection() const;
    NDSize size() const;

    void vlenReclaim(h5x::DataType mem_type, void *data, DataSpace *dspace = nullptr) const;

    h5x::DataType dataType(void) const;

    DataSpace getSpace() const;

};



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
    h5x::DataType memType = data_type_to_h5_memtype(dtype);
    read(memType, hydra.shape(), hydra.data());
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
    h5x::DataType memType = data_type_to_h5_memtype(dtype);
    this->read(memType, hydra.data(), fileSel, memSel);
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
