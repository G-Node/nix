// Copyright (c) 2017, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
//
// Author: Christian Kellner <ckellner@redhat.com>

#include <nix/util/util.hpp>
#include <nix/Compression.hpp>

#include "DataFrameHDF5.hpp"

#include "h5x/H5DataSet.hpp"

#include <cstring>

namespace nix {
namespace hdf5 {


DataFrameHDF5::DataFrameHDF5(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const H5Group &group)
        : EntityWithSourcesHDF5(file, block, group) {
}


DataFrameHDF5::DataFrameHDF5(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const H5Group &group, const std::string &id, const std::string &type,  const std::string &name)
    : DataFrameHDF5(file, block, group, id, type, name, util::getTime()) {
}


DataFrameHDF5::DataFrameHDF5(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const H5Group &group, const std::string &id, const std::string &type, const std::string &name, time_t time)
    : EntityWithSourcesHDF5(file, block, group, id, type, name, time) {
}

void DataFrameHDF5::createData(const std::vector<Column> &cols) {

    if (group().hasData("data")) {
        throw ConsistencyError("DataFrame's hdf5 data group already exists!");
    }

    std::vector<size_t> offset(cols.size());
    std::vector<h5x::DataType> dtypes(cols.size());

    size_t s = 0;
    for (size_t i = 0; i < cols.size(); i++) {
        h5x::DataType ft = data_type_to_h5_filetype(cols[i].dtype);
        dtypes[i] = ft;
        offset[i] = s;
        s += ft.size();
    }

    h5x::DataType ct = h5x::DataType::makeCompound(s);

    for (size_t i = 0; i < cols.size(); i++) {
        ct.insert(cols[i].name, offset[i], dtypes[i]);
    }

    DataSet ds = group().createData("data", ct, {0}, Compression::None);

    std::vector<std::string> units(cols.size());

    s = 0;
    std::generate(units.begin(), units.end(), [&s, &cols]{ return cols[s++].unit; });
    ds.setAttr("units", units);
}

std::vector<Column> DataFrameHDF5::columns() const {
    if (! group().hasData("data")) {
        throw ConsistencyError("DataFrame's hdf5 data group is missing!");
    }

    DataSet ds = group().openData("data");
    h5x::DataType dt = ds.dataType();

    const unsigned n = dt.member_count();
    std::vector<Column> cols(n);

    std::vector<std::string> units(n);
    ds.getAttr("units", units);

    for (unsigned i = 0; i < n; i++) {
        cols[i].dtype = data_type_from_h5(dt.member_type(i));
        cols[i].name = dt.member_name(i);
        cols[i].unit = units[i];
    }

    return cols;
}

ndsize_t DataFrameHDF5::rows() const {
    DataSet ds = group().openData("data");
    NDSize s = ds.size();

    //assert size
    return s[0];
}

void DataFrameHDF5::rows(ndsize_t n) {
    DataSet ds = group().openData("data");
    ds.setExtent({n});
}

struct MemRef {
    char *data;

    MemRef() : data(NULL) {}
    MemRef(size_t size) : data(NULL) {
        data = static_cast<char *>(std::malloc(sizeof(Variant)));
    }

    MemRef(const Variant &v, char *mem = NULL) : data(NULL) {
        if (mem == NULL) {
            mem = data = static_cast<char *>(std::malloc(sizeof(Variant)));
        }

        switch (v.type()) {
        case DataType::Bool:
            bool b;
            v.get(b);
            std::memcpy(mem, &b, sizeof(b));
            break;

        case DataType::Double:
            double d;
            v.get(d);
            std::memcpy(mem, &d, sizeof(d));
            break;

        case DataType::UInt32:
            uint32_t ui32;
            v.get(ui32);
            std::memcpy(mem, &ui32, sizeof(ui32));
            break;

        case DataType::Int32:
            int32_t i32;
            v.get(i32);
            std::memcpy(mem, &i32, sizeof(i32));
            break;

        case DataType::UInt64:
            uint64_t ui64;
            v.get(ui64);
            std::memcpy(mem, &ui64, sizeof(ui64));
            break;

        case DataType::Int64:
            int64_t i64;
            v.get(i64);
            std::memcpy(mem, &i64, sizeof(i64));
            break;

        case DataType::String:
            char *str;
            str = const_cast<char *>(v.get<const char *>());
            std::memcpy(mem, &str, sizeof(char *));
            break;

        default:
            throw "FIXME";
        };
    }

