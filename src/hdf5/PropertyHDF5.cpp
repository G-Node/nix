// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <iostream>
#include <stdexcept>

#include <nix/hdf5/PropertyHDF5.hpp>

using namespace std;

namespace nix {
namespace hdf5 {


PropertyHDF5::PropertyHDF5(const PropertyHDF5 &property)
    : NamedEntityHDF5(property.file(), property.group(), property.id())
{
}


PropertyHDF5::PropertyHDF5(const File &file, const Group &group, const string &id)
    : NamedEntityHDF5(file, group, id)
{
}


PropertyHDF5::PropertyHDF5(const File &file, const Group &group, const string &id, time_t time)
    : NamedEntityHDF5(file, group, id, time)
{
}


void PropertyHDF5::dataType(const string &dataType) {
    string dt = this->dataType();
    if (dt.compare(dataType) == 0) {
        return;
    } else {
        if (valueCount() > 0 && this->dataType().length() > 0) {
            throw runtime_error("Cannot change data type of a not empty property!");
            return;
        }
    }
    group().setAttr("data_type", dataType);
}


string PropertyHDF5::dataType() const {
    string dataType;
    group().getAttr("data_type", dataType);
    return dataType;
}


void PropertyHDF5::mapping(const string &mapping) {
    group().setAttr("mapping", mapping);
}


string PropertyHDF5::mapping() const {
    string mapping;
    group().getAttr("mapping", mapping);
    return mapping;
}


void PropertyHDF5::unit(const string &unit) {
    if (valueCount() > 0 && this->unit().length() > 0) {
        throw runtime_error("Cannot change unit of a not-empty property!");
        return;
    }
    group().setAttr("unit", unit);
}


string PropertyHDF5::unit() const {
    string unit;
    group().getAttr("unit", unit);
    return unit;
}


void PropertyHDF5::removeValue(size_t index){
    if (group().hasData("values")) {
        if (index >= valueCount()) {
            throw runtime_error("Property::stringValue(index): Index out of bounds!");
        }
    }
}


void PropertyHDF5::removeValues(){
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


PropertyHDF5::~PropertyHDF5() {}


} // namespace hdf5
} // namespace nix
