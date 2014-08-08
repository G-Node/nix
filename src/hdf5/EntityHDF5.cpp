// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <ctime>

#include <nix/util/util.hpp>
#include <nix/hdf5/EntityHDF5.hpp>

using namespace std;
using namespace nix;
using namespace nix::base;
using namespace nix::hdf5;


EntityHDF5::EntityHDF5(shared_ptr<IFile> file, Group group)
    : entity_file(file), entity_group(group)
{
    setUpdatedAt();
    setCreatedAt();
}


EntityHDF5::EntityHDF5(shared_ptr<IFile> file, Group group, const string &id, time_t time)
    : entity_file(file), entity_group(group)
{
    group.setAttr("entity_id", id);
    setUpdatedAt();
    forceCreatedAt(time);
}


string EntityHDF5::id() const {
    string t;
    
    if (group().hasAttr("entity_id")) {
        group().getAttr("entity_id", t);
    }
    else {
        throw runtime_error("Entity has no id!");
    }
    
    return t;
}


time_t EntityHDF5::updatedAt() const {
    string t;
    group().getAttr("updated_at", t);
    return util::strToTime(t);
}


void EntityHDF5::setUpdatedAt() {
    if (!group().hasAttr("updated_at")) {
        time_t t = util::getTime();
        group().setAttr("updated_at", util::timeToStr(t));
    }
}


void EntityHDF5::forceUpdatedAt() {
    time_t t = util::getTime();
    group().setAttr("updated_at", util::timeToStr(t));
}


time_t EntityHDF5::createdAt() const {
    string t;
    group().getAttr("created_at", t);
    return util::strToTime(t);
}


void EntityHDF5::setCreatedAt() {
    if (!group().hasAttr("created_at")) {
        time_t t = util::getTime();
        group().setAttr("created_at", util::timeToStr(t));
    }
}


void EntityHDF5::forceCreatedAt(time_t t) {
    group().setAttr("created_at", util::timeToStr(t));
}


void EntityHDF5::swap(EntityHDF5 &other) {
    using std::swap;

    swap(entity_file, other.entity_file);
    swap(entity_group, other.entity_group);
}


bool EntityHDF5::operator==(const EntityHDF5 &other) const {
    return group() == other.group() && id() == other.id();
}


bool EntityHDF5::operator!=(const EntityHDF5 &other) const {
    return group() != other.group() || id() != other.id();
}


EntityHDF5::~EntityHDF5() {}

