// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/DataArray.hpp>

#include <nix/util/util.hpp>
#include <nix/hdf5/DataTypeHDF5.hpp>

#include <cstring>

using namespace nix;

static void convertData(DataType source, DataType destination, void *data, size_t nelms)
{
    H5::DataType h5_src = hdf5::data_type_to_h5_memtype(source);
    H5::DataType h5_dst = hdf5::data_type_to_h5_memtype(destination);

    h5_src.convert(h5_dst, nelms, data, nullptr);
}


void DataArray::ioRead(DataType dtype, void *data, const NDSize &count, const NDSize &offset) const {
    const std::vector<double> poly = polynomCoefficients();
    boost::optional<double> opt_origin = expansionOrigin();

    if (poly.size() || opt_origin) {
        size_t data_esize = data_type_to_size(dtype);
        size_t nelms = count.nelms();
        std::vector<double> tmp;
        double *read_buffer;

        if (data_esize < sizeof(double)) {
            //need temporary buffer
            tmp.resize(nelms);
            read_buffer = tmp.data();
        } else {
            read_buffer = reinterpret_cast<double *>(data);
        }

        getDataDirect(DataType::Double, read_buffer, count, offset);
        const double origin = opt_origin ? *opt_origin : 0.0;

        util::applyPolynomial(poly, origin, read_buffer, read_buffer, nelms);
        convertData(DataType::Double, dtype, read_buffer, nelms);

        if (tmp.size()) {
            memcpy(data, read_buffer, nelms * data_esize);
        }

    } else {
        getDataDirect(dtype, data, count, offset);
    }
}

void DataArray::ioWrite(DataType dtype, const void *data, const NDSize &count, const NDSize &offset) {
    setDataDirect(dtype, data, count, offset);
}

void DataArray::appendData(DataType dtype, const void *data, const NDSize &count, size_t axis) {

    //first some sanity checks
    NDSize extent = dataExtent();

    if (axis >= extent.size()) {
        throw InvalidRank("axis is out of bounds");
    }

    if (extent.size() != count.size()) {
        throw IncompatibleDimensions("Data and DataArray must have the same dimensionality", "appendData");
    }

    for (size_t i = 0; i < count.size(); i ++) {
        if (i == axis) {
            continue;
        }

        if (extent[i] != count[i]) {
            throw IncompatibleDimensions("Shape of data and shape of DataArray must match in all dimension but axis!",
                                         "appenData");
        }
    }

    NDSize offset(extent.size(), 0);
    offset[axis] = extent[axis];
    extent[axis] += count[axis];

    //enlarge the DataArray to fit the new data
    dataExtent(extent);

    setData(dtype, data, count, offset);

}

void DataArray::unit(const std::string &unit) {
    if (!(util::isSIUnit(unit) || util::isCompoundSIUnit(unit))) {
        throw InvalidUnit("Unit is not SI or composite of SI units.", "DataArray::unit(const string &unit)");
    }
    backend()->unit(unit);
}


std::vector<Dimension> DataArray::dimensions(const util::Filter<Dimension>::type &filter) const {
    auto f = [this] (size_t i) { return getDimension(i+1); }; // +1 since index starts at 1
    return getEntities<Dimension>(f,
                                  dimensionCount(),
                                  filter);
}


std::ostream& nix::operator<<(std::ostream &out, const DataArray &ent) {
    out << "DataArray: {name = " << ent.name();
    out << ", type = " << ent.type();
    out << ", id = " << ent.id() << "}";
    return out;
}
