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
#include <numeric>
#include <algorithm>

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

void DataFrameHDF5::createData(const std::vector<Column> &cols, const Compression &compression) {

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

    DataSet ds = group().createData("data", ct, {0}, compression);

    std::vector<std::string> units(cols.size());

    s = 0;
    std::generate(units.begin(), units.end(), [&s, &cols]{ return cols[s++].unit; });
    ds.setAttr("units", units);
}

std::vector<Column> DataFrameHDF5::columns() const {
    DataSet ds = data();
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

unsigned DataFrameHDF5::colIndex(const std::string &name) const {
    DataSet ds = data();
    h5x::DataType dtype = ds.dataType();
    return dtype.member_index(name);
}

std::string DataFrameHDF5::colName(unsigned col) const {
    DataSet ds = data();
    h5x::DataType dtype = ds.dataType();
    return dtype.member_name(col);
}

std::vector<unsigned> DataFrameHDF5::colIndex(const std::vector<std::string> &names) const {
    DataSet ds = data();
    h5x::DataType dtype = ds.dataType();

    std::vector<unsigned> cols(names.size());

    for (size_t i = 0; i < names.size(); i++) {
        cols[i] = dtype.member_index(names[i]);
    }

    return cols;
}

std::vector<std::string> DataFrameHDF5::colName(const std::vector<unsigned> &cols) const {
    DataSet ds = data();
    h5x::DataType dtype = ds.dataType();

    std::vector<std::string> names(cols.size());

    for (size_t i = 0; i < cols.size(); i++) {
        names[i] = dtype.member_name(cols[i]);
    }

    return names;
}

ndsize_t DataFrameHDF5::rows() const {
    DataSet ds = data();
    NDSize s = ds.size();
    return s.size() > 0 ? s[0] : 0;
}

void DataFrameHDF5::rows(ndsize_t n) {
    DataSet ds = data();
    ds.setExtent({n});
}

struct Janus {

    explicit Janus(const h5x::DataType &dst, const std::vector<Cell> &cells) {

        std::vector<h5x::DataType> dtypes(cells.size());

        std::transform(cells.cbegin(), cells.cend(), dtypes.begin(),
                       [](const Cell &c){
                           return data_type_to_h5_memtype(c.type());
                       });

        std::vector<size_t> sizes(cells.size());
        std::transform(dtypes.cbegin(), dtypes.cend(), sizes.begin(),
                       [](const h5x::DataType &t) {
                           return t.size();
                       });

        size_t ms = std::accumulate(sizes.cbegin(), sizes.cend(),
                                    size_t(0),
                                    [](size_t size, size_t s) {
                                        return size + s;
                                    });

        data = new char[ms];
        dtype = h5x::DataType::makeCompound(ms);

        size_t offset = 0;
        for (size_t i = 0; i < cells.size(); i++) {
            const Cell &c = cells[i];
            const h5x::DataType &t = dtypes[i];
            const size_t s = sizes[i];
            const std::string &name = c.haveName() ?
                                          c.name :
                                          dst.member_name(c.col);

            dtype.insert(name, offset, t);
            copyValue(offset, c);
            offset += s;
        }
    }

    explicit Janus(const h5x::DataType &dst, const std::vector<std::string> &cols) {

        std::vector<h5x::DataType> dtypes(cols.size());

        std::transform(cols.cbegin(), cols.cend(), dtypes.begin(),
                       [&dst](const std::string &c){
                           return dst.member_type(c);
                       });


        size_t ms = std::accumulate(dtypes.cbegin(), dtypes.cend(), size_t(0),
                                    [](size_t size, const h5x::DataType &dt) {
                                        return size + dt.size();
                                    });

        dtype = h5x::DataType::makeCompound(ms);
        data = new char[ms];

        size_t offset = 0;
        for (size_t i = 0; i < cols.size(); i++) {
            const std::string &name = cols[i];
            DataType nix_type = data_type_from_h5(dtypes[i]);
            dtype.insert(name, offset, data_type_to_h5_memtype(nix_type));
            offset += dtypes[i].size();
        }

    }

    void copyValue(size_t offset, const Variant &v) {
        char *mem = data + offset;

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
            const char *str;
            str = v.get<const char *>();
            std::memcpy(mem, &str, sizeof(str));
            break;

        default:
            throw std::invalid_argument("Unhandled DataType");
        };
    }

    void copyData(Variant &v, size_t offset, DataType data_type) {
        char *mem = data + offset;

        switch (data_type) {
        case DataType::Bool:
            bool b;
            std::memcpy(&b, mem, sizeof(b));
            v.set(b);
            break;

        case DataType::Double:
            double d;
            std::memcpy(&d, mem, sizeof(d));
            v.set(d);
            break;

        case DataType::UInt32:
            uint32_t ui32;
            std::memcpy(&ui32, mem, sizeof(ui32));
            v.set(ui32);
            break;

        case DataType::Int32:
            int32_t i32;
            std::memcpy(&i32, mem, sizeof(i32));
            v.set(i32);
            break;

        case DataType::UInt64:
            uint64_t ui64;
            std::memcpy(&ui64, mem, sizeof(ui64));
            v.set(ui64);
            break;

        case DataType::Int64:
            int64_t i64;
            std::memcpy(&i64, mem, sizeof(i64));
            v.set(i64);
            break;

        case DataType::String:
            const char *str;
            std::memcpy(&str, mem, sizeof(str));
            v.set(str);
            break;

        default:
            throw std::invalid_argument("Unhandled DataType");
        };
    }

    void copyData(Variant &v, unsigned i) {
        size_t offset = dtype.member_offset(i);
        h5x::DataType dt = dtype.member_type(i);
        DataType nt = data_type_from_h5(dt);

        copyData(v, offset, nt);
    }

    std::vector<Cell> copyData() {
        unsigned n = dtype.member_count();

        std::vector<Cell> res(n);
        for (unsigned i = 0; i < n; i++) {
            copyData(res[i], i);
            res[i].name = dtype.member_name(i);
            res[i].col = i;
        }

        return res;
    }

    void copyData(std::vector<Variant> &res) {
        unsigned n = dtype.member_count();
        res.resize(n);

        for (unsigned i = 0; i < n; i++) {
            copyData(res[i], i);
        }
    }

    ~Janus() {
        delete[] data;
    }

    char *data;
    h5x::DataType dtype;
};


