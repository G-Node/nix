// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "H5DataSet.hpp"
#include "H5Exception.hpp"

#include <iostream>
#include <cmath>

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

DataSet::DataSet(hid_t hid)
        : LocID(hid) {

}

DataSet::DataSet(const DataSet &other)
        : LocID(other)  {

}

void DataSet::read(hid_t memType, void *data) const
{
    HErr res = H5Dread(hid, memType, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);
    res.check("DataSet::read() IO error");
}

void DataSet::write(hid_t memType, const void *data)
{
    HErr res = H5Dwrite(hid, memType, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);
    res.check("DataSet::write() IOError");
}

void DataSet::read(DataType dtype, const NDSize &size, void *data) const
{
    h5x::DataType memType = data_type_to_h5_memtype(dtype);

    if (dtype == DataType::String) {
        StringWriter writer(size, static_cast<std::string *>(data));
        read(memType.h5id(), *writer);
        writer.finish();
        vlenReclaim(memType.h5id(), *writer);
    } else {
        read(memType.h5id(), data);
    }
}


void DataSet::write(DataType dtype, const NDSize &size, const void *data)
{
    h5x::DataType memType = data_type_to_h5_memtype(dtype);
    if (dtype == DataType::String) {
        StringReader reader(size, static_cast<const std::string *>(data));
        write(memType.h5id(), *reader);
    } else {
        write(memType.h5id(), data);
    }
}


void DataSet::read(DataType        dtype,
                   void            *data,
                   const Selection &fileSel,
                   const Selection &memSel) const
{
    h5x::DataType memType = data_type_to_h5_memtype(dtype);

    HErr res;
    if (dtype == DataType::String) {
        NDSize size = memSel.size();
        StringWriter writer(size, static_cast<std::string *>(data));
        res = H5Dread(hid, memType.h5id(), memSel.h5space().h5id(), fileSel.h5space().h5id(), H5P_DEFAULT, *writer);
        writer.finish();
        vlenReclaim(memType.h5id(), *writer);
    } else {
        res = H5Dread(hid, memType.h5id(), memSel.h5space().h5id(), fileSel.h5space().h5id(), H5P_DEFAULT, data);
    }

    res.check("DataSet::read() IO error");
}

void DataSet::write(DataType         dtype,
                    const void      *data,
                    const Selection &fileSel,
                    const Selection &memSel)
{
    h5x::DataType memType = data_type_to_h5_memtype(dtype);
    HErr res;

    if (dtype == DataType::String) {
        NDSize size = memSel.size();
        StringReader reader(size, static_cast<const std::string *>(data));
        res = H5Dwrite(hid, memType.h5id(), memSel.h5space().h5id(), fileSel.h5space().h5id(), H5P_DEFAULT, *reader);
    } else {
        res = H5Dwrite(hid, memType.h5id(), memSel.h5space().h5id(), fileSel.h5space().h5id(), H5P_DEFAULT, data);
    }

    res.check("DataSet::write(): IO error");
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
    DataSpace space = getSpace();

    if (space.extent().size() != dims.size()) {
        throw InvalidRank("Cannot change the dimensionality via setExtent()");
    }

    HErr res = H5Dset_extent(hid, dims.data());
    res.check("DataSet::setExtent(): Could not set the extent of the DataSet.");

}

Selection DataSet::createSelection() const
{
    DataSpace space = getSpace();
    return Selection(space);
}


NDSize DataSet::size() const
{
    return getSpace().extent();
}

void DataSet::vlenReclaim(h5x::DataType mem_type, void *data, DataSpace *dspace) const
{
    HErr res;
    if (dspace != nullptr) {
        res = H5Dvlen_reclaim(mem_type.h5id(), dspace->h5id(), H5P_DEFAULT, data);
    } else {
        DataSpace space = getSpace();
        res = H5Dvlen_reclaim(mem_type.h5id(), space.h5id(), H5P_DEFAULT, data);
    }

    res.check("DataSet::vlenReclaim(): could not reclaim dynamic bufferes");
}


DataType DataSet::dataType(void) const
{
    hid_t ftype = H5Dget_type(hid);
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
    } else if (ftclass == H5T_OPAQUE) {
      return DataType::Opaque;
    } else {
        size = H5Tget_size(ftype);
        sign = H5Tget_sign(ftype);
    }

    DataType dtype = nix::hdf5::data_type_from_h5(ftclass, size, sign);
    H5Tclose(ftype);

    return dtype;
}

DataSpace DataSet::getSpace() const {
    DataSpace space = H5Dget_space(hid);
    space.check("DataSet::getSpace(): Could not obtain dataspace");
    return space;
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

    inline T val() const { return value; }
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
    explicit FileValue(const bool &vref) :
            value(static_cast<unsigned char>(vref ? 1 : 0)) {
    }

    inline bool val() const { return value > 0; }
};

//

class CompoundType {
public:

    explicit CompoundType(size_t size) : strType(H5I_INVALID_HID) {
        hid = H5Tcreate(H5T_COMPOUND, size);
        if (hid < 0) {
            throw H5Exception("Could not create compound type");
        }
    }

    void insert(const std::string &name, size_t offset, hid_t memberType) {
        HErr result = H5Tinsert(hid, name.c_str(), offset, memberType);
        result.check("CompoundType::insert(): Could not insert member into compound type");
    }

