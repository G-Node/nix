// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/Block.hpp>

using namespace std;

namespace nix {

std::vector<Source> Block::findSources(util::Filter<Source>::type filter,
        size_t max_depth) const {
    const vector<Source> probes = sources();
    vector<Source> matches;
    vector<Source> result;

    for (auto &probe : probes) {
        matches = probe.findSources(filter, max_depth);
        result.insert(result.end(), matches.begin(), matches.end());
    }

    return result;
}

bool Block::hasSource(const Source &source) const {
    if (source == none) {
        throw std::runtime_error("Empty Source entity given");
    }
    return backend()->hasSource(source.id());
}

std::vector<Source> Block::sources(util::Filter<Source>::type filter) const {
    auto f = [this](size_t i) { return getSource(i); };
    return getEntities<Source>(f,
            sourceCount(),
            filter);
}

bool Block::deleteSource(const Source &source) {
    if (source == none) {
        throw std::runtime_error("Empty Source entity given");
    }
    return backend()->deleteSource(source.id());
}

bool Block::hasDataArray(const DataArray &data_array) const {
    if (data_array == none) {
        throw std::runtime_error("Empty DataArray entity given!");
    }
    return backend()->hasDataArray(data_array.id());
}

std::vector<DataArray> Block::dataArrays(util::AcceptAll<DataArray>::type filter) const {
    auto f = [this] (size_t i) { return getDataArray(i); };
    return getEntities<DataArray>(f,
                                  dataArrayCount(),
                                  filter);
}

bool Block::deleteDataArray(const DataArray &data_array) {
    if (data_array == none) {
        throw std::runtime_error("Empty DataArray entity given!");
    }
    return backend()->deleteDataArray(data_array.id());
}

bool Block::hasTag(const Tag &tag) const {
    if (tag == none) {
        throw std::runtime_error("Empty Tag entity given!");
    }
    return backend()->hasTag(tag.id());
}

std::vector<Tag> Block::tags(util::Filter<Tag>::type filter) const {
    auto f = [this] (size_t i) { return getTag(i); };
    return getEntities<Tag>(f,
                            tagCount(),
                            filter);
}

bool Block::deleteTag(const Tag &tag) {
    if (tag == none) {
        throw std::runtime_error("Block::deleteTag: Empty Tag entity given!");
    }
    return backend()->deleteTag(tag.id());
}

bool Block::hasMultiTag(const MultiTag &multi_tag) const {
    if (multi_tag == none) {
        throw std::runtime_error("Block::hasMultiTag: Empty MultiTag entitiy given!");
    }
    return backend()->hasMultiTag(multi_tag.id());
}

std::vector<MultiTag> Block::multiTags(util::AcceptAll<MultiTag>::type filter) const {
    auto f = [this] (size_t i) { return getMultiTag(i); };
    return getEntities<MultiTag>(f,
                                multiTagCount(),
                                filter);
}

bool Block::deleteMultiTag(const MultiTag &multi_tag) {
    if (multi_tag == none) {
        throw std::runtime_error("Block::deleteMultiTag: Empty MultiTag entitiy given!");
    }
    return backend()->deleteMultiTag(multi_tag.id());
}

std::ostream &operator<<(std::ostream &out, const Block &ent) {
    out << "Block: {name = " << ent.name();
    out << ", type = " << ent.type();
    out << ", id = " << ent.id() << "}";
    return out;
}


}
