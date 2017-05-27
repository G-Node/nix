// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "GroupHDF5.hpp"
#include <nix/Block.hpp>
#include "TagHDF5.hpp"
#include "MultiTagHDF5.hpp"
#include "BlockHDF5.hpp"
#include <boost/range/irange.hpp>

using namespace nix::base;

namespace nix {
namespace hdf5 {

GroupHDF5::GroupHDF5(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block,
                     const H5Group &h5group)  : EntityWithSourcesHDF5(file, block, h5group) {
    data_array_group = this->group().openOptGroup("data_arrays");
    tag_group = this->group().openOptGroup("tags");
    multi_tag_group = this->group().openOptGroup("multi_tags");
}


GroupHDF5::GroupHDF5(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block,
                     const H5Group &h5group, const std::string &id, const std::string &type,
                     const std::string &name) : GroupHDF5(file, block, h5group, id, type, name, util::getTime())  {

}


GroupHDF5::GroupHDF5(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block,
                     const H5Group &h5group, const std::string &id, const std::string &type, const std::string &name,
                     time_t time): EntityWithSourcesHDF5(file, block, h5group, id, type, name, time) {
    data_array_group = this->group().openOptGroup("data_arrays");
    tag_group = this->group().openOptGroup("tags");
    multi_tag_group = this->group().openOptGroup("multi_tags");
}


bool GroupHDF5::hasDataArray(const std::string &name_or_id) const {
    std::string id = block()->resolveEntityId({name_or_id, ObjectType::DataArray});
    return data_array_group(false) ? data_array_group(false)->hasGroup(id) : false;
}


ndsize_t GroupHDF5::dataArrayCount() const {
    boost::optional<H5Group> g = data_array_group(false);
    return g ? g->objectCount() : size_t(0);
}


void GroupHDF5::addDataArray(const std::string &name_or_id) {
    boost::optional<H5Group> g = data_array_group(true);
    std::shared_ptr<IDataArray> ida = block()->getEntity<IDataArray>(name_or_id);

    if (!ida)
        throw std::runtime_error("GroupHDF5::addDataArray: DataArray not found in block!");

    auto target = std::dynamic_pointer_cast<DataArrayHDF5>(ida);
    g->createLink(target->group(), target->id());
}


std::shared_ptr<base::IDataArray> GroupHDF5::getDataArray(const std::string &name_or_id) const {
    std::shared_ptr<IDataArray> da;
    boost::optional<H5Group> g = data_array_group(false);
    std::string id = block()->resolveEntityId({name_or_id, ObjectType::DataArray});

    if (g && hasDataArray(id)) {
        H5Group h5g = g->openGroup(id);
        da = std::make_shared<DataArrayHDF5>(file(), block(), h5g);
    }
    return da;
}


std::shared_ptr<IDataArray>  GroupHDF5::getDataArray(ndsize_t index) const {
    boost::optional<H5Group> g = data_array_group(false);
    std::string id = g ? g->objectName(index) : "";
    return getDataArray(id);
}


bool GroupHDF5::removeDataArray(const std::string &name_or_id) {
    boost::optional<H5Group> g = data_array_group(false);
    bool removed = false;

    if (g && hasDataArray(name_or_id)) {
        std::shared_ptr<IDataArray> data_array = getDataArray(name_or_id);

        g->removeGroup(data_array->id());
        removed = true;
    }
    return removed;
}


void GroupHDF5::dataArrays(const std::vector<DataArray> &data_arrays) {
    auto cmp = [](const DataArray &a, const DataArray& b) { return a.name() < b.name(); };

    std::vector<DataArray> new_arrays(data_arrays);
    size_t array_count = nix::check::fits_in_size_t(dataArrayCount(), "dataArrayCount() failed; count > size_t.");
    std::vector<DataArray> old_arrays(array_count);
    for (size_t i = 0; i < old_arrays.size(); i++) {
        old_arrays[i] = getDataArray(i);
    }
    std::sort(new_arrays.begin(), new_arrays.end(), cmp);
    std::sort(old_arrays.begin(), old_arrays.end(), cmp);
    std::vector<DataArray> add;
    std::vector<DataArray> rem;

    std::set_difference(new_arrays.begin(), new_arrays.end(), old_arrays.begin(), old_arrays.end(),
                        std::inserter(add, add.begin()), cmp);
    std::set_difference(old_arrays.begin(), old_arrays.end(), new_arrays.begin(), new_arrays.end(),
                        std::inserter(rem, rem.begin()), cmp);

    for (const auto &da : add) {
        DataArray a = block()->getEntity<base::IDataArray>(da);
        if (!a)
            throw std::runtime_error("One or more data arrays do not exist in this block!");
        addDataArray(a.id());
    }

    for (const auto &da : rem) {
        removeDataArray(da.id());
    }
}


bool GroupHDF5::hasTag(const std::string &name_or_id) const {
    std::string id = name_or_id;
    if (!util::looksLikeUUID(name_or_id) && block()->hasTag(name_or_id)) {
        id = block()->getTag(name_or_id)->id();
    }
    return tag_group(false) ? tag_group(false)->hasGroup(id) : false;
}


ndsize_t GroupHDF5::tagCount() const {
    boost::optional<H5Group> g = tag_group(false);
    return g ? g->objectCount() : size_t(0);
}


void GroupHDF5::addTag(const std::string &name_or_id) {
    boost::optional<H5Group> g = tag_group(true);

    if (!block()->hasTag(name_or_id))
        throw std::runtime_error("GroupHDF5::addTag: Tag not found in block!");

    auto target = std::dynamic_pointer_cast<TagHDF5>(block()->getTag(name_or_id));
    g->createLink(target->group(), target->id());
}


std::shared_ptr<base::ITag> GroupHDF5::getTag(const std::string &name_or_id) const {
    std::shared_ptr<ITag> da;
    boost::optional<H5Group> g = tag_group(false);
    std::string id = name_or_id;
    if (!util::looksLikeUUID(name_or_id) && block()->hasTag(name_or_id)) {
        id = block()->getTag(name_or_id)->id();
    }

    if (g && hasTag(id)) {
        H5Group h5g = g->openGroup(id);
        da = std::make_shared<TagHDF5>(file(), block(), h5g);
    }
    return da;
}


std::shared_ptr<ITag>  GroupHDF5::getTag(ndsize_t index) const {
    boost::optional<H5Group> g = tag_group(false);
    std::string id = g ? g->objectName(index) : "";
    return getTag(id);
}


bool GroupHDF5::removeTag(const std::string &name_or_id) {
    boost::optional<H5Group> g = tag_group(false);
    bool removed = false;

    if (g && hasTag(name_or_id)) {
        std::shared_ptr<ITag> tag = getTag(name_or_id);

        g->removeGroup(tag->id());
        removed = true;
    }
    return removed;
}


void GroupHDF5::tags(const std::vector<Tag> &tags) {
    auto cmp = [](const Tag &a, const Tag& b) { return a.name() < b.name(); };

    std::vector<Tag> new_tags(tags);
    size_t tag_count = nix::check::fits_in_size_t(tagCount(), "tagCount() failed; count > size_t.");
    std::vector<Tag> old_tags(tag_count);
    for (size_t i = 0; i < old_tags.size(); i++) {
        old_tags[i] = getTag(i);
    }
    std::sort(new_tags.begin(), new_tags.end(), cmp);
    std::sort(old_tags.begin(), old_tags.end(), cmp);
    std::vector<Tag> add;
    std::vector<Tag> rem;

    std::set_difference(new_tags.begin(), new_tags.end(), old_tags.begin(), old_tags.end(),
                        std::inserter(add, add.begin()), cmp);
    std::set_difference(old_tags.begin(), old_tags.end(), new_tags.begin(), new_tags.end(),
                        std::inserter(rem, rem.begin()), cmp);

    auto blck = std::dynamic_pointer_cast<BlockHDF5>(block());
    for (const auto &t : add) {
        Tag tag = blck->getTag(t.name());
        if (!tag || tag.id() != t.id())
            throw std::runtime_error("One or more data tags do not exist in this block!");
        addTag(t.id());
    }

    for (const auto &t : rem) {
        removeTag(t.id());
    }
}


bool GroupHDF5::hasMultiTag(const std::string &name_or_id) const {
    std::string id = name_or_id;
    if (!util::looksLikeUUID(name_or_id) && block()->hasMultiTag(name_or_id)) {
        id = block()->getMultiTag(name_or_id)->id();
    }
    return multi_tag_group(false) ? multi_tag_group(false)->hasGroup(id) : false;
}


ndsize_t GroupHDF5::multiTagCount() const {
    boost::optional<H5Group> g = multi_tag_group(false);
    return g ? g->objectCount() : size_t(0);
}


void GroupHDF5::addMultiTag(const std::string &name_or_id) {
    boost::optional<H5Group> g = multi_tag_group(true);

    if (!block()->hasMultiTag(name_or_id))
        throw std::runtime_error("GroupHDF5::addMultiTag: MultiTag not found in block!");

    auto target = std::dynamic_pointer_cast<MultiTagHDF5>(block()->getMultiTag(name_or_id));
    g->createLink(target->group(), target->id());
}


std::shared_ptr<base::IMultiTag> GroupHDF5::getMultiTag(const std::string &name_or_id) const {
    std::shared_ptr<IMultiTag> da;
    boost::optional<H5Group> g = multi_tag_group(false);
    std::string id = name_or_id;
    if (!util::looksLikeUUID(name_or_id) && block()->hasMultiTag(name_or_id)) {
        id = block()->getMultiTag(name_or_id)->id();
    }

    if (g && hasMultiTag(id)) {
        H5Group h5g = g->openGroup(id);
        da = std::make_shared<MultiTagHDF5>(file(), block(), h5g);
    }
    return da;
}


std::shared_ptr<IMultiTag>  GroupHDF5::getMultiTag(ndsize_t index) const {
    boost::optional<H5Group> g = multi_tag_group(false);
    std::string id = g ? g->objectName(index) : "";
    return getMultiTag(id);
}


bool GroupHDF5::removeMultiTag(const std::string &name_or_id) {
    boost::optional<H5Group> g = multi_tag_group(false);
    bool removed = false;

    if (g && hasMultiTag(name_or_id)) {
        std::shared_ptr<IMultiTag> mtag = getMultiTag(name_or_id);

        g->removeGroup(mtag->id());
        removed = true;
    }
    return removed;
}


void GroupHDF5::multiTags(const std::vector<MultiTag> &multi_tags) {
    auto cmp = [](const MultiTag &a, const MultiTag& b) { return a.name() < b.name(); };

    std::vector<MultiTag> new_tags(multi_tags);
    size_t tag_count = nix::check::fits_in_size_t(multiTagCount(), "multiTagCount() failed; count > size_t.");
    std::vector<MultiTag> old_tags(tag_count);
    for (size_t i = 0; i < old_tags.size(); i++) {
        old_tags[i] = getMultiTag(i);
    }
    std::sort(new_tags.begin(), new_tags.end(), cmp);
    std::sort(old_tags.begin(), old_tags.end(), cmp);
    std::vector<MultiTag> add;
    std::vector<MultiTag> rem;

    std::set_difference(new_tags.begin(), new_tags.end(), old_tags.begin(), old_tags.end(),
                        std::inserter(add, add.begin()), cmp);
    std::set_difference(old_tags.begin(), old_tags.end(), new_tags.begin(), new_tags.end(),
                        std::inserter(rem, rem.begin()), cmp);

    auto blck = std::dynamic_pointer_cast<BlockHDF5>(block());
    for (const auto &t : add) {
        MultiTag tag = blck->getMultiTag(t.name());
        if (!tag || tag.id() != t.id())
            throw std::runtime_error("One or more data multiTags do not exist in this block!");
        addMultiTag(t.id());
    }

    for (const auto &t : rem) {
        removeMultiTag(t.id());
    }
}

} // hdf5
} // nix
