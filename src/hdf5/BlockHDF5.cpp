// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/hdf5/BlockHDF5.hpp>

#include <nix/util/util.hpp>
#include <nix/Block.hpp>
#include <nix/hdf5/SourceHDF5.hpp>
#include <nix/hdf5/DataArrayHDF5.hpp>
#include <nix/hdf5/TagHDF5.hpp>
#include <nix/hdf5/MultiTagHDF5.hpp>
#include <nix/hdf5/GroupHDF5.hpp>

#include <boost/range/irange.hpp>

using namespace std;
using namespace nix::base;

namespace nix {
namespace hdf5 {


BlockHDF5::BlockHDF5(const std::shared_ptr<base::IFile> &file, const H5Group &group)
        : EntityWithMetadataHDF5(file, group) {
    data_array_group = this->group().openOptGroup("data_arrays");
    tag_group = this->group().openOptGroup("tags");
    multi_tag_group = this->group().openOptGroup("multi_tags");
    source_group = this->group().openOptGroup("sources");
    groups_group = this->group().openOptGroup("groups");
}

BlockHDF5::BlockHDF5(const shared_ptr<IFile> &file, const H5Group &group, const string &id, const string &type, const string &name)
        : BlockHDF5(file, group, id, type, name, util::getTime()) {
}


BlockHDF5::BlockHDF5(const shared_ptr<IFile> &file, const H5Group &group, const string &id, const string &type, const string &name, time_t time)
        : EntityWithMetadataHDF5(file, group, id, type, name, time) {
    data_array_group = this->group().openOptGroup("data_arrays");
    tag_group = this->group().openOptGroup("tags");
    multi_tag_group = this->group().openOptGroup("multi_tags");
    source_group = this->group().openOptGroup("sources");
    groups_group = this->group().openOptGroup("groups");
}


//--------------------------------------------------
// Methods concerning sources
//--------------------------------------------------


bool BlockHDF5::hasSource(const string &name_or_id) const {
    return getSource(name_or_id) != nullptr;
}


shared_ptr<ISource> BlockHDF5::getSource(const string &name_or_id) const {
    shared_ptr<SourceHDF5> source;
    boost::optional<H5Group> g = source_group();

    if (g) {
        boost::optional<H5Group> group = g->findGroupByNameOrAttribute("entity_id", name_or_id);
        if (group)
            source = make_shared<SourceHDF5>(file(), *group);
    }

    return source;
}


shared_ptr<ISource> BlockHDF5::getSource(ndsize_t index) const {
    boost::optional<H5Group> g = source_group();
    string name = g ? g->objectName(index) : "";
    return getSource(name);
}


ndsize_t BlockHDF5::sourceCount() const {
    boost::optional<H5Group> g = source_group();
    return g ? g->objectCount() : size_t(0);
}


shared_ptr<ISource> BlockHDF5::createSource(const string &name, const string &type) {
    string id = util::createId();
    boost::optional<H5Group> g = source_group(true);

    H5Group group = g->openGroup(name, true);
    return make_shared<SourceHDF5>(file(), group, id, type, name);
}


bool BlockHDF5::deleteSource(const string &name_or_id) {
    boost::optional<H5Group> g = source_group();
    bool deleted = false;

    if (g) {
        // call deleteSource on sources to trigger recursive call to all sub-sources
        if (hasSource(name_or_id)) {
            // get instance of source about to get deleted
            Source source = getSource(name_or_id);
            // loop through all child sources and call deleteSource on them
            for (auto &child : source.sources()) {
                source.deleteSource(child.id());
            }
            // if hasSource is true then source_group always exists
            deleted = g->removeAllLinks(source.name());
        }
    }

    return deleted;
}


//--------------------------------------------------
// Methods related to Tag
//--------------------------------------------------


shared_ptr<ITag> BlockHDF5::createTag(const std::string &name, const std::string &type,
                                      const std::vector<double> &position) {
    string id = util::createId();
    boost::optional<H5Group> g = tag_group(true);

    H5Group group = g->openGroup(name);
    return make_shared<TagHDF5>(file(), block(), group, id, type, name, position);
}


bool BlockHDF5::hasTag(const string &name_or_id) const {
    return getTag(name_or_id) != nullptr;
}


shared_ptr<ITag> BlockHDF5::getTag(const string &name_or_id) const {
    shared_ptr<TagHDF5> tag;
    boost::optional<H5Group> g = tag_group();

    if (g) {
        boost::optional<H5Group> group = g->findGroupByNameOrAttribute("entity_id", name_or_id);
        if (group)
            tag = make_shared<TagHDF5>(file(), block(), *group);
    }

    return tag;
}


shared_ptr<ITag> BlockHDF5::getTag(ndsize_t index) const {
    boost::optional<H5Group> g = tag_group();
    string name = g ? g->objectName(index) : "";
    return getTag(name);
}


ndsize_t BlockHDF5::tagCount() const {
    boost::optional<H5Group> g = tag_group();
    return g ? g->objectCount() : size_t(0);
}


bool BlockHDF5::deleteTag(const std::string &name_or_id) {
    boost::optional<H5Group> g = tag_group();
    bool deleted = false;

    if (hasTag(name_or_id) && g) {
        // we get first "entity" link by name, but delete all others whatever their name with it
        deleted = g->removeAllLinks(getTag(name_or_id)->name());
    }

    return deleted;
}


//--------------------------------------------------
// Methods related to DataArray
//--------------------------------------------------


bool BlockHDF5::hasDataArray(const string &name_or_id) const {
    return getDataArray(name_or_id) != nullptr;
}


shared_ptr<IDataArray> BlockHDF5::getDataArray(const string &name_or_id) const {
    shared_ptr<DataArrayHDF5> da;
    boost::optional<H5Group> g = data_array_group();

    if (g) {
        boost::optional<H5Group> group = g->findGroupByNameOrAttribute("entity_id", name_or_id);
        if (group)
            da = make_shared<DataArrayHDF5>(file(), block(), *group);
    }

    return da;
}


shared_ptr<IDataArray> BlockHDF5::getDataArray(ndsize_t index) const {
    boost::optional<H5Group> g = data_array_group();
    string name = g ? g->objectName(index) : "";
    return getDataArray(name);
}


ndsize_t BlockHDF5::dataArrayCount() const {
    boost::optional<H5Group> g = data_array_group();
    return g ? g->objectCount() : size_t(0);
}


shared_ptr<IDataArray> BlockHDF5::createDataArray(const std::string &name,
                                                  const std::string &type,
                                                  nix::DataType data_type,
                                                  const NDSize &shape) {
    string id = util::createId();
    boost::optional<H5Group> g = data_array_group(true);

    H5Group group = g->openGroup(name, true);
    auto da = make_shared<DataArrayHDF5>(file(), block(), group, id, type, name);

    // now create the actual H5::DataSet
    da->createData(data_type, shape);
    return da;
}


bool BlockHDF5::deleteDataArray(const string &name_or_id) {
    bool deleted = false;
    boost::optional<H5Group> g = data_array_group();

    if (hasDataArray(name_or_id) && g) {
        // we get first "entity" link by name, but delete all others whatever their name with it
        deleted = g->removeAllLinks(getDataArray(name_or_id)->name());
    }

    return deleted;
}


//--------------------------------------------------
// Methods related to MultiTag
//--------------------------------------------------


shared_ptr<IMultiTag> BlockHDF5::createMultiTag(const std::string &name, const std::string &type,
                                                const DataArray &positions) {
    string id = util::createId();
    boost::optional<H5Group> g = multi_tag_group(true);

    H5Group group = g->openGroup(name);
    return make_shared<MultiTagHDF5>(file(), block(), group, id, type, name, positions);
}


bool BlockHDF5::hasMultiTag(const string &name_or_id) const {
    return getMultiTag(name_or_id) != nullptr;
}


shared_ptr<IMultiTag> BlockHDF5::getMultiTag(const string &name_or_id) const {
    shared_ptr<MultiTagHDF5> mtag;
    boost::optional<H5Group> g = multi_tag_group();

    if (g) {
        boost::optional<H5Group> group = g->findGroupByNameOrAttribute("entity_id", name_or_id);
        if (group)
            mtag = make_shared<MultiTagHDF5>(file(), block(), *group);
    }

    return mtag;
}


shared_ptr<IMultiTag> BlockHDF5::getMultiTag(ndsize_t index) const {
    boost::optional<H5Group> g = multi_tag_group();
    string name = g ? g->objectName(index) : "";
    return getMultiTag(name);
}


ndsize_t BlockHDF5::multiTagCount() const {
    boost::optional<H5Group> g = multi_tag_group();
    return g ? g->objectCount() : size_t(0);
}


bool BlockHDF5::deleteMultiTag(const std::string &name_or_id) {
    boost::optional<H5Group> g = multi_tag_group();
    bool deleted = false;

    if (hasMultiTag(name_or_id) && g) {
        // we get first "entity" link by name, but delete all others whatever their name with it
        deleted = g->removeAllLinks(getMultiTag(name_or_id)->name());
    }

    return deleted;
}

//--------------------------------------------------
// Methods related to Groups
//--------------------------------------------------

shared_ptr<IGroup> BlockHDF5::createGroup(const std::string &name, const std::string &type) {
    string id = util::createId();
    boost::optional<H5Group> g = groups_group(true);

    H5Group group = g->openGroup(name);
    return make_shared<GroupHDF5>(file(), block(), group, id, type, name);
}


