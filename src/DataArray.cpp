// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/DataArray.hpp>

#include "hdf5/h5x/H5DataType.hpp"

#include <cstring>

using namespace nix;

static void convertData(DataType source, DataType destination, void *data, size_t nelms)
{
    hdf5::h5x::DataType h5_src = hdf5::data_type_to_h5_memtype(source);
    hdf5::h5x::DataType h5_dst = hdf5::data_type_to_h5_memtype(destination);

    hdf5::HErr res = H5Tconvert(h5_src.h5id(), h5_dst.h5id(), nelms, data, nullptr, H5P_DEFAULT);
    res.check("Could not convert data");
}


void DataArray::ioRead(DataType dtype, void *data, const NDSize &count, const NDSize &offset) const {
    const std::vector<double> poly = polynomCoefficients();
    std::optional<double> opt_origin = expansionOrigin();

    if (poly.size() || opt_origin) {
        size_t data_esize = data_type_to_size(dtype);
        size_t nelms = check::fits_in_size_t(count.nelms(),
			"Cannot apply polynom or origin transform. Buffer needed exceeds memory.");
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
    std::string dblnk_unit = util::deblankString(unit);
    util::checkEmptyString(dblnk_unit, "unit");
    if (this->dimensionCount() == 1 && this->getDimension(1).dimensionType() == nix::DimensionType::Range) {
        nix::RangeDimension rd;
        rd = this->getDimension(1);
        if (rd.alias() && !(util::isSIUnit(dblnk_unit) || util::isCompoundSIUnit(dblnk_unit))) {
                throw InvalidUnit("Non-SI units are not allowed if the DataArray has an AliasRangeDimension. Data retrieval might not work!",
                                  "DataArray::unit(const string &unit)");
        }
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

void DataArray::label(const std::string &label) {
    util::checkEmptyString(label, "label");
    backend()->label(label);
}