    Variant copy(DataType dtype) {

        switch (dtype) {
        case DataType::Bool:
            bool b;
            std::memcpy(&b, data, sizeof(b));
            return Variant(b);

        case DataType::Double:
            double d;
            std::memcpy(&d, data, sizeof(d));
            return Variant(d);

        case DataType::UInt32:
            uint32_t ui32;
            std::memcpy(&ui32, data, sizeof(ui32));
            return Variant(ui32);

        case DataType::Int32:
            int32_t i32;
            std::memcpy(&i32, data, sizeof(i32));
            return Variant(i32);

        case DataType::UInt64:
            uint64_t ui64;
            std::memcpy(&ui64, data, sizeof(ui64));
            return Variant(ui64);

        case DataType::Int64:
            int64_t i64;
            std::memcpy(&i64, data, sizeof(i64));
            return Variant(i64);

        case DataType::String:
            char **str;
            std::memcpy(&str, data, sizeof(char **));
            return Variant(str);

        default:
            throw "FIXME";
        };
    }

    ~MemRef() {
        std::free(data);
    }
};

void DataFrameHDF5::writeCell(ndsize_t row, ndsize_t col, const Variant &v) {

    DataSet ds = group().openData("data");

    h5x::DataType dtype = ds.dataType();
    h5x::DataType mtype = data_type_to_h5_memtype(v.type());

    h5x::DataType ctype = h5x::DataType::makeCompound(mtype.size());
    const std::string &name = dtype.member_name(col);
    ctype.insert(name, 0, mtype);

    MemRef r = MemRef(v);
    ds.write(r.data, ctype, NDSize{1}, NDSize{row});
}

void DataFrameHDF5::writeRow(ndsize_t row, const std::vector<Variant> &vals) {
    DataSet ds = group().openData("data");

    std::vector<size_t> offset(vals.size());
    std::vector<h5x::DataType> dtypes(vals.size());

    //Just use the upper limit
    size_t s = sizeof(nix::Variant) * vals.size();
    char *base = static_cast<char *>(std::malloc(s));

    s = 0;
    for (size_t i = 0; i < vals.size(); i++) {
        h5x::DataType ft = data_type_to_h5_memtype(vals[i].type());
        char *ptr = base + s;

        dtypes[i] = ft;
        offset[i] = s;
        s += ft.size();

        // This will actually copy the data
        MemRef(vals[i], ptr);

    }

    h5x::DataType ct = h5x::DataType::makeCompound(s);

    h5x::DataType dfile = ds.dataType();
    for (size_t i = 0; i < vals.size(); i++) {
        std::string name = dfile.member_name(i);
        ct.insert(name, offset[i], dtypes[i]);
    }

    ds.write(base, ct, NDSize{1}, NDSize{row});

    std::free(base);
}

Variant DataFrameHDF5::readCell(ndsize_t row, ndsize_t col) {
    DataSet ds = group().openData("data");

    h5x::DataType dtype = ds.dataType();
    h5x::DataType coltype = dtype.member_type(col);

    DataType memtype = data_type_from_h5(coltype);
    h5x::DataType mtype = data_type_to_h5_memtype(memtype);
    h5x::DataType ctype = h5x::DataType::makeCompound(mtype.size());
    const std::string &name = dtype.member_name(col);
    ctype.insert(name, 0, mtype);

    MemRef r{sizeof(Variant)};
    ds.read(r.data, ctype, NDSize{1}, NDSize{row});

    return r.copy(memtype);
}

}
}
