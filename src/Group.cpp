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

void Group::dataArrays(const std::vector<DataArray> &data_arrays) {
    auto cmp = [](const DataArray &a, const DataArray& b) { return a.name() < b.name(); };

    std::vector<DataArray> new_arrays(data_arrays);
    size_t array_count = nix::check::fits_in_size_t(dataArrayCount(), "dataArrayCount() failed; count > size_t.");
    std::vector<DataArray> old_arrays(array_count);

    for (size_t i = 0; i < old_arrays.size(); i++) {//check if this can be replaced
        old_arrays[i] = getDataArray(i);
    }
    std::sort(new_arrays.begin(), new_arrays.end(), cmp);
    std::sort(old_arrays.begin(), old_arrays.end(), cmp);
    std::vector<DataArray> add;
    std::vector<DataArray> rem;

    std::set_difference(new_arrays.begin(), new_arrays.end(), old_arrays.begin(),
                        old_arrays.end(), std::inserter(add, add.begin()), cmp);
    std::set_difference(old_arrays.begin(), old_arrays.end(), new_arrays.begin(),
                        new_arrays.end(), std::inserter(rem, rem.begin()), cmp);

    for (const auto &da : add) {
        addDataArray(da);
    }

    for (const auto &da : rem) {
        removeDataArray(da);
    }
}


std::vector<DataArray> Group::dataArrays(const util::Filter<DataArray>::type &filter) const {
    auto f = [this] (size_t i) { return getDataArray(i); };
    return getEntities<DataArray>(f,
                                  dataArrayCount(),
                                  filter);
}


bool Group::hasTag(const Tag &tag) const {
    if (!util::checkEntityInput(tag, false)) {
        return false;
    }
    return backend()->hasTag(tag.id());
}


Tag Group::getTag(size_t index) const {
    if(index >= backend()->tagCount()) {
        throw OutOfBounds("No tag at given index", index);
    }
    return backend()->getTag(index);
}


void Group::addTag(const Tag &tag) {
    if (!util::checkEntityInput(tag, false)) {
        throw UninitializedEntity();
    }
    backend()->addTag(tag.id());
}


void Group::addTag(const std::string &id) {
    util::checkNameOrId(id);
    backend()->addTag(id);
}


bool Group::removeTag(const Tag &tag) {
    if (!util::checkEntityInput(tag, false)) {
        return false;
    }
    return backend()->removeTag(tag.id());
}


std::vector<Tag> Group::tags(const util::Filter<Tag>::type &filter) const {
    auto f = [this] (size_t i) { return getTag(i); };
    return getEntities<Tag>(f, tagCount(), filter);
}

bool Group::hasMultiTag(const MultiTag &multi_tag) const {
    if (!util::checkEntityInput(multi_tag, false)) {
        return false;
    }
    return backend()->hasMultiTag(multi_tag.id());
}


MultiTag Group::getMultiTag(size_t index) const {
    if(index >= backend()->multiTagCount()) {
        throw OutOfBounds("No multi tag at given index", index);
    }
    return backend()->getMultiTag(index);
}


void Group::addMultiTag(const MultiTag &multi_tag) {
    if (!util::checkEntityInput(multi_tag)) {
        throw UninitializedEntity();
    }
    backend()->addMultiTag(multi_tag.id());
}


void Group::addMultiTag(const std::string &id) {
    util::checkNameOrId(id);
    backend()->addMultiTag(id);
}


bool Group::removeMultiTag(const MultiTag &multi_tag) {
    if (!util::checkEntityInput(multi_tag, false)) {
        return false;
    }
    return backend()->removeMultiTag(multi_tag.id());
}


std::vector<MultiTag> Group::multiTags(const util::Filter<MultiTag>::type &filter) const {
    auto f = [this] (size_t i) { return getMultiTag(i); };
    return getEntities<MultiTag>(f, multiTagCount(), filter);
}


std::ostream& nix::operator<<(std::ostream &out, const Group &ent) {
    out << "Group: {name = " << ent.name();
    out << ", type = " << ent.type();
    out << ", id = " << ent.id() << "}";
    return out;
}
