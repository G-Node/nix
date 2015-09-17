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

Source Block::createSource(const std::string &name, const std::string &type){
    util::checkEntityNameAndType(name, type);
    if (backend()->hasSource(name)) {
        throw DuplicateName("createSource");
    }
    return backend()->createSource(name, type);
}

std::vector<Source> Block::findSources(const util::Filter<Source>::type &filter,
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
    util::checkEntityInput(source);
    return backend()->hasSource(source.id());
}

std::vector<Source> Block::sources(const util::Filter<Source>::type &filter) const {
    auto f = [this](ndsize_t i) { return getSource(i); };
    return getEntities<Source>(f,
            sourceCount(),
            filter);
}

bool Block::deleteSource(const Source &source) {
    util::checkEntityInput(source);
    return backend()->deleteSource(source.id());
}

DataArray Block::createDataArray(const std::string &name, const std::string &type, nix::DataType data_type,
                                 const NDSize &shape) {
    util::checkEntityNameAndType(name, type);
    if (backend()->hasDataArray(name)){
        throw DuplicateName("create DataArray");
    }
    return backend()->createDataArray(name, type, data_type, shape);
}

bool Block::hasDataArray(const DataArray &data_array) const {
    util::checkEntityInput(data_array);
    return backend()->hasDataArray(data_array.id());
}

std::vector<DataArray> Block::dataArrays(const util::AcceptAll<DataArray>::type &filter) const {
    auto f = [this] (size_t i) { return getDataArray(i); };
    return getEntities<DataArray>(f,
                                  dataArrayCount(),
                                  filter);
}

bool Block::deleteDataArray(const DataArray &data_array) {
    util::checkEntityInput(data_array);
    return backend()->deleteDataArray(data_array.id());
}

Tag Block::createTag(const std::string &name, const std::string &type, const std::vector<double> &position) {
    util::checkEntityNameAndType(name, type);
    if (backend()->hasTag(name)){
        throw DuplicateName("create Tag");
    }
    return backend()->createTag(name, type, position);
}

bool Block::hasTag(const Tag &tag) const {
    util::checkEntityInput(tag);
    return backend()->hasTag(tag.id());
}

std::vector<Tag> Block::tags(const util::Filter<Tag>::type &filter) const {
    auto f = [this] (ndsize_t i) { return getTag(i); };
    return getEntities<Tag>(f,
                            tagCount(),
                            filter);
}

bool Block::deleteTag(const Tag &tag) {
    util::checkEntityInput(tag);
    return backend()->deleteTag(tag.id());
}

MultiTag Block::createMultiTag(const std::string &name, const std::string &type, const DataArray &positions) {
    util::checkEntityNameAndType(name, type);
    util::checkEntityInput(positions);
    if (backend()->hasMultiTag(name)) {
        throw DuplicateName("createMultiTag");
    }
    return backend()->createMultiTag(name, type, positions);
}

bool Block::hasMultiTag(const MultiTag &multi_tag) const {
    util::checkEntityInput(multi_tag);
    return backend()->hasMultiTag(multi_tag.id());
}

std::vector<MultiTag> Block::multiTags(const util::AcceptAll<MultiTag>::type &filter) const {
    auto f = [this] (ndsize_t i) { return getMultiTag(i); };
    return getEntities<MultiTag>(f,
                                multiTagCount(),
                                filter);
}

bool Block::deleteMultiTag(const MultiTag &multi_tag) {
    util::checkEntityInput(multi_tag);
    return backend()->deleteMultiTag(multi_tag.id());
}

std::ostream &operator<<(std::ostream &out, const Block &ent) {
    out << "Block: {name = " << ent.name();
    out << ", type = " << ent.type();
    out << ", id = " << ent.id() << "}";
    return out;
}


}
