// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/util/util.hpp>
#include <nix/hdf5/EntityWithSourcesHDF5.hpp>


using namespace std;
using namespace nix;
using namespace nix::hdf5;
using namespace nix::base;


EntityWithSourcesHDF5::EntityWithSourcesHDF5(shared_ptr<IFile> file, Block block, Group group, const string &id,
                                             const string &type, const string &name)
    : EntityWithSourcesHDF5(file, block, group, id, type, name, util::getTime())
{
}


EntityWithSourcesHDF5::EntityWithSourcesHDF5 (shared_ptr<IFile> file, Block block, Group group, const string &id,
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

    vector<Source> result_vect = entity_block.findSources(util::IdFilter<Source>(id));
    if(!result_vect.empty())
        source = result_vect.back().impl();

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

void EntityWithSourcesHDF5::addSource(const std::string &id) {
    util::IdFilter<Source> checkID(id);
    vector<Source> result_vect = entity_block.findSources(checkID);
    if(!result_vect.empty()) {
        sources_refs.add(id);
    }
    else {
        throw std::runtime_error("Given source does not exist in this block!");
    }
}


void EntityWithSourcesHDF5::sources(const vector<Source> &s) {
    // gather IDs
    vector<string> ids(s.size());
    for (size_t i = 0; i < s.size(); i++) {
        ids[i] = s[i].id();
    }
    // reduce them to those actually existing in block
    util::IdsFilter<Source> checkIDs(ids);
    vector<Source> result_vect = entity_block.findSources(checkIDs);
    // set remaining IDs
    if(!result_vect.empty()) {
        ids.resize(result_vect.size());
        for (size_t i = 0; i < result_vect.size(); i++) {
            ids[i] = result_vect[i].id();
        }
        sources_refs.set(ids);
    }
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

