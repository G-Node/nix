// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/util/util.hpp>
#include <nix/hdf5/EntityWithMetadataHDF5.hpp>

using namespace std;

namespace nix {
namespace hdf5 {


EntityWithMetadataHDF5::EntityWithMetadataHDF5(File file, Group group, const string &id)
    : NamedEntityHDF5(file, group, id)
{
}


EntityWithMetadataHDF5::EntityWithMetadataHDF5(File file, Group group, const string &id, time_t time)
    : NamedEntityHDF5(file, group, id, time)
{
}


void EntityWithMetadataHDF5::metadata(const Section &metadata) {
    if (metadata == none) {
        EntityWithMetadataHDF5::metadata(none);
    }
    else {
        if (!file().hasSection(metadata.id())) {
            throw runtime_error("EntityWithMetadataHDF5::metadata: cannot set metadata because Section does not exist in this file!");
        } else {
            group().setAttr("metadata", metadata.id());
            forceUpdatedAt();
        }
    }
}


Section EntityWithMetadataHDF5::metadata() const{
    if(!hasMetadata()) {
        throw runtime_error("EntityWithMetadataHDF5::metadata: This entity does not reference metadata!");
    }
    std::string sectionId;
    group().getAttr("metadata", sectionId);
    return file().getSection(sectionId);
}



void EntityWithMetadataHDF5::metadata(const none_t t) {
    if(group().hasAttr("metadata")) {
        group().removeAttr("metadata");
    }
}


bool EntityWithMetadataHDF5::hasMetadata() const{
    std::string sectionId;
    group().getAttr("metadata", sectionId);
    return (sectionId.length() > 0);
}


EntityWithMetadataHDF5::~EntityWithMetadataHDF5() {

}


} // namespace hdf5
} // namespace nix
