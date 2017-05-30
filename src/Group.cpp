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


void Group::tags(const std::vector<Tag> &tags) {
    auto cmp = [](const Tag &a, const Tag& b) { return a.name() < b.name(); };

    std::vector<Tag> new_tags(tags);
    size_t tag_count = nix::check::fits_in_size_t(tagCount(), "tagCount() failed; count > size_t.");
    std::vector<Tag> old_tags(tag_count);

    for (size_t i = 0; i < old_tags.size(); i++) {//check if this can be replaced
        old_tags[i] = getTag(i);
    }
    std::sort(new_tags.begin(), new_tags.end(), cmp);
    std::sort(old_tags.begin(), old_tags.end(), cmp);
    std::vector<Tag> add;
    std::vector<Tag> rem;

    std::set_difference(new_tags.begin(), new_tags.end(), old_tags.begin(),
                        old_tags.end(), std::inserter(add, add.begin()), cmp);
    std::set_difference(old_tags.begin(), old_tags.end(), new_tags.begin(),
                        new_tags.end(), std::inserter(rem, rem.begin()), cmp);

    for (const auto &t : add) {
        addTag(t);
    }

    for (const auto &t : rem) {
        removeTag(t);
    }
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
    auto cmp = [](const MultiTag &a, const MultiTag& b) { return a.name() < b.name(); };

    std::vector<MultiTag> new_tags(tags);
    size_t tag_count = nix::check::fits_in_size_t(multiTagCount(), "multiTagCount() failed; count > size_t.");
    std::vector<MultiTag> old_tags(tag_count);

    for (size_t i = 0; i < old_tags.size(); i++) {//check if this can be replaced
        old_tags[i] = getMultiTag(i);
    }
    std::sort(new_tags.begin(), new_tags.end(), cmp);
    std::sort(old_tags.begin(), old_tags.end(), cmp);
    std::vector<MultiTag> add;
    std::vector<MultiTag> rem;

    std::set_difference(new_tags.begin(), new_tags.end(), old_tags.begin(),
                        old_tags.end(), std::inserter(add, add.begin()), cmp);
    std::set_difference(old_tags.begin(), old_tags.end(), new_tags.begin(),
                        new_tags.end(), std::inserter(rem, rem.begin()), cmp);

    for (const auto &t : add) {
        addMultiTag(t);
    }

    for (const auto &t : rem) {
        removeMultiTag(t);
    }
}


std::ostream& nix::operator<<(std::ostream &out, const Group &ent) {
    out << "Group: {name = " << ent.name();
    out << ", type = " << ent.type();
    out << ", id = " << ent.id() << "}";
    return out;
}
