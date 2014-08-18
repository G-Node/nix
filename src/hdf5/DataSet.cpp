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


//special to_data_type for our purporses (Value)
template<>
struct to_data_type<char *> {
    static const bool is_valid = true;
    static const DataType value = DataType::String;
};

template<>
struct to_data_type<const char *> {
    static const bool is_valid = true;
    static const DataType value = DataType::String;
};


namespace hdf5 {


DataSet::DataSet(H5::DataSet dset)
    : h5dset(dset)
{
}


DataSet DataSet::create(const H5::CommonFG &parent,
                        const std::string &name,
                        DataType dtype,
                        const NDSize &size)
{
    H5::DataType fileType = data_type_to_h5_filetype(dtype);
    return create(parent, name, fileType, size);
}


DataSet DataSet::create(const H5::CommonFG &parent,
                        const std::string &name,
                        const H5::DataType &fileType,
                        const NDSize &size,
                        const NDSize &maxsize,
                        const NDSize &chunks,
                        bool max_size_unlimited,
                        bool guess_chunks)
{
    H5::DataSpace space;

    if (size) {
        if (maxsize) {
            space = DataSpace::create(size, maxsize);
        } else {
            space = DataSpace::create(size, max_size_unlimited);
        }
    }

    H5::DSetCreatPropList plcreate = H5::DSetCreatPropList::DEFAULT;

    if (chunks) {
        int rank = static_cast<int>(chunks.size());
        plcreate.setChunk(rank, chunks.data());
    } else if (guess_chunks) {
        NDSize guessedChunks = DataSet::guessChunking(size, fileType.getSize());
        plcreate.setChunk(static_cast<int>(guessedChunks.size()), guessedChunks.data());
    }

    H5::DataSet dset = parent.createDataSet(name, fileType, space);
    return DataSet(dset);
}


void DataSet::read(DataType dtype, const NDSize &size, void *data) const
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


void DataSet::write(DataType dtype, const NDSize &size, const void *data)
{
    H5::DataType memType = data_type_to_h5_memtype(dtype);
    if (dtype == DataType::String) {
        StringReader reader(size, static_cast<const std::string *>(data));
        h5dset.write(*reader, memType);
    } else {
        h5dset.write(data, memType);
    }
}


void DataSet::read(DataType        dtype,
                   void            *data,
                   const Selection &fileSel,
                   const Selection &memSel) const
{
    H5::DataType memType = data_type_to_h5_memtype(dtype);

    if (dtype == DataType::String) {
        NDSize size = memSel.size();
        StringWriter writer(size, static_cast<std::string *>(data));
        h5dset.read(*writer, memType, memSel.h5space(), fileSel.h5space());
        writer.finish();
        H5::DataSpace space = h5dset.getSpace();
        H5::DataSet::vlenReclaim(*writer, memType, memSel.h5space());
    } else {
        h5dset.read(data, memType, memSel.h5space(), fileSel.h5space());
    }
}

void DataSet::write(DataType         dtype,
                    const void      *data,
                    const Selection &fileSel,
                    const Selection &memSel)
{
    H5::DataType memType = data_type_to_h5_memtype(dtype);
    if (dtype == DataType::String) {
        NDSize size = memSel.size();
        StringReader reader(size, static_cast<const std::string *>(data));
        h5dset.write(*reader, memType, memSel.h5space(), fileSel.h5space());
    } else {
        h5dset.write(data, memType, memSel.h5space(), fileSel.h5space());
    }
}


bool DataSet::hasAttr(const std::string &name) const {
    return H5Aexists(h5dset.getId(), name.c_str());
}


void DataSet::removeAttr(const std::string &name) const {
    h5dset.removeAttr(name);
}


void DataSet::readAttr(const H5::Attribute &attr, H5::DataType mem_type, const NDSize &size, void *data) {
    attr.read(mem_type, data);
}


void DataSet::readAttr(const H5::Attribute &attr, H5::DataType mem_type, const NDSize &size, std::string *data) {
    StringWriter writer(size, data);
    attr.read(mem_type, *writer);
    writer.finish();
    H5::DataSet::vlenReclaim(*writer, mem_type, attr.getSpace()); //recycle space?
}


void DataSet::writeAttr(const H5::Attribute &attr, H5::DataType mem_type, const NDSize &size, const void *data) {
    attr.write(mem_type, data);
}


void DataSet::writeAttr(const H5::Attribute &attr, H5::DataType mem_type, const NDSize &size, const std::string *data) {
    StringReader reader(size, data);
    attr.write(mem_type, *reader);
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
 * @param chunks        Size information to base the guessing on
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
NDSize DataSet::guessChunking(NDSize chunks, size_t element_size)
{
    // original source:
    //    https://github.com/h5py/h5py/blob/2.1.3/h5py/_hl/filters.py

    if (chunks.size() == 0) {
        throw InvalidRank("Cannot guess chunks for 0-dimensional data");
    }

    double product = 1;
    std::for_each(chunks.begin(), chunks.end(), [&](hsize_t &val) {
        //todo: check for +infinity
        if (val == 0)
            val = 1024;

        product *= val;
    });

    product *= element_size;
    double target_size = CHUNK_BASE * pow(2, log10(product/(1024.0 * 1024.0)));
    if (target_size > CHUNK_MAX)
        target_size = CHUNK_MAX;
    else if (target_size < CHUNK_MIN)
        target_size = CHUNK_MIN;

    size_t i = 0;
    while (true) {
        double csize = static_cast<double>(chunks.nelms());
        if (csize == 1.0) {
            break;
        }

        double cbytes = csize * element_size;
        if ((cbytes < target_size || (std::abs(cbytes - target_size) / target_size) < 0.5)
                && cbytes < CHUNK_MAX) {
            break;
        }

        //not done yet, one more iteration
        size_t idx = i % chunks.size();
        if (chunks[idx] > 1) {
            chunks[idx] = chunks[idx] >> 1; //divide by two
        }
        i++;
    }
    return chunks;
}

void DataSet::setExtent(const NDSize &dims)
{
    H5::DataSpace space = h5dset.getSpace();
    size_t rank = static_cast<size_t>(space.getSimpleExtentNdims());

    if (rank != dims.size()) {
        throw InvalidRank("Cannot change the dimensionality via setExtent()");
    }

    herr_t err = H5Dset_extent(h5dset.getId(), dims.data());
    if (err < 0) {
        throw H5::DataSetIException("H5Dset_extent", "Could not set the extent of the DataSet.");
    }
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

void DataSet::vlenReclaim(DataType mem_type, void *data, H5::DataSpace *dspace) const
{
    H5::DataType h5MemType = data_type_to_h5_memtype(mem_type);

    if (dspace != nullptr) {
        H5::DataSet::vlenReclaim(data, h5MemType, *dspace);
    } else {
        H5::DataSpace space = h5dset.getSpace();
        H5::DataSet::vlenReclaim(data, h5MemType, space);
    }
}


DataType DataSet::dataType(void) const
{
    hid_t ftype = H5Dget_type(h5dset.getId());
    H5T_class_t ftclass = H5Tget_class(ftype);

    size_t     size;
    H5T_sign_t sign;

    if (ftclass == H5T_COMPOUND) {
        //if it is a compound data type then it must be a
        //a property dataset, we can handle that
        int nmems = H5Tget_nmembers(ftype);
        assert(nmems == 6);
        hid_t vtype = H5Tget_member_type(ftype, 0);

        ftclass = H5Tget_class(vtype);
        size = H5Tget_size(vtype);
        sign = H5Tget_sign(vtype);

        H5Tclose(vtype);
    } else {
        size = H5Tget_size(ftype);
        sign = H5Tget_sign(ftype);
    }

    DataType dtype = nix::hdf5::data_type_from_h5(ftclass, size, sign);
    H5Tclose(ftype);

    return dtype;
}

/* Value related functions */

template<typename T>
struct FileValue  {

    T       value;

    double  uncertainty;
    char   *reference;
    char   *filename;
    char   *encoder;
    char   *checksum;

    //ctors
    FileValue() {}
    explicit FileValue(const T &vref) : value(vref) { }
};

template<>
struct FileValue<bool>  {

    unsigned char value;

    double  uncertainty;
    char   *reference;
    char   *filename;
    char   *encoder;
    char   *checksum;

    //ctors
    FileValue() {}
    explicit FileValue(const bool &vref) : value(vref) { }
};

//

template<typename T>
H5::DataType h5_type_for_value(bool for_memory)
{
    typedef FileValue<T> file_value_t;

    H5::CompType h5type(sizeof(file_value_t));

    DataType dtype = to_data_type<T>::value;
    if (for_memory) {
        h5type.insertMember("value", HOFFSET(file_value_t, value), hdf5::data_type_to_h5_memtype(dtype));
    } else {
        h5type.insertMember("value", HOFFSET(file_value_t, value), hdf5::data_type_to_h5_filetype(dtype));
    }

    if (for_memory) {
        h5type.insertMember("uncertainty", HOFFSET(file_value_t, uncertainty), hdf5::data_type_to_h5_memtype(DataType::Double));
    } else {
       h5type.insertMember("uncertainty", HOFFSET(file_value_t, uncertainty), hdf5::data_type_to_h5_filetype(DataType::Double));
    }

    h5type.insertMember("reference", HOFFSET(file_value_t, reference), H5::StrType(H5::PredType::C_S1, H5T_VARIABLE));
    h5type.insertMember("filename", HOFFSET(file_value_t, filename), H5::StrType(H5::PredType::C_S1, H5T_VARIABLE));
    h5type.insertMember("encoder", HOFFSET(file_value_t, encoder), H5::StrType(H5::PredType::C_S1, H5T_VARIABLE));
    h5type.insertMember("checksum", HOFFSET(file_value_t, checksum), H5::StrType(H5::PredType::C_S1, H5T_VARIABLE));

    return h5type;
}

#if 0 //set to one to check that all supported DataTypes are handled
#define CHECK_SUPOORTED_VALUES
#endif
#define DATATYPE_SUPPORT_NOT_IMPLEMENTED false
//
static H5::DataType h5_type_for_value_dtype(DataType dtype, bool for_memory)
{
    switch(dtype) {
    case DataType::Bool:   return h5_type_for_value<bool>(for_memory);
    case DataType::Int32:  return h5_type_for_value<int32_t>(for_memory);
    case DataType::UInt32: return h5_type_for_value<uint32_t>(for_memory);
    case DataType::Int64:  return h5_type_for_value<int64_t>(for_memory);
    case DataType::UInt64: return h5_type_for_value<uint64_t>(for_memory);
    case DataType::Double: return h5_type_for_value<double>(for_memory);
    case DataType::String: return h5_type_for_value<char *>(for_memory);
#ifndef CHECK_SUPOORTED_VALUES
    default: assert(DATATYPE_SUPPORT_NOT_IMPLEMENTED); return H5::DataType{};
#endif
    }
}

H5::DataType DataSet::fileTypeForValue(DataType dtype)
{
    return h5_type_for_value_dtype(dtype, false);
}

H5::DataType DataSet::memTypeForValue(DataType dtype)
{
    return h5_type_for_value_dtype(dtype, true);
}

template<typename T>
void do_read_value(const H5::DataSet &h5ds, size_t size, std::vector<Value> &values)
{
    H5::DataType memType = h5_type_for_value<T>(true);

    typedef FileValue<T> file_value_t;
    std::vector<file_value_t> fileValues;

    fileValues.resize(size);
    values.resize(size);
    h5ds.read(fileValues.data(), memType);

    std::transform(fileValues.begin(), fileValues.end(), values.begin(), [](const file_value_t &val) {
            Value temp(static_cast<T>(val.value)); //we cast because of the bool specialization
            temp.uncertainty = val.uncertainty;
            temp.reference = val.reference;
            temp.filename = val.filename;
            temp.encoder = val.encoder;
            temp.checksum = val.checksum;
            return temp;
        });

    H5::DataSet::vlenReclaim(fileValues.data(), memType, h5ds.getSpace());
}

void DataSet::read(std::vector<Value> &values) const
{
    DataType dtype = dataType();
    NDSize shape = size();

    if (shape.size() < 1 || shape[0] < 1) {
        return;
    }

    assert(shape.size() == 1);
    size_t nvalues = shape[0];

    switch (dtype) {
    case DataType::Bool:   do_read_value<bool>(h5dset, nvalues, values);     break;
    case DataType::Int32:  do_read_value<int32_t>(h5dset, nvalues, values);  break;
    case DataType::UInt32: do_read_value<uint32_t>(h5dset, nvalues, values); break;
    case DataType::Int64:  do_read_value<int64_t>(h5dset, nvalues, values);  break;
    case DataType::UInt64: do_read_value<uint64_t>(h5dset, nvalues, values); break;
    case DataType::String: do_read_value<char *>(h5dset, nvalues, values);   break;
    case DataType::Double: do_read_value<double>(h5dset, nvalues, values);   break;
#ifndef CHECK_SUPOORTED_VALUES
    default: assert(DATATYPE_SUPPORT_NOT_IMPLEMENTED);
#endif
    }

}

#define NOT_IMPLEMENTED 1

template<typename T>
void do_write_value(const H5::DataSet &h5ds, const std::vector<Value> &values)
{
    typedef FileValue<T> file_value_t;
    std::vector<file_value_t> fileValues;

    fileValues.resize(values.size());

    std::transform(values.begin(), values.end(), fileValues.begin(), [](const Value &val) {
            file_value_t fileVal(val.get<T>());
            fileVal.uncertainty = val.uncertainty;
            fileVal.reference = const_cast<char *>(val.reference.c_str());
            fileVal.filename = const_cast<char *>(val.filename.c_str());
            fileVal.encoder = const_cast<char *>(val.encoder.c_str());
            fileVal.checksum = const_cast<char *>(val.checksum.c_str());

            return fileVal;
        });

    H5::DataType memType = h5_type_for_value<T>(true);
    h5ds.write(fileValues.data(), memType);
}

void DataSet::write(const std::vector<Value> &values)
{
    setExtent(NDSize{values.size()});

    if (values.size() < 1) {
        return; //nothing to do
    }

    switch(values[0].type()) {

    case DataType::Bool:   do_write_value<bool>(h5dset, values); break;
    case DataType::Int32:  do_write_value<int32_t>(h5dset, values); break;
    case DataType::UInt32: do_write_value<uint32_t>(h5dset, values); break;
    case DataType::Int64:  do_write_value<int64_t>(h5dset, values); break;
    case DataType::UInt64: do_write_value<uint64_t>(h5dset, values); break;
    case DataType::String: do_write_value<const char *>(h5dset, values); break;
    case DataType::Double: do_write_value<double>(h5dset, values); break;
#ifndef CHECK_SUPOORTED_VALUES
    default: assert(DATATYPE_SUPPORT_NOT_IMPLEMENTED);
#endif
    }

}

} // namespace hdf5
} // namespace nix