    bool BlockHDF5::hasGroup(const string &name_or_id) const {
    return getGroup(name_or_id) != nullptr;
}


shared_ptr<IGroup> BlockHDF5::getGroup(const string &name_or_id) const {
    shared_ptr<GroupHDF5> group;
    boost::optional<H5Group> g = groups_group();

    if (g) {
        boost::optional<H5Group> h5g = g->findGroupByNameOrAttribute("entity_id", name_or_id);
        if (h5g)
            group = make_shared<GroupHDF5>(file(), block(), *h5g);
    }
    return group;
}


shared_ptr<IGroup> BlockHDF5::getGroup(ndsize_t index) const {
    boost::optional<H5Group> g = groups_group();
    string name = g ? g->objectName(index) : "";
    return getGroup(name);
}


ndsize_t BlockHDF5::groupCount() const {
    boost::optional<H5Group> g = groups_group();
    return g ? g->objectCount() : size_t(0);
}


bool BlockHDF5::deleteGroup(const std::string &name_or_id) {
    boost::optional<H5Group> g = groups_group();
    bool deleted = false;

    if (hasGroup(name_or_id) && g) {
        deleted = g->removeAllLinks(getGroup(name_or_id)->name());
    }
    return deleted;
}


shared_ptr<IBlock> BlockHDF5::block() const {
    return const_pointer_cast<BlockHDF5>(shared_from_this());
}


BlockHDF5::~BlockHDF5() {
}

} // ns nix::hdf5
} // ns nix