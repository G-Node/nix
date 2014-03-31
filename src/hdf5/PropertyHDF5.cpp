// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <iostream>

#include <nix/hdf5/PropertyHDF5.hpp>

using namespace std;

namespace nix {
namespace hdf5 {


PropertyHDF5::PropertyHDF5(const PropertyHDF5 &property)
    : NamedEntityHDF5(property.file(), property.group(), property.id(), property.type())
{
}


PropertyHDF5::PropertyHDF5(const File &file, const Group &group, const string &id, const string &type)
    : PropertyHDF5(file, group, id, type, util::getTime())
{
}


PropertyHDF5::PropertyHDF5(const File &file, const Group &group, const string &id, const string &type, time_t time)
    : NamedEntityHDF5(file, group, id, type, time)
{
}


boost::optional<DataType> PropertyHDF5::dataType() const {
    boost::optional<DataType> result;
    if (this->valueCount() > 0) {
        result = this->values()[0].type();
    }
    return result;
}


void PropertyHDF5::mapping(const string &mapping) {
    if(mapping.empty()) {
        throw EmptyString("mapping");
    }
    else {
        group().setAttr("mapping", mapping);
        forceUpdatedAt();
    }
}


boost::optional<string> PropertyHDF5::mapping() const {
    boost::optional<string> ret;
    string mapping;
    if(group().getAttr("mapping", mapping)) {
        ret = mapping;
    }
    return ret;
}


void PropertyHDF5::mapping(const none_t t) {
    if(group().hasAttr("mapping")) {
        group().removeAttr("mapping");
    }
    forceUpdatedAt();
}


void PropertyHDF5::unit(const string &unit) {
    if(unit.empty()) {
        throw EmptyString("unit");
    }
    else {
        group().setAttr("unit", unit);
        forceUpdatedAt();
    }
}


boost::optional<string> PropertyHDF5::unit() const {
    boost::optional<std::string> ret;
    string unit;
    if(group().getAttr("unit", unit)) {
        ret = unit;
    }
    return ret;
}


void PropertyHDF5::unit(const none_t t) {
    if(group().hasAttr("unit")) {
        group().removeAttr("unit");
    }
    forceUpdatedAt();
}


void PropertyHDF5::deleteValues() {
    group().removeData("values");
}


// TODO should this be moved to Property.hpp/cpp
/*
bool PropertyHDF5::checkDataType(const H5::DataSet &dataset, H5T_class_t destType) const {
    H5::DataType type = dataset.getDataType();
    if (type.getClass() != H5T_COMPOUND) {
        return false;
    }
    H5::CompType ct(dataset);
    if (ct.getMemberDataType(ct.getMemberIndex("value")).getClass() != destType) {
        return false;
    }
    return true;
}
*/


size_t PropertyHDF5::valueCount() const {
    size_t count = 0;
    if (group().hasData("values")) {
        DataSet dataset = group().openData("values");
        NDSize size = dataset.size();
        return size[0];
    }
    return count;
}

void PropertyHDF5::values(const std::vector<Value> &values)
{
    DataSet dataset;

    if (group().hasData("values")) {
        dataset = group().openData("values");
    } else {

        if (values.size() < 1) {
            return; //Nothing to do, since we also cannot guess the correct DataType
        }

        NDSize size = {1};
        DataType dtype = values[0].type();
        H5::DataType fileType = DataSet::fileTypeForValue(dtype);

        dataset = DataSet::create(group().h5Group(), "values", fileType, size);
    }

    dataset.write(values);
}

std::vector<Value> PropertyHDF5::values(void) const
{
    std::vector<Value> values;
    if (!group().hasData("values")) {
        return values;
    }

    DataSet dataset = group().openData("values");
    dataset.read(values);
    return values;
}


void PropertyHDF5::values(const none_t t) {
    if(group().hasData("values")) {
        group().removeData("values");
    }
    forceUpdatedAt();
}


PropertyHDF5::~PropertyHDF5() {}


} // namespace hdf5
} // namespace nix
