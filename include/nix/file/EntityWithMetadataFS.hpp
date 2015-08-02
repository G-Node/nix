// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_ENTITY_WITH_METADATA_FS_H
#define NIX_ENTITY_WITH_METADATA_FS_H

#include <nix/base/ISection.hpp>
#include <nix/file/NamedEntityFS.hpp>

#include <string>
#include <memory>

namespace nix {
namespace file {

/**
 * Filesystem implementation of IEntityWithMetadata.
 */
class EntityWithMetadataFS : virtual public base::IEntityWithMetadata, public NamedEntityFS {

public:

    /**
     * Standard constructor for existing entity
     */
    EntityWithMetadataFS(const std::shared_ptr<base::IFile> &file, const std::string &loc);

    /**
     * Standard constructor for new entity
     */
    EntityWithMetadataFS(const std::shared_ptr<base::IFile> &file, const std::string &loc, const std::string &id,
                         const std::string &type, const std::string &name);

    /**
     * Standard constructor for new entity that preserves the creation time.
     */
    EntityWithMetadataFS(const std::shared_ptr<base::IFile> &file, const std::string &loc, const std::string &id,
                         const std::string &type, const std::string &name, time_t time);


    std::shared_ptr<base::ISection> metadata() const;


    void metadata(const std::string &id);


    void metadata(const none_t t);


    virtual ~EntityWithMetadataFS();

private:

    /**
     * checks if there is a link to some metadata folder
     */
    bool hasMetadata() const;

};


} // namespace nix::file
} // namespace nix

#endif // NIX_ENTITY_WITH_METADATA_FS_H
