// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/util/util.hpp>
#include <nix/Block.hpp>
#include <nix/hdf5/BlockHDF5.hpp>
#include <nix/hdf5/SourceHDF5.hpp>
#include <nix/hdf5/DataArrayHDF5.hpp>
#include <nix/hdf5/TagHDF5.hpp>
#include <nix/hdf5/MultiTagHDF5.hpp>

#include <boost/range/irange.hpp>

using namespace std;
using namespace nix;
using namespace nix::hdf5;
using namespace nix::base;


BlockHDF5::BlockHDF5(std::shared_ptr<base::IFile> file, Group group)
    : EntityWithMetadataHDF5(file, group)
{
    data_array_group = group.openGroup("data_arrays", false);
    tag_group = group.openGroup("tags", false);
    multi_tag_group = group.openGroup("multi_tags", false);
    source_group = group.openOptGroup("sources");
}

BlockHDF5::BlockHDF5(shared_ptr<IFile> file, Group group, const string &id, const string &type, const string &name)
    : BlockHDF5(file, group, id, type, name, util::getTime())
{
}


BlockHDF5::BlockHDF5(shared_ptr<IFile> file, Group group, const string &id, const string &type, const string &name, time_t time)
    : EntityWithMetadataHDF5(file, group, id, type, name, time)
{
    data_array_group = group.openGroup("data_arrays", true);
    tag_group = group.openGroup("tags", true);
    multi_tag_group = group.openGroup("multi_tags", true);
    source_group = group.openOptGroup("sources");
}


//--------------------------------------------------
// Methods concerning sources
//--------------------------------------------------


bool BlockHDF5::hasSource(const string &id) const {
    boost::optional<Group> g = source_group(false);
    // let getSource try to look up object by id
    return g ? (getSource(id) != nullptr) : false;
}


bool BlockHDF5::hasSourceByName(const string &name) const {
    boost::optional<Group> g = source_group(false);
    // let getTag try to look up object by id
    return g ? (getSourceByName(name) != nullptr) : false;
}


shared_ptr<ISource> BlockHDF5::getSourceByName(const string &name) const {
    shared_ptr<SourceHDF5> source;
    boost::optional<Group> g = source_group(false);

    if(g) {
        if (g->hasObject(name)) {
            Group group = g->openGroup(name, false);
            source = make_shared<SourceHDF5>(file(), group);
        }
    }

    return source;
}


shared_ptr<ISource> BlockHDF5::getSource(const string &id) const {
    shared_ptr<SourceHDF5> source;
    boost::optional<Group> g = source_group(false);

    if(g) {
        boost::optional<Group> group = g->findGroupByAttribute("entity_id", id);
        if (group)
            source = make_shared<SourceHDF5>(file(), *group);
    }

    return source;
}


shared_ptr<ISource> BlockHDF5::getSource(size_t index) const {
    boost::optional<Group> g = source_group(false);
    string name = g ? (*g).objectName(index) : "";
    return getSourceByName(name);
}


size_t BlockHDF5::sourceCount() const {
    boost::optional<Group> g = source_group(false);
    return g ? (*g).objectCount() : size_t(0);
}


shared_ptr<ISource> BlockHDF5::createSource(const string &name, const string &type) {
    if (hasSourceByName(name)) {
        throw DuplicateName("createSource");
    }
    boost::optional<Group> g = source_group(true);

    string id = util::createId("source");

    Group group = g->openGroup(name, true);
    return make_shared<SourceHDF5>(file(), group, id, type, name);
}


bool BlockHDF5::deleteSource(const string &id) {
    boost::optional<Group> g = source_group(false);
    
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
            g->removeAllLinks(source.name());
        }
    }

    return hasSource(id);
}


// Methods related to Tag

shared_ptr<ITag> BlockHDF5::createTag(const std::string &name, const std::string &type,
                                      const std::vector<double> &position) {
    if (hasTagByName(name)) {
        throw DuplicateName("createTag");
    }
    string id = util::createId("tag");

    Group group = tag_group.openGroup(name);
    return make_shared<TagHDF5>(file(), block(), group, id, type, name, position);
}


bool BlockHDF5::hasTag(const string &id) const {
    // let getTag try to look up object by id    
    return getTag(id) != nullptr;
}


bool BlockHDF5::hasTagByName(const string &name) const {
    // let getTagByName try to look up object by name    
    return getTagByName(name) != nullptr;
}


shared_ptr<ITag> BlockHDF5::getTagByName(const string &name) const {
    shared_ptr<TagHDF5> da;
    
    if (tag_group.hasObject(name)) {
        Group group = tag_group.openGroup(name, false);
        da = make_shared<TagHDF5>(file(), block(), group);
    }
    
    return da;
}


shared_ptr<ITag> BlockHDF5::getTag(const string &id) const {
    shared_ptr<TagHDF5> da;

    boost::optional<Group> group = tag_group.findGroupByAttribute("entity_id", id);
    if (group)
        da = make_shared<TagHDF5>(file(), block(), *group);

    return da;
}


