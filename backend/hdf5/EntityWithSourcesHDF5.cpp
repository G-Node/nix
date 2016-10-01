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
    boost::optional<H5Group> g = sources_refs(false);
    return g ? g->objectCount() : size_t(0);
}


bool EntityWithSourcesHDF5::hasSource(const std::string &id) const {
    return sources_refs(false) ? sources_refs(false)->hasGroup(id) : false;
}


std::shared_ptr<ISource> EntityWithSourcesHDF5::getSource(const std::string &name_or_id) const {
    std::shared_ptr<SourceHDF5> source;
    boost::optional<H5Group> g = sources_refs(false);

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
    boost::optional<H5Group> g = sources_refs(false);
    std::string id = g ? g->objectName(index) : "";
    return getSource(id);
}

void EntityWithSourcesHDF5::sources(const std::vector<Source> &sources) {
    // extract vectors of ids from vectors of new & old sources
    std::vector<std::string> ids_new(sources.size());
    std::transform(sources.begin(), sources.end(), ids_new.begin(), util::toId<Source>);

    size_t src_count = nix::check::fits_in_size_t(sourceCount(), "sourceCount() failed, count > size_t!");
    std::vector<Source> sources_old(src_count);
    for (size_t i = 0; i < sources_old.size(); i++) sources_old[i] = getSource(i);
    std::vector<std::string> ids_old(sources_old.size());
    std::transform(sources_old.begin(), sources_old.end(), ids_old.begin(), util::toId<Source>);
    // sort them
    std::sort(ids_new.begin(), ids_new.end());
    std::sort(ids_old.begin(), ids_old.end());
    // get ids only in ids_new (add), ids only in ids_old (remove) & ignore rest
    std::vector<std::string> ids_add;
    std::vector<std::string> ids_rem;
    std::set_difference(ids_new.begin(), ids_new.end(), ids_old.begin(), ids_old.end(), 
                        std::inserter(ids_add, ids_add.begin()));
    std::set_difference(ids_old.begin(), ids_old.end(), ids_new.begin(), ids_new.end(), 
                        std::inserter(ids_rem, ids_rem.begin()));
    
    // check if all new sources exist
    Block tmp(entity_block);
    auto found = tmp.findSources(util::IdsFilter<Source>(ids_add));
    if (ids_add.size() != found.size())
        throw std::runtime_error("One or more sources do not exist in this block!");
    // add sources
    for (auto id : ids_add) {
        addSource(id);
    }
    // remove sources
    for (auto id : ids_rem) {
        removeSource(id);
    }
}
    
void EntityWithSourcesHDF5::addSource(const std::string &id) {
    if (id.empty())
        throw EmptyString("addSource");
    boost::optional<H5Group> g = sources_refs(true);

    Block tmp(entity_block);
    auto found = tmp.findSources(util::IdFilter<Source>(id));
    if (found.empty())
        throw std::runtime_error("EntityWithSourcesHDF5::addSource: Given source does not exist in this block!");

    auto target = std::dynamic_pointer_cast<SourceHDF5>(found.front().impl());

    g->createLink(target->group(), id);
}


bool EntityWithSourcesHDF5::removeSource(const std::string &id) {
    boost::optional<H5Group> g = sources_refs(false);
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