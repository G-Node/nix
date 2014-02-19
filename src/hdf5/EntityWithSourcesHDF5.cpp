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


bool EntityWithSourcesHDF5::hasSource(const Source &source) const {
    return sources_refs.has(source.id());
}


bool EntityWithSourcesHDF5::hasSource(const string &id) const {
    return sources_refs.has(id);
}


Source EntityWithSourcesHDF5::getSource(const string &id) const{
    return entity_block.getSource(id);
}

Source EntityWithSourcesHDF5::getSource(const size_t index) const{
    return entity_block.getSource(index);
}

void EntityWithSourcesHDF5::addSource(const Source &source) {
    sources_refs.add(source.id());
}


void EntityWithSourcesHDF5::sources(const vector<Source> &s) {
    vector<string> ids(s.size());
    for (size_t i = 0; i < s.size(); i++) {
        ids[i] = s[i].id();
    }
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


} // namespace hdf5
} // namespace nix
