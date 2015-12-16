// Copyright (c) 2013 - 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_ENTITY_FS_H
#define NIX_ENTITY_FS_H

#include <nix/base/IFile.hpp>
#include <nix/base/IEntity.hpp>
#include <nix/file/DirectoryWithAttributes.hpp>

#include <string>
#include <memory>

namespace nix {
namespace file {


/**
 * Filesystem implementation of IEntity
 */
class EntityFS : virtual public base::IEntity, public DirectoryWithAttributes {

private:

    std::shared_ptr<base::IFile>  entity_file;

public:

    EntityFS(const std::shared_ptr<base::IFile> &file, const boost::filesystem::path &loc);


    EntityFS(const std::shared_ptr<base::IFile> &file, const boost::filesystem::path &loc, const std::string &id, time_t time);


    std::string id() const;


    time_t updatedAt() const;


    time_t createdAt() const;


    void setUpdatedAt();


    void forceUpdatedAt();


    void setCreatedAt();


    void forceCreatedAt(time_t t);
    

    bool isValidEntity() const;


    bool operator==(const EntityFS &other) const;


    bool operator!=(const EntityFS &other) const;


    virtual ~EntityFS();

protected:

    std::shared_ptr<base::IFile> file() const;

};


} // namespace hdf5
} // namespace nix

#endif // NIX_ENTITY_FS_H
