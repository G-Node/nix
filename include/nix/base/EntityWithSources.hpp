// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_ENTITY_WITH_SOURCES_H
#define NIX_ENTITY_WITH_SOURCES_H


#include <nix/base/EntityWithMetadata.hpp>
#include <nix/base/IEntityWithSources.hpp>
#include <nix/Source.hpp>

namespace nix {
namespace base {

// TODO what about TNode?
template <typename T>
class EntityWithSources : virtual public IEntityWithSources, public EntityWithMetadata<T> {

public:

    EntityWithSources()
        : EntityWithMetadata<T>()
    {
    }


    EntityWithSources(const std::shared_ptr<T> &p_impl)
        : EntityWithMetadata<T>(p_impl)
    {
    }

    /**
     * Returns the number of sources that are direct descendants of this source.
     *
     * @return The number of direct child sources.
     */
    size_t sourceCount() const {
        return EntityWithMetadata<T>::impl_ptr->sourceCount();
    }

    /**
     * Checks if this source has a specific source as direct descendant.
     *
     * @param id        The id of the source.
     *
     * @return True if a source with the given id is a direct descendant, false
     *         otherwise.
     */
    bool hasSource(const std::string &id) const {
        return EntityWithMetadata<T>::impl_ptr->hasSource(id);
    }

    /**
     * Retrieves a specific child source that is a direct descendant.
     *
     * @param id        The id of the source.
     *
     * @return The source with the given id. If it doesn't exist an exception
     *         will be thrown.
     */
    Source getSource(const std::string &id) const {
        return EntityWithMetadata<T>::impl_ptr->getSource(id);
    }

    /**
     * Returns all sources that are direct descendant of this source as a vector.
     *
     * @return All direct child sources.
     */
    std::vector<Source> sources() const {
        return EntityWithMetadata<T>::impl_ptr->sources();
    }

    /**
     * Set all sources associated with this entity. All previously
     * associated sources, that are not in the vector will be removed.
     *
     * @param sources     A vector with all sources.
     */
    virtual void sources(const std::vector<Source> &sources) {
        return EntityWithMetadata<T>::impl_ptr->sources(sources);
    }

    /**
     * Create a new root source.
     *
     * @param name      The name of the source to create.
     * @param type      The type of the source.
     *
     * @return The created source object.
     */
    void addSource(const Source &source) {
        EntityWithMetadata<T>::impl_ptr->addSource(source);
    }

    /**
     * Remove a root source and all its child sources from
     * the source.
     *
     * @param id        The id of the source to remove.
     *
     * @return True if the source was removed, false otherwise.
     */
    bool removeSource(const std::string &id) {
        return EntityWithMetadata<T>::impl_ptr->removeSource(id);
    }

};

} // namespace base
} // namespace nix


#endif // NIX_ENTITY_WITH_SOURCES_H
