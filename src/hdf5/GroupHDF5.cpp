// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/hdf5/GroupHDF5.hpp>
#include <nix/Block.hpp>
#include <nix/hdf5/TagHDF5.hpp>
#include <nix/hdf5/MultiTagHDF5.hpp>
#include <nix/hdf5/BlockHDF5.hpp>
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
    std::string id = name_or_id;
    if (!util::looksLikeUUID(name_or_id) && block()->hasDataArray(name_or_id)) {
        id = block()->getDataArray(name_or_id)->id();
    }
    return data_array_group(false) ? data_array_group(false)->hasGroup(id) : false;
}


ndsize_t GroupHDF5::dataArrayCount() const {
    boost::optional<H5Group> g = data_array_group(false);
    return g ? g->objectCount() : size_t(0);
}


void GroupHDF5::addDataArray(const std::string &name_or_id) {
    boost::optional<H5Group> g = data_array_group(true);

    if (!block()->hasDataArray(name_or_id))
        throw std::runtime_error("GroupHDF5::addDataArray: DataArray not found in block!");

    auto target = std::dynamic_pointer_cast<DataArrayHDF5>(block()->getDataArray(name_or_id));
    g->createLink(target->group(), target->id());
}


std::shared_ptr<base::IDataArray> GroupHDF5::getDataArray(const std::string &name_or_id) const {
    std::shared_ptr<IDataArray> da;
    boost::optional<H5Group> g = data_array_group(false);
    std::string id = name_or_id;
    if (!util::looksLikeUUID(name_or_id) && block()->hasDataArray(name_or_id)) {
        id = block()->getDataArray(name_or_id)->id();
    }

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

    // extract vectors of names from vectors of new & old references
    std::vector<std::string> names_new(data_arrays.size());
    std::transform(data_arrays.begin(), data_arrays.end(), names_new.begin(), util::toName<DataArray>);

    size_t array_count = nix::check::fits_in_size_t(dataArrayCount(), "dataArrayCount() failed; count > size_t.");
    std::vector<DataArray> refs_old(array_count);
    for (size_t i = 0; i < refs_old.size(); i++) refs_old[i] = getDataArray(i);
    std::vector<std::string> names_old(refs_old.size());
    std::transform(refs_old.begin(), refs_old.end(), names_old.begin(), util::toName<DataArray>);

    // sort them
    std::sort(names_new.begin(), names_new.end());
    std::sort(names_new.begin(), names_new.end());

    // get names only in names_new (add), names only in names_old (remove) & ignore rest
    std::vector<std::string> names_add;
    std::vector<std::string> names_rem;
    std::set_difference(names_new.begin(), names_new.end(), names_old.begin(), names_old.end(),
                        std::inserter(names_add, names_add.begin()));
    std::set_difference(names_old.begin(), names_old.end(), names_new.begin(), names_new.end(),
                        std::inserter(names_rem, names_rem.begin()));

    // check if all new data_arrays exist & add them
    auto blck = std::dynamic_pointer_cast<BlockHDF5>(block());
    for (auto name : names_add) {
        if (!blck->hasDataArray(name))
            throw std::runtime_error("One or more data arrays do not exist in this block!");
        addDataArray(blck->getDataArray(name)->id());
    }
    // remove references
    for (auto name : names_rem) {
        if (!blck->hasDataArray(name))
            removeDataArray(blck->getDataArray(name)->id());
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

    // extract vectors of names from vectors of new & old references
    std::vector<std::string> names_new(tags.size());
    std::transform(tags.begin(), tags.end(), names_new.begin(), util::toName<Tag>);

    size_t tag_count = nix::check::fits_in_size_t(tagCount(), "tagCount() failed; count > size_t.");
    std::vector<Tag> refs_old(tag_count);
    for (size_t i = 0; i < refs_old.size(); i++) refs_old[i] = getTag(i);
    std::vector<std::string> names_old(refs_old.size());
    std::transform(refs_old.begin(), refs_old.end(), names_old.begin(), util::toName<Tag>);

    // sort them
    std::sort(names_new.begin(), names_new.end());
    std::sort(names_new.begin(), names_new.end());

    // get names only in names_new (add), names only in names_old (remove) & ignore rest
    std::vector<std::string> names_add;
    std::vector<std::string> names_rem;
    std::set_difference(names_new.begin(), names_new.end(), names_old.begin(), names_old.end(),
                        std::inserter(names_add, names_add.begin()));
    std::set_difference(names_old.begin(), names_old.end(), names_new.begin(), names_new.end(),
                        std::inserter(names_rem, names_rem.begin()));

    // check if all new tags exist & add them
    auto blck = std::dynamic_pointer_cast<BlockHDF5>(block());
    for (auto name : names_add) {
        if (!blck->hasTag(name))
            throw std::runtime_error("One or more tags do not exist in this block!");
        addTag(blck->getTag(name)->id());
    }
    // remove references
    for (auto name : names_rem) {
        if (!blck->hasTag(name))
            removeTag(blck->getTag(name)->id());
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
    // extract vectors of names from vectors of new & old references
    std::vector<std::string> names_new(multi_tags.size());
    std::transform(multi_tags.begin(), multi_tags.end(), names_new.begin(), util::toName<MultiTag>);

    size_t mtag_count = nix::check::fits_in_size_t(multiTagCount(), "multiTagCount() failed; count > size_t.");
    std::vector<MultiTag> refs_old(mtag_count);
    for (size_t i = 0; i < refs_old.size(); i++) refs_old[i] = getMultiTag(i);
    std::vector<std::string> names_old(refs_old.size());
    std::transform(refs_old.begin(), refs_old.end(), names_old.begin(), util::toName<MultiTag>);

    // sort them
    std::sort(names_new.begin(), names_new.end());
    std::sort(names_new.begin(), names_new.end());

    // get names only in names_new (add), names only in names_old (remove) & ignore rest
    std::vector<std::string> names_add;
    std::vector<std::string> names_rem;
    std::set_difference(names_new.begin(), names_new.end(), names_old.begin(), names_old.end(),
                        std::inserter(names_add, names_add.begin()));
    std::set_difference(names_old.begin(), names_old.end(), names_new.begin(), names_new.end(),
                        std::inserter(names_rem, names_rem.begin()));

    // check if all new tags exist & add them
    auto blck = std::dynamic_pointer_cast<BlockHDF5>(block());
    for (auto name : names_add) {
        if (!blck->hasMultiTag(name))
            throw std::runtime_error("One or more MultiTag do not exist in this block!");
        addMultiTag(blck->getMultiTag(name)->id());
    }
    // remove references
    for (auto name : names_rem) {
        if (!blck->hasMultiTag(name))
            removeMultiTag(blck->getMultiTag(name)->id());
    }
}

} // hdf5
} // nix
