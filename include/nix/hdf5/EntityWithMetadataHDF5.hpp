// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef PAN_ENTITY_WITH_METADATA_H_INCLUDED
#define PAN_ENTITY_WITH_METADATA_H_INCLUDED

#include <string>
#include <iostream>


#include <nix.hpp>
#include <nix/hdf5/NamedEntityHDF5.hpp>

namespace nix {
namespace hdf5 {

/**
 * Base class for entities that are associated with metadata such
 * as Block, Source etc.
 *
 * TODO Implement all methods of EntityWithMetadata.
 */
class EntityWithMetadataHDF5 : virtual public base::IEntityWithMetadata, public NamedEntityHDF5 {

public:

    /**
     * Standard constructor
     */
    EntityWithMetadataHDF5(File file, Group group, const std::string &id);

    /**
     * Standard constructor that preserves the creation time.
     */
    EntityWithMetadataHDF5(File file, Group group, const std::string &id, time_t time);

    /**
     * Checks if the block has associated metadata.
     *
     * @return True if the block has metadata (odML section),
     *         false otherwise.
     */
    bool hasMetadata() const;

    /**
     * Get metadata associated with this entity.
     *
     * @return The associated section, if no such section exists
     *         an exception will be thrown.
     */
    Section metadata() const;

    /**
     * Associate the entity with some metadata. Calling this method will replace
     * previously stored information.
     *
     *
     * @param metadata    The section that should be associated
     *                    with this entity.
     */
    void metadata(const Section &metadata);

    /**
     * Remove associated metadata from the entity.
     * The section will not be deleted.
     *
     * @return True if the section was removed, false otherwise.
     */
    bool removeMetadata();

    /**
     * Destructor of this class.
     */
    virtual ~EntityWithMetadataHDF5();

};


} // namespace hdf5
} // namespace nix

#endif /* PAN_ENTITY_WITH_METADATA_H_INCLUDED */
