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

namespace nix {
namespace hdf5 {

DataArrayHDF5::DataArrayHDF5(const DataArrayHDF5 &data_array)
    : EntityWithSourcesHDF5(data_array.file(), data_array.block(), data_array.group(), data_array.id(), data_array.type(), data_array.name()),
      dimension_group(data_array.dimension_group)
{
}


DataArrayHDF5::DataArrayHDF5(const File &file, const Block &block, const Group &group, 
                             const string &id, const string &type, const string &name)
    : DataArrayHDF5(file, block, group, id, type, name, util::getTime())
{
}


DataArrayHDF5::DataArrayHDF5(const File &file, const Block &block, const Group &group, 
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
    group().getAttr("label", value);
    ret = value;
    return ret;
}


void DataArrayHDF5::label(const string &label) {
    if(label.empty()) {
        throw EmptyString("label");
    }
    else {
        group().setAttr("label", label);
        forceUpdatedAt();
    }
}


void DataArrayHDF5::label(const none_t t) {
    if(group().hasAttr("label")) {
        group().removeAttr("label");
    }
    forceUpdatedAt();
}


boost::optional<std::string> DataArrayHDF5::unit() const {
    boost::optional<std::string> ret;
    string value;
    group().getAttr("unit", value);
    ret = value;
    return ret;
}


void DataArrayHDF5::unit(const string &unit) {
    if(unit.empty()) {
        throw EmptyString("unit");
    }
    else {
        group().setAttr("unit", unit);
        forceUpdatedAt();
    }
}


void DataArrayHDF5::unit(const none_t t) {
    if(group().hasAttr("unit")) {
        group().removeAttr("unit");
    }
    forceUpdatedAt();
}


// TODO use defaults
boost::optional<double> DataArrayHDF5::expansionOrigin() const {
    boost::optional<double> ret;
    double expansion_origin;
    group().getAttr("expansion_origin", expansion_origin);
    ret = expansion_origin;
    return ret;
}


void DataArrayHDF5::expansionOrigin(double expansion_origin) {
    group().setAttr("expansion_origin", expansion_origin);
    forceUpdatedAt();
}


void DataArrayHDF5::expansionOrigin(const none_t t) {
    if(group().hasAttr("expansionOrigin")) {
        group().removeAttr("expansionOrigin");
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


void DataArrayHDF5::polynomCoefficients(vector<double> &coefficients) {
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
    if(group().hasAttr("polynom_coefficients")) {
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


Dimension DataArrayHDF5::getDimension(size_t id) const {
    string str_id = util::numToStr(id);

    if (dimension_group.hasGroup(str_id)) {
        Group dim_group = dimension_group.openGroup(str_id, false);
        string dim_type_name;
        dim_group.getAttr("dimension_type", dim_type_name);
        DimensionType dim_type = dimensionTypeFromStr(dim_type_name);

        Dimension dim;

        if (dim_type == DimensionType::Set ) {
            auto tmp = make_shared<SetDimensionHDF5>(dim_group, id);
            dim = SetDimension(tmp);
        } else if (dim_type == DimensionType::Range) {
            std::vector<double> ticks;
            dim_group.getData("ticks", ticks);
            auto tmp = make_shared<RangeDimensionHDF5>(dim_group, id, ticks);
            dim = RangeDimension(tmp);
        } else if (dim_type == DimensionType::Sample) {
            double samplingInterval;
            dim_group.getAttr("sampling_interval", samplingInterval);
            auto tmp = make_shared<SampledDimensionHDF5>(dim_group, id, samplingInterval);
            dim = SampledDimension(tmp);
        } else {
            throw runtime_error("Invalid dimension type");
        }

        return dim;
    } else {
        return Dimension();
    }
}


template<DimensionType dtype, typename T>
Dimension DataArrayHDF5::_createDimension(size_t id, T var) {
    size_t dim_count = dimensionCount();

    if (id > (dim_count + 1) || id <= 0) { // dim_count+1 since index starts at 1
        runtime_error("Invalid dimension id: has to be 0 < id <= #(dimensions)+1");
    }

    string str_id = util::numToStr(id);

    if (dimension_group.hasGroup(str_id)) {
        dimension_group.removeGroup(str_id);
    }

    Group dim_group = dimension_group.openGroup(str_id, true);
    Dimension dim;

    if (dtype == DimensionType::Range || dtype == DimensionType::Sample) {
        typedef typename std::conditional<dtype == DimensionType::Range, 
                                          RangeDimensionHDF5, 
                                          SampledDimensionHDF5>::type 
                                          dimTypeHDF5;
        typedef typename std::conditional<dtype == DimensionType::Range, 
                                          RangeDimension, 
                                          SampledDimension>::type 
                                          dimType;
        auto tmp = make_shared<dimTypeHDF5>(dim_group, id, var);
        dim = dimType(tmp);
    } else {
        throw runtime_error("Invalid dimension type");
    }

    return dim;
}


template<>
Dimension DataArrayHDF5::_createDimension<DimensionType::Set, none_t>(size_t id, none_t var) {
    size_t dim_count = dimensionCount();

    if (id > (dim_count + 1) || id <= 0) { // dim_count+1 since index starts at 1
        runtime_error("Invalid dimension id: has to be 0 < id <= #(dimensions)+1");
    }

    string str_id = util::numToStr(id);

    if (dimension_group.hasGroup(str_id)) {
        dimension_group.removeGroup(str_id);
    }

    Group dim_group = dimension_group.openGroup(str_id, true);
    Dimension dim;

    // no dim-type check needed since method only called if type=DimensionType::Set
    auto tmp = make_shared<SetDimensionHDF5>(dim_group, id);
    dim = SetDimension(tmp);

    return dim;
}


Dimension DataArrayHDF5::createSetDimension(size_t id) {
    return _createDimension<DimensionType::Set>(id);
}


Dimension DataArrayHDF5::createRangeDimension(size_t id, std::vector<double> ticks) {
    return _createDimension<DimensionType::Range>(id, ticks);
}


Dimension DataArrayHDF5::createSampledDimension(size_t id, double samplingInterval) {
    return _createDimension<DimensionType::Sample>(id, samplingInterval);
}


bool DataArrayHDF5::deleteDimension(size_t id) {
    bool deleted = false;
    size_t dim_count = dimensionCount();
    string str_id = util::numToStr(id);

    if (dimension_group.hasGroup(str_id)) {
        dimension_group.removeGroup(str_id);
        deleted = true;
    }

    if (deleted && id < dim_count) {
        for (size_t old_id = id + 1; old_id <= dim_count; old_id++) {
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

NDSize DataArrayHDF5::getExtent(void) const
{
    if (!group().hasData("data")) {
        return NDSize{};
    }

    DataSet ds = group().openData("data");
    return ds.size();
}

void DataArrayHDF5::setExtent(const NDSize &extent)
{
    if (!group().hasData("data")) {
        throw runtime_error("Data field not found in DataArray!");
    }

    DataSet ds = group().openData("data");
    ds.setExtent(extent);
}

DataType DataArrayHDF5::getDataType(void) const
{
    if (!group().hasData("data")) {
        //we could also throw an exception but I think returning
        //Nothing here is better (ck) - agreed (bm)
        return DataType::Nothing;
    }

    DataSet ds = group().openData("data");
    return ds.dataType();
}

} // namespace hdf5
} // namespace nix
