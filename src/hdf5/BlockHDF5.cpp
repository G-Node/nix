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
#include <nix/hdf5/SimpleTagHDF5.hpp>
#include <nix/hdf5/DataTagHDF5.hpp>

using namespace std;
using namespace nix;
using namespace nix::hdf5;
using namespace nix::base;

// TODO unnecessary IO (see #316)
BlockHDF5::BlockHDF5(const BlockHDF5 &block)
    : EntityWithMetadataHDF5(block.file(), block.group(), block.id(), block.type(), block.name()),
      source_group(block.source_group), data_array_group(block.data_array_group),
      simple_tag_group(block.simple_tag_group), data_tag_group(block.data_tag_group)
{
}


BlockHDF5::BlockHDF5(shared_ptr<IFile> file, Group group, const string &id, const string &type, const string &name)
    : BlockHDF5(file, group, id, type, name, util::getTime())
{
}


BlockHDF5::BlockHDF5(shared_ptr<IFile> file, Group group, const string &id, const string &type, const string &name, time_t time)
    : EntityWithMetadataHDF5(file, group, id, type, name, time)
{
    source_group = group.openGroup("sources");
    data_array_group = group.openGroup("data_arrays");
    simple_tag_group = group.openGroup("simple_tags");
    data_tag_group = group.openGroup("data_tags");
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
        // TODO unnecessary IO (see #316)
        string type;
        string name;
        group.getAttr("type", type);
        group.getAttr("name", name);
        source = make_shared<SourceHDF5>(file(), group, id, type, name);
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
    bool deleted = false;

    if (hasSource(id)) {
        source_group.removeGroup(id);
        deleted = true;
    }

    return deleted;
}


// SimpleTag methods

bool BlockHDF5::hasSimpleTag(const string &id) const {
    return simple_tag_group.hasObject(id);
}


shared_ptr<ISimpleTag> BlockHDF5::getSimpleTag(const string &id) const {
    shared_ptr<SimpleTagHDF5> tag;

    if (hasSimpleTag(id)) {
        Group tag_group = simple_tag_group.openGroup(id, false);
	tag = make_shared<SimpleTagHDF5>(file(), block(), tag_group, id);
    }

    return tag;
}


shared_ptr<ISimpleTag> BlockHDF5::getSimpleTag(size_t index) const {
    string id = simple_tag_group.objectName(index);
    return getSimpleTag(id);
}


size_t BlockHDF5::simpleTagCount() const {
    return simple_tag_group.objectCount();
}


shared_ptr<ISimpleTag> BlockHDF5::createSimpleTag(const string &name, const string &type,
                                                  const std::vector<double> &position) {
    string id = util::createId("simple_tag");
    while (hasSimpleTag(id)) {
        id = util::createId("simple_tag");
    }

    Group group = simple_tag_group.openGroup(id, true);
    return make_shared<SimpleTagHDF5>(file(), block(), group, id, type, name, position);
}


bool BlockHDF5::deleteSimpleTag(const string &id) {
    bool deleted = false;

    if (hasSimpleTag(id)) {
        simple_tag_group.removeGroup(id);
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
        // TODO unnecessary IO (see #316)
        std::string type;
        std::string name;
        group.getAttr("type", type);
        group.getAttr("name", name);
        da = make_shared<DataArrayHDF5>(file(), block(), group, id, type, name);
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
        data_array_group.removeGroup(id);
        deleted = true;
    }

    return deleted;
}


// Methods related to DataTag

shared_ptr<IDataTag> BlockHDF5::createDataTag(const std::string &name, const std::string &type,
                                              const DataArray &positions) {
    string id = util::createId("data_tag");
    while (hasDataTag(id)) {
        id = util::createId("data_tag");
    }

    Group group = data_tag_group.openGroup(id);
    return make_shared<DataTagHDF5>(file(), block(), group, id, type, name, positions);
}


bool BlockHDF5::hasDataTag(const std::string &id) const {
    return data_tag_group.hasObject(id);
}


shared_ptr<IDataTag> BlockHDF5::getDataTag(const std::string &id) const {
    shared_ptr<DataTagHDF5> tag;

    if (hasDataTag(id)) {
        Group tag_group = data_tag_group.openGroup(id);
        // TODO unnecessary IO (see #316)
        std::string positions_id;
        tag_group.getAttr("positions", positions_id);
        DataArray positions = getDataArray(positions_id);
        std::string type;
        std::string name;
        tag_group.getAttr("type", type);
        tag_group.getAttr("name", name);
        tag = make_shared<DataTagHDF5>(file(), block(), tag_group, id, type, name, positions);
    }

    return tag;
}


shared_ptr<IDataTag> BlockHDF5::getDataTag(size_t index) const {
    string id = data_tag_group.objectName(index);
    return getDataTag(id);
}


size_t BlockHDF5::dataTagCount() const{
    return data_tag_group.objectCount();
}


bool BlockHDF5::deleteDataTag(const std::string &id) {
    bool deleted = false;
    if (hasDataTag(id)) {
        data_tag_group.removeGroup(id);
        deleted = true;
    }
    return deleted;
}


void BlockHDF5::swap(BlockHDF5 &other) {
    using std::swap;

    EntityHDF5::swap(other);
    swap(source_group, other.source_group);
    swap(data_array_group, other.data_array_group);
    swap(simple_tag_group, other.simple_tag_group);
    swap(data_tag_group, other.data_tag_group);
}


BlockHDF5& BlockHDF5::operator=(const BlockHDF5 &other) {
    if (*this != other) {
        BlockHDF5 tmp(other);
        swap(tmp);
    }
    return *this;
}


shared_ptr<IBlock> BlockHDF5::block() const {
    return const_pointer_cast<BlockHDF5>(shared_from_this());
}


BlockHDF5::~BlockHDF5() {}

