// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/util/util.hpp>

#include <nix/hdf5/DataArrayHDF5.hpp>
#include <nix/hdf5/DataSet.hpp>
#include <nix/hdf5/DimensionHDF5.hpp>

using namespace std;
using namespace nix;
using namespace nix::base;
using namespace nix::hdf5;

// TODO unnecessary IO (see #316)
DataArrayHDF5::DataArrayHDF5(const DataArrayHDF5 &data_array)
    : EntityWithSourcesHDF5(data_array.file(), data_array.block(), data_array.group(), data_array.id(), data_array.type(), data_array.name()),
      dimension_group(data_array.dimension_group)
{
}


DataArrayHDF5::DataArrayHDF5(shared_ptr<IFile> file, shared_ptr<IBlock> block, const Group &group,
                             const string &id, const string &type, const string &name)
    : DataArrayHDF5(file, block, group, id, type, name, util::getTime())
{
}


DataArrayHDF5::DataArrayHDF5(shared_ptr<IFile> file, shared_ptr<IBlock> block, const Group &group,
                             const string &id, const string &type, const string &name, time_t time)
    : EntityWithSourcesHDF5(file, block, group, id, type, name, time)
{
    dimension_group = this->group().openGroup("dimensions", true);
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
    if (label.empty()) {
        throw EmptyString("label");
    }
    else {
        group().setAttr("label", label);
        forceUpdatedAt();
    }
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
    if (unit.empty()) {
        throw EmptyString("unit");
    }
    else {
        group().setAttr("unit", unit);
        forceUpdatedAt();
    }
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
vector<double> DataArrayHDF5::polynomCoefficients()const{
    vector<double> polynom_coefficients;

    if (group().hasData("polynom_coefficients")) {
        DataSet ds = group().openData("polynom_coefficients");
        ds.read(polynom_coefficients, true);
    }

    return polynom_coefficients;
}


void DataArrayHDF5::polynomCoefficients(const vector<double> &coefficients) {
    DataSet ds;
    if (group().hasData("polynom_coefficients")) {
        ds = group().openData("polynom_coefficients");
        ds.setExtent({coefficients.size()});
    } else {
        ds = DataSet::create(group().h5Group(), "polynom_coefficients", coefficients);
    }
    ds.write(coefficients);
    forceUpdatedAt();
}


void DataArrayHDF5::polynomCoefficients(const none_t t) {
    if (group().hasAttr("polynom_coefficients")) {
        group().removeAttr("polynom_coefficients");
    }
    forceUpdatedAt();
}

//--------------------------------------------------
// Methods concerning dimensions
//--------------------------------------------------


size_t DataArrayHDF5::dimensionCount() const {
    return dimension_group.objectCount();
}


shared_ptr<IDimension> DataArrayHDF5::getDimension(size_t index) const {
    shared_ptr<IDimension> dim;

    string str_id = util::numToStr(index);
    if (dimension_group.hasGroup(str_id)) {
        Group g = dimension_group.openGroup(str_id, false);
        dim = openDimensionHDF5(g, index);
    }

    return dim;
}


std::shared_ptr<base::ISetDimension> DataArrayHDF5::createSetDimension(size_t index) {
    Group g = createDimensionGroup(index);
    return make_shared<SetDimensionHDF5>(g, index);
}


std::shared_ptr<base::IRangeDimension> DataArrayHDF5::createRangeDimension(size_t index, std::vector<double> ticks) {
    Group g = createDimensionGroup(index);
    return make_shared<RangeDimensionHDF5>(g, index, ticks);
}


std::shared_ptr<base::ISampledDimension> DataArrayHDF5::createSampledDimension(size_t index, double sampling_interval) {
    Group g = createDimensionGroup(index);
    return make_shared<SampledDimensionHDF5>(g, index, sampling_interval);
}


Group DataArrayHDF5::createDimensionGroup(size_t index) {
    size_t dim_max   = dimensionCount() + 1;
    if (index > dim_max || index <= 0)
        throw new runtime_error("Invalid dimension index: has to be 0 < index <= " + util::numToStr(dim_max));

    string str_id = util::numToStr(index);
    if (dimension_group.hasGroup(str_id)) {
        dimension_group.removeGroup(str_id);
    }

    return dimension_group.openGroup(str_id, true);
}


bool DataArrayHDF5::deleteDimension(size_t index) {
    bool deleted = false;
    size_t dim_count = dimensionCount();
    string str_id = util::numToStr(index);

    if (dimension_group.hasGroup(str_id)) {
        dimension_group.removeGroup(str_id);
        deleted = true;
    }

    if (deleted && index < dim_count) {
        for (size_t old_id = index + 1; old_id <= dim_count; old_id++) {
            string str_old_id = util::numToStr(old_id);
            string str_new_id = util::numToStr(old_id - 1);
            dimension_group.renameGroup(str_old_id, str_new_id);
        }
    }

    return deleted;
}

//--------------------------------------------------
// Other methods and functions
//--------------------------------------------------


void DataArrayHDF5::swap(DataArrayHDF5 &other) {
    using std::swap;

    EntityWithSourcesHDF5::swap(other);
    swap(dimension_group, other.dimension_group);
}


DataArrayHDF5& DataArrayHDF5::operator=(const DataArrayHDF5 &other) {
    if (*this != other) {
        DataArrayHDF5 tmp(other);
        swap(tmp);
    }

    return *this;
}


DataArrayHDF5::~DataArrayHDF5() {}


void DataArrayHDF5::createData(DataType dtype, const NDSize &size)
{
    if (group().hasData("data")) {
        throw new std::runtime_error("DataArray alread exists"); //TODO: FIXME, better exception
    }

    DataSet::create(group().h5Group(), "data", dtype, size);
    DataSet ds = group().openData("data");
}

bool DataArrayHDF5::hasData() const
{
    return group().hasData("data");
}

void DataArrayHDF5::write(DataType dtype, const void *data, const NDSize &count, const NDSize &offset)
{
    DataSet ds;

    if (!group().hasData("data")) {
        ds = DataSet::create(group().h5Group(), "data", dtype, count);
    } else {
        ds = group().openData("data");
    }

    if (offset.size()) {
        Selection fileSel = ds.createSelection();
        fileSel.select(count, offset);
        Selection memSel(DataSpace::create(count, false));

        ds.write(dtype, data, fileSel, memSel);
    } else {
        ds.write(dtype, count, data);
    }
}

void DataArrayHDF5::read(DataType dtype, void *data, const NDSize &count, const NDSize &offset) const
{
    if (!group().hasData("data")) {
        return;
    }

    DataSet ds = group().openData("data");

    if (offset.size()) {
        Selection fileSel = ds.createSelection();
        fileSel.select(count, offset);
        Selection memSel(DataSpace::create(count, false));

        ds.read(dtype, data, fileSel, memSel);
    } else {
        ds.read(dtype, count, data);
    }

}

NDSize DataArrayHDF5::dataExtent(void) const
{
    if (!group().hasData("data")) {
        return NDSize{};
    }

    DataSet ds = group().openData("data");
    return ds.size();
}

void DataArrayHDF5::dataExtent(const NDSize &extent)
{
    if (!group().hasData("data")) {
        throw runtime_error("Data field not found in DataArray!");
    }

    DataSet ds = group().openData("data");
    ds.setExtent(extent);
}

DataType DataArrayHDF5::dataType(void) const
{
    if (!group().hasData("data")) {
        return DataType::Nothing;
    }

    DataSet ds = group().openData("data");
    return ds.dataType();
}

