
// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "EntityWithSourcesHDF5.hpp"

#include <nix/util/util.hpp>
#include <nix/Block.hpp>

#include <algorithm>
#include <functional>

using namespace nix::base;

namespace nix {
namespace hdf5 {

EntityWithSourcesHDF5::EntityWithSourcesHDF5(const std::shared_ptr<IFile> &file, const std::shared_ptr<IBlock> &block,
                                             const H5Group &group)
    : EntityWithMetadataHDF5(file, group), entity_block(block)
{
    sources_refs = this->group().openOptGroup("sources");
}


EntityWithSourcesHDF5::EntityWithSourcesHDF5(const std::shared_ptr<IFile> &file, const std::shared_ptr<IBlock> &block,
                                             const H5Group &group, const std::string &id, const std::string &type,
                                             const std::string &name)
    : EntityWithSourcesHDF5(file, block, group, id, type, name, util::getTime())
{
}


EntityWithSourcesHDF5::EntityWithSourcesHDF5 (const std::shared_ptr<IFile> &file, const std::shared_ptr<IBlock> &block,
                                              const H5Group &group, const std::string &id, const std::string &type,
                                              const std::string &name, time_t time)
    : EntityWithMetadataHDF5(file, group, id, type, name, time), entity_block(block)
{
    sources_refs = this->group().openOptGroup("sources");
}


ndsize_t EntityWithSourcesHDF5::sourceCount() const {
    std::optional<H5Group> g = sources_refs(false);
    return g ? g->objectCount() : size_t(0);
}


bool EntityWithSourcesHDF5::hasSource(const std::string &id) const {
    return sources_refs(false) ? sources_refs(false)->hasGroup(id) : false;
}


std::shared_ptr<ISource> EntityWithSourcesHDF5::getSource(const std::string &name_or_id) const {
    std::shared_ptr<SourceHDF5> source;
    std::optional<H5Group> g = sources_refs(false);

    std::string id = name_or_id;

    if (!util::looksLikeUUID(name_or_id)) {
        Block tmp(entity_block);
        auto found = tmp.findSources(util::NameFilter<Source>(name_or_id));

        if (!found.empty())
            id = found.front().id();
    }

    if (g && hasSource(id)) {
        H5Group group = g->openGroup(id);
        source = std::make_shared<SourceHDF5>(file(), entity_block, group);
    }

    return source;
}

std::shared_ptr<ISource> EntityWithSourcesHDF5::getSource(const size_t index) const {
    std::optional<H5Group> g = sources_refs(false);
    std::string id = g ? g->objectName(index) : "";
    return getSource(id);
}

void EntityWithSourcesHDF5::sources(const std::vector<Source> &sources) {
    while (sourceCount() > 0) {
        removeSource(getSource(0)->id());
    }
    for (const auto &src : sources) {
        if (block()->hasEntity(src) ) {
            addSource(src.id());
        }
    }
}

void EntityWithSourcesHDF5::addSource(const std::string &id) {
    if (id.empty())
        throw EmptyString("addSource");
    std::optional<H5Group> g = sources_refs(true);

    Block tmp(entity_block);
    auto found = tmp.findSources(util::IdFilter<Source>(id));
    if (found.empty())
        throw std::runtime_error("EntityWithSourcesHDF5::addSource: Given source does not exist in this block!");

    auto target = std::dynamic_pointer_cast<SourceHDF5>(found.front().impl());

    g->createLink(target->group(), id);
}


bool EntityWithSourcesHDF5::removeSource(const std::string &id) {
    std::optional<H5Group> g = sources_refs(false);
    bool removed = false;

    if (g) {
        g->removeGroup(id);
        removed = true;
    }

    return removed;
}


std::shared_ptr<base::IBlock> EntityWithSourcesHDF5::block() const {
    return entity_block;
}


EntityWithSourcesHDF5::~EntityWithSourcesHDF5() {}

} // ns nix::hdf5
} // ns nix