void DataFrameHDF5::writeCells(ndsize_t row, const std::vector<Cell> &cells) {
    DataSet ds = data();
    h5x::DataType dt = ds.dataType();
    Janus j{dt, cells};

    ds.write(j.data, j.dtype, NDSize{1}, NDSize{row});
}

void DataFrameHDF5::writeRow(ndsize_t row, const std::vector<Variant> &vals) {
    DataSet ds = data();
    h5x::DataType dt = ds.dataType();
    std::vector<Cell> cells;

    size_t i = 0;
    std::transform(vals.cbegin(), vals.cend(), std::back_inserter(cells),
                   [&dt, &i](const Variant &v) {
                       const unsigned k = static_cast<unsigned>(i++);
                       return Cell{dt.member_name(k), v};
                   });

    Janus j{dt, cells};

    ds.write(j.data, j.dtype, NDSize{1}, NDSize{row});
}

std::vector<Cell> DataFrameHDF5::readCells(ndsize_t row, const std::vector<std::string> &cols) const {
    DataSet ds = data();
    h5x::DataType dtype = ds.dataType();

    Janus j{dtype, cols};
    NDSize count = {1};
    NDSize offset = {row};
    DataSpace fileSpace, memSpace;
    std::tie(memSpace, fileSpace) = ds.offsetCount2DataSpaces(count, offset);

    ds.read(j.data, j.dtype, count, offset);

    std::vector<Cell> res = j.copyData();
    ds.vlenReclaim(j.dtype.h5id(), j.data, &memSpace);

    return res;
}

std::vector<Variant> DataFrameHDF5::readRow(ndsize_t row) const {
    DataSet ds = data();
    h5x::DataType dts = ds.dataType();

    unsigned n = 0;
    std::vector<std::string> cols(dts.member_count());
    std::generate(cols.begin(), cols.end(), [&n, &dts]{
            const unsigned i = n++;
            return dts.member_name(i);
        });

    Janus j{dts, cols};

    NDSize count = {1};
    NDSize offset = {row};
    DataSpace fileSpace, memSpace;
    std::tie(memSpace, fileSpace) = ds.offsetCount2DataSpaces(count, offset);

    ds.read(j.data, j.dtype, count, offset);

    std::vector<Variant> res(cols.size());
    j.copyData(res);

    ds.vlenReclaim(j.dtype.h5id(), j.data, &memSpace);

    return res;
}

void DataFrameHDF5::writeColumn(const std::string &name,
                                ndsize_t offset,
                                ndsize_t count,
                                DataType dtype,
                                const void *data) {
    DataSet ds = this->data();
    h5x::DataType dts = ds.dataType();
    h5x::DataType memType = data_type_to_h5_memtype(dtype);
    size_t ms = memType.size();

    h5x::DataType ct = h5x::DataType::makeCompound(ms);
    ct.insert(name, 0, memType);

    NDSize ndcount = {count};
    NDSize ndoffset = {offset};
    DataSpace fileSpace, memSpace;
    std::tie(memSpace, fileSpace) = ds.offsetCount2DataSpaces(ndcount, ndoffset);

    if (dtype == DataType::String) {
        StringReader reader(ndcount, data);
        ds.write(*reader, ct, memSpace, fileSpace);
    } else {
        ds.write(data, ct, memSpace, fileSpace);
    }
}

void DataFrameHDF5::readColumn(const std::string &name,
                               ndsize_t offset,
                               ndsize_t count,
                               DataType dtype,
                               void *data) const {
    DataSet ds = this->data();
    h5x::DataType dts = ds.dataType();
    h5x::DataType memType = data_type_to_h5_memtype(dtype);

    size_t ms = memType.size();
    h5x::DataType ct = h5x::DataType::makeCompound(ms);
    ct.insert(name, 0, memType);

    NDSize ndcount = {count};
    NDSize ndoffset = {offset};
    DataSpace fileSpace, memSpace;
    std::tie(memSpace, fileSpace) = ds.offsetCount2DataSpaces(ndcount, ndoffset);

    if (dtype == DataType::String) {
        StringWriter writer(ndcount, data);
        ds.read(*writer, ct, memSpace, fileSpace);
        writer.finish();
        ds.vlenReclaim(memType.h5id(), *writer, &memSpace);
    } else {
        ds.read(data, ct, memSpace, fileSpace);
    }
}

}
}
