// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <iostream>
#include <cmath>

#include <nix/hdf5/DataSet.hpp>

namespace nix {
namespace hdf5 {


DataSet::DataSet(H5::DataSet dset)
    : h5dset(dset)
{
}


DataSet DataSet::create(const H5::CommonFG &parent,
                        const std::string &name,
                        DataType dtype,
                        const NDSize &size,
                        const NDSize *maxsize,
                        const NDSize *chunks)
{
    H5::DataType fileType = data_type_to_h5_filetype(dtype);
    H5::DataSpace space;

    if (size.size() > 0) {
        int rank = static_cast<int>(size.size());
        const hsize_t *maxdims = maxsize != nullptr ? maxsize->data() : nullptr;
        space = H5::DataSpace(rank, size.data(), maxdims);
    }

    H5::DSetCreatPropList plcreate;

    if (chunks != nullptr) {
        int rank = static_cast<int>(chunks->size());
        plcreate.setChunk(rank, chunks->data());
    }

    H5::DataSet dset = parent.createDataSet(name, fileType, space, plcreate);
    return DataSet(dset);
}


DataSet DataSet::create(const H5::CommonFG &parent, const H5::DataType &fileType,
                        const std::string &name, const NDSize &size, const NDSize *maxsize, const NDSize *chunks) {
    H5::DataSpace space;

    if (size.size() > 0) {
        int rank = static_cast<int>(size.size());
        const hsize_t *maxdims = maxsize != nullptr ? &(*maxsize)[0] : nullptr;
        space = H5::DataSpace(rank, &size[0], maxdims);
    }

    H5::DSetCreatPropList plcreate = H5::DSetCreatPropList::DEFAULT;

    if (chunks != nullptr) {
        int rank = static_cast<int>(chunks->size());
        plcreate.setChunk(rank, &(*chunks)[0]);
    }

    H5::DataSet dset = parent.createDataSet(name, fileType, space);
    return DataSet(dset);
}


void DataSet::get(DataType dtype, const NDSize &size, void *data) const
{
    H5::DataType memType = data_type_to_h5_memtype(dtype);

    if (dtype == DataType::String) {
        StringWriter writer(size, static_cast<std::string *>(data));
        h5dset.read(*writer, memType);
        writer.finish();
        H5::DataSpace space = h5dset.getSpace();
        H5::DataSet::vlenReclaim(*writer, memType, space);
    } else {
        h5dset.read(data, memType);

    }
}


void DataSet::set(DataType dtype, const NDSize &size, const void *data)
{
    //FIXME
    H5::DataType memType = data_type_to_h5_memtype(dtype);
    if (dtype == DataType::String) {
        StringReader reader(size, static_cast<const std::string *>(data));
        h5dset.write(*reader, memType);
    } else {
        h5dset.write(data, memType);
    }

}


double psize_product(const NDSize &dims)
{
    double product = 1;
    std::for_each(dims.begin(), dims.end(), [&](hsize_t val) {
        product *= val;
    });

    return product;
}


#define CHUNK_BASE   16*1024
#define CHUNK_MIN     8*1024
#define CHUNK_MAX  1024*1024

/**
 * Infer the chunk size from the supplied size information
 *
 * @param dims    Size information to base the guessing on
 * @param dtype   The type of the data to guess the chunks for
 *
 * Internally uses guessChunking(NDSize, size_t) for calculations.
 *
 * @return An (maybe not at all optimal) guess for chunk size
 */
NDSize DataSet::guessChunking(NDSize dims, DataType dtype)
{

    const size_t type_size = data_type_to_size(dtype);
    NDSize chunks = guessChunking(dims, type_size);
    return chunks;
}

/**
 * Infer the chunk size from the supplied size information
 *
 * @param dims          Size information to base the guessing on
 * @param elementSize   The size of a single element in bytes
 *
 * This function is a port of the guess_chunk() function from h5py
 * low-level Python interface to the HDF5 library.\n
 * http://h5py.alfven.org\n
 *
 * @copyright Copyright 2008 - 2013 Andrew Collette & contributers\n
 * License: BSD 3-clause (see LICENSE.h5py)\n
 *
 * @return An (maybe not at all optimal) guess for chunk size
 */
NDSize DataSet::guessChunking(NDSize dims, size_t elementSize)
{
    // original source:
    //    https://github.com/h5py/h5py/blob/2.1.3/h5py/_hl/filters.py

    if(dims.size() == 0) {
        throw 1;
    }

    NDSize chunks(dims);
    double product = 1;
    std::for_each(dims.begin(), dims.end(), [&](hsize_t &val) {
        //todo: check for +infinity
        if (val == 0)
            val = 1024;

        product *= val;
    });

    product *= elementSize;

    double target_size = CHUNK_BASE * pow(2, log10(product/(1024.0L * 1024.0L)));
    if (target_size > CHUNK_MAX)
        target_size = CHUNK_MAX;
    else if (target_size < CHUNK_MIN)
        target_size = CHUNK_MIN;

    size_t i = 0;
    while(true) {

        double csize = chunks.nelms();
        if (csize == 1.0) {
            break;
        }

        double cbytes = csize * elementSize;
        if ((cbytes < target_size || (abs(cbytes - target_size) / target_size) < 0.5)
                && cbytes < CHUNK_MAX) {
            break;
        }

        //not done yet, one more iteration
        size_t idx = i % chunks.size();
        chunks[idx] = chunks[idx] >> 1; //divide by two
        i++;
    }

    return chunks;
}


void DataSet::extend(const NDSize &dims)
{
    //FIXME check for same rank
    h5dset.extend(dims.data());
}


Selection DataSet::createSelection() const
{
    H5::DataSpace space = h5dset.getSpace();
    return Selection(space);
}


NDSize DataSet::size() const
{
    H5::DataSpace space = h5dset.getSpace();
    size_t rank = static_cast<size_t>(space.getSimpleExtentNdims());
    NDSize dims(rank);
    space.getSimpleExtentDims (dims.data(), nullptr);
    return dims;
}

void DataSet::vlenReclaim(DataType memType, void *data, H5::DataSpace *dspace)
{
    H5::DataType h5MemType = data_type_to_h5_memtype(memType);

    if (dspace != nullptr) {
        H5::DataSet::vlenReclaim(data, h5MemType, *dspace);
    } else {
        H5::DataSpace space = h5dset.getSpace();
        H5::DataSet::vlenReclaim(data, h5MemType, space);
    }
}

} // namespace hdf5
} // namespace nix
