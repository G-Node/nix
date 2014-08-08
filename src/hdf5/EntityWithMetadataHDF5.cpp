// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <memory>

#include <nix/util/util.hpp>
#include <nix/util/filter.hpp>
#include <nix/File.hpp>
#include <nix/hdf5/SectionHDF5.hpp>
#include <nix/hdf5/EntityWithMetadataHDF5.hpp>

using namespace std;
using namespace nix;
using namespace nix::hdf5;
using namespace nix::base;


EntityWithMetadataHDF5::EntityWithMetadataHDF5(shared_ptr<IFile> file, Group group)
    : NamedEntityHDF5(file, group)
{
}


EntityWithMetadataHDF5::EntityWithMetadataHDF5(shared_ptr<IFile> file, Group group, const string &id, const string &type, const string &name)
    : EntityWithMetadataHDF5(file, group, id, type, name, util::getTime())
{
}


EntityWithMetadataHDF5::EntityWithMetadataHDF5(shared_ptr<IFile> file, Group group, const string &id, const string &type, const string &name, time_t time)
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
        Group       other_group = group().openGroup("metadata", false);
        std::string other_id;
        other_group.getAttr("metadata", other_id);
        
        auto found = File(file()).findSections(util::IdFilter<Section>(other_id));
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

