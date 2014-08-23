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


SourceHDF5::SourceHDF5(const std::shared_ptr<IFile> &file, const Group &group)
    : EntityWithMetadataHDF5(file, group)
{
    source_group = this->group().openOptGroup("sources");
}
    
    
SourceHDF5::SourceHDF5(const shared_ptr<IFile> &file, const Group &group, const std::string &id, const string &type, const string &name)
    : SourceHDF5(file, group, id, type, name, util::getTime())
{
}


SourceHDF5::SourceHDF5(const shared_ptr<IFile> &file, const Group &group, const std::string &id, const string &type, const string &name, time_t time)
    : EntityWithMetadataHDF5(file, group, id, type, name, time)
{
    source_group = this->group().openOptGroup("sources");
}


bool SourceHDF5::hasSource(const string &id) const {
    // let getSource try to look up object by id
    return getSource(id) != nullptr;
}


bool SourceHDF5::hasSourceByName(const string &name) const {
    // let getTag try to look up object by id
    return getSourceByName(name) != nullptr;
}


shared_ptr<ISource> SourceHDF5::getSourceByName(const string &name) const {
    shared_ptr<SourceHDF5> source;
    boost::optional<Group> g = source_group();

    if (g) {
        if (g->hasObject(name)) {
            Group group = g->openGroup(name);
            source = make_shared<SourceHDF5>(file(), group);
        }
    }

    return source;
}


shared_ptr<ISource> SourceHDF5::getSource(const string &id) const {
    shared_ptr<SourceHDF5> source;
    boost::optional<Group> g = source_group();

    if (g) {
        boost::optional<Group> group = g->findGroupByAttribute("entity_id", id);
        if (group)
            source = make_shared<SourceHDF5>(file(), *group);
    }

    return source;
}


shared_ptr<ISource> SourceHDF5::getSource(size_t index) const {
    boost::optional<Group> g = source_group();
    string name = g ? g->objectName(index) : "";
    return getSourceByName(name);
}


size_t SourceHDF5::sourceCount() const {
    boost::optional<Group> g = source_group(false);
    return g ? g->objectCount() : size_t(0);
}


shared_ptr<ISource> SourceHDF5::createSource(const string &name, const string &type) {
    if (name.empty()) {
        throw EmptyString("name");
    }
    if (hasSourceByName(name)) {
        throw DuplicateName("createSource");
    }
    string id = util::createId();
    boost::optional<Group> g = source_group(true);


    Group group = g->openGroup(name, true);
    return make_shared<SourceHDF5>(file(), group, id, type, name);
}


bool SourceHDF5::deleteSource(const string &id) {
    boost::optional<Group> g = source_group();
    bool deleted = false;
    
    if(g) {
        // call deleteSource on sources to trigger recursive call to all sub-sources
        if (hasSource(id)) {
            // get instance of source about to get deleted
            Source source = getSource(id);
            // loop through all child sources and call deleteSource on them
            for(auto &child : source.sources()) {
                source.deleteSource(child.id());
            }
            // if hasSource is true then source_group always exists
            deleted = g->removeAllLinks(source.name());
        }
    }

    return deleted;
}


SourceHDF5::~SourceHDF5() {}

