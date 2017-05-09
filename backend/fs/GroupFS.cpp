// Copyright (c) 2013 - 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "GroupFS.hpp"

#include "DataArrayFS.hpp"
#include "TagFS.hpp"
#include "MultiTagFS.hpp"
#include "BlockFS.hpp"

namespace bfs= boost::filesystem;

namespace nix {
namespace file {


GroupFS::GroupFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block,
                 const std::string &loc)
    : EntityWithSourcesFS(file, block, loc)
{
    createSubFolders(file);
}


GroupFS::GroupFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block,
                 const std::string &loc, const std::string &id, const std::string &type, const std::string &name)
    : GroupFS(file, block, loc, id, type, name, util::getTime())
{
}


GroupFS::GroupFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block,
                 const std::string &loc, const std::string &id, const std::string &type, const std::string &name, time_t time)
    : EntityWithSourcesFS(file, block, loc, id, type, name, time)
{
    createSubFolders(file);
}


void GroupFS::createSubFolders(const std::shared_ptr<base::IFile> &file) {
    bfs::path das("data_arrays");
    bfs::path tags("tags");
    bfs::path mtags("multi_tags");
    bfs::path p(location());

    data_array_group = Directory(p / das, file->fileMode());
    tag_group = Directory(p / tags, file->fileMode());
    multi_tag_group = Directory(p / mtags, file->fileMode());
}


bool GroupFS::hasDataArray(const std::string &name_or_id) const {
    std::string id = name_or_id;

    if (!util::looksLikeUUID(name_or_id) && block()->hasDataArray(name_or_id)) {
        id = block()->getDataArray(name_or_id)->id();
    }

    return data_array_group.hasObject(id);
}


ndsize_t GroupFS::dataArrayCount() const {
    return data_array_group.subdirCount();
}


void GroupFS::addDataArray(const std::string &name_or_id) {
    if (name_or_id.empty())
        throw EmptyString("addDataArray");

    if (!block()->hasDataArray(name_or_id))
        throw std::runtime_error("GroupFS::addDataArray: DataArray not found in block!");

    auto target = std::dynamic_pointer_cast<DataArrayFS>(block()->getDataArray(name_or_id));
    data_array_group.createDirectoryLink(target->location(), target->id());
}


std::shared_ptr<base::IDataArray> GroupFS::getDataArray(const std::string &name_or_id) const {
    std::shared_ptr<base::IDataArray> da;

    std::string id = name_or_id;
    if (!util::looksLikeUUID(name_or_id) && block()->hasDataArray(name_or_id)) {
        id = block()->getDataArray(name_or_id)->id();
    }

    if (hasDataArray(id)) {
        boost::optional<bfs::path> path = data_array_group.findByNameOrAttribute("name", name_or_id);
        if (path) {
            return std::make_shared<DataArrayFS>(file(), block(), path->string());
        }
    }
    return da;
}


std::shared_ptr<base::IDataArray>  GroupFS::getDataArray(ndsize_t index) const {
    if(index > dataArrayCount()) {
        throw OutOfBounds("No reference at given index", index);
    }
    bfs::path p = data_array_group.sub_dir_by_index(index);
    return std::make_shared<DataArrayFS>(file(), block(), p.string());
}


bool GroupFS::removeDataArray(const std::string &name_or_id) {
    return data_array_group.removeObjectByNameOrAttribute("name", name_or_id);
}


void GroupFS::dataArrays(const std::vector<DataArray> &data_arrays) {
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

    auto blck = std::dynamic_pointer_cast<BlockFS>(block());
    for (const auto &da : add) {
        DataArray a = blck->getDataArray(da.name());
        if (!a || a.id() != da.id())
            throw std::runtime_error("One or more data arrays do not exist in this block!");
        addDataArray(a.id());
    }
    for (const auto &da : rem) {
        removeDataArray(da.id());
    }
}


bool GroupFS::hasTag(const std::string &name_or_id) const {
    std::string id = name_or_id;
    if (!util::looksLikeUUID(name_or_id) && block()->hasTag(name_or_id)) {
        id = block()->getTag(name_or_id)->id();
    }
    return tag_group.hasObject(id);
}


ndsize_t GroupFS::tagCount() const {
    return tag_group.subdirCount();
}


