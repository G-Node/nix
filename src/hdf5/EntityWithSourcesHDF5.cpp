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

namespace nix {
namespace hdf5 {


EntityWithSourcesHDF5::EntityWithSourcesHDF5(File file, Block block, Group group, const string &id)
    : EntityWithMetadataHDF5(file, group, id), entity_block(block), sources_refs(group, "sources")
{
}


EntityWithSourcesHDF5::EntityWithSourcesHDF5(File file, Block block, Group group, const string &id, time_t time)
    : EntityWithMetadataHDF5(file, group, id, time), entity_block(block), sources_refs(group, "sources")
{
}


size_t EntityWithSourcesHDF5::sourceCount() const {
    return sources_refs.count();
}


bool EntityWithSourcesHDF5::hasSource(const string &id) const {
    return sources_refs.has(id);
}


Source EntityWithSourcesHDF5::getSource(const string &id) const {
    if (hasSource(id)) {
        util::IdFilter<Source> checkID(id);
        vector<Source> result_vect = entity_block.findSources(checkID);
        if(!result_vect.empty()) return result_vect.back();
    }
    // if above block did not return
    return nix::Source();
}

Source EntityWithSourcesHDF5::getSource(const size_t index) const {
    std::vector<std::string> refs = sources_refs.get();
    std::string id;
    
    // get reference id
    if(index < refs.size()) {
        id = refs[index];
    } else {
        throw OutOfBounds("No data array at given index", index);
    }
    if (hasSource(id)) {
        util::IdFilter<Source> checkID(id);
        vector<Source> result_vect = entity_block.findSources(checkID);
        if(!result_vect.empty()) return result_vect.back();
    }
    // if above block did not return
    return nix::Source();
}

void EntityWithSourcesHDF5::addSource(const std::string &id) {
    sources_refs.add(id);
}

/**
 * Add a Source to this entity.
 *
 * @param Source    The source.
 */
void EntityWithSourcesHDF5::addSource(const Source &source) {
    util::IdFilter<Source> checkID(source.id());
    vector<Source> result_vect = entity_block.findSources(checkID);
    if(!result_vect.empty()) {
        sources_refs.add(source.id());
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


} // namespace hdf5
} // namespace nix
