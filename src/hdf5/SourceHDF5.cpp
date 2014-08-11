// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/util/util.hpp>
#include <nix/hdf5/SourceHDF5.hpp>
#include <nix/Source.hpp>

using namespace std;
using namespace nix;
using namespace nix::hdf5;
using namespace nix::base;


SourceHDF5::SourceHDF5(std::shared_ptr<base::IFile> file, Group group)
    : EntityWithMetadataHDF5(file, group)
{
    source_group = group.openGroup("sources", false);
}
    
    
SourceHDF5::SourceHDF5(shared_ptr<IFile> file, Group group, const std::string &id, const string &type, const string &name)
    : SourceHDF5(file, group, id, type, name, util::getTime())
{
}


SourceHDF5::SourceHDF5(shared_ptr<IFile> file, Group group, const std::string &id, const string &type, const string &name, time_t time)
    : EntityWithMetadataHDF5(file, group, id, type, name, time)
{
    source_group = group.openGroup("sources", true);
}


bool SourceHDF5::hasSource(const string &id) const {
    return source_group.hasGroup(id);
}


shared_ptr<ISource> SourceHDF5::getSource(const string &id) const {
    shared_ptr<ISource> source;

    if (source_group.hasGroup(id)) {
        Group group = source_group.openGroup(id, false);
        source = make_shared<SourceHDF5>(file(), group);
    }

    return source;
}


shared_ptr<ISource> SourceHDF5::getSource(size_t index) const {
    string id = source_group.objectName(index);
    // all checks done by "getSource(const string &id)"
    return getSource(id);
}


size_t SourceHDF5::sourceCount() const {
    return source_group.objectCount();
}


shared_ptr<ISource> SourceHDF5::createSource(const string &name, const string &type) {
    string id = util::createId("source");
    while (source_group.hasObject(id)) {
        id = util::createId("source");
    }

    Group grp = source_group.openGroup(id, true);
    return make_shared<SourceHDF5>(file(), grp, id, type, name);
}


bool SourceHDF5::deleteSource(const string &id) {
    // call deleteSource on sources to trigger recursive call to all sub-sources
    if (hasSource(id)) {
        // get instance of source about to get deleted
        Source source = getSource(id);
        // loop through all child sources and call deleteSource on them
        for(auto &child : source.sources()) {
            child.deleteSource(child.id());
        }
        source_group.removeAllLinks(id);
    }
    
    return hasSource(id);
}


SourceHDF5::~SourceHDF5() {}