void GroupFS::addTag(const std::string &name_or_id) {
    if (name_or_id.empty())
        throw EmptyString("addTag");

    if (!block()->hasTag(name_or_id))
        throw std::runtime_error("GroupFS::addTag: Tag not found in block!");

    auto target = std::dynamic_pointer_cast<TagFS>(block()->getTag(name_or_id));
    tag_group.createDirectoryLink(target->location(), target->id());
}


std::shared_ptr<base::ITag> GroupFS::getTag(const std::string &name_or_id) const {
    std::shared_ptr<base::ITag> tag;

    std::string id = name_or_id;
    if (!util::looksLikeUUID(name_or_id) && block()->hasTag(name_or_id)) {
        id = block()->getTag(name_or_id)->id();
    }

    if (hasTag(id)) {
        boost::optional<bfs::path> path = tag_group.findByNameOrAttribute("name", name_or_id);
        if (path) {
            return std::make_shared<TagFS>(file(), block(), path->string());
        }
    }
    return tag;
}


std::shared_ptr<base::ITag>  GroupFS::getTag(ndsize_t index) const {
    if(index > tagCount()) {
        throw OutOfBounds("No tag at given index", index);
    }
    bfs::path p = tag_group.sub_dir_by_index(index);
    return std::make_shared<TagFS>(file(), block(), p.string());
}


bool GroupFS::removeTag(const std::string &name_or_id) {
    return tag_group.removeObjectByNameOrAttribute("name", name_or_id);
}


void GroupFS::tags(const std::vector<Tag> &tags) {
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

    auto blck = std::dynamic_pointer_cast<BlockFS>(block());
    for (const auto &t : add) {
        Tag tag = blck->getTag(t.name());
        if (!tag || tag.id() != t.id())
            throw std::runtime_error("One or more tags do not exist in this block!");
        addTag(t.id());
    }
    for (const auto &t : rem) {
        removeTag(t.id());
    }
}


bool GroupFS::hasMultiTag(const std::string &name_or_id) const {
    std::string id = name_or_id;
    if (!util::looksLikeUUID(name_or_id) && block()->hasMultiTag(name_or_id)) {
        id = block()->getMultiTag(name_or_id)->id();
    }
    return multi_tag_group.hasObject(id);
}


ndsize_t GroupFS::multiTagCount() const {
    return multi_tag_group.subdirCount();
}


void GroupFS::addMultiTag(const std::string &name_or_id) {
    if (name_or_id.empty())
        throw EmptyString("addTag");

    if (!block()->hasMultiTag(name_or_id))
        throw std::runtime_error("GroupFS::addMultiTag: MultiTag not found in block!");

    auto target = std::dynamic_pointer_cast<MultiTagFS>(block()->getMultiTag(name_or_id));
    multi_tag_group.createDirectoryLink(target->location(), target->id());
}


std::shared_ptr<base::IMultiTag> GroupFS::getMultiTag(const std::string &name_or_id) const {
    std::shared_ptr<base::IMultiTag> mtag;

    std::string id = name_or_id;
    if (!util::looksLikeUUID(name_or_id) && block()->hasMultiTag(name_or_id)) {
        id = block()->getMultiTag(name_or_id)->id();
    }

    if (hasMultiTag(id)) {
        boost::optional<bfs::path> path = multi_tag_group.findByNameOrAttribute("name", name_or_id);
        if (path) {
            return std::make_shared<MultiTagFS>(file(), block(), path->string());
        }
    }
    return mtag;
}


std::shared_ptr<base::IMultiTag>  GroupFS::getMultiTag(ndsize_t index) const {
    if(index > multiTagCount()) {
        throw OutOfBounds("No multi tag at given index", index);
    }
    bfs::path p = multi_tag_group.sub_dir_by_index(index);
    return std::make_shared<MultiTagFS>(file(), block(), p.string());
}


bool GroupFS::removeMultiTag(const std::string &name_or_id) {
    return multi_tag_group.removeObjectByNameOrAttribute("name", name_or_id);
}


void GroupFS::multiTags(const std::vector<MultiTag> &multi_tags) {
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

    auto blck = std::dynamic_pointer_cast<BlockFS>(block());
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

} // file
} // nix

