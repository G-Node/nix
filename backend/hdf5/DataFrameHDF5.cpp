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

std::vector<unsigned> DataFrameHDF5::nameToCol(const std::vector<std::string> &names) const {
    DataSet ds = group().openData("data");
    h5x::DataType dtype = ds.dataType();

    std::vector<unsigned> cols(names.size());

    for (size_t i = 0; i < names.size(); i++) {
        cols[i] = dtype.member_index(names[i]);
    }

    return cols;
}

std::vector<std::string> DataFrameHDF5::colToName(const std::vector<unsigned> &cols) const {
    DataSet ds = group().openData("data");
    h5x::DataType dtype = ds.dataType();

    std::vector<std::string> names(cols.size());

    for (size_t i = 0; i < cols.size(); i++) {
        names[i] = dtype.member_name(cols[i]);
    }

    return names;
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

struct Janus {

    explicit Janus(const std::vector<Cell> &cells) {

        std::vector<h5x::DataType> dtypes(cells.size());

        std::transform(cells.cbegin(), cells.cend(), dtypes.begin(),
                       [](const Cell &c){
                           return data_type_to_h5_memtype(c.value.type());
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
            dtype.insert(c.name, offset, t);

            copyValue(offset, c.value);

            offset += s;
        }
    }

    explicit Janus(const h5x::DataType &dst, const std::vector<unsigned> &cols) {

        std::vector<h5x::DataType> dtypes(cols.size());

        std::transform(cols.cbegin(), cols.cend(), dtypes.begin(),
                       [&dst](const unsigned &c){
                           return dst.member_type(c);
                       });


        size_t ms = std::accumulate(dtypes.cbegin(), dtypes.cend(), size_t(0),
                                    [&dst](size_t size, const h5x::DataType &dt) {
                                        return size + dt.size();
                                    });

        dtype = h5x::DataType::makeCompound(ms);
        data = new char[ms];

        size_t offset = 0;
        for (size_t i = 0; i < cols.size(); i++) {
            unsigned n = cols[i];
            const std::string &name = dst.member_name(n);
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

    Variant copyData(size_t offset, DataType data_type) {
        char *mem = data + offset;

        switch (data_type) {
        case DataType::Bool:
            bool b;
            std::memcpy(&b, mem, sizeof(b));
            return Variant(b);

        case DataType::Double:
            double d;
            std::memcpy(&d, mem, sizeof(d));
            return Variant(d);

        case DataType::UInt32:
            uint32_t ui32;
            std::memcpy(&ui32, mem, sizeof(ui32));
            return Variant(ui32);

        case DataType::Int32:
            int32_t i32;
            std::memcpy(&i32, mem, sizeof(i32));
            return Variant(i32);

        case DataType::UInt64:
            uint64_t ui64;
            std::memcpy(&ui64, mem, sizeof(ui64));
            return Variant(ui64);

        case DataType::Int64:
            int64_t i64;
            std::memcpy(&i64, mem, sizeof(i64));
            return Variant(i64);

        case DataType::String:
            const char *str;
            std::memcpy(&str, mem, sizeof(str));
            return Variant(str);

        default:
            throw std::invalid_argument("Unhandled DataType");
        };
    }

    std::vector<Variant> copyData() {
        unsigned n = dtype.member_count();

        std::vector<Variant> res(n);

        unsigned i = 0;
        std::generate(res.begin(), res.end(),
                      [&i, this](){
                          size_t offset = dtype.member_offset(i);
                          h5x::DataType dt = dtype.member_type(i);
                          DataType nt = data_type_from_h5(dt);
                          i++;

                          return copyData(offset, nt);
                      });
        return res;
    }

    static std::vector<Cell> resolveIndex(const h5x::DataType ct,
                                          const ndsize_t col,
                                          const Variant &v) {
        return {Cell{ct.member_name(col), v}};

    }

    static std::vector<Cell> resolveIndex(const h5x::DataType ct,
                                          const std::vector<Cell> &cells) {

        std::vector<Cell> res;

        std::transform(cells.cbegin(), cells.cend(), std::back_inserter(res),
                       [&ct](const Cell &c) {
                           if (c.haveName()) {
                               return c;
                           }

                           return Cell(ct.member_name(c.col), c.value);

                       });
        return res;
    }

    ~Janus() {
        delete[] data;
    }

    char *data;
    h5x::DataType dtype;
};

void DataFrameHDF5::writeCell(ndsize_t row, ndsize_t col, const Variant &v) {
    DataSet ds = group().openData("data");
    Janus j{Janus::resolveIndex(ds.dataType(), col, v)};

    ds.write(j.data, j.dtype, NDSize{1}, NDSize{row});
}

void DataFrameHDF5::writeCells(ndsize_t row, const std::vector<Cell> &cells) {
    DataSet ds = group().openData("data");
    Janus j{Janus::resolveIndex(ds.dataType(), cells)};

    ds.write(j.data, j.dtype, NDSize{1}, NDSize{row});
}

void DataFrameHDF5::writeRow(ndsize_t row, const std::vector<Variant> &vals) {
    DataSet ds = group().openData("data");
    h5x::DataType dt = ds.dataType();
    std::vector<Cell> cells;

    size_t i = 0;
    std::transform(vals.cbegin(), vals.cend(), std::back_inserter(cells),
                   [&dt, &i](const Variant &v) {
                       return Cell{dt.member_name(i++), v};
                   });

    Janus j{cells};

    ds.write(j.data, j.dtype, NDSize{1}, NDSize{row});
}

Variant DataFrameHDF5::readCell(ndsize_t row, ndsize_t col) {
    DataSet ds = group().openData("data");
    h5x::DataType dtype = ds.dataType();

    Janus j{dtype, {(unsigned) col}};
    ds.read(j.data, j.dtype, NDSize{1}, NDSize{row});
    std::vector<Variant> res = j.copyData();
    return res[0];
}

std::vector<Variant> DataFrameHDF5::readRow(ndsize_t row) {
    DataSet ds = group().openData("data");
    h5x::DataType dts = ds.dataType();

    std::vector<unsigned> cols(dts.member_count());
    std::iota(cols.begin(), cols.end(), 0);

    Janus j{dts, cols};
    ds.read(j.data, j.dtype, NDSize{1}, NDSize{row});
    //TODO: reclaim vlan data
    return j.copyData();
}

void DataFrameHDF5::writeColumn(ndsize_t col,
                                ndsize_t offset,
                                ndsize_t count,
                                DataType dtype,
                                const char *data) {
    DataSet ds = group().openData("data");
    h5x::DataType dts = ds.dataType();
    h5x::DataType memtype = data_type_to_h5_memtype(dtype);
    size_t ms = memtype.size();
    std::string name = dts.member_name(static_cast<unsigned>(col));

    h5x::DataType ct = h5x::DataType::makeCompound(ms);
    ct.insert(name, 0, memtype);

    ds.write(data, ct, NDSize{count}, NDSize{offset});
}

void DataFrameHDF5::readColumn(ndsize_t col,
                               ndsize_t offset,
                               ndsize_t count,
                               DataType dtype,
                               void *data) {
    DataSet ds = group().openData("data");
    h5x::DataType dts = ds.dataType();
    h5x::DataType memtype = data_type_to_h5_memtype(dtype);
    std::string name = dts.member_name(static_cast<unsigned>(col));

    size_t ms = memtype.size();
    h5x::DataType ct = h5x::DataType::makeCompound(ms);
    ct.insert(name, 0, memtype);

    ds.read(data, ct, NDSize{count}, NDSize{offset});

}

}
}
