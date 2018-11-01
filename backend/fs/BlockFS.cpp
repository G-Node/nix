// Copyright (c) 2013-2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include "BlockFS.hpp"
#include "MultiTagFS.hpp"
#include "GroupFS.hpp"

namespace bfs = boost::filesystem;

namespace nix {
namespace file {

BlockFS::BlockFS(const std::shared_ptr<base::IFile> &file, const std::string &loc)
    : EntityWithMetadataFS(file, loc), compr(Compression::Auto)
{
    createSubFolders(file);
}


BlockFS::BlockFS(const std::shared_ptr<base::IFile> &file, const std::string &loc, const std::string &id,
                 const std::string &type, const std::string &name, const Compression &compression)
     : EntityWithMetadataFS(file, loc, id, type, name), compr(compression)
{
    createSubFolders(file);
}


BlockFS::BlockFS(const std::shared_ptr<base::IFile> &file, const std::string &loc, const std::string &id,
                 const std::string &type, const std::string &name, time_t time, const Compression &compression)
     : EntityWithMetadataFS(file, loc, id, type, name, time), compr(compression)
{
    createSubFolders(file);
}


void nix::file::BlockFS::createSubFolders(const std::shared_ptr<base::IFile> &file) {
    bfs::path data_arrays("data_arrays");
    bfs::path tags("tags");
    bfs::path multi_tags("multi_tags");
    bfs::path sources("sources");
    bfs::path p(location());
    bfs::path groups("groups");

    data_array_dir = Directory(p / data_arrays, file->fileMode());
    tag_dir = Directory(p / tags, file->fileMode());
    multi_tag_dir = Directory(p / multi_tags, file->fileMode());
    source_dir = Directory(p / sources, file->fileMode());
    group_dir = Directory(p / groups, file->fileMode());
}

//--------------------------------------------------
// Generic access methods
//--------------------------------------------------

boost::optional<Directory> BlockFS::groupForObjectType(ObjectType type) const {
    boost::optional<Directory> p;

    switch (type) {
    case ObjectType::DataArray:
        p = data_array_dir;
        break;
    case ObjectType::Group:
        p = group_dir;
        break;
    case ObjectType::MultiTag:
        p = multi_tag_dir;
        break;
    case ObjectType::Source:
        p = source_dir;
        break;
    case ObjectType::Tag:
        p = tag_dir;
        break;
    default:
        p = boost::optional<Directory>();
    }

    return p;
}


boost::optional<bfs::path> BlockFS::findEntityGroup(const nix::Identity &ident) const {
    boost::optional<Directory> p = groupForObjectType(ident.type());

    if (!p) {
        return  boost::optional<bfs::path>();
    }

    boost::optional<bfs::path> g;
    const std::string &iname = ident.name();
    const std::string &iid = ident.id();

    bool haveName = !iname.empty();
    bool haveId = !iid.empty();

    if (!haveName && !haveId) {
        return g;
    }

    std::string needle = haveName ? iname : iid;
    bool foundNeedle = p->hasObject(needle);

    if (foundNeedle) {
        g = boost::make_optional(bfs::path(p->location()) / needle);
    } else if (haveId) {
        g = p->findByNameOrAttribute("entity_id", iid);
    }

    if (g && haveName && haveId) {
        std::string eid;
        DirectoryWithAttributes temp(*g);
        temp.getAttr("entity_id", eid);

        if (eid != iid) {
            return boost::optional<bfs::path>();
        }
    }

    return g;
}

std::string BlockFS::resolveEntityId(const nix::Identity &ident) const {
    if (!ident.id().empty()) {
        return ident.id();
    }

    boost::optional<bfs::path> g = findEntityGroup(ident);
    if (!g) {
        return "";
    }

     std::string eid = "";
     DirectoryWithAttributes temp(*g);
     temp.getAttr("entity_id", eid);

     return eid;
}

bool BlockFS::hasEntity(const nix::Identity &ident) const {
    boost::optional<bfs::path> p = findEntityGroup(ident);
    return !!p;
}

std::shared_ptr<base::IEntity> BlockFS::getEntity(const nix::Identity &ident) const {
    boost::optional<bfs::path> eg = findEntityGroup(ident);

    switch (ident.type()) {
    case ObjectType::DataArray: {
        std::shared_ptr<DataArrayFS> da;
        if (eg) {
            da = std::make_shared<DataArrayFS>(file(), block(), *eg);
        }
        return da;
    }
    case ObjectType::Tag: {
        std::shared_ptr<TagFS> tag;
        if (eg) {
            tag = std::make_shared<TagFS>(file(), block(), eg->string());
        }
        return tag;
    }
    case ObjectType::MultiTag: {
        std::shared_ptr<MultiTagFS> tag;
        if (eg) {
            tag = std::make_shared<MultiTagFS>(file(), block(), eg->string());
        }
        return tag;
    }
    case ObjectType::Group: {
        std::shared_ptr<GroupFS> groups;
        if (eg) {
            groups = std::make_shared<GroupFS>(file(), block(), eg->string());
        }
        return groups;
    }
    case ObjectType::Source: {
        std::shared_ptr<SourceFS> source;
        if (eg) {
            source = std::make_shared<SourceFS>(file(), block(), eg->string());
        }
        return source;
    }
    default:
        return std::shared_ptr<base::IEntity>();
    }

    return std::shared_ptr<base::IEntity>();
}

std::shared_ptr<base::IEntity> BlockFS::getEntity(ObjectType type, ndsize_t index) const {
    boost::optional<Directory> eg = groupForObjectType(type);
    std::string full_path = eg ? eg->sub_dir_by_index(index).string() : "";
    std::size_t pos = full_path.find_last_of(bfs::path::preferred_separator);      // position of "live" in str
    if (pos == std::string::npos)
        return getEntity({"", "", type});
    std::string name = full_path.substr (pos+1);
    return getEntity({name, "", type});
}

ndsize_t BlockFS::entityCount(ObjectType type) const {
    boost::optional<Directory> g = groupForObjectType(type);
    return g ? g->subdirCount() : ndsize_t(0);
}

bool BlockFS::removeEntity(const nix::Identity &ident) {
    boost::optional<Directory> p = groupForObjectType(ident.type());
    boost::optional<bfs::path> eg = findEntityGroup(ident);

    if (!p || !eg) {
        return false;
    }

    std::string name;
    DirectoryWithAttributes temp(*eg);
    temp.getAttr("name", name);
    return p->removeObjectByNameOrAttribute("entity_id", name);
}

//--------------------------------------------------
// Methods concerning sources
//--------------------------------------------------

bool BlockFS::hasSource(const std::string &name_or_id) const {
    return hasEntity({name_or_id, ObjectType::Source});
}

std::shared_ptr<base::ISource> BlockFS::createSource(const std::string &name, const std::string &type) {
    if (name.empty()) {
        throw EmptyString("name");
    }
    if (hasSource(name)) {
        throw DuplicateName("createSource");
    }
    std::string id = util::createId();
    return std::make_shared<SourceFS>(file(), block(), source_dir.location(), id, type, name);
}


bool BlockFS::deleteSource(const std::string &name_or_id) {
    return source_dir.removeObjectByNameOrAttribute("entity_id", name_or_id);
}

//--------------------------------------------------
// Methods concerning data arrays
//--------------------------------------------------

std::shared_ptr<base::IDataArray> BlockFS::createDataArray(const std::string &name, const std::string &type,
                                                           nix::DataType data_type, const NDSize &shape,
                                                           const Compression &compression) {
    if (name.empty()) {
        throw EmptyString("Block::createDataArray empty name provided!");
    }
    if (hasEntity({name, ObjectType::DataArray})) {
        throw DuplicateName("Block::createDataArray: an entity with the same name already exists!");
    }
    std::string id = util::createId();
    DataArrayFS da(file(), block(), data_array_dir.location(), id, type, name);
    da.createData(data_type, shape, compression);
    return std::make_shared<DataArrayFS>(da);
}

//--------------------------------------------------
// Methods concerning tags
//--------------------------------------------------

std::shared_ptr<base::ITag> BlockFS::createTag(const std::string &name, const std::string &type,
                                               const std::vector<double> &position) {
    if (name.empty()) {
        throw EmptyString("Block::createTag empty name provided!");
    }
    if (hasEntity({name, ObjectType::Tag})) {
        throw DuplicateName("Block::createTag: an entity with the same name already exists!");
    }
    std::string id = util::createId();
    return std::make_shared<TagFS>(file(), block(), tag_dir.location(), id, type, name, position);
}

//--------------------------------------------------
// Methods concerning multi tags.
//--------------------------------------------------

std::shared_ptr<base::IMultiTag> BlockFS::createMultiTag(const std::string &name, const std::string &type,
                                                         const DataArray &positions) {
    if (name.empty()) {
        throw EmptyString("Block::createMultiTag empty name provided!");
    }
    if (!positions) {
        throw UninitializedEntity();
    }
    if (hasEntity({name, ObjectType::MultiTag})) {
        throw DuplicateName("Block::createMultiTag: an entity with the same name already exists!");
    }
    std::string id = util::createId();
    return std::make_shared<MultiTagFS>(file(), block(), multi_tag_dir.location(), id, type, name, positions);
}

//--------------------------------------------------
// Methods concerning groups.
//--------------------------------------------------

std::shared_ptr<base::IGroup> BlockFS::createGroup(const std::string &name, const std::string &type) {
    if (name.empty()) {
        throw EmptyString("Block::createGroup empty name provided!");
    }
    if (hasEntity({name, ObjectType::Group})) {
        throw DuplicateName("Block::createGroup: an entity with the same name already exists!");
    }
    std::string id = util::createId();
    return std::make_shared<GroupFS>(file(), block(), group_dir.location(), id, type, name);
}


std::shared_ptr<base::IBlock> BlockFS::block() const {
    return std::const_pointer_cast<BlockFS>(shared_from_this());
}

BlockFS::~BlockFS() {
}

} // nix::file
} // nix
