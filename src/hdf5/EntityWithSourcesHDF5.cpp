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


EntityWithSourcesHDF5::EntityWithSourcesHDF5(shared_ptr<IFile> file, shared_ptr<IBlock> block, Group group, const string &id,
                                             const string &type, const string &name)
    : EntityWithSourcesHDF5(file, block, group, id, type, name, util::getTime())
{
}


EntityWithSourcesHDF5::EntityWithSourcesHDF5 (shared_ptr<IFile> file, shared_ptr<IBlock> block, Group group, const string &id,
                                              const string &type, const string &name, time_t time)
    : EntityWithMetadataHDF5(file, group, id, type, name, time), entity_block(block), sources_refs(group, "sources")
{
}


size_t EntityWithSourcesHDF5::sourceCount() const {
    return sources_refs.count();
}


bool EntityWithSourcesHDF5::hasSource(const string &id) const {
    return sources_refs.has(id);
}


shared_ptr<ISource> EntityWithSourcesHDF5::getSource(const string &id) const {
    shared_ptr<ISource> source;

    Block tmp(entity_block);
    auto found = tmp.findSources(util::IdFilter<Source>(id));
    if(!found.empty())
        source = found.back().impl();

    return source;
}

shared_ptr<ISource> EntityWithSourcesHDF5::getSource(const size_t index) const {
    shared_ptr<ISource> source;

    std::vector<std::string> refs = sources_refs.get();

    // get reference id
    if(index < refs.size()) {
        string id = refs[index];
        source = getSource(id);
    } else {
        throw OutOfBounds("No data array at given index", index);
    }

    return source;
}

void EntityWithSourcesHDF5::addSource(const string &id) {
    Block tmp(entity_block);
    auto found = tmp.findSources(util::IdFilter<Source>(id));

    if (found.empty())
        throw std::runtime_error("Given source does not exist in this block!");

    sources_refs.add(id);
}


void EntityWithSourcesHDF5::sources(const vector<Source> &sources) {
    vector<string> ids(sources.size());
    transform(sources.begin(), sources.end(), ids.begin(), [](const Source &src) -> string { return src.id(); });

    Block tmp(entity_block);
    auto found = tmp.findSources( util::IdsFilter<Source>(ids));
    if (sources.size() != found.size())
        throw std::runtime_error("One or more sources do not exist in this block!");

    sources_refs.set(ids);
}


bool EntityWithSourcesHDF5::removeSource(const string &id) {
    return sources_refs.remove(id);
}


void EntityWithSourcesHDF5::swap(EntityWithSourcesHDF5 &other) {
    using std::swap;

    EntityWithMetadataHDF5::swap(other);
    swap(entity_block, other.entity_block);
    swap(sources_refs, other.sources_refs);
}


EntityWithSourcesHDF5::~EntityWithSourcesHDF5() {}

