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


boost::optional<Directory> GroupFS::groupForObjectType(ObjectType type) const {
    boost::optional<Directory> p;

    switch (type) {
    case ObjectType::DataArray:
        p = data_array_group;
        break;
    case ObjectType::Tag:
        p = tag_group;
        break;
    case ObjectType::MultiTag:
        p = multi_tag_group;
        break;
    default:
        p = boost::optional<Directory>();
    }

    return p;
}

boost::optional<bfs::path> GroupFS::findEntityGroup(const nix::Identity &ident) const {
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
    std::string needle = haveId ? iid : iname;
    bool foundNeedle = p->hasObject(needle);

    if (foundNeedle) {
        g = boost::make_optional(bfs::path(p->location()) / needle);
    } else if (haveName) {
        g = p->findByNameOrAttribute("name", iname);
    }

    if (g && haveName && haveId) {
        std::string ename;
        DirectoryWithAttributes temp(*g);
        temp.getAttr("name", ename);

        if (ename != iname) {
            return boost::optional<bfs::path>();
        }
    }

    return g;
}

bool GroupFS::hasEntity(const nix::Identity &ident) const {
    boost::optional<bfs::path> p = findEntityGroup(ident);
    return !!p;
}


std::shared_ptr<base::IEntity> GroupFS::getEntity(const nix::Identity &ident) const {
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
        std::shared_ptr<TagFS> t;
        if (eg) {
            t = std::make_shared<TagFS>(file(), block(), eg->string());
        }
        return t;
    }
    case ObjectType::MultiTag: {
        std::shared_ptr<MultiTagFS> t;
        if (eg) {
            t = std::make_shared<MultiTagFS>(file(), block(), eg->string());
        }
        return t;
    }
    default:
        return std::shared_ptr<base::IEntity>();
    }
}


std::shared_ptr<base::IEntity> GroupFS::getEntity(ObjectType type, ndsize_t index) const {
    boost::optional<Directory> eg = groupForObjectType(type);
    std::string name = eg ? eg->sub_dir_by_index(index).string() : "";
    std::string full_path = eg ? eg->sub_dir_by_index(index).string() : "";
    std::size_t pos = full_path.find_last_of(bfs::path::preferred_separator);      // position of "live" in str
    if (pos == std::string::npos)
        return getEntity({"", "", type});
    std::string id = full_path.substr (pos+1);
    return getEntity({id, "", type});
}


ndsize_t GroupFS::entityCount(ObjectType type) const {
    boost::optional<Directory> g = groupForObjectType(type);
    return g ? g->subdirCount() : ndsize_t(0);
}


bool GroupFS::removeEntity(const nix::Identity &ident) {
    boost::optional<Directory> p = groupForObjectType(ident.type());
    bool have_name = ident.name() != "";
    bool have_id = ident.id() != "";
    if (!have_name && !have_id) {
        return false;
    }
    if (have_id) {
        return p->removeObjectByNameOrAttribute("entity_id", ident.id());
    }
    return p->removeObjectByNameOrAttribute("name", ident.name());
}


void GroupFS::addEntity(const nix::Identity &ident) {
    boost::optional<Directory> p = groupForObjectType(ident.type());
    if(!block()->hasEntity(ident)) {
        throw std::runtime_error("Entity do not exist in this block!");
    }
    auto target = std::dynamic_pointer_cast<EntityFS>(block()->getEntity(ident));
    p->createDirectoryLink(target->location(), target->id());
}

} // file
} // nix