    void insertString(const std::string &name, size_t offset) {
        if (strType == H5I_INVALID_HID) {
            strType = H5Tcopy (H5T_C_S1);

            if (strType < 0) {
                throw H5Exception("H5Tcopy: Could not copy C_S1 type");
            }

            HErr status = H5Tset_size (strType, H5T_VARIABLE);
            status.check("CompoundType::insertString(): H5Tset_size failed");
        }

        insert(name, offset, strType);
    }

    void insert(const std::string &name, size_t offset, DataType memberType, bool forMemory) {
        h5x::DataType h5dt;

        if (forMemory) {
            h5dt = hdf5::data_type_to_h5_memtype(memberType);
        } else {
            h5dt = hdf5::data_type_to_h5_filetype(memberType);
        }

        insert(name, offset, h5dt.h5id());
    }

    hid_t h5id() {
        return hid;
    }

private:
    hid_t hid;
    hid_t strType;
};

template<typename T>
h5x::DataType h5_type_for_value(bool for_memory)
{
    typedef FileValue<T> file_value_t;
    CompoundType ct(sizeof(file_value_t));

    ct.insert("value", HOFFSET(file_value_t, value), to_data_type<T>::value, for_memory);
    ct.insert("uncertainty", HOFFSET(file_value_t, uncertainty), DataType::Double, for_memory);
    ct.insertString("reference", HOFFSET(file_value_t, reference));
    ct.insertString("filename", HOFFSET(file_value_t, filename));
    ct.insertString("encoder", HOFFSET(file_value_t, encoder));
    ct.insertString("checksum", HOFFSET(file_value_t, checksum));

    return ct.h5id();
}

#if 0 //set to one to check that all supported DataTypes are handled
#define CHECK_SUPOORTED_VALUES
#endif
#define DATATYPE_SUPPORT_NOT_IMPLEMENTED false
//
static h5x::DataType h5_type_for_value_dtype(DataType dtype, bool for_memory)
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
    default: assert(DATATYPE_SUPPORT_NOT_IMPLEMENTED); return h5x::DataType{};
#endif
    }
}

h5x::DataType DataSet::fileTypeForValue(DataType dtype)
{
    return h5_type_for_value_dtype(dtype, false);
}

h5x::DataType DataSet::memTypeForValue(DataType dtype)
{
    return h5_type_for_value_dtype(dtype, true);
}

template<typename T>
void do_read_value(const DataSet &h5ds, size_t size, std::vector<Value> &values)
{
    h5x::DataType memType = h5_type_for_value<T>(true);

    typedef FileValue<T> file_value_t;
    std::vector<file_value_t> fileValues;

    fileValues.resize(size);
    values.resize(size);

    h5ds.read(memType.h5id(), fileValues.data());

    std::transform(fileValues.begin(), fileValues.end(), values.begin(), [](const file_value_t &val) {
            Value temp(val.val());
            temp.uncertainty = val.uncertainty;
            temp.reference = val.reference;
            temp.filename = val.filename;
            temp.encoder = val.encoder;
            temp.checksum = val.checksum;
            return temp;
        });

    h5ds.vlenReclaim(memType, fileValues.data());
}

void DataSet::read(std::vector<Value> &values) const
{
    DataType dtype = dataType();
    NDSize shape = size();

    if (shape.size() < 1 || shape[0] < 1) {
        return;
    }

    assert(shape.size() == 1);
    size_t nvalues = nix::check::fits_in_size_t(shape[0], "Can't resize: data to big for memory");

    switch (dtype) {
    case DataType::Bool:   do_read_value<bool>(*this, nvalues, values);     break;
    case DataType::Int32:  do_read_value<int32_t>(*this, nvalues, values);  break;
    case DataType::UInt32: do_read_value<uint32_t>(*this, nvalues, values); break;
    case DataType::Int64:  do_read_value<int64_t>(*this, nvalues, values);  break;
    case DataType::UInt64: do_read_value<uint64_t>(*this, nvalues, values); break;
    case DataType::String: do_read_value<char *>(*this, nvalues, values);   break;
    case DataType::Double: do_read_value<double>(*this, nvalues, values);   break;
#ifndef CHECK_SUPOORTED_VALUES
    default: assert(DATATYPE_SUPPORT_NOT_IMPLEMENTED);
#endif
    }

}

#define NOT_IMPLEMENTED 1

template<typename T>
void do_write_value(DataSet &h5ds, const std::vector<Value> &values)
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

    h5x::DataType memType = h5_type_for_value<T>(true);
    h5ds.write(memType.h5id(), fileValues.data());
}

void DataSet::write(const std::vector<Value> &values)
{
    setExtent(NDSize{values.size()});

    if (values.size() < 1) {
        return; //nothing to do
    }

    switch(values[0].type()) {

    case DataType::Bool:   do_write_value<bool>(*this, values); break;
    case DataType::Int32:  do_write_value<int32_t>(*this, values); break;
    case DataType::UInt32: do_write_value<uint32_t>(*this, values); break;
    case DataType::Int64:  do_write_value<int64_t>(*this, values); break;
    case DataType::UInt64: do_write_value<uint64_t>(*this, values); break;
    case DataType::String: do_write_value<const char *>(*this, values); break;
    case DataType::Double: do_write_value<double>(*this, values); break;
#ifndef CHECK_SUPOORTED_VALUES
    default: assert(DATATYPE_SUPPORT_NOT_IMPLEMENTED);
#endif
    }

}


} // namespace hdf5
} // namespace nix
