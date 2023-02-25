// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "NamedEntityHDF5.hpp"

#include <nix/util/util.hpp>

#include <ctime>

using namespace std;

using namespace nix::base;

namespace nix {
namespace hdf5 {


NamedEntityHDF5::NamedEntityHDF5(const std::shared_ptr<IFile> &file, const H5Group &group)
    : EntityHDF5(file, group)
{
}


NamedEntityHDF5::NamedEntityHDF5(const std::shared_ptr<IFile> &file, const H5Group &group, const string &id, const string &type,
                                 const string &name)
    : NamedEntityHDF5(file, group, id, type, name, util::getTime())
{
}


NamedEntityHDF5::NamedEntityHDF5(const std::shared_ptr<IFile> &file, const H5Group &group, const string &id, const string &type,
                                 const string &name, time_t time)
    : EntityHDF5(file, group, id, time)
{
    this->type(type);
    // set name
    if (name.empty()) {
        throw EmptyString("name");
    } else {
        group.setAttr("name", name);
        forceUpdatedAt();
    }

}


void NamedEntityHDF5::type(const string &type) {
    if (type.empty()) {
        throw EmptyString("type");
    } else {
        group().setAttr("type", type);
        forceUpdatedAt();
    }
}


string NamedEntityHDF5::type() const {
    string type;
    if (group().hasAttr("type")) {
        group().getAttr("type", type);
        return type;
    } else {
        throw MissingAttr("type");
    }
}


string NamedEntityHDF5::name() const {
    string name;
    if (group().hasAttr("name")) {
        group().getAttr("name", name);
        return name;
    } else {
        throw MissingAttr("name");
    }
}


void NamedEntityHDF5::definition(const string &definition) {
    if (definition.empty()) {
        throw EmptyString("definition");
    } else {
        group().setAttr("definition", definition);
        forceUpdatedAt();
    }
}


std::optional<string> NamedEntityHDF5::definition() const {
    std::optional<string> ret;
    string definition;
    bool have_attr = group().getAttr("definition", definition);
    if (have_attr) {
        ret = definition;
    }
    return ret;
}


void NamedEntityHDF5::definition(const nix::none_t t) {
    if (group().hasAttr("definition")) {
        group().removeAttr("definition");
    }
    forceUpdatedAt();
}


int NamedEntityHDF5::compare(const std::shared_ptr<INamedEntity> &other) const {
    int cmp = 0;
    if (!name().empty() && !other->name().empty()) {
        cmp = (name()).compare(other->name());
    }
    if (cmp == 0) {
        cmp = id().compare(other->id());
    }
    return cmp;
}


NamedEntityHDF5::~NamedEntityHDF5() {}

} // ns nix::hdf5
} // ns nix
