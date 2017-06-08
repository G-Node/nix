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
    auto cmp = [](const Source &a, const Source& b) { return a.name() < b.name(); };

    std::vector<Source> new_arrays(sources);

    ndsize_t current = sourceCount();
    size_t count = nix::check::fits_in_size_t(current, "entityCount() failed; count > size_t.");
    std::vector<Source> old_arrays(count);

    //check if this can be replaced
    for (size_t i = 0; i < old_arrays.size(); i++) {
        old_arrays[i] = getSource(i);
    }

    std::sort(new_arrays.begin(), new_arrays.end(), cmp);
    std::sort(old_arrays.begin(), old_arrays.end(), cmp);
    std::vector<Source> add;
    std::vector<Source> rem;

    std::set_difference(new_arrays.begin(), new_arrays.end(), old_arrays.begin(),
                        old_arrays.end(), std::inserter(add, add.begin()), cmp);
    std::set_difference(old_arrays.begin(), old_arrays.end(), new_arrays.begin(),
                        new_arrays.end(), std::inserter(rem, rem.begin()), cmp);

    for (const auto &e : add) {
        addSource(e.id());
    }

    for (const auto &e : rem) {
        removeSource(e.id());
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
