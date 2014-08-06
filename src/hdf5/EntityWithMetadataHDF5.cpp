// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/util/util.hpp>
#include <nix/util/filter.hpp>
#include <nix/File.hpp>
#include <nix/hdf5/EntityWithMetadataHDF5.hpp>

using namespace std;
using namespace nix;
using namespace nix::hdf5;
using namespace nix::base;


EntityWithMetadataHDF5::EntityWithMetadataHDF5(shared_ptr<IFile> file, Group group, const string &id, const string &type, const string &name)
    : EntityWithMetadataHDF5(file, group, id, type, name, util::getTime())
{
}


EntityWithMetadataHDF5::EntityWithMetadataHDF5(shared_ptr<IFile> file, Group group, const string &id, const string &type, const string &name, time_t time)
    : NamedEntityHDF5(file, group, id, type, name, time)
{
}


void EntityWithMetadataHDF5::metadata(const std::string &id) {
    File tmp = file();
    auto  found = tmp.findSections(util::IdFilter<Section>(id));
    if (found.empty()) {
        throw runtime_error("EntityWithMetadataHDF5::metadata: cannot set metadata because Section does not exist in this file!");
    } else {
        group().setAttr("metadata", id);
        forceUpdatedAt();
    }
}


shared_ptr<ISection> EntityWithMetadataHDF5::metadata() const {
    shared_ptr<ISection> section;

    if (group().hasAttr("metadata")) {
        std::string sectionId;
        group().getAttr("metadata", sectionId);
        section = file()->getSection(sectionId);
    }

    return section;
}



void EntityWithMetadataHDF5::metadata(const none_t t) {
    if (group().hasAttr("metadata")) {
        group().removeAttr("metadata");
    }
}


EntityWithMetadataHDF5::~EntityWithMetadataHDF5() {}

