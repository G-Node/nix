// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_ENTITY_WITH_SOURCES_HDF5_H
#define NIX_ENTITY_WITH_SOURCES_HDF5_H

#include <nix.hpp>
#include <nix/base/IEntityWithSources.hpp>
#include <nix/hdf5/ReferenceList.hpp>
#include <nix/hdf5/EntityWithMetadataHDF5.hpp>

namespace nix {
namespace hdf5 {


/**
 * Base class for entities that are associated with sources.
 */
class EntityWithSourcesHDF5: public virtual base::IEntityWithSources, public EntityWithMetadataHDF5 {

private:

    Block entity_block;
    ReferenceList sources_refs;

public:

    /**
     * Standard constructor.
     */
    EntityWithSourcesHDF5(File file, Block block, Group group, const std::string &id);

    /**
     * Standard constructor that preserves the creation time.
     */
    EntityWithSourcesHDF5(File file, Block block, Group group, const std::string &id, time_t time);

    /**
     * Get the number of sources associated with this entity.
     *
     * @return The number sources.
     */
    size_t sourceCount() const;


    bool hasSource(const std::string &id) const;


    void addSource(const std::string &id);


    void addSource(const Source &source);


    bool removeSource(const std::string &id);


    void sources(const std::vector<Source> &s);


    Source getSource(const std::string &id) const;


    Source getSource(const size_t index) const;
    

    void swap(EntityWithSourcesHDF5 &other);


    /**
     * Destructor.
     */
    virtual ~EntityWithSourcesHDF5();

protected:

    Block block() const {
        return entity_block;
    }

};


} // namespace hdf5
} // namespace nix

#endif /* NIX_ENTITY_WITH_SOURCES_HDF5_H */
