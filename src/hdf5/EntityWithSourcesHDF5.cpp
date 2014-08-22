// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <algorithm>
#include <functional>

#include <nix/util/util.hpp>
#include <nix/Block.hpp>
#include <nix/hdf5/EntityWithSourcesHDF5.hpp>


using namespace std;
using namespace nix;
using namespace nix::hdf5;
using namespace nix::base;

EntityWithSourcesHDF5::EntityWithSourcesHDF5(const shared_ptr<IFile> &file, const shared_ptr<IBlock> &block, Group group)
    : EntityWithMetadataHDF5(file, group), entity_block(block)
{
    sources_refs = group.openOptGroup("sources");
}


EntityWithSourcesHDF5::EntityWithSourcesHDF5(const shared_ptr<IFile> &file, const shared_ptr<IBlock> &block, Group group, const string &id,
                                             const string &type, const string &name)
    : EntityWithSourcesHDF5(file, block, group, id, type, name, util::getTime())
{
}


EntityWithSourcesHDF5::EntityWithSourcesHDF5 (const shared_ptr<IFile> &file, const shared_ptr<IBlock> &block, Group group, const string &id,
                                              const string &type, const string &name, time_t time)
    : EntityWithMetadataHDF5(file, group, id, type, name, time), entity_block(block)
{
    sources_refs = group.openOptGroup("sources");
}


size_t EntityWithSourcesHDF5::sourceCount() const {
    boost::optional<Group> g = sources_refs();
    return g ? g->objectCount() : size_t(0);
}


bool EntityWithSourcesHDF5::hasSource(const string &id) const {
    return sources_refs() ? sources_refs()->hasGroup(id) : false;
}


shared_ptr<ISource> EntityWithSourcesHDF5::getSource(const string &id) const {
    shared_ptr<SourceHDF5> source;
    boost::optional<Group> g = sources_refs();

    if (g && hasSource(id)) {
        Group group = g->openGroup(id);
        source = make_shared<SourceHDF5>(file(), group);
    }

    return source;
}

shared_ptr<ISource> EntityWithSourcesHDF5::getSource(const size_t index) const {
    boost::optional<Group> g = sources_refs();
    string id = g ? g->objectName(index) : "";
    return getSource(id);
}

void EntityWithSourcesHDF5::sources(const std::vector<Source> &sources) {
    // extract vectors of ids from vectors of new & old sources
    std::vector<std::string> ids_new(sources.size());
    transform(sources.begin(), sources.end(), ids_new.begin(), util::toId<Source>);
    std::vector<Source> sources_old(sourceCount());
    for (size_t i = 0; i < sources_old.size(); i++) sources_old[i] = getSource(i);
    std::vector<std::string> ids_old(sources_old.size());
    transform(sources_old.begin(), sources_old.end(), ids_old.begin(), util::toId<Source>);
    // sort them
    std::sort(ids_new.begin(), ids_new.end());
    std::sort(ids_new.begin(), ids_new.end());
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
    
void EntityWithSourcesHDF5::addSource(const string &id) {
    if (id.empty())
        throw EmptyString("addSource");
    boost::optional<Group> g = sources_refs(true);

    Block tmp(entity_block);
    auto found = tmp.findSources(util::IdFilter<Source>(id));
    if (found.empty())
        throw std::runtime_error("EntityWithSourcesHDF5::addSource: Given source does not exist in this block!");

    auto target = dynamic_pointer_cast<SourceHDF5>(found.front().impl());

    g->createLink(target->group(), id);
}


bool EntityWithSourcesHDF5::removeSource(const string &id) {
    boost::optional<Group> g = sources_refs();
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

