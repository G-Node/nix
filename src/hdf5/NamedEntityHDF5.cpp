// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <ctime>

#include <nix/hdf5/NamedEntityHDF5.hpp>

using namespace std;
using namespace nix::base;
using namespace boost;
namespace nix {
namespace hdf5 {


NamedEntityHDF5::NamedEntityHDF5(File file, Group group, const string &id, const string &_type)
    : NamedEntityHDF5(file, group, id, _type, util::getTime())
{
}


NamedEntityHDF5::NamedEntityHDF5(File file, Group group, const string &id, const string &_type, time_t time)
    : EntityHDF5(file, group, id, time)
{
    type(_type);
}


void NamedEntityHDF5::type(const string &type) {
    if(type.empty()) {
        throw EmptyString("type");
    }
    else {
        group().setAttr("type", type);
        forceUpdatedAt();
    }
}


string NamedEntityHDF5::type() const {
    string type;
    if(group().hasAttr("type")) {
        group().getAttr("type", type);
        return type;
    } else {
        throw MissingAttr("type");
    }
}


void NamedEntityHDF5::name(const string &name) {
    if(name.empty()) {
        throw EmptyString("name");
    }
    else {
        group().setAttr("name", name);
        forceUpdatedAt();
    }
}


optional<string> NamedEntityHDF5::name() const {
    optional<string> ret;
    string name;
    group().getAttr("name", name);
    ret = name;
    return ret;
}


void NamedEntityHDF5::name(const none_t t) {
    if(group().hasAttr("name")) {
        group().removeAttr("name");
    }
    forceUpdatedAt();
}


void NamedEntityHDF5::definition(const string &definition) {
    if(definition.empty()) {
        throw EmptyString("definition");
    }
    else {
        group().setAttr("definition", definition);
        forceUpdatedAt();
    }
}


optional<string> NamedEntityHDF5::definition() const {
    optional<string> ret;
    string definition;
    group().getAttr("definition", definition);
    ret = definition;
    return ret;
}


void NamedEntityHDF5::definition(const none_t t) {
    if(group().hasAttr("definition")) {
        group().removeAttr("definition");
    }
    forceUpdatedAt();
}


int NamedEntityHDF5::compare(const INamedEntity &other) const {
    int cmp = 0;
    if (name() && other.name()) {
        cmp = (*name()).compare(*other.name());
    }
    if (cmp == 0) {
        cmp = id().compare(other.id());
    }
    return cmp;
}


NamedEntityHDF5::~NamedEntityHDF5() {}


} // namespace hdf5
} // namespace nix
