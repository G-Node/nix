// Copyright (c) 2013-2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/file/BlockFS.hpp>
#include <nix/file/MultiTagFS.hpp>

namespace nix {
namespace file {

using namespace std;


BlockFS::BlockFS(const shared_ptr<base::IFile> &file, const string &loc)
    : EntityWithMetadataFS(file, loc)
{
    createSubFolders(file);
}


BlockFS::BlockFS(const shared_ptr<base::IFile> &file, const string &loc, const string &id, const string &type, const string &name)
    : EntityWithMetadataFS(file, loc, id, type, name)
{
    createSubFolders(file);
}


BlockFS::BlockFS(const shared_ptr<base::IFile> &file, const string &loc, const string &id, const string &type,
                 const string &name, time_t time)
    : EntityWithMetadataFS(file, loc, id, type, name, time)
{
    createSubFolders(file);
}


void nix::file::BlockFS::createSubFolders(const std::shared_ptr<base::IFile> &file) {
    boost::filesystem::path data_arrays("data_arrays");
    boost::filesystem::path tags("tags");
    boost::filesystem::path multi_tags("multi_tags");
    boost::filesystem::path sources("sources");
    boost::filesystem::path p(location());

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
    shared_ptr<base::ISource> source;
    boost::optional<boost::filesystem::path> path = source_dir.findByNameOrAttribute("entity_id", name_or_id);
    if (path) {
        return make_shared<SourceFS>(file(), path->string());
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
    boost::filesystem::path p = source_dir.sub_dir_by_index(index);
    shared_ptr<SourceFS> source;
    source = make_shared<SourceFS>(file(), p.string());
    return source;
}


std::shared_ptr<base::ISource> BlockFS::createSource(const std::string &name, const std::string &type) {
    if (name.empty()) {
        throw EmptyString("name");
    }
    if (hasSource(name)) {
        throw DuplicateName("createSource");
    }
    string id = util::createId();
    SourceFS s(file(), source_dir.location(), id, type, name);
    return make_shared<SourceFS>(s);
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
    shared_ptr<base::IDataArray> array;
    boost::optional<boost::filesystem::path> path = data_array_dir.findByNameOrAttribute("entity_id", name_or_id);
    if (path) {
        DataArrayFS a(file(), block(), path->string());
        return make_shared<DataArrayFS>(a);
    }
    return array;
}


std::shared_ptr<base::IDataArray> BlockFS::getDataArray(ndsize_t index) const {
    if (index >= dataArrayCount()) {
        throw OutOfBounds("Trying to access block.dataArray with invalid index.", index);
    }
    boost::filesystem::path p = data_array_dir.sub_dir_by_index(index);
    DataArrayFS a(file(), block(), p.string());
    return make_shared<DataArrayFS>(a);
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
    string id = util::createId();
    DataArrayFS da(file(), block(), data_array_dir.location(), id, type, name);
    da.createData(data_type, shape);
    return make_shared<DataArrayFS>(da);
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
    shared_ptr<base::ITag> tag;
    boost::optional<boost::filesystem::path> path = tag_dir.findByNameOrAttribute("entity_id", name_or_id);
    if (path) {
        TagFS t(file(), block(), path->string());
        return make_shared<TagFS>(t);
    }
    return tag;
}


std::shared_ptr<base::ITag> BlockFS::getTag(ndsize_t index) const {
    if (index >= tagCount()) {
        throw OutOfBounds("Trying to access block.tag with invalid index.", index);
    }
    boost::filesystem::path p = tag_dir.sub_dir_by_index(index);
    TagFS t(file(), block(), p.string());
    return make_shared<TagFS>(t);
}


std::shared_ptr<base::ITag> BlockFS::createTag(const std::string &name, const std::string &type,
                                               const std::vector<double> &position) {
    if (name.empty()) {
        throw EmptyString("Block::createTag empty name provided!");
    }
    if (hasTag(name)) {
        throw DuplicateName("Block::createTag: an entity with the same name already exists!");
    }
    string id = util::createId();
    TagFS tag(file(), block(), tag_dir.location(), id, type, name, position);
    return make_shared<TagFS>(tag);
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
    shared_ptr<base::IMultiTag> mtag;
    boost::optional<boost::filesystem::path> path = multi_tag_dir.findByNameOrAttribute("entity_id", name_or_id);
    if (path) {
        MultiTagFS t(file(), block(), path->string());
        return make_shared<MultiTagFS>(t);
    }
    return mtag;
}


std::shared_ptr<base::IMultiTag> BlockFS::getMultiTag(ndsize_t index) const {
    if (index >= multiTagCount()) {
        throw OutOfBounds("Trying to access block.multiTag with invalid index.", index);
    }
    boost::filesystem::path p = multi_tag_dir.sub_dir_by_index(index);
    MultiTagFS t(file(), block(), p.string());
    return make_shared<MultiTagFS>(t);
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
    string id = util::createId();
    MultiTagFS tag(file(), block(), multi_tag_dir.location(), id, type, name, positions);
    return make_shared<MultiTagFS>(tag);
}


bool BlockFS::deleteMultiTag(const std::string &name_or_id) {
    return multi_tag_dir.removeObjectByNameOrAttribute("entity_id", name_or_id);
}



std::shared_ptr<base::IBlock> BlockFS::block() const {
    return const_pointer_cast<BlockFS>(shared_from_this());
}


BlockFS::~BlockFS() {
}

} // nix::file
} // nix