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

    while (ig->entityCount(ot) > 0) {
        ig->removeEntity(ig->getEntity<typename objectToType<T>::backendType>(0));
    }

    for (const auto &e : entities) {
        ig->addEntity(e);
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


void Group::dataFrames(const std::vector<DataFrame> &data_frames) {
    replaceEntities(data_frames);
}


std::vector<DataFrame> Group::dataFrames(const util::Filter<DataFrame>::type &filter) const {
    auto f = [this] (size_t i) { return getDataFrame(i); };
    return getEntities<DataFrame>(f,
                                  dataFrameCount(),
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
