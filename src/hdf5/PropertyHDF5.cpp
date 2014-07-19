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
using namespace boost;

namespace nix {
namespace hdf5 {


PropertyHDF5::PropertyHDF5(const PropertyHDF5 &property)
    : PropertyHDF5(property.file(), property.group(), property.dataset(), property.id(), property.name())
{
}


    PropertyHDF5::PropertyHDF5(const File &file, const Group &group, const DataSet &dataset, const string &id, const string &name)
        : PropertyHDF5(file, group, dataset, id, name, util::getTime())
{
}


    PropertyHDF5::PropertyHDF5(const File &file, const Group &group, const DataSet &dataset, const string &id, const string &name, time_t time)
    : EntityHDF5(file, group, id, time)
{
    this->entity_dataset = dataset;
    this->name(name);
}


void PropertyHDF5::name(const string &name) {
    if(name.empty()) {
        throw EmptyString("name");
    }
    else {
        dataset().setAttr("name", name);
        forceUpdatedAt();
    }
}


string PropertyHDF5::name() const {
    string name;
    if(dataset().hasAttr("name")) {
        dataset().getAttr("name", name);
        return name;
    } else {
        throw MissingAttr("name");
    }
}


void PropertyHDF5::definition(const string &definition) {
    if(definition.empty()) {
        throw EmptyString("definition");
    }
    else {
        dataset().setAttr("definition", definition);
        forceUpdatedAt();
    }
}


optional<string> PropertyHDF5::definition() const {
    optional<string> ret;
    string definition;
    bool have_attr = dataset().getAttr("definition", definition);
    if (have_attr) {
        ret = definition;
    }
    return ret;
}


void PropertyHDF5::definition(const none_t t) {
    if(dataset().hasAttr("definition")) {
        dataset().removeAttr("definition");
    }
    forceUpdatedAt();
}


boost::optional<DataType> PropertyHDF5::dataType() const {
    boost::optional<DataType> result;
    if (this->valueCount() > 0) {
        result = this->values()[0].type();
    }
    return result;
}


void PropertyHDF5::mapping(const string &mapping) {
    if (mapping.empty()) {
        throw EmptyString("mapping");
    }
    dataset().setAttr("mapping", mapping);
    forceUpdatedAt();
}


boost::optional<string> PropertyHDF5::mapping() const {
    boost::optional<string> ret;
    string mapping;
    if (dataset().getAttr("mapping", mapping)) {
        ret = mapping;
    }
    return ret;
}


void PropertyHDF5::mapping(const none_t t) {
    if (dataset().hasAttr("mapping")) {
        dataset().removeAttr("mapping");
        forceUpdatedAt();
    }
}


void PropertyHDF5::unit(const string &unit) {
    if (unit.empty()) {
        throw EmptyString("unit");
    }
    dataset().setAttr("unit", unit);
    forceUpdatedAt();
}


boost::optional<string> PropertyHDF5::unit() const {
    boost::optional<std::string> ret;
    string unit;
    if(dataset().getAttr("unit", unit)) {
        ret = unit;
    }
    return ret;
}


void PropertyHDF5::unit(const none_t t) {
    if(dataset().hasAttr("unit")) {
        dataset().removeAttr("unit");
    }
    forceUpdatedAt();
}


void PropertyHDF5::deleteValues() {
    dataset().setExtent({0});
}


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


int PropertyHDF5::compare(const IProperty &other) const {
    int cmp = 0;
    if (!name().empty() && !other.name().empty()) {
        cmp = (name()).compare(other.name());
    }
    if (cmp == 0) {
        cmp = id().compare(other.id());
    }
    return cmp;
}


PropertyHDF5::~PropertyHDF5() {}


} // namespace hdf5
} // namespace nix
