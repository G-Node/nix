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

bool Block::hasSimpleTag(const SimpleTag &simple_tag) const {
    if(simple_tag == none) {
        throw std::runtime_error("Empty SimpleTag entity given!");
    }
    return backend()->hasSimpleTag(simple_tag.id());
}

std::vector<SimpleTag> Block::simpleTags(util::Filter<SimpleTag>::type filter) const {
    auto f = [this] (size_t i) { return getSimpleTag(i); };
    return getEntities<SimpleTag>(f,
                                  simpleTagCount(),
                                  filter);
}

bool Block::deleteSimpleTag(const SimpleTag &simple_tag) {
    if (simple_tag == none) {
        throw std::runtime_error("Block::deleteSimpleTag: Empty SimpleTag entity given!");
    }
    return backend()->deleteSimpleTag(simple_tag.id());
}

bool Block::hasDataTag(const DataTag &data_tag) const {
    if (data_tag == none) {
        throw std::runtime_error("Block::hasDataTag: Empty DataTag entitiy given!");
    }
    return backend()->hasDataTag(data_tag.id());
}

std::vector<DataTag> Block::dataTags(util::AcceptAll<DataTag>::type filter) const {
    auto f = [this] (size_t i) { return getDataTag(i); };
    return getEntities<DataTag>(f,
                                dataTagCount(),
                                filter);
}

bool Block::deleteDataTag(const DataTag &data_tag) {
    if (data_tag == none) {
        throw std::runtime_error("Block::deleteDataTag: Empty DataTag entitiy given!");
    }
    return backend()->deleteDataTag(data_tag.id());
}

std::ostream &operator<<(std::ostream &out, const Block &ent) {
    out << "Block: {name = " << ent.name();
    out << ", type = " << ent.type();
    out << ", id = " << ent.id() << "}";
    return out;
}


}
