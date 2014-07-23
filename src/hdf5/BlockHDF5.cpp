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
    : EntityWithMetadataHDF5(block.file(), block.group(), block.id(), block.type(), block.name()),
      source_group(block.source_group), data_array_group(block.data_array_group),
      simple_tag_group(block.simple_tag_group), data_tag_group(block.data_tag_group)
{
}


BlockHDF5::BlockHDF5(File file, Group group, const string &id, const string &type, const string &name)
    : BlockHDF5(file, group, id, type, name, util::getTime())
{
}


BlockHDF5::BlockHDF5(File file, Group group, const string &id, const string &type, const string &name, time_t time)
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


Source BlockHDF5::getSource(const string &id) const {
    if (hasSource(id)) {
        Group group = source_group.openGroup(id, false);
        string type;
        string name;
        group.getAttr("type", type);
        group.getAttr("name", name);
        return Source(make_shared<SourceHDF5>(file(), group, id, type, name));
    } else {
        return nix::Source();
    }
}


Source BlockHDF5::getSource(size_t index) const {
    string id = source_group.objectName(index);
    return getSource(id);
}


size_t BlockHDF5::sourceCount() const {
    return source_group.objectCount();
}


Source BlockHDF5::createSource(const string &name, const string &type) {
    string id = util::createId("source");

    while(source_group.hasObject(id)) {
        id = util::createId("source");
    }

    Group group = source_group.openGroup(id, true);
    return Source(make_shared<SourceHDF5>(file(), group, id, type, name));
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
        Group tag_group = simple_tag_group.openGroup(id, false);
        string type;
        string name;
        tag_group.getAttr("type", type);
        tag_group.getAttr("name", name);
        vector<string> ref_ids;
        tag_group.getData("references", ref_ids);
        vector<DataArray> refs;
        for(auto it = ref_ids.begin(); it != ref_ids.end(); ++it) {
            // NOTE: arrays might not exist & be empty, but let SimpleTag ctor called below handle that!
            refs.push_back(getDataArray(*it));
        }
        return SimpleTag(make_shared<SimpleTagHDF5>(file(), block(), tag_group, id, type, name, refs));
    } else {
        return nix::SimpleTag();
    }
}


SimpleTag BlockHDF5::getSimpleTag(size_t index) const {
    string id = simple_tag_group.objectName(index);
    return getSimpleTag(id);
}


size_t BlockHDF5::simpleTagCount() const {
    return simple_tag_group.objectCount();
}


SimpleTag BlockHDF5::createSimpleTag(const string &name, const string &type, 
                                     const std::vector<DataArray> &refs) {
    string id = util::createId("simple_tag");

    while(hasSimpleTag(id)) {
        id = util::createId("simple_tag");
    }

    Group group = simple_tag_group.openGroup(id, true);
    return SimpleTag(make_shared<SimpleTagHDF5>(file(), block(), group, id, type, name, refs));
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
        Group group = data_array_group.openGroup(id, false);
        std::string type;
        std::string name;
        group.getAttr("type", type);
        group.getAttr("name", name);
        auto tmp = make_shared<DataArrayHDF5>(file(), block(), group, id, type, name);
        return DataArray(tmp);
    } else {
        return nix::DataArray();
    }
}


DataArray BlockHDF5::getDataArray(size_t index) const {
    string id = data_array_group.objectName(index);
    return getDataArray(id);
}


size_t BlockHDF5::dataArrayCount() const {
    return data_array_group.objectCount();
}


DataArray BlockHDF5::createDataArray(const std::string &name,
                                     const std::string &type,
                                     nix::DataType      data_type,
                                     const NDSize      &shape) {
    string id = util::createId("data_array");

    while (hasDataArray(id)) {
        id = util::createId("data_array");
    }

    Group group = data_array_group.openGroup(id, true);
    auto tmp = make_shared<DataArrayHDF5>(file(), block(), group, id, type, name);

    //now create the actual H5::DataSet
    tmp->createData(data_type, shape);

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

DataTag BlockHDF5::createDataTag(const std::string &name, const std::string &type, 
                                 const DataArray positions) {
    string id = util::createId("data_tag");
    while (hasDataTag(id)) {
        id = util::createId("data_tag");
    }

    Group group = data_tag_group.openGroup(id);
    auto tmp = make_shared<DataTagHDF5>(file(), block(), group, id, type, name, positions);

    return DataTag(tmp);
}


bool BlockHDF5::hasDataTag(const std::string &id) const {
    return data_tag_group.hasObject(id);
}


DataTag BlockHDF5::getDataTag(const std::string &id) const {
    if (hasDataTag(id)) {
        Group tag_group = data_tag_group.openGroup(id);
        std::string positions_id;
        tag_group.getAttr("positions", positions_id);
        DataArray positions = getDataArray(positions_id);
        std::string type;
        std::string name;
        tag_group.getAttr("type", type);
        tag_group.getAttr("name", name);
        auto tmp = make_shared<DataTagHDF5>(file(), block(), tag_group, id, type, name, positions);
        return DataTag(tmp);
    } else {
        return nix::DataTag();
    }
}


DataTag BlockHDF5::getDataTag(size_t index) const {
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


Block BlockHDF5::block() const {
    shared_ptr<BlockHDF5> tmp = const_pointer_cast<BlockHDF5>(shared_from_this());
    return Block(tmp);
}


BlockHDF5::~BlockHDF5() {}


} // namespace hdf5
} // namespace nix
