// Copyright (c) 2013-2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/file/BlockFS.hpp>
#include <nix/file/MultiTagFS.hpp>

namespace bfs = boost::filesystem;

namespace nix {
namespace file {

BlockFS::BlockFS(const std::shared_ptr<base::IFile> &file, const std::string &loc)
    : EntityWithMetadataFS(file, loc)
{
    createSubFolders(file);
}


BlockFS::BlockFS(const std::shared_ptr<base::IFile> &file, const std::string &loc, const std::string &id,
                 const std::string &type, const std::string &name)
    : EntityWithMetadataFS(file, loc, id, type, name)
{
    createSubFolders(file);
}


BlockFS::BlockFS(const std::shared_ptr<base::IFile> &file, const std::string &loc, const std::string &id,
                 const std::string &type, const std::string &name, time_t time)
    : EntityWithMetadataFS(file, loc, id, type, name, time)
{
    createSubFolders(file);
}


void nix::file::BlockFS::createSubFolders(const std::shared_ptr<base::IFile> &file) {
    bfs::path data_arrays("data_arrays");
    bfs::path tags("tags");
    bfs::path multi_tags("multi_tags");
    bfs::path sources("sources");
    bfs::path p(location());

    data_array_dir = Directory(p / data_arrays, file->fileMode());
    tag_dir = Directory(p / tags, file->fileMode());
    multi_tag_dir = Directory(p / multi_tags, file->fileMode());
    source_dir = Directory(p / sources, file->fileMode());
}

//--------------------------------------------------
// Methods concerning sources
//--------------------------------------------------

bool BlockFS::hasSource(const std::string &name_or_id) const {
    return getSource(name_or_id) != nullptr;
}


std::shared_ptr<base::ISource> BlockFS::getSource(const std::string &name_or_id) const {
    std::shared_ptr<base::ISource> source;
    boost::optional<bfs::path> path = source_dir.findByNameOrAttribute("entity_id", name_or_id);
    if (path) {
        return std::make_shared<SourceFS>(file(), path->string());
    }
    return source;
}


ndsize_t BlockFS::sourceCount() const {
    return source_dir.subdirCount();
}


std::shared_ptr<base::ISource> BlockFS::getSource(ndsize_t index) const {
    if (index >= sourceCount()) {
        throw OutOfBounds("Trying to access block.source with invalid index.", index);
    }
    bfs::path p = source_dir.sub_dir_by_index(index);
    return std::make_shared<SourceFS>(file(), p.string());
}


std::shared_ptr<base::ISource> BlockFS::createSource(const std::string &name, const std::string &type) {
    if (name.empty()) {
        throw EmptyString("name");
    }
    if (hasSource(name)) {
        throw DuplicateName("createSource");
    }
    std::string id = util::createId();
    return std::make_shared<SourceFS>(file(), source_dir.location(), id, type, name);
}


bool BlockFS::deleteSource(const std::string &name_or_id) {
    return source_dir.removeObjectByNameOrAttribute("entity_id", name_or_id);
}

//--------------------------------------------------
// Methods concerning data arrays
//--------------------------------------------------

bool BlockFS::hasDataArray(const std::string &name_or_id) const {
    return getDataArray(name_or_id) != nullptr;
}


std::shared_ptr<base::IDataArray> BlockFS::getDataArray(const std::string &name_or_id) const {
    std::shared_ptr<base::IDataArray> array;
    boost::optional<bfs::path> path = data_array_dir.findByNameOrAttribute("entity_id", name_or_id);
    if (path) {
        return std::make_shared<DataArrayFS>(file(), block(), path->string());
    }
    return array;
}


std::shared_ptr<base::IDataArray> BlockFS::getDataArray(ndsize_t index) const {
    if (index >= dataArrayCount()) {
        throw OutOfBounds("Trying to access block.dataArray with invalid index.", index);
    }
    bfs::path p = data_array_dir.sub_dir_by_index(index);
    return std::make_shared<DataArrayFS>(file(), block(), p.string());
}


ndsize_t BlockFS::dataArrayCount() const {
    return data_array_dir.subdirCount();
}


std::shared_ptr<base::IDataArray> BlockFS::createDataArray(const std::string &name, const std::string &type,
                                                           nix::DataType data_type, const NDSize &shape) {
    if (name.empty()) {
        throw EmptyString("Block::createDataArray empty name provided!");
    }
    if (hasDataArray(name)) {
        throw DuplicateName("Block::createDataArray: an entity with the same name already exists!");
    }
    std::string id = util::createId();
    DataArrayFS da(file(), block(), data_array_dir.location(), id, type, name);
    da.createData(data_type, shape);
    return std::make_shared<DataArrayFS>(da);
}


bool BlockFS::deleteDataArray(const std::string &name_or_id) {
    return data_array_dir.removeObjectByNameOrAttribute("entity_id", name_or_id);
}

//--------------------------------------------------
// Methods concerning tags
//--------------------------------------------------

bool BlockFS::hasTag(const std::string &name_or_id) const {
    return getTag(name_or_id) != nullptr;
}


ndsize_t BlockFS::tagCount() const {
    return tag_dir.subdirCount();
}


std::shared_ptr<base::ITag> BlockFS::getTag(const std::string &name_or_id) const {
    std::shared_ptr<base::ITag> tag;
    boost::optional<bfs::path> path = tag_dir.findByNameOrAttribute("entity_id", name_or_id);
    if (path) {
        return std::make_shared<TagFS>(file(), block(), path->string());
    }
    return tag;
}


std::shared_ptr<base::ITag> BlockFS::getTag(ndsize_t index) const {
    if (index >= tagCount()) {
        throw OutOfBounds("Trying to access block.tag with invalid index.", index);
    }
    boost::filesystem::path p = tag_dir.sub_dir_by_index(index);
    return std::make_shared<TagFS>(file(), block(), p.string());
}


std::shared_ptr<base::ITag> BlockFS::createTag(const std::string &name, const std::string &type,
                                               const std::vector<double> &position) {
    if (name.empty()) {
        throw EmptyString("Block::createTag empty name provided!");
    }
    if (hasTag(name)) {
        throw DuplicateName("Block::createTag: an entity with the same name already exists!");
    }
    std::string id = util::createId();
    return std::make_shared<TagFS>(file(), block(), tag_dir.location(), id, type, name, position);
}


bool BlockFS::deleteTag(const std::string &name_or_id) {
    return tag_dir.removeObjectByNameOrAttribute("entity_id", name_or_id);
}

//--------------------------------------------------
// Methods concerning multi tags.
//--------------------------------------------------

bool BlockFS::hasMultiTag(const std::string &name_or_id) const {
    return getMultiTag(name_or_id) != nullptr;
}


ndsize_t BlockFS::multiTagCount() const {
    return multi_tag_dir.subdirCount();
}


std::shared_ptr<base::IMultiTag> BlockFS::getMultiTag(const std::string &name_or_id) const {
    std::shared_ptr<base::IMultiTag> mtag;
    boost::optional<bfs::path> path = multi_tag_dir.findByNameOrAttribute("entity_id", name_or_id);
    if (path) {
        return std::make_shared<MultiTagFS>(file(), block(), path->string());
    }
    return mtag;
}


std::shared_ptr<base::IMultiTag> BlockFS::getMultiTag(ndsize_t index) const {
    if (index >= multiTagCount()) {
        throw OutOfBounds("Trying to access block.multiTag with invalid index.", index);
    }
    bfs::path p = multi_tag_dir.sub_dir_by_index(index);
    return std::make_shared<MultiTagFS>(file(), block(), p.string());
}


std::shared_ptr<base::IMultiTag> BlockFS::createMultiTag(const std::string &name, const std::string &type,
                                                         const DataArray &positions) {
    if (name.empty()) {
        throw EmptyString("Block::createMultiTag empty name provided!");
    }
    if (!positions) {
        throw UninitializedEntity();
    }
    if (hasMultiTag(name)) {
        throw DuplicateName("Block::createMultiTag: an entity with the same name already exists!");
    }
    std::string id = util::createId();
    return std::make_shared<MultiTagFS>(file(), block(), multi_tag_dir.location(), id, type, name, positions);
}


bool BlockFS::deleteMultiTag(const std::string &name_or_id) {
    return multi_tag_dir.removeObjectByNameOrAttribute("entity_id", name_or_id);
}


std::shared_ptr<base::IBlock> BlockFS::block() const {
    return std::const_pointer_cast<BlockFS>(shared_from_this());
}


BlockFS::~BlockFS() {
}

} // nix::file
} // nix