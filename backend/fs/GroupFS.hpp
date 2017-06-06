// Copyright (c) 2013 - 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.
#ifndef NIX_GROUPFS_HPP
#define NIX_GROUPFS_HPP


#include "EntityWithSourcesFS.hpp"
#include "Directory.hpp"
#include <nix/util/util.hpp>
#include <nix/Group.hpp>

namespace nix {
namespace file {

/**
 * Class that represents a NIX Group entity.
 */
class GroupFS : virtual public base::IGroup, public  EntityWithSourcesFS,
                public std::enable_shared_from_this<GroupFS> {

private:

    Directory data_array_group, tag_group, multi_tag_group;

    void createSubFolders(const std::shared_ptr<base::IFile> &file);

    // Helper methods for generic entity related methods below
    boost::optional<Directory> groupForObjectType(ObjectType ot) const;

    boost::optional<boost::filesystem::path> findEntityGroup(const nix::Identity &ident) const;
public:
    //--------------------------------------------------
    // Generic entity methods
    //--------------------------------------------------
    bool hasEntity(const nix::Identity &ident) const;

    std::shared_ptr<base::IEntity> getEntity(const nix::Identity &ident) const;

    std::shared_ptr<base::IEntity> getEntity(ObjectType type, ndsize_t index) const;

    ndsize_t entityCount(ObjectType type) const;

    bool removeEntity(const nix::Identity &ident);

    void addEntity(const nix::Identity &ident);

    /**
    * Standard constructor for an existing Group
    */
    GroupFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const std::string &loc);

    /**
    * Standard constructor for a new Group Tag
    */
    GroupFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const std::string &loc, const std::string &id,
              const std::string &type, const std::string &name);

    /**
    * Standard constructor for new Group that preserves the creation time.
    */
    GroupFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const std::string &loc, const std::string &id,
              const std::string &type, const std::string &name, const time_t time);

};
}
}

#endif //NIX_GROUPFS_HPP
