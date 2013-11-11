// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/hdf5/BlockHDF5.hpp>

using namespace std;

namespace nix {
namespace hdf5 {

BlockHDF5::BlockHDF5(const BlockHDF5 &block)
    : NamedEntityHDF5(block.file(), block.group(), block.id()),
      source_group(block.source_group), data_array_group(block.data_array_group),
      simple_tag_group(block.simple_tag_group), data_tag_group(block.data_tag_group)
{
}


BlockHDF5::BlockHDF5(File file, Group group, const string &id)
    : NamedEntityHDF5(file, group, id)
{
    source_group = group.openGroup("sources");
    data_array_group = group.openGroup("data_arrays");
    simple_tag_group = group.openGroup("simple_tags");
    data_tag_group = group.openGroup("data_tags");
}


BlockHDF5::BlockHDF5(File file, Group group, const string &id, time_t time)
    : NamedEntityHDF5(file, group, id, time)
{
    source_group = group.openGroup("sources");
    data_array_group = group.openGroup("data_arrays");
    simple_tag_group = group.openGroup("simple_tags");
    data_tag_group = group.openGroup("data_tags");
}


// source methods

/*
bool BlockHDF5::hasSource(const string &id) const {
    return source_group.hasGroup(id);
}


Source BlockHDF5::getSource(const string &id) const {
    return Source(file, source_group.openGroup(id, false), id);
}


Source BlockHDF5::getSource(size_t index) const {
    string id = source_group.objectName(index);
    return Source(file, source_group.openGroup(id, false), id);
    // TODO handle exceptions!!
}


size_t BlockHDF5::sourceCount() const {
    return source_group.objectCount();
}


std::vector<Source> BlockHDF5::sources() const {
    vector<Source> source_obj;

    size_t source_count = sourceCount();
    for (size_t i = 0; i < source_count; i++) {
        Source s = getSource(i);
        source_obj.push_back(s);
    }

    return source_obj;
}


std::vector<Source> BlockHDF5::findSources(std::function<bool(const Source &)> predicate) const
{
    vector<Source> result;

    size_t source_count = sourceCount();
    for (size_t i = 0; i < source_count; i++) {
        Source s = getSource(i);
        vector<Source> tmp = s.collectIf(predicate);
        result.insert(result.begin(), tmp.begin(), tmp.end());
    }

    return result;
}


Source BlockHDF5::createSource(const string &name,const string &type) {
    string id = util::createId("source");

    while(source_group.hasObject(id)) {
        id = util::createId("source");
    }

    Source s(file, source_group.openGroup(id, true), id);
    s.name(name);
    s.type(type);

    return s;
}


bool BlockHDF5::removeSource(const string &id) {
    bool removed = false;

    if (hasSource(id)) {
        source_group.removeGroup(id);
        removed = true;
    }

    return removed;
}
*/

// SimpleTag methods

/*
bool BlockHDF5::hasSimpleTag(const string &id) const {
    return simple_tag_group.hasObject(id);
}


SimpleTag BlockHDF5::getSimpleTag(const string &id) const {
    if (hasSimpleTag(id)) {
        SimpleTag st(file, *this, simple_tag_group.openGroup(id, true), id);
        return st;
    } else {
        throw runtime_error("Unable to find SimpleTag with id " + id + "!");
    }
}


SimpleTag BlockHDF5::getSimpleTag(size_t index) const {
    if (index < simpleTagCount()) {
        string id = simple_tag_group.objectName(index);
        SimpleTag st(file, *this, simple_tag_group.openGroup(id, true), id);
        return st;
    } else {
        throw runtime_error("Unable to find SimpleTag with the given index!");
    }
}


size_t BlockHDF5::simpleTagCount() const {
    return simple_tag_group.objectCount();
}


vector<SimpleTag> BlockHDF5::simpleTags() const {
    vector<SimpleTag> tag_obj;

    size_t tag_count = simpleTagCount();
    for (size_t i = 0; i < tag_count; i++) {
        string id = simple_tag_group.objectName(i);
        SimpleTag st(file, *this, simple_tag_group.openGroup(id), id);
        tag_obj.push_back(st);
    }

    return tag_obj;
}


SimpleTag BlockHDF5::createSimpleTag(const string &name, const string &type) {
    string id = util::createId("simple_tag");

    while(hasSimpleTag(id)) {
        id = util::createId("simple_tag");
    }

    SimpleTag st(file, *this, simple_tag_group.openGroup(id, true), id);
    st.name(name);
    st.type(type);

    return st;
}


bool BlockHDF5::removeSimpleTag(const string &id) {
    bool removed = false;

    if (hasSimpleTag(id)) {
        simple_tag_group.removeGroup(id);
        removed = true;
    }

    return removed;
}
*/

// Methods related to DataArray

/*
bool BlockHDF5::hasDataArray(const string &id) const {
    return data_array_group.hasObject(id);
}


DataArray BlockHDF5::getDataArray(const string &id) const {
    if (hasDataArray(id)) {
        DataArray da(file, *this, data_array_group.openGroup(id, true), id);
        return da;
    } else {
        throw runtime_error("Unable to find DataArray with id " + id + "!");
    }
}


DataArray BlockHDF5::getDataArray(size_t index) const {
    if (index < dataArrayCount()) {
        string id = data_array_group.objectName(index);
        DataArray da(file, *this, data_array_group.openGroup(id, true), id);
        return da;
    } else {
        throw runtime_error("Unable to find DataArray with the given index!");
    }
}


size_t BlockHDF5::dataArrayCount() const {
    return data_array_group.objectCount();
}


vector<DataArray> BlockHDF5::dataArrays() const {
    vector<DataArray> array_obj;

    size_t array_count = dataArrayCount();
    for (size_t i = 0; i < array_count; i++) {
        string id = data_array_group.objectName(i);
        DataArray da(file, *this, data_array_group.openGroup(id), id);
        array_obj.push_back(da);
    }

    return array_obj;
}


DataArray BlockHDF5::createDataArray(const std::string &name, const std::string &type) {
    string id = util::createId("data_array");

    while (hasDataArray(id)) {
        id = util::createId("data_array");
    }

    DataArray da(this->file, *this, data_array_group.openGroup(id, true), id);
    da.name(name);
    da.type(type);

    return da;
}


bool BlockHDF5::removeDataArray(const string &id) {
    bool removed = false;

    if (hasDataArray(id)) {
        data_array_group.removeGroup(id);
        removed = true;
    }

    return removed;
}

*/

// Methods related to DataTag

/*
DataTag BlockHDF5::createDataTag(const std::string &name, const std::string &type){
    string id = util::createId("data_tag");
    while (hasDataTag(id)) {
        id = util::createId("data_tag");
    }
    DataTag dt(this->file, *this, data_tag_group.openGroup(id, true), id);
    dt.name(name);
    dt.type(type);

    return dt;
}


bool BlockHDF5::hasDataTag(const std::string &id) const{
    return data_tag_group.hasObject(id);
}


DataTag BlockHDF5::getDataTag(const std::string &id) const{
    if (hasDataTag(id)) {
        DataTag dt(file, *this, data_tag_group.openGroup(id, true), id);
        return dt;
    } else {
        throw runtime_error("Unable to find DataTag with id " + id + "!");
    }
}


DataTag BlockHDF5::getDataTag(size_t index) const {
    if (index < dataTagCount()) {
        string id = data_tag_group.objectName(index);
        DataTag dt(file, *this, data_tag_group.openGroup(id, true), id);
        return dt;
    } else {
        throw runtime_error("Unable to find DataTag with the given index!");
    }
}


size_t BlockHDF5::dataTagCount() const{
    return data_tag_group.objectCount();
}


std::vector<DataTag> BlockHDF5::dataTags() const{
    vector<DataTag> tag_obj;

    size_t tag_count = dataTagCount();
    for (size_t i = 0; i < tag_count; i++) {
        string id = data_tag_group.objectName(i);
        DataTag dt(file, *this, data_tag_group.openGroup(id), id);
        tag_obj.push_back(dt);
    }

    return tag_obj;
}


bool BlockHDF5::removeDataTag(const std::string &id){
    bool removed = false;
    if (hasDataTag(id)) {
        data_tag_group.removeGroup(id);
        removed = true;
    }
    return removed;
}

*/
// Other methods and functions


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


ostream& operator<<(ostream &out, const BlockHDF5 &ent) {
    out << "Block: {name = " << ent.name();
    out << ", type = " << ent.type();
    out << ", id = " << ent.id() << "}";
    return out;
}


BlockHDF5::~BlockHDF5() {}


} // namespace hdf5
} // namespace nix
