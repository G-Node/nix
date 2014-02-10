// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_I_ENTITY_WITH_METADATA_H
#define NIX_I_ENTITY_WITH_METADATA_H

#include <nix/base/INamedEntity.hpp>

namespace nix {

class Section;

namespace base {

/**
 * Base class for entities that are associated with metadata such
 * as Block, Source etc.
 */
class IEntityWithMetadata : virtual public INamedEntity {

public:

    /**
     * Checks if the block has associated metadata.
     *
     * @return True if the block has metadata (odML section),
     *         false otherwise.
     */
    virtual bool hasMetadata() const = 0;

    /**
     * Get metadata associated with this entity.
     *
     * @return The associated section, if no such section exists
     *         an exception will be thrown.
     */
    virtual Section metadata() const = 0;

    /**
     * Associate the entity with some metadata. Calling this method will replace
     * previously stored information.
     *
     *
     * @param metadata    The section that should be associated
     *                    with this entity.
     */
    virtual void metadata(const Section &metadata) = 0;

    /**
     * Remove associated metadata from the entity.
     * The section will not be deleted.
     *
     * @return True if the section was removed, false otherwise.
     */
    virtual bool removeMetadata() = 0;


    virtual ~IEntityWithMetadata() {}

};


} // namespace base
} // namespace nix

#endif // NIX_I_ENTITY_WITH_METADATA_H
