// Copyright (c) 2013 - 2015, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_ENTITYWITHSOURCESFS_HPP
#define NIX_ENTITYWITHSOURCESFS_HPP

#include <nix/base/IBlock.hpp>
#include <nix/base/IEntityWithSources.hpp>
#include "SourceFS.hpp"
#include "EntityWithMetadataFS.hpp"

namespace bfs = boost::filesystem;

namespace nix {
namespace file {


/**
 * Base class for entities that are associated with sources.
 */
class EntityWithSourcesFS: public virtual base::IEntityWithSources, public EntityWithMetadataFS {

private:
    Directory sources_dir;
    std::shared_ptr<base::IBlock> entity_block;

public:

    /**
     * Standard constructor for existing entity.
     */
    EntityWithSourcesFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const bfs::path &loc);

    /**
     * Standard constructor for new entity.
     */
    EntityWithSourcesFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const bfs::path &loc,
                          const std::string &id, const std::string &type, const std::string &name);

    /**
     * Standard constructor for new entity that preserves the creation time.
     */
    EntityWithSourcesFS(const std::shared_ptr<base::IFile> &file, const std::shared_ptr<base::IBlock> &block, const bfs::path &loc,
                          const std::string &id, const std::string &type, const std::string &name, time_t time);

    ndsize_t sourceCount() const;


    bool hasSource(const std::string &id) const;

    /**
      * @brief Set all sources associations for this entity.
      *
      * All previously existing associations will be overwritten.
      *
      * @param sources     A vector with all sources.
      */
    void sources(const std::vector<Source> &sources);

    void addSource(const std::string &id);


    bool removeSource(const std::string &id);


    std::shared_ptr<base::ISource> getSource(const std::string &id) const;


    std::shared_ptr<base::ISource> getSource(const size_t index) const;

    /**
     * Destructor.
     */
    virtual ~EntityWithSourcesFS();


    std::shared_ptr<base::IBlock> block() const;

};


} // namespace file
} // namespace nix

#endif //NIX_ENTITYWITHSOURCESFS_HPP
