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

template<typename T>
void Group::replaceEntities(const std::vector<T> &entities)
{
    base::IGroup *ig = backend();
    ObjectType ot = objectToType<T>::value;

    auto cmp = [](const T &a, const T& b) { return a.name() < b.name(); };

    std::vector<T> new_arrays(entities);

    ndsize_t current = ig->entityCount(ot);
    size_t count = nix::check::fits_in_size_t(current, "entityCount() failed; count > size_t.");
    std::vector<T> old_arrays(count);

    //check if this can be replaced
    for (size_t i = 0; i < old_arrays.size(); i++) {
        old_arrays[i] = ig->getEntity<typename objectToType<T>::backendType>(i);
    }

    std::sort(new_arrays.begin(), new_arrays.end(), cmp);
    std::sort(old_arrays.begin(), old_arrays.end(), cmp);
    std::vector<T> add;
    std::vector<T> rem;

    std::set_difference(new_arrays.begin(), new_arrays.end(), old_arrays.begin(),
                        old_arrays.end(), std::inserter(add, add.begin()), cmp);
    std::set_difference(old_arrays.begin(), old_arrays.end(), new_arrays.begin(),
                        new_arrays.end(), std::inserter(rem, rem.begin()), cmp);

    for (const auto &e : add) {
        ig->addEntity(e);
    }

    for (const auto &e : rem) {
        ig->removeEntity(e);
    }
}

void Group::dataArrays(const std::vector<DataArray> &data_arrays) {
    replaceEntities(data_arrays);
}


std::vector<DataArray> Group::dataArrays(const util::Filter<DataArray>::type &filter) const {
    auto f = [this] (size_t i) { return getDataArray(i); };
    return getEntities<DataArray>(f,
                                  dataArrayCount(),
                                  filter);
}


void Group::tags(const std::vector<Tag> &tags) {
    replaceEntities(tags);
}

std::vector<Tag> Group::tags(const util::Filter<Tag>::type &filter) const {
    auto f = [this] (size_t i) { return getTag(i); };
    return getEntities<Tag>(f, tagCount(), filter);
}


std::vector<MultiTag> Group::multiTags(const util::Filter<MultiTag>::type &filter) const {
    auto f = [this] (size_t i) { return getMultiTag(i); };
    return getEntities<MultiTag>(f, multiTagCount(), filter);
}


void Group::multiTags(const std::vector<MultiTag> &tags) {
    replaceEntities(tags);
}


std::ostream& nix::operator<<(std::ostream &out, const Group &ent) {
    out << "Group: {name = " << ent.name();
    out << ", type = " << ent.type();
    out << ", id = " << ent.id() << "}";
    return out;
}
