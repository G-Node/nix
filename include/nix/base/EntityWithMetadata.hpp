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
#include <nix/Section.hpp>

namespace nix {
namespace base {

/**
 * @brief Base class for entities that can be associated with additional metadata.
 *
 * The data part of the NIX data model consists of five main elements which all inherit from
 * {@link nix::base::EntityWithMetadata}: {@link nix::Block}, {@link nix::DataArray}, {@link nix::MultiTag},
 * {@link nix::Tag}, and {@link nix::Source} that serve different purposes.
 * Common to all those entities is an optional property {@link metadata} which  provides a link to a
 * {@link nix::Section} entity and therefore makes it possible to annotate the entities with additional
 * metadata.
 */
template <typename T>
class EntityWithMetadata : public NamedEntity<T> {

public:

    /**
     * @brief Constructor that creates a null entity.
     */
    EntityWithMetadata()
        : NamedEntity<T>()
    {
    }

    /**
     * @brief Constructor that creates a new entity from a shared pointer to
     * an implementation instance.
     */
    EntityWithMetadata(const std::shared_ptr<T> &p_impl)
        : NamedEntity<T>(p_impl)
    {
    }

    /**
     * @brief Constructor with move semantics that creates a new entity from a
     * shared pointer to an implementation instance.
     */
    EntityWithMetadata(std::shared_ptr<T> &&ptr)
        : NamedEntity<T>(std::move(ptr))
    {
    }

    /**
     * @brief Get metadata associated with this entity.
     *
     * @return The associated section, if no such section exists an
     *         uninitialized {@link nix::Section} will be returned.
     */
    Section metadata() const {
        return NamedEntity<T>::backend()->metadata();
    }

    /**
     * @brief Associate the entity with some metadata.
     *
     * Calling this method will replace previously stored information.
     *
     * @param metadata The {@link nix::Section} that should be associated
     *                 with this entity.
     */
    void metadata(const Section &metadata) {
        if(metadata == none){
            NamedEntity<T>::backend()->metadata(none);
        }
        else{
            NamedEntity<T>::backend()->metadata(metadata.id());
        }
    }

    /**
     * @brief Associate the entity with some metadata.
     *
     * Calling this method will replace previously stored information.
     *
     * @param id        The id of the {@link nix::Section} that should be associated
     *                  with this entity.
     */
    void metadata(const std::string &id) {
        NamedEntity<T>::backend()->metadata(id);
    }

    /**
     * @brief Remove associated metadata from the entity.
     *
     * This method just removes the association between the respective
     * {@link nix::Section} but will not remove it from the file.
     *
     * @param t         None
     */
    void metadata(const none_t t) {
        NamedEntity<T>::backend()->metadata(t);
    }

    /**
     * Destructor
     */
    virtual ~EntityWithMetadata() {}

};


} // namespace base
} // namespace nix

#endif // NIX_ENTITY_WITH_METADATA_H
