// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/util/util.hpp>
#include <nix/util/filter.hpp>
#include <nix/hdf5/EntityWithMetadataHDF5.hpp>

using namespace std;
using namespace nix::util;

namespace nix {
namespace hdf5 {


EntityWithMetadataHDF5::EntityWithMetadataHDF5(File file, Group group, const string &id, const string &type, const string &name)
    : EntityWithMetadataHDF5(file, group, id, type, name, util::getTime())
{
}


EntityWithMetadataHDF5::EntityWithMetadataHDF5(File file, Group group, const string &id, const string &type, const string &name, time_t time)
    : NamedEntityHDF5(file, group, id, type, name, time)
{
}


void EntityWithMetadataHDF5::metadata(const std::string &id) {
    vector<Section> found = file().findSections(IdFilter<Section>(id));
    if (found.size() == 0) {
        throw runtime_error("EntityWithMetadataHDF5::metadata: cannot set metadata because Section does not exist in this file!");
    } else {
        group().setAttr("metadata", id);
        forceUpdatedAt();
    }
}


Section EntityWithMetadataHDF5::metadata() const {
    if (group().hasAttr("metadata")) {
        std::string sectionId;
        group().getAttr("metadata", sectionId);
        return file().getSection(sectionId);
    }
    else {
        return nix::Section();
    }
}



void EntityWithMetadataHDF5::metadata(const none_t t) {
    if(group().hasAttr("metadata")) {
        group().removeAttr("metadata");
    }
}


EntityWithMetadataHDF5::~EntityWithMetadataHDF5() {

}


} // namespace hdf5
} // namespace nix
