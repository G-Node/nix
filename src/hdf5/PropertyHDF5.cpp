// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/hdf5/PropertyHDF5.hpp>

#include <nix/util/util.hpp>

#include <iostream>

using namespace std;

using namespace nix::base;

namespace nix {
namespace hdf5 {



    PropertyHDF5::PropertyHDF5(const std::shared_ptr<IFile> &file, const DataSet &dataset)
    : entity_file(file)
{
    this->entity_dataset = dataset;
}


    PropertyHDF5::PropertyHDF5(const std::shared_ptr<IFile> &file, const DataSet &dataset, const string &id,
                               const string &name)
    : PropertyHDF5(file, dataset, id, name, util::getTime())
{
}


    PropertyHDF5::PropertyHDF5(const std::shared_ptr<IFile> &file, const DataSet &dataset, const string &id,
                               const string &name, time_t time)
    : entity_file(file)
{
    this->entity_dataset = dataset;
    // set name
    if (name.empty()) {
        throw EmptyString("name");
    } else {
        dataset.setAttr("name", name);
        forceUpdatedAt();
    }
    
    dataset.setAttr("entity_id", id);
    setUpdatedAt();
    forceCreatedAt(time);
}


string PropertyHDF5::id() const {
    string t;
    
    if (dataset().hasAttr("entity_id")) {
        dataset().getAttr("entity_id", t);
    }
    else {
        throw runtime_error("Entity has no id!");
    }
    
    return t;
}


time_t PropertyHDF5::updatedAt() const {
    string t;
    dataset().getAttr("updated_at", t);
    return util::strToTime(t);
}


void PropertyHDF5::setUpdatedAt() {
    if (!dataset().hasAttr("updated_at")) {
        time_t t = util::getTime();
        dataset().setAttr("updated_at", util::timeToStr(t));
    }
}


void PropertyHDF5::forceUpdatedAt() {
    time_t t = util::getTime();
    dataset().setAttr("updated_at", util::timeToStr(t));
}


time_t PropertyHDF5::createdAt() const {
    string t;
    dataset().getAttr("created_at", t);
    return util::strToTime(t);
}


void PropertyHDF5::setCreatedAt() {
    if (!dataset().hasAttr("created_at")) {
        time_t t = util::getTime();
        dataset().setAttr("created_at", util::timeToStr(t));
    }
}


void PropertyHDF5::forceCreatedAt(time_t t) {
    dataset().setAttr("created_at", util::timeToStr(t));
}


string PropertyHDF5::name() const {
    string name;
    if (dataset().hasAttr("name")) {
        dataset().getAttr("name", name);
        return name;
    } else {
        throw MissingAttr("name");
    }
}


void PropertyHDF5::definition(const string &definition) {
    if (definition.empty()) {
        throw EmptyString("definition");
    } else {
        dataset().setAttr("definition", definition);
        forceUpdatedAt();
    }
}


boost::optional<string> PropertyHDF5::definition() const {
    boost::optional<string> ret;
    string definition;
    bool have_attr = dataset().getAttr("definition", definition);
    if (have_attr) {
        ret = definition;
    }
    return ret;
}


void PropertyHDF5::definition(const nix::none_t t) {
    if (dataset().hasAttr("definition")) {
        dataset().removeAttr("definition");
    }
    forceUpdatedAt();
}


DataType PropertyHDF5::dataType() const {
    return this->dataset().dataType();
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


void PropertyHDF5::mapping(const nix::none_t t) {
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
    if (dataset().getAttr("unit", unit)) {
        ret = unit;
    }
    return ret;
}


void PropertyHDF5::unit(const nix::none_t t) {
    if (dataset().hasAttr("unit")) {
        dataset().removeAttr("unit");
    }
    forceUpdatedAt();
}


void PropertyHDF5::deleteValues() {
    dataset().setExtent({0});
}


size_t PropertyHDF5::valueCount() const {
    NDSize size = dataset().size();
    return size[0];
}


void PropertyHDF5::values(const std::vector<Value> &values)
{
    if (values.size() < 1) {
        deleteValues();
        return;
    }
    DataType dtype = values[0].type();
    H5::DataType fileType = DataSet::fileTypeForValue(dtype);
    dataset().write(values);
}


std::vector<Value> PropertyHDF5::values(void) const
{
    std::vector<Value> values;
    dataset().read(values);
    return values;
}


void PropertyHDF5::values(const nix::none_t t) {
    // TODO: rethink if we want two methods for same thing
    deleteValues();
}


int PropertyHDF5::compare(const std::shared_ptr<IProperty> &other) const {
    int cmp = 0;
    if (!name().empty() && !other->name().empty()) {
        cmp = (name()).compare(other->name());
    }
    if (cmp == 0) {
        cmp = id().compare(other->id());
    }
    return cmp;
}


PropertyHDF5::~PropertyHDF5() {}

} // ns nix::hdf5
} // ns nix
