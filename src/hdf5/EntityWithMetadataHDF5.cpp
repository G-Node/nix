// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/hdf5/EntityWithMetadataHDF5.hpp>

#include <nix/util/util.hpp>
#include <nix/util/filter.hpp>
#include <nix/File.hpp>
#include <nix/hdf5/SectionHDF5.hpp>

#include <memory>

using namespace std;
using namespace nix::base;

namespace nix {
namespace hdf5 {

EntityWithMetadataHDF5::EntityWithMetadataHDF5(const shared_ptr<IFile> &file, const Group &group)
    : NamedEntityHDF5(file, group)
{
}


EntityWithMetadataHDF5::EntityWithMetadataHDF5(const shared_ptr<IFile> &file, const Group &group, const string &id, const string &type, const string &name)
    : EntityWithMetadataHDF5(file, group, id, type, name, util::getTime())
{
}


EntityWithMetadataHDF5::EntityWithMetadataHDF5(const shared_ptr<IFile> &file, const Group &group, const string &id, const string &type, const string &name, time_t time)
    : NamedEntityHDF5(file, group, id, type, name, time)
{
}


void EntityWithMetadataHDF5::metadata(const std::string &id) {
    if (id.empty())
        throw EmptyString("metadata");

    if (group().hasGroup("metadata"))
        metadata(none);
        
    File tmp = file();
    auto found = tmp.findSections(util::IdFilter<Section>(id));
    if (found.empty())
        throw std::runtime_error("EntityWithMetadataHDF5::metadata: Section not found in file!");
    
    auto target = dynamic_pointer_cast<SectionHDF5>(found.front().impl());

    group().createLink(target->group(), "metadata");
}


shared_ptr<ISection> EntityWithMetadataHDF5::metadata() const {
    shared_ptr<ISection> sec;

    if (group().hasGroup("metadata")) {
        Group other_group = group().openGroup("metadata", false);
        auto sec_tmp = make_shared<EntityWithMetadataHDF5>(file(), other_group);
        // re-get above section "sec_tmp": we just got it to have id, parent is missing, 
        // findSections will return it with parent!
        auto found = File(file()).findSections(util::IdFilter<Section>(sec_tmp->id()));
        if (found.size() > 0) {
            sec = found.front().impl();
        }
    }

    return sec;
}


void EntityWithMetadataHDF5::metadata(const none_t t) {
    if (group().hasGroup("metadata")) {
        group().removeGroup("metadata");
    }
    forceUpdatedAt();
}


EntityWithMetadataHDF5::~EntityWithMetadataHDF5() {}

} // ns nix::hdf5
} // ns nix