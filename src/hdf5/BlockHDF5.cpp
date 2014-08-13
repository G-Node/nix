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

using namespace std;
using namespace nix;
using namespace nix::hdf5;
using namespace nix::base;


BlockHDF5::BlockHDF5(std::shared_ptr<base::IFile> file, Group group)
    : EntityWithMetadataHDF5(file, group)
{
    source_group = group.openGroup("sources", false);
    data_array_group = group.openGroup("data_arrays", false);
    tag_group = group.openGroup("tags", false);
    data_tag_group = group.openGroup("data_tags", false);
}
    
BlockHDF5::BlockHDF5(shared_ptr<IFile> file, Group group, const string &id, const string &type, const string &name)
    : BlockHDF5(file, group, id, type, name, util::getTime())
{
}


BlockHDF5::BlockHDF5(shared_ptr<IFile> file, Group group, const string &id, const string &type, const string &name, time_t time)
    : EntityWithMetadataHDF5(file, group, id, type, name, time)
{
    source_group = group.openGroup("sources", true);
    data_array_group = group.openGroup("data_arrays", true);
    tag_group = group.openGroup("tags", true);
    data_tag_group = group.openGroup("data_tags", true);
}


//--------------------------------------------------
// Methods concerning sources
//--------------------------------------------------


bool BlockHDF5::hasSource(const string &id) const {
    return source_group.hasGroup(id);
}


shared_ptr<ISource> BlockHDF5::getSource(const string &id) const {
    shared_ptr<SourceHDF5> source;

    if (hasSource(id)) {
        Group group = source_group.openGroup(id, false);
        source = make_shared<SourceHDF5>(file(), group);
    }

    return source;
}


shared_ptr<ISource> BlockHDF5::getSource(size_t index) const {
    string id = source_group.objectName(index);
    return getSource(id);
}


size_t BlockHDF5::sourceCount() const {
    return source_group.objectCount();
}


shared_ptr<ISource> BlockHDF5::createSource(const string &name, const string &type) {
    string id = util::createId("source");
    while (source_group.hasObject(id)) {
        id = util::createId("source");
    }

    Group group = source_group.openGroup(id, true);
    return make_shared<SourceHDF5>(file(), group, id, type, name);
}


bool BlockHDF5::deleteSource(const string &id) {
    // call deleteSource on sources to trigger recursive call to all sub-sources
    if (hasSource(id)) {
        // get instance of source about to get deleted
        Source source = getSource(id);
        // loop through all child sources and call deleteSource on them
        for(auto &child : source.sources()) {
            source.deleteSource(child.id());
        }
        source_group.removeAllLinks(id);
    }
    
    return hasSource(id);
}


// Tag methods

bool BlockHDF5::hasTag(const string &id) const {
    return tag_group.hasObject(id);
}


shared_ptr<ITag> BlockHDF5::getTag(const string &id) const {
    shared_ptr<TagHDF5> tag;

    if (hasTag(id)) {
        Group tag_group = tag_group.openGroup(id, false);
        tag = make_shared<TagHDF5>(file(), block(), tag_group);
    }

    return tag;
}


shared_ptr<ITag> BlockHDF5::getTag(size_t index) const {
    string id = tag_group.objectName(index);
    return getTag(id);
}


size_t BlockHDF5::tagCount() const {
    return tag_group.objectCount();
}


shared_ptr<ITag> BlockHDF5::createTag(const string &name, const string &type,
                                                  const std::vector<double> &position) {
    string id = util::createId("tag");
    while (hasTag(id)) {
        id = util::createId("tag");
    }

    Group group = tag_group.openGroup(id, true);
    return make_shared<TagHDF5>(file(), block(), group, id, type, name, position);
}


bool BlockHDF5::deleteTag(const string &id) {
    bool deleted = false;

    if (hasTag(id)) {
        tag_group.removeGroup(id);
        deleted = true;
    }

    return deleted;
}


// Methods related to DataArray

bool BlockHDF5::hasDataArray(const string &id) const {
    return data_array_group.hasObject(id);
}


shared_ptr<IDataArray> BlockHDF5::getDataArray(const string &id) const {
    shared_ptr<DataArrayHDF5> da;

    if (hasDataArray(id)) {
        Group group = data_array_group.openGroup(id, false);
        da = make_shared<DataArrayHDF5>(file(), block(), group);
    }

    return da;
}


shared_ptr<IDataArray> BlockHDF5::getDataArray(size_t index) const {
    string id = data_array_group.objectName(index);
    return getDataArray(id);
}


size_t BlockHDF5::dataArrayCount() const {
    return data_array_group.objectCount();
}


shared_ptr<IDataArray> BlockHDF5::createDataArray(const std::string &name,
                                     const std::string &type,
                                     nix::DataType      data_type,
                                     const NDSize      &shape) {
    string id = util::createId("data_array");
    while (hasDataArray(id)) {
        id = util::createId("data_array");
    }

    Group group = data_array_group.openGroup(id, true);
    auto da = make_shared<DataArrayHDF5>(file(), block(), group, id, type, name);

    // now create the actual H5::DataSet
    da->createData(data_type, shape);

    return da;
}


bool BlockHDF5::deleteDataArray(const string &id) {
    bool deleted = false;

    if (hasDataArray(id)) {
        data_array_group.removeAllLinks(id);
    }

    return deleted;
}


// Methods related to MultiTag

shared_ptr<IMultiTag> BlockHDF5::createMultiTag(const std::string &name, const std::string &type,
                                              const DataArray &positions) {
    string id = util::createId("data_tag");
    while (hasMultiTag(id)) {
        id = util::createId("data_tag");
    }

    Group group = data_tag_group.openGroup(id);
    return make_shared<MultiTagHDF5>(file(), block(), group, id, type, name, positions);
}


bool BlockHDF5::hasMultiTag(const std::string &id) const {
    return data_tag_group.hasObject(id);
}


shared_ptr<IMultiTag> BlockHDF5::getMultiTag(const std::string &id) const {
    shared_ptr<MultiTagHDF5> tag;

    if (hasMultiTag(id)) {
        Group tag_group = data_tag_group.openGroup(id);
        tag = make_shared<MultiTagHDF5>(file(), block(), tag_group);
    }

    return tag;
}


shared_ptr<IMultiTag> BlockHDF5::getMultiTag(size_t index) const {
    string id = data_tag_group.objectName(index);
    return getMultiTag(id);
}


size_t BlockHDF5::dataTagCount() const{
    return data_tag_group.objectCount();
}


bool BlockHDF5::deleteMultiTag(const std::string &id) {
    bool deleted = false;
    if (hasMultiTag(id)) {
        data_tag_group.removeGroup(id);
        deleted = true;
    }
    return deleted;
}


shared_ptr<IBlock> BlockHDF5::block() const {
    return const_pointer_cast<BlockHDF5>(shared_from_this());
}


BlockHDF5::~BlockHDF5() {}

