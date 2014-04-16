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
                        bool maxSizeUnlimited,
                        bool guessChunks)
{
    H5::DataSpace space;

    if (size) {
        if (maxsize) {
            space = DataSpace::create(size, maxsize);
        } else {
            space = DataSpace::create(size, maxSizeUnlimited);
        }
    }

    H5::DSetCreatPropList plcreate = H5::DSetCreatPropList::DEFAULT;

    if (chunks) {
        int rank = static_cast<int>(chunks.size());
        plcreate.setChunk(rank, chunks.data());
    } else if (guessChunks) {
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

        double csize = static_cast<double>(chunks.nelms());
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

void DataSet::setExtent(const NDSize &dims)
{
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

void DataSet::vlenReclaim(DataType memType, void *data, H5::DataSpace *dspace) const
{
    H5::DataType h5MemType = data_type_to_h5_memtype(memType);

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
    //if is a compound type, we should catched that here

    size_t size = H5Tget_size(ftype);
    H5T_sign_t sign = H5Tget_sign(ftype);

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
H5::DataType h5_type_for_value(bool forMemory)
{
    typedef FileValue<T> file_value_t;

    H5::CompType h5type(sizeof(file_value_t));

    DataType dtype = to_data_type<T>::value;
    if(forMemory) {
        h5type.insertMember("value", HOFFSET(file_value_t, value), hdf5::data_type_to_h5_memtype(dtype));
    } else {
        h5type.insertMember("value", HOFFSET(file_value_t, value), hdf5::data_type_to_h5_filetype(dtype));
    }

    if(forMemory) {
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

H5::DataType DataSet::fileTypeForValue(DataType dtype)
{
    switch(dtype) {
    case DataType::Bool:   return h5_type_for_value<bool>(false);
    case DataType::Int32:  return h5_type_for_value<int32_t>(false);
    case DataType::UInt32: return h5_type_for_value<uint32_t>(false);
    case DataType::Int64:  return h5_type_for_value<int64_t>(false);
    case DataType::UInt64: return h5_type_for_value<uint64_t>(false);
    case DataType::Double: return h5_type_for_value<double>(false);
    case DataType::String: return h5_type_for_value<char *>(false);
#ifndef CHECK_SUPOORTED_VALUES
    default: assert(DATATYPE_SUPPORT_NOT_IMPLEMENTED); return H5::DataType{};
#endif
    }
}

H5::DataType DataSet::memTypeForValue(DataType dtype)
{
    switch(dtype) {
    case DataType::Bool:   return h5_type_for_value<bool>(true);
    case DataType::Int32:  return h5_type_for_value<int32_t>(true);
    case DataType::UInt32: return h5_type_for_value<uint32_t>(true);
    case DataType::Int64:  return h5_type_for_value<int64_t>(true);
    case DataType::UInt64: return h5_type_for_value<uint64_t>(true);
    case DataType::Double: return h5_type_for_value<double>(true);
    case DataType::String: return h5_type_for_value<char *>(true);
#ifndef CHECK_SUPOORTED_VALUES
    default: assert(DATATYPE_SUPPORT_NOT_IMPLEMENTED); return H5::DataType{};
#endif
    }
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
    //we first have to obtain the DataType from hdf5
    hid_t ftype = H5Dget_type(h5dset.getId());
    H5T_class_t ftclass = H5Tget_class(ftype);
    assert(ftclass == H5T_COMPOUND);

    int nmems = H5Tget_nmembers(ftype);
    assert(nmems == 6);
    hid_t vtype = H5Tget_member_type(ftype, 0);

    H5T_class_t vclass = H5Tget_class(vtype);
    size_t vsize = H5Tget_size(vtype);
    H5T_sign_t vsign = H5Tget_sign(vtype);

    DataType dtype = nix::hdf5::data_type_from_h5(vclass, vsize, vsign);

    H5Tclose(vtype);
    H5Tclose(ftype);

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
