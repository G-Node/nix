// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_ENTITY_WITH_SOURCES_HDF5_H
#define NIX_ENTITY_WITH_SOURCES_HDF5_H

#include <nix/base/IBlock.hpp>
#include <nix/base/IEntityWithSources.hpp>
#include <nix/hdf5/SourceHDF5.hpp>
#include <nix/hdf5/EntityWithMetadataHDF5.hpp>

namespace nix {
namespace hdf5 {


/**
 * Base class for entities that are associated with sources.
 */
class EntityWithSourcesHDF5: public virtual base::IEntityWithSources, public EntityWithMetadataHDF5 {

private:

    std::shared_ptr<base::IBlock> entity_block;
    Group sources_refs;

public:

    /**
     * Standard constructor for existing entity.
     */
    EntityWithSourcesHDF5(std::shared_ptr<base::IFile> file, std::shared_ptr<base::IBlock> block, Group group);
    
    /**
     * Standard constructor for new entity.
     */
    EntityWithSourcesHDF5(std::shared_ptr<base::IFile> file, std::shared_ptr<base::IBlock> block, Group group,
                          const std::string &id, const std::string &type, const std::string &name);

    /**
     * Standard constructor for new entity that preserves the creation time.
     */
    EntityWithSourcesHDF5(std::shared_ptr<base::IFile> file, std::shared_ptr<base::IBlock> block, Group group,
                          const std::string &id, const std::string &type, const std::string &name, time_t time);

    size_t sourceCount() const;


    bool hasSource(const std::string &id) const;

    /**
      * @brief Set all sources associations for this entity.
      *
      * All previously existing associations will be overwritten.
      *
      * @param sources     A vector with all sources.
      */
    void sources(const std::vector<Source> &srcs_new);

    void addSource(const std::string &id);


    bool removeSource(const std::string &id);


    std::shared_ptr<base::ISource> getSource(const std::string &id) const;


    std::shared_ptr<base::ISource> getSource(const size_t index) const;

    /**
     * Destructor.
     */
    virtual ~EntityWithSourcesHDF5();


    std::shared_ptr<base::IBlock> block() const;

};


} // namespace hdf5
} // namespace nix

#endif /* NIX_ENTITY_WITH_SOURCES_HDF5_H */
