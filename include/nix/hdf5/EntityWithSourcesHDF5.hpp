// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef PAN_ENTITY_WITH_SOURCES_H_INCLUDED
#define PAN_ENTITY_WITH_SOURCES_H_INCLUDED

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

protected:

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

    /**
     * Checks if a specific source is associated with this entity.
     *
     * @param source      The source to check.
     *
     * @return True if the source is associated with this entity, false otherwise.
     */
    bool hasSource(const Source &source) const;

    /**
     * Checks if a specific source is associated with this entity.
     *
     * @param id      The source id to check.
     *
     * @return True if the source is associated with this entity, false otherwise.
     */
    bool hasSource(const std::string &id) const;

    /**
     * Add a specific source to the list of associated sources.
     *
     * @param source      The source to associate with this entity.
     */
    void addSource(const Source &source);

    /**
     * Remove a source from the list of associated sources.
     * This source will not be deleted from the file.
     *
     * @param source      The source to remove.
     *
     * @return True if the source was removed, false otherwise.
     */
    bool removeSource(const Source &source);

    /**
     * Set all sources associated with this entity. All previously
     * associated sources, that are not in the vector will be removed.
     *
     * @param s     A vector with all sources.
     */
    void sources(const std::vector<Source> &s);

    /**
     * Get all associated sources of this entity.
     *
     * @return A vector with all sources.
     */
    std::vector<Source> sources() const;

    /**
     * Returns the source identified by the given id.
     *
     * @param string the source id
     */
    Source getSource(const std::string id) const;

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

#endif /* PAN_ENTITY_WITH_SOURCES_H_INCLUDED */
