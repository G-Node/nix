// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/util/util.hpp>

#include "DataArrayHDF5.hpp"
#include "h5x/H5DataSet.hpp"
#include "DimensionHDF5.hpp"

using namespace std;
using namespace nix::base;

namespace nix {
namespace hdf5 {


DataArrayHDF5::DataArrayHDF5(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const H5Group &group)
        : EntityWithSourcesHDF5(file, block, group) {
    dimension_group = this->group().openOptGroup("dimensions");
}


DataArrayHDF5::DataArrayHDF5(const shared_ptr<IFile> &file, const shared_ptr<IBlock> &block, const H5Group &group,
                             const string &id, const string &type, const string &name)
        : DataArrayHDF5(file, block, group, id, type, name, util::getTime()) {
}


DataArrayHDF5::DataArrayHDF5(const shared_ptr<IFile> &file, const shared_ptr<IBlock> &block, const H5Group &group,
                             const string &id, const string &type, const string &name, time_t time)
        : EntityWithSourcesHDF5(file, block, group, id, type, name, time) {
    dimension_group = this->group().openOptGroup("dimensions");
}

//--------------------------------------------------
// Element getters and setters
//--------------------------------------------------

boost::optional<std::string> DataArrayHDF5::label() const {
    boost::optional<std::string> ret;
    string value;
    bool have_attr = group().getAttr("label", value);

    if (have_attr) {
        ret = value;
    }

    return ret;
}


void DataArrayHDF5::label(const string &label) {
    group().setAttr("label", label);
    forceUpdatedAt();
}


void DataArrayHDF5::label(const none_t t) {
    if (group().hasAttr("label")) {
        group().removeAttr("label");
    }
    forceUpdatedAt();
}


boost::optional<std::string> DataArrayHDF5::unit() const {
    boost::optional<std::string> ret;
    string value;
    bool have_attr = group().getAttr("unit", value);
    if (have_attr) {
        ret = value;
    }
    return ret;
}


void DataArrayHDF5::unit(const string &unit) {
    group().setAttr("unit", unit);
    forceUpdatedAt();
}


void DataArrayHDF5::unit(const none_t t) {
    if (group().hasAttr("unit")) {
        group().removeAttr("unit");
    }
    forceUpdatedAt();
}


// TODO use defaults
boost::optional<double> DataArrayHDF5::expansionOrigin() const {
    boost::optional<double> ret;
    double expansion_origin;
    bool have_attr = group().getAttr("expansion_origin", expansion_origin);
    if (have_attr) {
        ret = expansion_origin;
    }
    return ret;
}


void DataArrayHDF5::expansionOrigin(double expansion_origin) {
    group().setAttr("expansion_origin", expansion_origin);
    forceUpdatedAt();
}


void DataArrayHDF5::expansionOrigin(const none_t t) {
    if (group().hasAttr("expansion_origin")) {
        group().removeAttr("expansion_origin");
    }
    forceUpdatedAt();
}

// TODO use defaults
vector<double> DataArrayHDF5::polynomCoefficients() const {
    vector<double> polynom_coefficients;

    if (group().hasData("polynom_coefficients")) {
        DataSet ds = group().openData("polynom_coefficients");
        ds.read(polynom_coefficients, true);
    }

    return polynom_coefficients;
}


void DataArrayHDF5::polynomCoefficients(const vector<double> &coefficients, const Compression &compression) {
    DataSet ds;
    if (group().hasData("polynom_coefficients")) {
        ds = group().openData("polynom_coefficients");
        ds.setExtent({coefficients.size()});
    } else {
        ds = group().createData("polynom_coefficients", H5T_NATIVE_DOUBLE, {coefficients.size()}, compression);
    }
    ds.write(coefficients);
    forceUpdatedAt();
}


void DataArrayHDF5::polynomCoefficients(const none_t t) {
    if (group().hasData("polynom_coefficients")) {
        group().removeData("polynom_coefficients");
    }
    forceUpdatedAt();
}

//--------------------------------------------------
// Methods concerning dimensions
//--------------------------------------------------


ndsize_t DataArrayHDF5::dimensionCount() const {
    boost::optional<H5Group> g = dimension_group();
	ndsize_t count = 0;
	if (g) {
		count = g->objectCount();
	}
    return count;
}


shared_ptr<IDimension> DataArrayHDF5::getDimension(ndsize_t index) const {
    shared_ptr<IDimension> dim;
    boost::optional<H5Group> g = dimension_group();

    if (g) {
        string str_id = util::numToStr(index);
        if (g->hasGroup(str_id)) {
            H5Group group = g->openGroup(str_id, false);
            dim = openDimensionHDF5(group, index);
        }
    }

    return dim;
}


std::shared_ptr<base::ISetDimension> DataArrayHDF5::createSetDimension(ndsize_t index) {
    H5Group g = createDimensionGroup(index);
    return make_shared<SetDimensionHDF5>(g, index);
}


std::shared_ptr<base::IRangeDimension> DataArrayHDF5::createRangeDimension(ndsize_t index, const std::vector<double> &ticks) {
    H5Group g = createDimensionGroup(index);
    return make_shared<RangeDimensionHDF5>(g, index, ticks);
}


std::shared_ptr<base::IRangeDimension> DataArrayHDF5::createAliasRangeDimension() {
    H5Group g = createDimensionGroup(1);
    return make_shared<RangeDimensionHDF5>(g, 1, *this);
}


std::shared_ptr<base::ISampledDimension> DataArrayHDF5::createSampledDimension(ndsize_t index, double sampling_interval) {
    H5Group g = createDimensionGroup(index);
    return make_shared<SampledDimensionHDF5>(g, index, sampling_interval);
}


std::shared_ptr<base::IColumnDimension> DataArrayHDF5::createColumnDimension(ndsize_t index, const nix::DataFrame &df, unsigned col_index) {
    H5Group g = createDimensionGroup(index);
    return make_shared<ColumnDimensionHDF5>(g, index, file(), block(), df, col_index);
}

H5Group DataArrayHDF5::createDimensionGroup(ndsize_t index) {
    boost::optional<H5Group> g = dimension_group(true);

    ndsize_t dim_max = dimensionCount() + 1;
    if (index > dim_max || index <= 0)
        throw runtime_error("Invalid dimension index: has to be 0 < index <= " + util::numToStr(dim_max));

    string str_id = util::numToStr(index);
    if (g->hasGroup(str_id)) {
        g->removeGroup(str_id);
    }

    return g->openGroup(str_id, true);
}


bool DataArrayHDF5::deleteDimensions() {
    string dim_id;
    boost::optional<H5Group> g = dimension_group();
    for (ndsize_t i = dimensionCount(); i > 0; --i) {
        dim_id = util::numToStr(i);
        if (g->hasGroup(dim_id)) {
            g->removeGroup(dim_id);
        }
    }
    return true;
}


//--------------------------------------------------
// Other methods and functions
//--------------------------------------------------


DataArrayHDF5::~DataArrayHDF5() {
}

void DataArrayHDF5::createData(DataType dtype, const NDSize &size, const Compression &compression) {
    if (group().hasData("data")) {
        throw ConsistencyError("DataArray's hdf5 data group already exists!");
    }

    h5x::DataType fileType = data_type_to_h5_filetype(dtype);
    group().createData("data", fileType, size, compression);
}

bool DataArrayHDF5::hasData() const {
    return group().hasData("data");
}

void DataArrayHDF5::write(DataType dtype, const void *data, const NDSize &count, const NDSize &offset) {

    if (!group().hasData("data")) {
        throw ConsistencyError("DataArray with missing h5df DataSet");
    }

    DataSet ds = group().openData("data");
    h5x::DataType memType = data_type_to_h5_memtype(dtype);

    DataSpace fileSpace, memSpace;
    std::tie(memSpace, fileSpace) = ds.offsetCount2DataSpaces(count, offset);

    if (dtype == DataType::String) {
        StringReader reader(count, data);
        ds.write(*reader, memType, memSpace, fileSpace);
    } else {
        ds.write(data, memType, memSpace, fileSpace);
    }
}

void DataArrayHDF5::read(DataType dtype, void *data, const NDSize &count, const NDSize &offset) const {
    if (!group().hasData("data")) {
        throw ConsistencyError("DataArray with missing h5df DataSet");
    }

    DataSet ds = group().openData("data");
    h5x::DataType memType = data_type_to_h5_memtype(dtype);
    DataSpace fileSpace, memSpace;
    std::tie(memSpace, fileSpace) = ds.offsetCount2DataSpaces(count, offset);

    if (dtype == DataType::String) {
        StringWriter writer(count, data);
        ds.read(*writer, memType, memSpace, fileSpace);
        writer.finish();
        ds.vlenReclaim(memType.h5id(), *writer, &memSpace);
    } else {
        ds.read(data, memType, memSpace, fileSpace);
    }
}

NDSize DataArrayHDF5::dataExtent(void) const {
    if (!group().hasData("data")) {
        return NDSize{};
    }

    DataSet ds = group().openData("data");
    return ds.size();
}

void DataArrayHDF5::dataExtent(const NDSize &extent) {
    if (!group().hasData("data")) {
        throw runtime_error("Data field not found in DataArray!");
    }

    DataSet ds = group().openData("data");
    ds.setExtent(extent);
}

DataType DataArrayHDF5::dataType(void) const {
    if (!group().hasData("data")) {
        return DataType::Nothing;
    }

    DataSet ds = group().openData("data");
    const h5x::DataType dtype = ds.dataType();
    return data_type_from_h5(dtype);
}

} // ns nix::hdf5
} // ns nix
