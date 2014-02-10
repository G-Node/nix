// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_ENTITY_WITH_METADATA_H
#define NIX_ENTITY_WITH_METADATA_H

#include <nix/base/NamedEntity.hpp>
#include <nix/base/IEntityWithMetadata.hpp>
#include <nix/Section.hpp>

namespace nix {
namespace base {

// TODO what about TNode?
template <typename T>
class EntityWithMetadata : virtual public IEntityWithMetadata, public NamedEntity<T> {

public:

    EntityWithMetadata()
        : NamedEntity<T>()
    {
    }


    EntityWithMetadata(const std::shared_ptr<T> &p_impl)
        : NamedEntity<T>(p_impl)
    {
    }

    /**
     * Checks if the block has associated metadata.
     *
     * @return True if the block has metadata (odML section),
     *         false otherwise.
     */
    bool hasMetadata() const {
        return NamedEntity<T>::impl_ptr->hasMetadata();
    }

    /**
     * Get metadata associated with this entity.
     *
     * @return The associated section, if no such section exists
     *         an exception will be thrown.
     */
    Section metadata() const {
        return NamedEntity<T>::impl_ptr->metadata();
    }

    /**
     * Associate the entity with some metadata. Calling this method will replace
     * previously stored information.
     *
     *
     * @param metadata    The section that should be associated
     *                    with this entity.
     */
    void metadata(const Section &metadata) {
        NamedEntity<T>::impl_ptr->metadata(metadata);
    }

    /**
     * Remove associated metadata from the entity.
     * The section will not be deleted.
     *
     * @return True if the section was removed, false otherwise.
     */
    bool removeMetadata() {
        return NamedEntity<T>::impl_ptr->removeMetadata();
    }


    virtual ~EntityWithMetadata() {}

};


} // namespace base
} // namespace nix

#endif // NIX_ENTITY_WITH_METADATA_H
