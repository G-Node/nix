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

    DataSet ds = group().createData("data", ct, nix::NDSize({0}), Compression::None);

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

}
}
