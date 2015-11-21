// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/Group.hpp>
#include <nix/util/util.hpp>

using namespace nix;

bool Group::hasDataArray(const DataArray &data_array) const {
    util::checkEntityInput(data_array);
    return backend()->hasDataArray(data_array.id());
}


DataArray Group::getDataArray(size_t index) const {
    if(index >= backend()->dataArrayCount()) {
        throw OutOfBounds("No dataArray at given index", index);
    }
    return backend()->getDataArray(index);
}


void Group::addDataArray(const DataArray &data_array) {
    util::checkEntityInput(data_array);
    backend()->addDataArray(data_array.id());
}


void Group::addDataArray(const std::string &id) {
    util::checkNameOrId(id);
    backend()->addDataArray(id);
}


bool Group::removeDataArray(const DataArray &data_array) {
    util::checkEntityInput(data_array);
    return backend()->removeDataArray(data_array.id());
}


std::vector<DataArray> Group::dataArrays(const util::Filter<DataArray>::type &filter) const {
    auto f = [this] (size_t i) { return getDataArray(i); };
    return getEntities<DataArray>(f,
                                  dataArrayCount(),
                                  filter);
}


bool Group::hasTag(const Tag &tag) const {
    util::checkEntityInput(tag);
    return backend()->hasDataArray(tag.id());
}


Tag Group::getTag(size_t index) const {
    if(index >= backend()->tagCount()) {
        throw OutOfBounds("No tag at given index", index);
    }
    return backend()->getTag(index);
}


void Group::addTag(const Tag &tag) {
    util::checkEntityInput(tag);
    backend()->addTag(tag.id());
}


void Group::addTag(const std::string &id) {
    util::checkNameOrId(id);
    backend()->addTag(id);
}


bool Group::removeTag(const Tag &tag) {
    util::checkEntityInput(tag);
    return backend()->removeTag(tag.id());
}


std::vector<Tag> Group::tags(const util::Filter<Tag>::type &filter) const {
    auto f = [this] (size_t i) { return getTag(i); };
    return getEntities<Tag>(f, tagCount(), filter);
}



std::ostream& nix::operator<<(std::ostream &out, const Group &ent) {
    out << "Group: {name = " << ent.name();
    out << ", type = " << ent.type();
    out << ", id = " << ent.id() << "}";
    return out;
}
