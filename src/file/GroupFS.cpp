// Copyright (c) 2013 - 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/file/GroupFS.hpp>

#include <nix/file/DataArrayFS.hpp>
#include <nix/file/TagFS.hpp>
#include <nix/file/MultiTagFS.hpp>
#include <nix/file/BlockFS.hpp>

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
    // extract vectors of names from vectors of new & old references
    std::vector<std::string> names_new(data_arrays.size());
    transform(data_arrays.begin(), data_arrays.end(), names_new.begin(), util::toName<DataArray>);

    size_t count = nix::check::fits_in_size_t(dataArrayCount(), "dataArrayCount failed! count > than size_t!");
    std::vector<DataArray> refs_old(count);
    for (size_t i = 0; i < refs_old.size(); i++){
        refs_old[i] = getDataArray(i);
    }
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

    // check if all new references exist & add sources
    auto blck = std::dynamic_pointer_cast<BlockFS>(block());
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
    // extract vectors of names from vectors of new & old references
    std::vector<std::string> names_new(tags.size());
    transform(tags.begin(), tags.end(), names_new.begin(), util::toName<Tag>);

    size_t count = nix::check::fits_in_size_t(tagCount(), "tagCount() failed! count > than size_t!");
    std::vector<Tag> refs_old(count);
    for (size_t i = 0; i < refs_old.size(); i++){
        refs_old[i] = getTag(i);
    }
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

    // check if all new references exist & add sources
    auto blck = std::dynamic_pointer_cast<BlockFS>(block());
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
    // extract vectors of names from vectors of new & old references
    std::vector<std::string> names_new(multi_tags.size());
    transform(multi_tags.begin(), multi_tags.end(), names_new.begin(), util::toName<MultiTag>);

    size_t count = nix::check::fits_in_size_t(multiTagCount(), "multiTagCount() failed! count > than size_t!");
    std::vector<MultiTag> refs_old(count);
    for (size_t i = 0; i < refs_old.size(); i++){
        refs_old[i] = getMultiTag(i);
    }
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

    // check if all new references exist & add sources
    auto blck = std::dynamic_pointer_cast<BlockFS>(block());
    for (auto name : names_add) {
        if (!blck->hasMultiTag(name))
            throw std::runtime_error("One or more multiTags do not exist in this block!");
        addMultiTag(blck->getMultiTag(name)->id());
    }
    // remove references
    for (auto name : names_rem) {
        if (!blck->hasMultiTag(name))
            removeMultiTag(blck->getMultiTag(name)->id());
    }
}

} // file
} // nix

