// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/hdf5/DataArrayHDF5.hpp>

using namespace std;

namespace nix {
namespace hdf5 {


const PSize DataArrayHDF5::MIN_CHUNK_SIZE = {1};
const PSize DataArrayHDF5::MAX_SIZE_1D = {H5S_UNLIMITED};


DataArrayHDF5::DataArray(const DataArray &data_array)
    : EntityWithSources(data_array.file, data_array.block, data_array.group, data_array.entity_id)
{
    dimension_group = data_array.dimension_group;
    vector<double> pcs = data_array.polynomCoefficients();
    polynomCoefficients(pcs);
    expansionOrigin(data_array.expansionOrigin());
}


DataArrayHDF5::DataArray(File file, const Block block, Group group, const string &id)
    : EntityWithSources(file, block, group, id)
{
    dimension_group = group.openGroup("dimensions");

    if (!group.hasAttr("polynom_coefficients")) {
        vector<double> pc = {0,1};
        polynomCoefficients(pc);
    }

    if (!group.hasAttr("expansion_origin")) {
        expansionOrigin(0);
    }
}


DataArrayHDF5::DataArray(File file, const Block block, Group group, const string &id, time_t time)
    : EntityWithSources(file, block, group, id, time)
{
    dimension_group = group.openGroup("dimensions");

    if (!group.hasAttr("polynom_coefficients")) {
        vector<double> pc = {0,1};
        polynomCoefficients(pc);
    }

    if (!group.hasAttr("expansion_origin")) {
        expansionOrigin(0);
    }
}

//--------------------------------------------------
// Element getters and setters
//--------------------------------------------------

string DataArrayHDF5::label() const {
    string value;
    group.getAttr("label", value);
    return value;
}


void DataArrayHDF5::label(const string &label) {
    group.setAttr("label", label);
    forceUpdatedAt();
}


string DataArrayHDF5::unit() const {
    string value;
    group.getAttr("unit", value);
    return value;
}


void DataArrayHDF5::unit(const string &unit) {
    group.setAttr("unit", unit);
    forceUpdatedAt();
}


double DataArrayHDF5::expansionOrigin() const {
    double expansion_origin;
    group.getAttr("expansion_origin", expansion_origin);
    return expansion_origin;
}


void DataArrayHDF5::expansionOrigin(double expansion_origin) {
    group.setAttr("expansion_origin", expansion_origin);
    forceUpdatedAt();
}


vector<double> DataArrayHDF5::polynomCoefficients()const{
    vector<double> polynom_coefficients;

    if (group.hasData("polynom_coefficients")) {
        DataSet ds = group.openData("polynom_coefficients");
        ds.read(polynom_coefficients, true);
    }

    return polynom_coefficients;
}


void DataArrayHDF5::polynomCoefficients(vector<double> &coefficients){
    if (group.hasData("polynom_coefficients")) {
        DataSet ds = group.openData("polynom_coefficients");
        ds.extend({coefficients.size()});
        ds.write(coefficients);
    } else {
        DataSet ds = DataSet::create(group.h5Group(), "polynom_coefficients", coefficients, &MAX_SIZE_1D, &MIN_CHUNK_SIZE);
        ds.write(coefficients);
    }
    forceUpdatedAt();
}

//--------------------------------------------------
// Methods concerning dimensions
//--------------------------------------------------

vector<shared_ptr<Dimension>> DataArrayHDF5::dimensions() const {
                              vector<shared_ptr<Dimension>> dimensions;

                              size_t dim_count = dimensionCount();
                              for (size_t i = 0; i < dim_count; i++) {
    size_t dim_id = i + 1;
    string str_id = util::numToStr(dim_id);

    if (dimension_group.hasGroup(str_id)) {
        Group dim_group = dimension_group.openGroup(str_id, false);
        string dim_type_name;
        dim_group.getAttr("dimension_type", dim_type_name);
        DimensionType dim_type = dimensionTypeFromStr(dim_type_name);

        shared_ptr<Dimension> dim;
        switch (dim_type) {
            case DimensionType::SET_DIMENSION:
                dim = shared_ptr<Dimension>(new SetDimension(dim_group, dim_id));
                break;
            case DimensionType::RANGE_DIMENSION:
                dim = shared_ptr<Dimension>(new RangeDimension(dim_group, dim_id));
                break;
            case DimensionType::SAMPLED_DIMENSION:
                dim = shared_ptr<Dimension>(new SampledDimension(dim_group, dim_id));
                break;
            default:
                throw runtime_error("Invalid dimension type");
        }

        dimensions.push_back(dim);
    }
}

return dimensions;
}


size_t DataArrayHDF5::dimensionCount() const {
    return dimension_group.objectCount();
}


shared_ptr<Dimension> DataArrayHDF5::getDimension(size_t id) const {
    string str_id = util::numToStr(id);
    shared_ptr<Dimension> dim;

    if (dimension_group.hasGroup(str_id)) {
        Group dim_group = dimension_group.openGroup(str_id, false);
        string dim_type_name;
        dim_group.getAttr("dimension_type", dim_type_name);
        DimensionType dim_type = dimensionTypeFromStr(dim_type_name);

        switch (dim_type) {
            case DimensionType::SET_DIMENSION:
                dim = shared_ptr<Dimension>(new SetDimension(dim_group, id));
                break;
            case DimensionType::RANGE_DIMENSION:
                dim = shared_ptr<Dimension>(new RangeDimension(dim_group, id));
                break;
            case DimensionType::SAMPLED_DIMENSION:
                dim = shared_ptr<Dimension>(new SampledDimension(dim_group, id));
                break;
            default:
                throw runtime_error("Invalid dimension type");
        }
    }

    return dim;
}


shared_ptr<Dimension> DataArrayHDF5::createDimension(size_t id, DimensionType type) {
    size_t dim_count = dimensionCount();

    if (id <= dim_count) {
        id = dim_count + 1;
    }

    string str_id = util::numToStr(id);

    if (dimension_group.hasGroup(str_id)) {
        dimension_group.removeGroup(str_id);
    }

    Group dim_group = dimension_group.openGroup(str_id, true);
    shared_ptr<Dimension> dim;

    switch (type) {
        case DimensionType::SET_DIMENSION:
            dim = shared_ptr<Dimension>(new SetDimension(dim_group, id));
            break;
        case DimensionType::RANGE_DIMENSION:
            dim = shared_ptr<Dimension>(new RangeDimension(dim_group, id));
            break;
        case DimensionType::SAMPLED_DIMENSION:
            dim = shared_ptr<Dimension>(new SampledDimension(dim_group, id));
            break;
        default:
            throw runtime_error("Invalid dimension type");
    }

    return dim;
}


bool DataArrayHDF5::removeDimension(size_t id) {
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

double DataArrayHDF5::applyPolynomial(std::vector<double> &coefficients, double origin, double input) const{
    double value = 0.0;
    double term = 1.0;
    for(size_t i = 0; i < coefficients.size(); i++){
        value += coefficients[i] * term;
        term *= input - origin;
    }
    return value;
}


DataArrayHDF5& DataArrayHDF5::operator=(const DataArray &other) {
    if (*this != other) {
        this->file = other.file;
        this->block = other.block;
        this->group = other.group;
        this->entity_id = other.entity_id;
        this->dimension_group = other.dimension_group;
        this->sources_refs = other.sources_refs;
    }
    return *this;
}


ostream& operator<<(ostream &out, const DataArrayHDF5 &ent) {
    out << "DataArray: {name = " << ent.name();
    out << ", type = " << ent.type();
    out << ", id = " << ent.id() << "}";
    return out;
}


DataArrayHDF5::~DataArrayHDF5(){}


} // namespace hdf5
} // namespace nix
