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


const NDSize DataArrayHDF5::MIN_CHUNK_SIZE = {1};
const NDSize DataArrayHDF5::MAX_SIZE_1D = {H5S_UNLIMITED};


DataArrayHDF5::DataArrayHDF5(const DataArrayHDF5 &data_array)
    : EntityWithSourcesHDF5(data_array.file(), data_array.block(), data_array.group(), data_array.id()),
      dimension_group(data_array.dimension_group)
{
}


DataArrayHDF5::DataArrayHDF5(const File &file, const Block &block, const Group &group, const string &id)
    : EntityWithSourcesHDF5(file, block, group, id)
{
    dimension_group = this->group().openGroup("dimensions", true);
}


DataArrayHDF5::DataArrayHDF5(const File &file, const Block &block, const Group &group, const string &id, time_t time)
    : EntityWithSourcesHDF5(file, block, group, id, time)
{
    dimension_group = this->group().openGroup("dimensions", true);
}

//--------------------------------------------------
// Element getters and setters
//--------------------------------------------------

string DataArrayHDF5::label() const {
    string value;
    group().getAttr("label", value);
    return value;
}


void DataArrayHDF5::label(const string &label) {
    group().setAttr("label", label);
    forceUpdatedAt();
}


string DataArrayHDF5::unit() const {
    string value;
    group().getAttr("unit", value);
    return value;
}


void DataArrayHDF5::unit(const string &unit) {
    group().setAttr("unit", unit);
    forceUpdatedAt();
}


// TODO use defaults
double DataArrayHDF5::expansionOrigin() const {
    double expansion_origin;
    group().getAttr("expansion_origin", expansion_origin);
    return expansion_origin;
}


void DataArrayHDF5::expansionOrigin(double expansion_origin) {
    group().setAttr("expansion_origin", expansion_origin);
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


void DataArrayHDF5::polynomCoefficients(vector<double> &coefficients){
    DataSet ds;
    if (group().hasData("polynom_coefficients")) {
        ds = group().openData("polynom_coefficients");
        ds.setExtent({coefficients.size()});
    } else {
        ds = DataSet::create(group().h5Group(), "polynom_coefficients", coefficients, &MAX_SIZE_1D, &MIN_CHUNK_SIZE);
    }
    ds.write(coefficients);
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
            shared_ptr<SetDimensionHDF5> tmp(new SetDimensionHDF5(dim_group, id));
            dim = SetDimension(tmp);
        } else if (dim_type == DimensionType::Range) {
            shared_ptr<RangeDimensionHDF5> tmp(new RangeDimensionHDF5(dim_group, id));
            dim = RangeDimension(tmp);
        } else if (dim_type == DimensionType::Sample) {
            shared_ptr<SampledDimensionHDF5> tmp(new SampledDimensionHDF5(dim_group, id));
            dim = SampledDimension(tmp);
        } else {
            throw runtime_error("Invalid dimension type");
        }

        return dim;
    } else {
        throw runtime_error("No such dimension");
    }
}


Dimension DataArrayHDF5::createDimension(size_t id, DimensionType type) {
    size_t dim_count = dimensionCount();

    if (id <= dim_count) {
        id = dim_count + 1;
    }

    string str_id = util::numToStr(id);

    if (dimension_group.hasGroup(str_id)) {
        dimension_group.removeGroup(str_id);
    }

    Group dim_group = dimension_group.openGroup(str_id, true);
    Dimension dim;

    if (type == DimensionType::Set ) {
        shared_ptr<SetDimensionHDF5> tmp(new SetDimensionHDF5(dim_group, id));
        dim = SetDimension(tmp);
    } else if (type == DimensionType::Range) {
        shared_ptr<RangeDimensionHDF5> tmp(new RangeDimensionHDF5(dim_group, id));
        dim = RangeDimension(tmp);
    } else if (type == DimensionType::Sample) {
        shared_ptr<SampledDimensionHDF5> tmp(new SampledDimensionHDF5(dim_group, id));
        dim = SampledDimension(tmp);
    } else {
        throw runtime_error("Invalid dimension type");
    }

    return dim;
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


DataArrayHDF5::~DataArrayHDF5(){}


void DataArrayHDF5::write(DataType dtype, const void *data, const NDSize &count, const NDSize &offset)
{
    DataSet ds;

    if (!group().hasData("data")) {
        NDSize maxsize(count.size(), H5S_UNLIMITED);
        NDSize chunks(count.size(), 1); //FIXME
        ds = DataSet::create(group().h5Group(), "data", dtype, count, &maxsize, &chunks);
    } else {
        ds = group().openData("data");
    }

    if (offset.size()) {
        Selection fileSel = ds.createSelection();
        fileSel.select(count, offset);
        Selection memSel(DataSpace::create(count, nullptr));

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
        Selection memSel(DataSpace::create(count, nullptr));

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
        return; //FIXME throw exception?
    }

    DataSet ds = group().openData("data");
    ds.setExtent(extent);
}

} // namespace hdf5
} // namespace nix
