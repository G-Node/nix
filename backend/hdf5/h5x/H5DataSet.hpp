// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_DATASET_H
#define NIX_DATASET_H

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

    void read(void *data, h5x::DataType memType, const NDSize &count, const NDSize &offset=NDSize{}) const;
    void write(const void *data, h5x::DataType memType, const NDSize &count, const NDSize &offset=NDSize{});

    template<typename T> void read(T &value, bool resize = false) const;
    template<typename T> void write(const T &value);

    static NDSize guessChunking(NDSize dims, const h5x::DataType &dtype);

    static NDSize guessChunking(NDSize dims, size_t element_size);

    void setExtent(const NDSize &dims);
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
    read(hydra.data(), memType, hydra.shape());
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
    h5x::DataType memType = data_type_to_h5_memtype(dtype);
    NDSize size = hydra.shape();
    write(hydra.data(), memType, size);
}

} // namespace hdf5
} // namespace nix

#endif // NIX_DATASET_H
