// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/Block.hpp>
#include <nix/util/util.hpp>

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
    const std::vector<Source> probes = sources();
    std::vector<Source> matches;
    std::vector<Source> result;

    for (auto &probe : probes) {
        matches = probe.findSources(filter, max_depth);
        result.insert(result.end(), matches.begin(), matches.end());
    }

    return result;
}

bool Block::hasSource(const Source &source) const {
    if (!util::checkEntityInput(source, false)) {
        return false;
    }
    Source s = backend()->getSource(source.name());
    return s && s.id() == source.id();
}

Source Block::getSource(ndsize_t index) const {
    if (index >= backend()->sourceCount()) {
        throw OutOfBounds("Block::getSource: index is out of bounds!");
    }
    return backend()->getSource(index);
}

std::vector<Source> Block::sources(const util::Filter<Source>::type &filter) const {
    auto f = [this](ndsize_t i) { return getSource(i); };
    return getEntities<Source>(f, sourceCount(), filter);
}

bool Block::deleteSource(const Source &source) {
    if (!util::checkEntityInput(source, false)) {
        return false;
    }
    return backend()->deleteSource(source.name());
}

DataArray Block::createDataArray(const std::string &name, const std::string &type, nix::DataType data_type,
                                 const NDSize &shape) {
    util::checkEntityNameAndType(name, type);
    if (hasDataArray(name)){
        throw DuplicateName("create DataArray");
    }
    return backend()->createDataArray(name, type, data_type, shape);
}

std::vector<DataArray> Block::dataArrays(const util::AcceptAll<DataArray>::type &filter) const {
    auto f = [this] (size_t i) { return getDataArray(i); };
    return getEntities<DataArray>(f, dataArrayCount(), filter);
}

Tag Block::createTag(const std::string &name, const std::string &type, const std::vector<double> &position) {
    util::checkEntityNameAndType(name, type);
    if (backend()->hasTag(name)){
        throw DuplicateName("create Tag");
    }
    return backend()->createTag(name, type, position);
}

bool Block::hasTag(const Tag &tag) const {
    if (!util::checkEntityInput(tag, false)) {
        return false;
    }
    Tag t = backend()->getTag(tag.name());
    return  t && t.id() == tag.id();
}

Tag Block::getTag(ndsize_t index) const {
    if (index >= backend()->tagCount()) {
        throw nix::OutOfBounds("Block::getTag: Index is out of Bounds!");
    }
    return backend()->getTag(index);
}

std::vector<Tag> Block::tags(const util::Filter<Tag>::type &filter) const {
    auto f = [this] (ndsize_t i) { return getTag(i); };
    return getEntities<Tag>(f, tagCount(), filter);
}

bool Block::deleteTag(const Tag &tag) {
    if (!util::checkEntityInput(tag, false)) {
        return false;
    }
    return backend()->deleteTag(tag.name());
}

MultiTag Block::createMultiTag(const std::string &name, const std::string &type, const DataArray &positions) {
    util::checkEntityNameAndType(name, type);
    util::checkEntityInput(positions);
    if(!positions.isValidEntity()) {
        throw UninitializedEntity();
    }
    if (backend()->hasMultiTag(name)) {
        throw DuplicateName("createMultiTag");
    }
    return backend()->createMultiTag(name, type, positions);
}

MultiTag Block::getMultiTag(ndsize_t index) const {
    if (index >= backend()->multiTagCount()) {
        throw nix::OutOfBounds("Block::getMultiTag: index is out of bounds!");
    }
    return backend()->getMultiTag(index);
}

bool Block::hasMultiTag(const MultiTag &multi_tag) const {
    if (!util::checkEntityInput(multi_tag, false)) {
        return false;
    }
    MultiTag mt = backend()->getMultiTag(multi_tag.name());
    return mt && mt.id() == multi_tag.id();
}

std::vector<MultiTag> Block::multiTags(const util::AcceptAll<MultiTag>::type &filter) const {
    auto f = [this] (ndsize_t i) { return getMultiTag(i); };
    return getEntities<MultiTag>(f, multiTagCount(), filter);
}

bool Block::deleteMultiTag(const MultiTag &multi_tag) {
    if (!util::checkEntityInput(multi_tag, false)) {
        return false;
    }
    return backend()->deleteMultiTag(multi_tag.name());
}

Group Block::createGroup(const std::string &name, const std::string &type) {
    util::checkEntityNameAndType(name, type);
    if (backend()->hasGroup(name)) {
        throw DuplicateName("createGroup");
    }
    return backend()->createGroup(name, type);
}

bool Block::hasGroup(const Group &group) const {
    if (!util::checkEntityInput(group, false)) {
        return false;
    }
    Group g = backend()->getGroup(group.name());
    return g && g.id() == group.id();
}

std::vector<Group> Block::groups(const util::AcceptAll<Group>::type &filter) const {
    auto f = [this] (ndsize_t i) { return getGroup(i); };
    return getEntities<Group>(f, groupCount(), filter);
}

bool Block::deleteGroup(const Group &group) {
    if (!util::checkEntityInput(group, false)) {
        return false;
    }
    return backend()->deleteGroup(group.name());
}

std::ostream &operator<<(std::ostream &out, const Block &ent) {
    out << "Block: {name = " << ent.name();
    out << ", type = " << ent.type();
    out << ", id = " << ent.id() << "}";
    return out;
}


}