shared_ptr<ITag> BlockHDF5::getTag(size_t index) const {
    string name = tag_group.objectName(index);
    return getTagByName(name);
}


size_t BlockHDF5::tagCount() const{
    return tag_group.objectCount();
}


bool BlockHDF5::deleteTag(const std::string &id) {
    bool deleted = false;

    if (hasTag(id)) {
        // we get first "entity" link by name, but delete all others whatever their name with it
        deleted = tag_group.removeAllLinks(getTag(id)->name());
    }

    return deleted;
}


// Methods related to DataArray

bool BlockHDF5::hasDataArrayByName(const string &name) const {
    // let getDataArrayByName try to look up object by name    
    return getDataArrayByName(name) != nullptr;
}

bool BlockHDF5::hasDataArray(const string &id) const {
    // let getDataArray try to look up object by id    
    return getDataArray(id) != nullptr;
}


shared_ptr<IDataArray> BlockHDF5::getDataArrayByName(const string &name) const {
    shared_ptr<DataArrayHDF5> da;
    
    if (data_array_group.hasObject(name)) {
        Group group = data_array_group.openGroup(name, false);
        da = make_shared<DataArrayHDF5>(file(), block(), group);
    }
    
    return da;
}


shared_ptr<IDataArray> BlockHDF5::getDataArray(const string &id) const {
    shared_ptr<DataArrayHDF5> da;

    boost::optional<Group> group = data_array_group.findGroupByAttribute("entity_id", id);
    if (group)
        da = make_shared<DataArrayHDF5>(file(), block(), *group);

    return da;
}


shared_ptr<IDataArray> BlockHDF5::getDataArray(size_t index) const {
    string name = data_array_group.objectName(index);
    return getDataArrayByName(name);
}


size_t BlockHDF5::dataArrayCount() const {
    return data_array_group.objectCount();
}


shared_ptr<IDataArray> BlockHDF5::createDataArray(const std::string &name,
                                     const std::string &type,
                                     nix::DataType      data_type,
                                     const NDSize      &shape) {
    if (hasDataArrayByName(name)) {
        throw DuplicateName("createDataArray");
    }
    string id = util::createId("data_array");

    Group group = data_array_group.openGroup(name, true);
    auto da = make_shared<DataArrayHDF5>(file(), block(), group, id, type, name);

    // now create the actual H5::DataSet
    da->createData(data_type, shape);

    return da;
}


bool BlockHDF5::deleteDataArray(const string &id) {
    bool deleted = false;

    if (hasDataArray(id)) {
        // we get first "entity" link by name, but delete all others whatever their name with it
        deleted = data_array_group.removeAllLinks(getDataArray(id)->name());
    }

    return deleted;
}


// Methods related to MultiTag

shared_ptr<IMultiTag> BlockHDF5::createMultiTag(const std::string &name, const std::string &type,
                                              const DataArray &positions) {
    if (hasMultiTagByName(name)) {
        throw DuplicateName("createMultiTag");
    }
    string id = util::createId("multi_tag");

    Group group = multi_tag_group.openGroup(name);
    return make_shared<MultiTagHDF5>(file(), block(), group, id, type, name, positions);
}


bool BlockHDF5::hasMultiTag(const string &id) const {
    // let getMultiTag try to look up object by id    
    return getMultiTag(id) != nullptr;
}


bool BlockHDF5::hasMultiTagByName(const string &name) const {
    // let getMultiTagByName try to look up object by name    
    return getMultiTagByName(name) != nullptr;
}


shared_ptr<IMultiTag> BlockHDF5::getMultiTagByName(const string &name) const {
    shared_ptr<MultiTagHDF5> da;
    
    if (multi_tag_group.hasObject(name)) {
        Group group = multi_tag_group.openGroup(name, false);
        da = make_shared<MultiTagHDF5>(file(), block(), group);
    }
    
    return da;
}


shared_ptr<IMultiTag> BlockHDF5::getMultiTag(const string &id) const {
    shared_ptr<MultiTagHDF5> da;

    boost::optional<Group> group = multi_tag_group.findGroupByAttribute("entity_id", id);
    if (group)
        da = make_shared<MultiTagHDF5>(file(), block(), *group);

    return da;
}


shared_ptr<IMultiTag> BlockHDF5::getMultiTag(size_t index) const {
    string name = multi_tag_group.objectName(index);
    return getMultiTagByName(name);
}


size_t BlockHDF5::multiTagCount() const{
    return multi_tag_group.objectCount();
}


bool BlockHDF5::deleteMultiTag(const std::string &id) {
    bool deleted = false;

    if (hasMultiTag(id)) {
        // we get first "entity" link by name, but delete all others whatever their name with it
        deleted = multi_tag_group.removeAllLinks(getMultiTag(id)->name());
    }

    return deleted;
}


shared_ptr<IBlock> BlockHDF5::block() const {
    return const_pointer_cast<BlockHDF5>(shared_from_this());
}


BlockHDF5::~BlockHDF5() {}

