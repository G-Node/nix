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
    : NamedEntityHDF5(property.group, property.entity_id)
{
}


PropertyHDF5::PropertyHDF5(Group group, const std::string &id)
    : NamedEntityHDF5(group, id)
{
}


PropertyHDF5::PropertyHDF5(Group group,const std::string &id, time_t time)
    : NamedEntityHDF5(group,id,time)
{
}


void PropertyHDF5::dataType(const string &dataType) {
    string dt = this->dataType();
    if (dt.compare(dataType) == 0) {
        return;
    } else {
        if (valueCount() > 0 && this->dataType().length() > 0) {
            throw std::runtime_error("Cannot change data type of a not empty property!");
            return;
        }
    }
    group.setAttr("data_type", dataType);
}


string PropertyHDF5::dataType() const {
    string dataType;
    group.getAttr("data_type", dataType);
    return dataType;
}


void PropertyHDF5::mapping(const string &mapping) {
    group.setAttr("mapping", mapping);
}


string PropertyHDF5::mapping() const {
    string mapping;
    group.getAttr("mapping", mapping);
    return mapping;
}


void PropertyHDF5::unit(const string &unit) {
    if (valueCount() > 0 && this->unit().length() > 0) {
        throw std::runtime_error("Cannot change unit of a not-empty property!");
        return;
    }
    group.setAttr("unit", unit);
}


string PropertyHDF5::unit() const {
    string unit;
    group.getAttr("unit", unit);
    return unit;
}


void PropertyHDF5::removeValue(size_t index){
    if (group.hasData("values")) {
        if (index >= valueCount()) {
            throw std::runtime_error("Property::stringValue(index): Index out of bounds!");
        }
        //TODO
    }
}


void PropertyHDF5::removeValues(){
    this->group.removeData("values");
}


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


size_t PropertyHDF5::valueCount() const {
    size_t count = 0;
    if (group.hasData("values")) {
        DataSet dataset = group.openData("values");
        PSize size = dataset.size();
        return size[0];
    }
    return count;
}


bool PropertyHDF5::operator==(const PropertyHDF5 &other) const {
    return entity_id == other.entity_id;
}


bool PropertyHDF5::operator!=(const PropertyHDF5 &other) const {
    return entity_id != other.entity_id;
}


PropertyHDF5::~PropertyHDF5() {}


} // namespace hdf5
} // namespace nix
