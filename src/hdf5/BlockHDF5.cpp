// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/util/util.hpp>
#include <nix/hdf5/BlockHDF5.hpp>
#include <nix/hdf5/SourceHDF5.hpp>
#include <nix/hdf5/DataArrayHDF5.hpp>
#include <nix/hdf5/SimpleTagHDF5.hpp>
#include <nix/hdf5/DataTagHDF5.hpp>

using namespace std;

namespace nix {
namespace hdf5 {

BlockHDF5::BlockHDF5(const BlockHDF5 &block)
    : EntityWithMetadataHDF5(block.file(), block.group(), block.id()),
      source_group(block.source_group), data_array_group(block.data_array_group),
      simple_tag_group(block.simple_tag_group), data_tag_group(block.data_tag_group)
{
}


BlockHDF5::BlockHDF5(File file, Group group, const string &id)
    : EntityWithMetadataHDF5(file, group, id)
{
    source_group = group.openGroup("sources");
    data_array_group = group.openGroup("data_arrays");
    simple_tag_group = group.openGroup("simple_tags");
    data_tag_group = group.openGroup("data_tags");
}


BlockHDF5::BlockHDF5(File file, Group group, const string &id, time_t time)
    : EntityWithMetadataHDF5(file, group, id, time)
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


Source BlockHDF5::getSource(const string &id) const {
    shared_ptr<SourceHDF5> tmp(new SourceHDF5(file(), source_group.openGroup(id, false), id));
    return Source(tmp);
}


Source BlockHDF5::getSource(size_t index) const {
    string id = source_group.objectName(index);
    shared_ptr<SourceHDF5> tmp(new SourceHDF5(file(), source_group.openGroup(id, false), id));
    return Source(tmp);
}


size_t BlockHDF5::sourceCount() const {
    return source_group.objectCount();
}


Source BlockHDF5::createSource(const string &name,const string &type) {
    string id = util::createId("source");

    while(source_group.hasObject(id)) {
        id = util::createId("source");
    }

    Group group = source_group.openGroup(id, true);
    shared_ptr<SourceHDF5> tmp(new SourceHDF5(file(), group, id));
    tmp->name(name);
    tmp->type(type);

    return Source(tmp);
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


SimpleTag BlockHDF5::getSimpleTag(const string &id) const {
    if (hasSimpleTag(id)) {
        shared_ptr<SimpleTagHDF5> tmp(new SimpleTagHDF5(file(), block(), simple_tag_group.openGroup(id, true), id));
        return SimpleTag(tmp);
    } else {
        throw runtime_error("Unable to find SimpleTag with id " + id + "!");
    }
}


SimpleTag BlockHDF5::getSimpleTag(size_t index) const {
    if (index < simpleTagCount()) {
        string id = simple_tag_group.objectName(index);
        shared_ptr<SimpleTagHDF5> tmp(new SimpleTagHDF5(file(), block(), simple_tag_group.openGroup(id, true), id));
        return SimpleTag(tmp);
    } else {
        throw runtime_error("Unable to find SimpleTag with the given index!");
    }
}


size_t BlockHDF5::simpleTagCount() const {
    return simple_tag_group.objectCount();
}


SimpleTag BlockHDF5::createSimpleTag(const string &name, const string &type) {
    string id = util::createId("simple_tag");

    while(hasSimpleTag(id)) {
        id = util::createId("simple_tag");
    }

    shared_ptr<SimpleTagHDF5> tmp(new SimpleTagHDF5(file(), block(), simple_tag_group.openGroup(id, true), id));
    tmp->name(name);
    tmp->type(type);

    return SimpleTag(tmp);
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


DataArray BlockHDF5::getDataArray(const string &id) const {
    if (hasDataArray(id)) {
        shared_ptr<DataArrayHDF5> tmp(new DataArrayHDF5(file(), block(), data_array_group.openGroup(id, true), id));
        return DataArray(tmp);
    } else {
        throw runtime_error("Unable to find DataArray with id " + id + "!");
    }
}


DataArray BlockHDF5::getDataArray(size_t index) const {
    if (index < dataArrayCount()) {
        string id = data_array_group.objectName(index);
        shared_ptr<DataArrayHDF5> tmp(new DataArrayHDF5(file(), block(), data_array_group.openGroup(id, true), id));
        return DataArray(tmp);
    } else {
        throw runtime_error("Unable to find DataArray with the given index!");
    }
}


size_t BlockHDF5::dataArrayCount() const {
    return data_array_group.objectCount();
}


DataArray BlockHDF5::createDataArray(const std::string &name, const std::string &type) {
    string id = util::createId("data_array");

    while (hasDataArray(id)) {
        id = util::createId("data_array");
    }

    shared_ptr<DataArrayHDF5> tmp(new DataArrayHDF5(file(), block(), data_array_group.openGroup(id, true), id));
    tmp->name(name);
    tmp->type(type);

    return DataArray(tmp);
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

DataTag BlockHDF5::createDataTag(const std::string &name, const std::string &type){
    string id = util::createId("data_tag");
    while (hasDataTag(id)) {
        id = util::createId("data_tag");
    }

    shared_ptr<DataTagHDF5> tmp(new DataTagHDF5(file(), block(), data_tag_group.openGroup(id), id));
    tmp->name(name);
    tmp->type(type);

    return DataTag(tmp);
}


bool BlockHDF5::hasDataTag(const std::string &id) const{
    return data_tag_group.hasObject(id);
}


DataTag BlockHDF5::getDataTag(const std::string &id) const{
    if (hasDataTag(id)) {
        shared_ptr<DataTagHDF5> tmp(new DataTagHDF5(file(), block(), data_tag_group.openGroup(id), id));
        return DataTag(tmp);
    } else {
        throw runtime_error("Unable to find DataTag with id " + id + "!");
    }
}


DataTag BlockHDF5::getDataTag(size_t index) const {
    if (index < dataTagCount()) {
        string id = data_tag_group.objectName(index);
        shared_ptr<DataTagHDF5> tmp(new DataTagHDF5(file(), block(), data_tag_group.openGroup(id, true), id));
        return DataTag(tmp);
    } else {
        throw runtime_error("Unable to find DataTag with the given index!");
    }
}


size_t BlockHDF5::dataTagCount() const{
    return data_tag_group.objectCount();
}


bool BlockHDF5::deleteDataTag(const std::string &id){
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


Block BlockHDF5::block() const {
    shared_ptr<BlockHDF5> tmp = const_pointer_cast<BlockHDF5>(shared_from_this());
    return Block(tmp);
}


BlockHDF5::~BlockHDF5() {}


} // namespace hdf5
} // namespace nix
